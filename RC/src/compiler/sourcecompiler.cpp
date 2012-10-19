#include "sourcecompiler.h"

SourceCompiler::SourceCompiler()
{
    initDirectories();

    //Предопределенные типы MPI
    mpiTypes["int"]    = "MPI_INT";
    mpiTypes["long"]   = "MPI_LONG";
    mpiTypes["char"]   = "MPI_CHAR";
    mpiTypes["byte"]   = "MPI_BYTE";
    mpiTypes["float"]  = "MPI_FLOAT";
    mpiTypes["double"] = "MPI_DOUBLE";
}

QStringList SourceCompiler::compileData(QList<const Variable *> variables)
{
    QStringList errors = QStringList();
    QTime t;
    t.start();
    globalLogger->log(QObject::tr("Компиляция данных....."), Logger::Compile);
    //Получаем (пока) все типы ПО
    QList<const DataType *> myTypeList = globalDBManager->getDataTypeList();
    errors.append(compileUserTypes(myTypeList));
    errors.append(compileTpoData(variables));
    const int elapsed = qRound(t.elapsed()/1000);
    if (errors.isEmpty())
        globalLogger->log(QObject::tr("Компиляция завершена без ошибок за %1 с").arg(QString::number(elapsed)), Logger::Compile);
    else
        globalLogger->log(QObject::tr("Компиляция провалилась"), Logger::Compile);
    return errors;
}

QStringList SourceCompiler::compileTpoData(QList<const Variable *> variables)
{
    QStringList errors = QStringList();
    //Получаем (пока) все переменные ПО
    QList<const Variable *> varList = variables;

    //Создаем файл TPOData.h
    QString poDataText = getTemplate(myTemplateDirectory + "/" + QString(PODATA_FILE_NAME) + ".h.template");
    if (poDataText.isEmpty()) {
        errors.append(globalLogger->log(QObject::tr(ERR_DATACOMPI_EMPTY_TEMPL).arg(QString(PODATA_FILE_NAME) + ".h"), Logger::Warning));
    }

    //Заполняем блок Id
    QString varIdBlock;
    for (int i = 0; i < varList.count(); i++)
        if (varList[i]->isGlobal)
            varIdBlock.append("int " + varList[i]->name + "_id;\n");
    poDataText.replace("<#varId>", varIdBlock);

    //Заполняем блок указателей на данные
    QString varPtrBlock;
    foreach (const Variable *var, varList)
        varPtrBlock.append(var->type->name + (var->isGlobal ? " *_" : " *") + var->name + ";\n");
    poDataText.replace("<#varPtr>", varPtrBlock);

    //Заполняем блок свойств
    QString varPropertyBlock;

    //Заполняем блок setter/getter/property
    QString setGetBlock;
    foreach (const Variable *var, varList) {
        if (!var->isGlobal)
            continue;
        const DataType* type = var->type;
        Q_ASSERT(type != NULL);
        //Если встертился массив
        if (type->isArray()) {
            //Парсим тип элемента
            QRegExp r("(typedef\\s+(\\b.+\\b))");
            r.setMinimal(true);
            Q_ASSERT(r.indexIn(type->typedefStr) != -1);
            QString intType = r.cap(2);

            setGetBlock.append("ptr" + var->type->name + " get_" + var->name + "();\n");
            setGetBlock.append("ptr" + var->type->name + " set_" + var->name + "(const ptr" + var->type->name + "& value);\n");
            setGetBlock.append(intType + " get_" + var->name + "(const int& index);\n");
            setGetBlock.append(intType + " set_" + var->name + "(const int& index, const " + intType + "& value);\n");
            varPropertyBlock.append("__property_rw_indexed<" + intType + ", int, TPOData> " + var->name + ";\n");
        } else {
            setGetBlock.append(var->type->name + " get_" + var->name + "();\n");
            setGetBlock.append(var->type->name + " set_" + var->name + "(const " + var->type->name + "& value);\n");
            varPropertyBlock.append("__property_rw<" + var->type->name + ", TPOData> " + var->name + ";\n");
        }
    }
    poDataText.replace("<#setGet>", setGetBlock);
    poDataText.replace("<#varProperty>", varPropertyBlock);

    //Сохраняем файл
    QFile poDataH(myOutputDirectory + "/" + PODATA_FILE_NAME + ".h");
    poDataH.open(QFile::WriteOnly);
    poDataH.write(poDataText.toUtf8());
    poDataH.close();

    //Создаем файл TPOData.cpp
    poDataText = getTemplate(myTemplateDirectory + "/" + QString(PODATA_FILE_NAME) + ".cpp.template");
    if (poDataText.isEmpty()) {
        errors.append(globalLogger->log(QObject::tr(ERR_DATACOMPI_EMPTY_TEMPL).arg(QString(PODATA_FILE_NAME) + ".cpp"), Logger::Warning));
    }

    //Шаблон <#assignSetterGetter> В конструкторе присваевает методы доступа к свойствам
    QString assignSetterGetterBlock;
    //Шаблон <#delete> В деструкторе очищает память
    QString deleteBlock;
    //Шаблон <#initMemory> В функции initMemory() выделяет память для rank = 0, для остальных = NULL
    QString initMemoryBlock;
    initMemoryBlock.append("if (myRank == 0) {\n");
    //Шаблон <#initId> В функции initId() присваевает всем порядковый номер
    QString initIdBlock;
    //Шаблон <#getDataAddr> В функции getDataAddr() возвращает указатель на память в зависимостиот id
    QString getDataAddrBlock;
    getDataAddrBlock.append("void* result = NULL;\n"
                            "switch (id) {\n");
    //Шаблон <#getDataSize> В функции getDataSize() возвращает размер ОДНОГО!! элемента
    QString getDataSizeBlock;
    getDataSizeBlock.append("int result = 0;\n"
                            "switch (id) {\n");
    //Шаблон <#getMpiType> В функции getMpiType() возвращает тип ОДНОГО!! элемента
    QString getMpiTypeBlock;
    getMpiTypeBlock.append("MPI_Datatype result = MPI_INT;\n"
                           "switch (id) {\n");
    //Шаблон <#setGet> Функции доступа для свойств. Для массива 4 функции, для простого типа 2.
    setGetBlock.clear();

    int i = 0;
    foreach (const Variable* var, varList) {
        const DataType* type = var->type;
        Q_ASSERT(type != NULL);
        const QString tn = var->type->name;
        const QString vn = var->name;
        QString intType = tn;
        if (!type->typedefStr.isEmpty()) {
            //Парсим тип элемента
            QRegExp r("(typedef\\s+(\\b.+\\b))");
            r.setMinimal(true);
            Q_ASSERT(r.indexIn(type->typedefStr) != -1);
            intType = r.cap(2);
        }
        //Общее для всех типов
        if (var->isGlobal) {
            initIdBlock.append(vn + "_id = " + QString::number(i) + ";\n");
            getDataAddrBlock.append("case " + QString::number(i) + ":\n"
                                    "\tresult = _" + vn + ";\n"
                                    "\tbreak;\n");
            getMpiTypeBlock.append("case " + QString::number(i) + ":\n"
                                   "\tresult = " + (mpiTypes.contains(tn) ? mpiTypes[tn] : "MPI_USER_TYPE_" + tn.toUpper()) + ";\n"
                                   "\tbreak;\n");
        }
        //Блок очистки памяти
        deleteBlock.append("if (" + QString(var->isGlobal ? "_" : "") + vn + " != NULL)\r\n"
                           "\tdelete " + (var->isGlobal ? "_" : "") + vn + ";\n");

        //Блок инициализации памяти на менеджере памяти и блок setters/getters
        if (var->isGlobal) {
            //Если встертился массив
            if (type->isArray()) {
                initMemoryBlock.append("_" + vn + " = (" + tn + "*)" + "(new " + tn + ");\n");
                assignSetterGetterBlock.append(vn + ".Assign(this, &TPOData::set_" + vn + ", &TPOData::get_" + vn + ", &TPOData::set_" + vn + ", &TPOData::get_" + vn + ", " + tn + "_LENGTH);\n");
                getDataSizeBlock.append("case " + QString::number(i++) + ":\n"
                                        "\tresult = sizeof(" + tn + ") / " + tn + "_LENGTH;\n"
                                        "\tbreak;\n");
                //getter by index
                setGetBlock.append(intType + " TPOData::get_" + vn + "(const int& index)\n{\n"
                                   "\tif (_" + vn + " == NULL) _" + vn + " = (" + tn + "*)" + "(new " + tn + ");\n" +
                                   "\tgetData(" + vn + "_id, index, 1, &(*_" + vn + ")[index]);\n"
                                   "\treturn (*_" + vn + ")[index];\n}\n\n");
                //setter by index
                setGetBlock.append(intType + " TPOData::set_" + vn + "(const int& index, const " + intType + " &value)\n{\n"
                                   "\tsetData(" + vn + "_id, index, 1, &value);\n"
                                   "\treturn value;\n}\n\n");
                //getter all array
                setGetBlock.append("ptr" + tn + " TPOData::get_" + vn + "()\n{\n"
                                   "\tif (_" + vn + " == NULL) _" + vn + " = (" + tn + "*)" + "(new " + tn + ");\n" +
                                   "\tgetData(" + vn + "_id, 0, " + tn + "_LENGTH" + ", _" + vn +");\n"
                                   "\treturn *_" + vn + ";\n}\n\n");
                //setter all array
                setGetBlock.append("ptr" + tn + " TPOData::set_" + vn + "(const " + "ptr" + tn + " &value)\n{\n"
                                   "\tsetData(" + vn + "_id, 0, " + tn + "_LENGTH" ", *(&value));\n"
                                   "\treturn (" + intType + " *)value;\n}\n\n");
            } else {
                initMemoryBlock.append("*(_" + vn + " = new " + tn + ")" + (var->initValue.isEmpty() ? ";\n" : " = " + var->initValue + ";\n"));
                assignSetterGetterBlock.append(vn + ".Assign(this, &TPOData::set_" + vn + ", &TPOData::get_" + vn + ");\n");
                getDataSizeBlock.append("case " + QString::number(i++) + ":\n"
                                        "\tresult = sizeof(" + tn + ");\n"
                                        "\tbreak;\n");
                setGetBlock.append(tn + " TPOData::get_" + vn + "()\n{\n"
                                   "\tif (_" + vn + " == NULL) *(_" + vn + " = new " + tn + ")" + (var->initValue.isEmpty() ? ";\n" : " = " + var->initValue + ";\n") +
                                   "\tgetData(" + vn + "_id, 0, 1, _" + vn +");\n"
                                   "\treturn *_" + vn + ";\n}\n\n");
                setGetBlock.append(tn + " TPOData::set_" + vn + "(const " + tn + " &value)\n{\n"
                                   "\tsetData(" + vn + "_id, 0, 1, &value);\n"
                                   "\treturn value;\n}\n\n");
            }
        } else {
            //Не выделяем память под локальные переменные на менеджере памяти
            initMemoryBlock.append(var->name + " = NULL;\n");
        }
    }
    poDataText.replace("<#assignSetterGetter>", assignSetterGetterBlock);
    poDataText.replace("<#delete>", deleteBlock);
    poDataText.replace("<#initId>", initIdBlock);
    poDataText.replace("<#setGet>", setGetBlock);

    initMemoryBlock.append("} else {\n");
    getDataAddrBlock.append("}\nreturn result;\n");
    getDataSizeBlock.append("}\nreturn result;\n");
    getMpiTypeBlock.append("}\nreturn result;\n");
    poDataText.replace("<#getDataAddr>", getDataAddrBlock);
    poDataText.replace("<#getSizeAddr>", getDataSizeBlock);
    poDataText.replace("<#getMpiType>", getMpiTypeBlock);

    //Инициализация памяти для каждого процессора
    foreach (const Variable* var, varList) {
        const QString tn = var->type->name;
        const QString vn = var->name;
        initMemoryBlock.append(var->isGlobal
                               ? "_" + var->name + " = NULL"
                               : (var->type->isArray()
                                  ? QString("(%1 = (%2*)new %3)").arg(vn).arg(tn).arg(tn)
                                  : QString("*(%1 = new %2)" + QString(var->initValue.isEmpty() ? "" : " = ") + var->initValue).arg(vn).arg(tn)));
        initMemoryBlock.append(";\r\n");
    }
    initMemoryBlock.append("}\n");
    poDataText.replace("<#initMemory>", initMemoryBlock);

    //Сохраняем файл
    QFile poDataCpp(myOutputDirectory + "/" + PODATA_FILE_NAME + ".cpp");
    poDataCpp.open(QFile::WriteOnly);
    poDataCpp.write(poDataText.toUtf8());
    poDataCpp.close();

    return errors;
}

QStringList SourceCompiler::compileUserTypes(QList<const DataType *> dataTypes)
{
    QStringList errors = QStringList();
    QString utypeText = getTemplate(myTemplateDirectory + "/" + QString(USER_TYPES_FILE_NAME) + ".h.template");
    if (utypeText.isEmpty()) {
        errors.append(globalLogger->log(QObject::tr(ERR_DATACOMPI_EMPTY_TEMPL).arg(QString(USER_TYPES_FILE_NAME) + ".cpp"), Logger::Warning));
    }

    //Создаем типы
    QString userTypesBlock;
    foreach (const DataType *type, dataTypes)
        userTypesBlock.append(compileDataType(type));
    utypeText.replace("<#userTypes>", userTypesBlock);

    //Создаем типы MPI
    //TODO: ДЛЯ СТРУКТУР ПОКА НИЧЕГО НЕТ
    QString mpiTypesBlock;
    foreach (const DataType *type, dataTypes) {
        if (type->typedefStr.isEmpty() || type->isStruct())
            continue;
        QRegExp r("(typedef\\s+(\\b.+\\b))");
        r.setMinimal(true);
        Q_ASSERT(r.indexIn(type->typedefStr) != -1);
        const QString intType = r.cap(2);
        const QString baseMpiType = mpiTypes.contains(intType) ? mpiTypes[r.cap(2)] : "MPI_USER_TYPE_" + r.cap(2).toUpper();
        mpiTypesBlock.append("#define MPI_USER_TYPE_").append(type->name.toUpper()).append(" ").append(baseMpiType).append("\r\n");
    }
    utypeText.replace("<#mpiTypes>", mpiTypesBlock);

    //Сохраняем файл
    QFile userTypes(myOutputDirectory + "/" + USER_TYPES_FILE_NAME + ".h");
    userTypes.open(QFile::WriteOnly);
    userTypes.write(utypeText.toUtf8());
    userTypes.close();

    return errors;
}

void SourceCompiler::copyStaticTemplates()
{
    // копируем необходимые файлы
    Q_ASSERT(QFile::exists(myTemplateDirectory + "/graph.h.template"));
    Q_ASSERT(QFile::exists(myTemplateDirectory + "/memman.h.template"));
    Q_ASSERT(QFile::exists(myTemplateDirectory + "/graph.cpp.template"));
    Q_ASSERT(QFile::exists(myTemplateDirectory + "/property.h.template"));
    Q_ASSERT(QFile::exists(myTemplateDirectory + "/memman.cpp.template"));
    Q_ASSERT(QFile::exists(myTemplateDirectory + "/graphmv.cpp.template"));
    Q_ASSERT(QFile::exists(myTemplateDirectory + "/Makefile.template"));
    Q_ASSERT(QFile::exists(myTemplateDirectory + "/stype.h.template"));
    Q_ASSERT(QFile::exists(myTemplateDirectory + "/defines.h.template"));

    QFile::remove(myOutputDirectory + "/graph.h");
    QFile::remove(myOutputDirectory + "/memman.h");
    QFile::remove(myOutputDirectory + "/graph.cpp");
    QFile::remove(myOutputDirectory + "/property.h");
    QFile::remove(myOutputDirectory + "/memman.cpp");
    QFile::remove(myOutputDirectory + "/graphmv.cpp");
    QFile::remove(myOutputDirectory + "/Makefile");
    QFile::remove(myOutputDirectory + "/stype.h");
    QFile::copy(myTemplateDirectory + "/graph.h.template", myOutputDirectory + "/graph.h");
    QFile::copy(myTemplateDirectory + "/memman.h.template", myOutputDirectory + "/memman.h");
    QFile::copy(myTemplateDirectory + "/graph.cpp.template", myOutputDirectory + "/graph.cpp");
    QFile::copy(myTemplateDirectory + "/property.h.template", myOutputDirectory + "/property.h");
    QFile::copy(myTemplateDirectory + "/memman.cpp.template", myOutputDirectory + "/memman.cpp");
    QFile::copy(myTemplateDirectory + "/graphmv.cpp.template", myOutputDirectory + "/graphmv.cpp");
    QFile::copy(myTemplateDirectory + "/Makefile.template", myOutputDirectory + "/Makefile");
    QFile::copy(myTemplateDirectory + "/stype.h.template", myOutputDirectory + "/stype.h");
    QFile::copy(myTemplateDirectory + "/defines.h.template", myOutputDirectory + "/defines.h");

    foreach (const QString  name, myOtherFilesList) {
        Q_ASSERT(QFile::exists(myBaseDirectory + "/" + name));
        QFile::remove(myOutputDirectory + "/" + name);
        QFile::copy(myBaseDirectory + "/" + name, myOutputDirectory + "/" + name);
    }
}

void SourceCompiler::compileEnvironment() const
{
    QString runme = getTemplate(myTemplateDirectory + "/runme.bat.template");
    runme.replace("<#projectName>", globalDBManager->getProjectName());
    QFile runme_f(myOutputDirectory + "/runme.bat");
    runme_f.open(QFile::WriteOnly);
    runme_f.write(runme.toUtf8());
    runme_f.close();

    QSettings c("graph.ini", QSettings::IniFormat);
    QString root = c.value("SK/root", "/home").toString();
    QString host = c.value("SK/host", "sk.ssau.ru").toString();
    QString port = c.value("SK/port", "22").toString();
    QString user = c.value("SK/user", "user").toString();
    QString plink = c.value("Compiler/plink", "plink.exe").toString();
    QString pscp = c.value("Compiler/pscp", "pscp.exe").toString();
    QString password = c.value("SK/password", "****").toString();

    QString runlocal = getTemplate(myTemplateDirectory + "/runlocal.bat.template");
    runlocal.replace("<#projectName>", globalDBManager->getProjectName());
    runlocal.replace("<#user>", user);
    runlocal.replace("<#host>", host);
    runlocal.replace("<#port>", port);
    runlocal.replace("<#root>", root);
    runlocal.replace("<#password>", password);
    runlocal.replace("<#plink>", plink);
    runlocal.replace("<#pscp>", pscp);

    QFile runlocal_f(myOutputDirectory + "/runlocal.bat");
    runlocal_f.open(QFile::WriteOnly);
    runlocal_f.write(runlocal.toUtf8());
    runlocal_f.close();

    QString runmpi = getTemplate(myTemplateDirectory + "/runmpi.pbs.template");
    runmpi.replace("<#nodes>", "2");
    runmpi.replace("<#procsPerNode>", "8");
    runmpi.replace("<#projectName>", globalDBManager->getProjectName());

    QFile runmpi_f(myOutputDirectory + "/runmpi.pbs");
    runmpi_f.open(QFile::WriteOnly);
    runmpi_f.write(runmpi.toUtf8());
    runmpi_f.close();
}

void SourceCompiler::initDirectories()
{
    myOutputDirectory = QGraphSettings::getOutputDirectory();
    myBaseDirectory = QGraphSettings::getBaseDirectory();
    myTemplateDirectory = QGraphSettings::getTemplateDirectory();
    myOtherFilesList = globalDBManager->getOtherFilesDB();
}

QStringList SourceCompiler::compilePredicate(const Predicate *predicate) const
{
    QStringList errors = predicate->validate();
    if (!errors.isEmpty())
        return errors;

    QString outputData;
    QFile output;

    const BaseModule *currentBaseModule = predicate->baseModule;
    QStringList params;
    QStringList signature;
    // Создаем сpp файл
    outputData.append("#include \"tpodata.h\"\r\n");
    outputData.append("PROJECT_BEGIN_NAMESPACE\r\n");
    switch(predicate->type) {
    case Predicate::NormalType:
        Q_ASSERT(currentBaseModule);
        // Генерируем предикат
        foreach (BaseModuleParameter parameter, predicate->baseModule->parameterList) {
            signature << QString("%1 *%2")
                         .arg(parameter.type)
                         .arg(parameter.name);
        }
        outputData.append("extern int " + predicate->baseModule->uniqName + "(" + signature.join(", ") + ");\r\n");
        outputData.append("int " + predicate->name + "(TPOData *D)\r\n");
        outputData.append("{\r\n");
        outputData.append("//" + predicate->extName + "\r\n");
        // Инициализуем данные
        for (int i = 0; i < predicate->variableList.count(); i++) {
            BaseModuleParameter parameter = currentBaseModule->parameterList[i];
            outputData.append(predicate->variableList[i]->isGlobal ? QString("\t%1%2 _%3 = D->%4;\r\n") : QObject::tr("\tconst %1 *_%2 = D->%3;\r\n")
                              .arg(parameter.type)
                              .arg(parameter.name)
                              .arg(predicate->variableList[i]->name).toUtf8());
        }
        // Вызываем прототип
        outputData.append("\r\n\treturn ");
        outputData.append(predicate->baseModule->uniqName);
        outputData.append("(");
        for(int i = 0; i < predicate->variableList.count(); i++) {
            BaseModuleParameter parameter = currentBaseModule->parameterList[i];
            params << ((predicate->variableList[i]->isGlobal ? QObject::tr("&_") : QObject::tr("_")) + parameter.name);
        }
        outputData.append(params.join(", ").toUtf8());
        outputData.append(");\r\n\r\n}\r\n");
        break;
    case Predicate::InlineType:
        //генерируем с++ файл
        outputData.append("int " + predicate->name + "(TPOData *D)\r\n");
        outputData.append("{\r\n");
        QString code(predicate->extName);
        foreach (const Variable *variable, predicate->variableList) {
            QRegExp r("\\b" + variable->name + "\\b", Qt::CaseSensitive);
            r.setMinimal(true);
            code.replace(r, variable->isGlobal ? "D->" + variable->name : "(*(D->" + variable->name + "))");
        }
        outputData.append("  return (" + code + ");\r\n");
        outputData.append("}\r\n");
        break;
    }
    outputData.append("PROJECT_END_NAMESPACE\r\n");

    output.setFileName(myOutputDirectory + "/" + predicate->name + ".cpp");
    output.open(QFile::WriteOnly);
    output.write(outputData.toUtf8());
    output.close();
}

QStringList SourceCompiler::compileActor(const Actor *actor) const
{
    QStringList errors = actor->validate();
    if (!errors.isEmpty())
        return errors;

    QString outputData;
    QFile output;

    const BaseModule *currentBaseModule = actor->baseModule;
    QStringList params;
    QStringList signature;
    // Создаем сpp файл
    outputData.append("#include \"tpodata.h\"\r\n");
    outputData.append("PROJECT_BEGIN_NAMESPACE\r\n");
    switch(actor->type) {
    case Actor::NormalType:
        Q_ASSERT(currentBaseModule);
        // Генерируем актор
        foreach (BaseModuleParameter parameter, actor->baseModule->parameterList) {
            const QString constStr = parameter.accessMode == QObject::tr("Исходный") ? QObject::tr("const ") : "";
            signature << QString("%1%2 *%3").arg(constStr)
                         .arg(parameter.type)
                         .arg(parameter.name);
        }
        outputData.append("extern int " + actor->baseModule->uniqName + "(" + signature.join(", ") + ");\r\n");
        outputData.append("int " + actor->name + "(TPOData *D)\r\n");
        outputData.append("{\r\n");
        outputData.append("//" + actor->extName + "\r\n");
        // Инициализуем данные
        for (int i = 0; i < actor->variableList.count(); i++) {
            BaseModuleParameter parameter = currentBaseModule->parameterList[i];
            const QString constStr = parameter.accessMode == QObject::tr("Исходный") ? QObject::tr("const ") : "";
            outputData.append(actor->variableList[i]->isGlobal ? QObject::tr("\t%1%2 _%3 = D->%4;\r\n") : QObject::tr("\t%1%2 *_%3 = D->%4;\r\n")
                                                                .arg(constStr)
                                                                .arg(parameter.type)
                                                                .arg(parameter.name)
                                                                .arg(actor->variableList[i]->name).toUtf8());
        }
        // Вызываем прототип
        outputData.append("\r\n\tint result = ");
        outputData.append(actor->baseModule->uniqName);
        outputData.append("(");
        for(int i = 0; i < actor->variableList.count(); i++) {
            BaseModuleParameter parameter = currentBaseModule->parameterList[i];
            params << ((actor->variableList[i]->isGlobal ? QObject::tr("&_") : QObject::tr("_")) + parameter.name);
        }
        outputData.append(params.join(", ").toUtf8());
        outputData.append(");\r\n\r\n");
        // сохраняем данные
        for(int i = 0; i < actor->variableList.count(); i++) {
            BaseModuleParameter parameter = currentBaseModule->parameterList[i];
            if (parameter.accessMode != QObject::tr("Исходный"))
                outputData.append(QString(QObject::tr("\tD->%1 = _%2;\n"))
                                  .arg(actor->variableList[i]->name)
                                  .arg(parameter.name).toUtf8());
        }
        // выход
        outputData.append("\r\n\r\n\treturn result;\r\n}");
        break;
    case Actor::InlineType:
        //генерируем с++ файл
        outputData.append("int " + actor->name + "(TPOData *D)\r\n");
        outputData.append("{\r\n");
        QString code(actor->extName);
        foreach (const Variable *variable, actor->variableList) {
            QRegExp r("\\b" + variable->name + "\\b", Qt::CaseSensitive);
            r.setMinimal(true);
            code.replace(r, variable->isGlobal ? "D->" + variable->name : "(*(D->" + variable->name + "))");
        }
        outputData.append("  " + code + "\r\n");
        outputData.append("  return 1;\r\n");
        outputData.append("}\r\n");
        break;
    }
    outputData.append("PROJECT_END_NAMESPACE\r\n");

    output.setFileName(myOutputDirectory + "/" + actor->name + ".cpp");
    output.open(QFile::WriteOnly);
    output.write(outputData.toUtf8());
    output.close();

}

QString SourceCompiler::compileDataType(const DataType *dataType) const
{
    QString result;
    if (dataType->typedefStr.isEmpty())
        return result;

    result.append(dataType->typedefStr).append("\r\n");

    if (!QGraphSettings::isParallel())
        return result;

    if (dataType->isArray()) {
        //Парсим количество элементов
        QRegExp r("(\\[(.+)\\])");
        r.setMinimal(true);
        Q_ASSERT(r.indexIn(dataType->typedefStr) != -1);
        result.append("#define ").append(dataType->name).append("_LENGTH ").append(r.cap(2)).append("\r\n");

        //Парсим тип элемента
        QRegExp r2("(typedef\\s+(\\b.+\\b))");
        r2.setMinimal(true);
        Q_ASSERT(r2.indexIn(dataType->typedefStr) != -1);
        result.append("typedef ").append(r2.cap(2)).append("* ptr").append(dataType->name).append(";\r\n");
    }

    return result;
}

void SourceCompiler::copyUsedFiles(const QList<const Actor *> &actors, const QList<const Predicate *> &predicates, const QList<const BaseModule *> &baseModules)
{
    // копируем файлы используемых акторов и предикатов в выходной каталог
    foreach (const Actor *actor, actors)
        if (actor->type != Actor::GraphType)
            compileActor(actor);
    foreach (const Predicate *predicate, predicates)
        compilePredicate(predicate);
    foreach (const BaseModule *baseModule, baseModules) {
        const QString f1 = myBaseDirectory + "/" + baseModule->name + ".c";
        const QString f2 = myBaseDirectory + "/" + baseModule->uniqName + ".cpp";
        Q_ASSERT(QFile::exists(f1));
        Q_ASSERT(QFile::exists(f2));
        const QString nf1 = myOutputDirectory + "/" + baseModule->name + ".c";
        const QString nf2 = myOutputDirectory + "/" + baseModule->uniqName + ".cpp";
        QFile::remove(nf1);
        QFile::remove(nf2);
        QFile::copy(f1, nf1);
        QFile::copy(f2, nf2);
    }
}

QString SourceCompiler::buildGraph(const QString &name, const QString &extName, int root) const
{
    QString result;
    result.append("int " + name + "(TPOData *D)\r\n");
    result.append("{\r\n");
    result.append("\t//" + extName + "\r\n");
    result.append("\t//printf(\"" + extName + "\\r\\n\");\r\n");
    result.append("\tint topCount = " + QString::number(0) + "; //never used parameter\r\n");
    result.append("\tint rootTop = " + QString::number(root) + ";\r\n");
    result.append("\tGraphMV(D, rootTop, topCount, ListPred, ListTop, ListGraf);\r\n");
    result.append("\treturn 1;\r\n");
    result.append("}\r\n");
    return result;
}

void SourceCompiler::compileMakefile(QString target, const QList<const Actor *> &actors, const QList<const Predicate *> &predicates, const QList<const BaseModule *> &baseModules) const
{
    target = target.toLower();
    QStringList objects;
    QStringList sources;
    QStringList targets;
    QSet<QString> cFiles;
    QSet<QString> cppFiles;

    QStringList names;
    names << "main" << "graphmv" << "tpodata" << "graph" << "memman";
    foreach (const Predicate *predicate, predicates)
        names.append(predicate->name);
    foreach (const Actor *actor, actors)
        names.append(actor->name);
    foreach (const BaseModule *baseModule, baseModules)
        names.append(baseModule->uniqName);
    foreach (QString name, names) {
        if (cppFiles.contains(name) || cFiles.contains(name))
            continue;
        objects.append(target + "/" + name + ".o");
        sources.append(name + ".cpp");
//        targets.append(target + "/" + name + ".o:\r\n\t$(CXX) -c $(CXXFLAGS) $(INCPATH) -o " + target + QDir::separator() + name + ".o " + name + ".cpp" );
        targets.append(target + "/" + name + ".o:\r\n\t$(CXX) -c $(CXXFLAGS) $(INCPATH) -o " + target + "/" + name + ".o " + name + ".cpp" );
        cppFiles.insert(name);
    }
    foreach (const BaseModule *baseModule, baseModules) {
        if (cFiles.contains(baseModule->name))
            continue;
        objects.append(target + "/" + baseModule->name + ".o");
        sources.append(baseModule->name + ".c");
//        targets.append(target + "/" + baseModule->name + ".o:\r\n\t$(CC) -c $(CFLAGS) $(INCPATH) -o " + target + QDir::separator() + baseModule->name + ".o " + baseModule->name + ".c" );
        targets.append(target + "/" + baseModule->name + ".o:\r\n\t$(CC) -c $(CFLAGS) $(INCPATH) -o " + target + "/" + baseModule->name + ".o " + baseModule->name + ".c" );
        cFiles.insert(baseModule->name);
    }

    foreach (QString  name, myOtherFilesList) {
        if (cppFiles.contains(name) || cFiles.contains(name))
            continue;
        if (name.endsWith(".c")) {
            name = name.left(name.lastIndexOf(".c"));
            objects.append(target + "/" + name + ".o");
//            targets.append(target + "/" + name + ".o:\r\n\t$(CC) -c $(CFLAGS) $(INCPATH) -o " + target + QDir::separator() + name + ".o " + name + ".c" );
            targets.append(target + "/" + name + ".o:\r\n\t$(CC) -c $(CFLAGS) $(INCPATH) -o " + target + "/" + name + ".o " + name + ".c" );
            cFiles.insert(name);
        } else if (name.endsWith(".cpp")) {
            name = name.left(name.lastIndexOf(".cpp"));
            objects.append(target + "/" + name + ".o");
//            targets.append(target + "/" + name + ".o:\r\n\t$(CXX) -c $(CXXFLAGS) $(INCPATH) -o " + target + QDir::separator() + name + ".o " + name + ".cpp" );
            targets.append(target + "/" + name + ".o:\r\n\t$(CXX) -c $(CXXFLAGS) $(INCPATH) -o " + target + "/" + name + ".o " + name + ".cpp" );
            cppFiles.insert(name);
        }
    }

    QString result = getTemplate(myTemplateDirectory + "/" + "Makefile." + target + ".template");
    if (result.isEmpty())
        globalLogger->log(QObject::tr(ERR_GRAPHCOMPI_EMPTY_TEMPL).arg("Makefile." + target), Logger::Warning);
    result.replace("<#dateTime>", QDateTime::currentDateTime().toString());
    result.replace("<#project>", "Project" + QString::number(globalDBManager->getProjectId()));
    result.replace("<#objects>", objects.join(" \\\r\n\t\t"));
    result.replace("<#sources>", sources.join(" \\\r\n\t\t"));
    result.replace("<#targets>", targets.join("\r\n\r\n"));

    QFile f(myOutputDirectory + "/" + "Makefile." + target);
    f.open(QFile::WriteOnly);
    f.write(result.toUtf8());
    f.close();
}

void SourceCompiler::compileMain(const QString graphName, const QStringList &actors) const
{
    QString result = getTemplate(myTemplateDirectory + "/" + "main.cpp.template");
    if (result.isEmpty())
        globalLogger->log(QObject::tr(ERR_GRAPHCOMPI_EMPTY_TEMPL).arg("main.cpp"), Logger::Warning);
    QStringList allActors;
    QStringList getFuncAddrByName;
    foreach (const QString name, actors) {
        allActors.append("int " + name + "(TPOData *D);");
        getFuncAddrByName.append(QString("if (strcmp(\"%1\", name) == 0) return &%1;").arg(name));
    }

    result.replace("<#getFuncAddrByName>", getFuncAddrByName.join("\r\n"));
    result.replace("<#allActors>", allActors.join("\r\n"));
    result.replace("<#graphname>",graphName);

    QFile f(myOutputDirectory + "/" + "main.cpp");
    f.open(QFile::WriteOnly);
    f.write(result.toUtf8());
    f.close();
}

QStringList SourceCompiler::compileCode(const QList<GraphStruct> &graphStructs) {
    QStringList errors;
    QSet<QString> allActorNames;
    QSet<const Actor *> allActors;
    QSet<const Predicate *> allPredicates;
    QSet<const BaseModule *> allBasemodules;
    foreach (GraphStruct gs, graphStructs) {
        errors.append(compileStruct(gs));
        allActorNames.unite(QSet<QString>::fromList(gs.entries.keys()));
        allActors.unite(QSet<const Actor *>::fromList(gs.usedActors));
        allPredicates.unite(QSet<const Predicate *>::fromList(gs.usedPredicates));
        allBasemodules.unite(QSet<const BaseModule *>::fromList(gs.usedBasemodules));
    }

    compileMain(graphStructs.last().namepr, QStringList::fromSet(allActorNames));

    const QList<const Actor *> allActorsList = allActors.toList();
    const QList<const Predicate *> allPredicatesList = allPredicates.toList();
    const QList<const BaseModule *> allBasemoduleList = allBasemodules.toList();
    compileMakefile("debug", allActorsList, allPredicatesList, allBasemoduleList);
    compileMakefile("release", allActorsList, allPredicatesList, allBasemoduleList);
    copyUsedFiles(allActorsList, allPredicatesList, allBasemoduleList);
    compileEnvironment();
    copyStaticTemplates();
    return errors;
}

QStringList SourceCompiler::compileStruct(const GraphStruct &graphStruct)
{
    QStringList errors;

    //делаем заголовки для предикатов
    QString predicateStr;
    foreach (const Predicate *predicate, graphStruct.usedPredicates)
        predicateStr.append("int " + predicate->name + "(TPOData *D);\r\n");

    // делаем заголовки файл для акторов
    QString actorStr;
    foreach (const Actor *actor, graphStruct.usedActors)
        actorStr.append("int " + actor->name + "(TPOData *D);\r\n");

    // список _ListP
    QString _ListP;
    _ListP.append("static DefinePredicate ListPred[" + QString::number(graphStruct.predicatesTable.size()) + "] = {\r\n");
    QStringList list;
    foreach (const DefinePredicate dp, graphStruct.predicatesTable)
        list << "\tDefinePredicate(\"" + dp.namepr + "\", &" + dp.namepr + ")";
    _ListP.append(list.join(",\r\n"));
    _ListP.append("\r\n};\r\n");

    QString _ListGraph;
    _ListGraph.append("static DefineGraph ListGraf[" + QString::number(graphStruct.graphsTable.size()) + "] = {\r\n");
    QStringList listGraph;
    foreach (const DefineGraph dg, graphStruct.graphsTable)
        listGraph <<"\tDefineGraph(" + QString::number(dg.predicateIndex) + ", " + QString::number(dg.topIndex) + ", " + QString::number(dg.type) + ")";
    _ListGraph.append(listGraph.join(",\r\n"));
    _ListGraph.append("\r\n};\r\n");

    QStringList listTop;
    foreach (DefineTop dt, graphStruct.topsTable)
        listTop.append(dt.actor.isEmpty()
                       ? "\tDefineTop(\"GHOST TOP\", -77, -77, NULL)"
                       : "\tDefineTop(\"" + dt.actor + "\", " + QString::number(dt.firstIndex) + ", " + QString::number(dt.lastIndex) + ", &" + dt.actor + ")");
    QString _ListT;
    _ListT.append("static DefineTop ListTop[" + QString::number(listTop.size()) + "] = {\r\n");
    _ListT.append(listTop.join(",\r\n"));
    _ListT.append("\r\n};\r\n");

    QString main;
    main.append("#include \"graph.h\"\r\n");
    main.append("PROJECT_BEGIN_NAMESPACE\r\n");
    main.append(predicateStr);
    main.append(actorStr);

    main.append(_ListP);
    main.append(_ListT);
    main.append(_ListGraph);

    QList<QString> keys = graphStruct.entries.keys();
    foreach (QString key, keys)
        main.append(buildGraph(key, "", graphStruct.entries[key]));
    main.append("PROJECT_END_NAMESPACE\r\n");

    QFile f(myOutputDirectory + "/" + graphStruct.namepr + ".cpp");
    f.open(QFile::WriteOnly);
    f.write(main.toUtf8());
    f.close();

    return errors;
}

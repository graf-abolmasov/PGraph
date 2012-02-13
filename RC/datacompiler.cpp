#include "datacompiler.h"
#include "datatype.h"
#include "variable.h"
#include "globalvariables.h"
#include <QtCore>
#include <QtGui>
#include "qgraphsettings.h"

DataCompiler::DataCompiler(Type type) :
    myType(type)
{
    myOutputDirectory = QGraphSettings::getOutputDirectory();
    myTemplateDirectory = QGraphSettings::getTemplateDirectory();

    //Предопределенные типы MPI
    mpiTypes["int"]    = "MPI_INT";
    mpiTypes["long"]   = "MPI_LONG";
    mpiTypes["char"]   = "MPI_CHAR";
    mpiTypes["byte"]   = "MPI_BYTE";
    mpiTypes["float"]  = "MPI_FLOAT";
    mpiTypes["double"] = "MPI_DOUBLE";
}

bool DataCompiler::compile()
{
    QTime t;
    t.start();
    globalLogger->log(QObject::tr("Компиляция данных....."), Logger::Compile);
    compileUserTypes();
    compileTpoData();
    globalLogger->log(QObject::tr("Компиляция завершена без ошибок за %1 с").arg(QString::number(qRound(t.elapsed()/1000))), Logger::Compile);
    return true;
}

void DataCompiler::compileSimpleTpoData()
{
    QList<const Variable *> varList;
    varList = globalDBManager->getVariableList();

    QString poDataText = getTemplate(myTemplateDirectory + "/" + QString(PODATA_FILE_NAME) + ".h.template");
    if (poDataText.isEmpty())
        globalLogger->log(QObject::tr(ERR_DATACOMPI_EMPTY_TEMPL).arg(QString(PODATA_FILE_NAME) + ".h"), Logger::Warning);

    QString varPtrBlock;
    foreach (const Variable *var, varList)
        varPtrBlock.append(var->type->name + " *" + var->name + ";\n");
    poDataText.replace("<#varPtr>", varPtrBlock);

    //Сохраняем файл
    QFile poDataH(myOutputDirectory + "/" + PODATA_FILE_NAME + ".h");
    poDataH.open(QFile::WriteOnly);
    poDataH.write(poDataText.toUtf8());
    poDataH.close();

    //Создаем файл TPOData.cpp
    poDataText = getTemplate(myTemplateDirectory + "/" + QString(PODATA_FILE_NAME) + ".cpp.template");
    if (poDataText.isEmpty())
        globalLogger->log(QObject::tr(ERR_DATACOMPI_EMPTY_TEMPL).arg(QString(PODATA_FILE_NAME) + ".cpp"), Logger::Warning);

    //Шаблон <#delete> В деструкторе очищает память
    QString deleteBlock;

    //Шаблон <#initMemory> В функции initMemory() выделяет память для rank = 0, для остальных = NULL
    QString initMemoryBlock;

    foreach (const Variable* var, varList) {
        deleteBlock.append("delete " + var->name + ";\n");
        initMemoryBlock.append("*(" + var->name + " = new " + var->type->name + ")" + (var->initValue.isEmpty() ? ";\n" : " = " + var->initValue + ";\n"));
    }

    poDataText.replace("<#delete>", deleteBlock);
    poDataText.replace("<#initMemory>", initMemoryBlock);

    //Сохраняем файл
    QFile poDataCpp(myOutputDirectory + "/" + PODATA_FILE_NAME + ".cpp");
    poDataCpp.open(QFile::WriteOnly);
    poDataCpp.write(poDataText.toUtf8());
    poDataCpp.close();
}

void DataCompiler::compileTpoData()
{
    //Получаем (пока) все переменные ПО
    QList<const Variable *> varList;
    varList = globalDBManager->getVariableList();

    //Создаем файл TPOData.h
    QString poDataText = getTemplate(myTemplateDirectory + "/" + QString(PODATA_FILE_NAME) + ".h.template");
    if (poDataText.isEmpty())
        globalLogger->log(QObject::tr(ERR_DATACOMPI_EMPTY_TEMPL).arg(QString(PODATA_FILE_NAME) + ".h"), Logger::Warning);

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
    if (poDataText.isEmpty())
        globalLogger->log(QObject::tr(ERR_DATACOMPI_EMPTY_TEMPL).arg(QString(PODATA_FILE_NAME) + ".cpp"), Logger::Warning);

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
                                  ? QString("%1 = (%2*)(new %3)").arg(vn).arg(tn).arg(vn)
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
}

void DataCompiler::compileUserTypes()
{
    //Получаем (пока) все типы ПО
    QList<const DataType *> myTypeList;
    myTypeList = globalDBManager->getDataTypeList();

    QString utypeText = getTemplate(myTemplateDirectory + "/" + QString(USER_TYPES_FILE_NAME) + ".h.template");
    if (utypeText.isEmpty())
        globalLogger->log(QObject::tr(ERR_DATACOMPI_EMPTY_TEMPL).arg(QString(USER_TYPES_FILE_NAME) + ".cpp"), Logger::Warning);

    //Создаем типы
    QString userTypesBlock;
    foreach (const DataType *type, myTypeList)
        userTypesBlock.append(type->build());
    utypeText.replace("<#userTypes>", userTypesBlock);

    //Создаем типы MPI
    //TODO: ДЛЯ СТРУКТУР ПОКА НИЧЕГО НЕТ
    QString mpiTypesBlock;
    foreach (const DataType *type, myTypeList) {
        if (type->typedefStr.isEmpty() || type->isStruct())
            continue;
        QRegExp r("(typedef\\s+(\\b.+\\b))");
        r.setMinimal(true);
//        Q_ASSERT(r.indexIn(type->typedefStr) != -1);
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
}


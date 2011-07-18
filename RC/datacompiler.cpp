#include "datacompiler.h"
#include "datatype.h"
#include "variable.h"
#include "globalvariables.h"
#include <QtCore>
#include <QtGui>

DataCompiler::DataCompiler()
{

}

QString DataCompiler::getTemplate(const QString &fileName) const
{
    QSettings settings("graph.ini", QSettings::IniFormat);
    QString baseDir = settings.value("Location/TemplateDir", QApplication::applicationDirPath() + "\\BaseDir\\").toString();
    QString name = baseDir + "/" + fileName;
    QFile f(name);
    f.open(QIODevice::ReadOnly);
    QTextStream ts(&f);
    ts.setCodec("UTF-8");
    QString result = ts.readAll();
    f.close();
    return result;
}

void DataCompiler::init()
{
    QSettings settings("graph.ini", QSettings::IniFormat);
    myOutputDirectory = settings.value("Location/OutputDir", QApplication::applicationDirPath() + "\\BaseDir\\").toString();
}

void DataCompiler::compile()
{
    //Получаем (пока) все типы ПО
    QList<const DataType *> myTypeList;
    myTypeList = globalDBManager->getDataTypeList();

    //Получаем (пока) все переменные ПО
    QList<const Variable *> varList;
    varList = globalDBManager->getVariableList();

    //Предопределенные типы MPI
    QMap<QString, QString> mpiTypes;
    mpiTypes["int"]    = "MPI_INT";
    mpiTypes["long"]   = "MPI_LONG";
    mpiTypes["char"]   = "MPI_CHAR";
    mpiTypes["byte"]   = "MPI_BYTE";
    mpiTypes["float"]  = "MPI_FLOAT";
    mpiTypes["double"] = "MPI_DOUBLE";

    //Создаем файл utype.h
    {
        QString utypeText = getTemplate(QString(USER_TYPES_FILE_NAME) + ".h.template");
        Q_ASSERT(!utypeText.isEmpty());

        //Создаем типы
        QString userTypesBlock;

        foreach (const DataType *type, myTypeList) {
            if (!type->typedefStr.isEmpty()) {
                userTypesBlock.append(type->typedefStr + "\n");

                //Если встертился массив
                if (type->typedefStr.contains("[")) {
                    //Парсим количество элементов
                    QRegExp r("(\\[(\\d+)\\])");
                    r.setMinimal(true);
                    Q_ASSERT(r.indexIn(type->typedefStr) != -1);
                    userTypesBlock.append("#define " + type->name + "_LENGTH " + r.cap(2) + "\n");

                    //Парсим тип элемента
                    r.setPattern("(typedef\\s+(\\b.+\\b))");
                    Q_ASSERT(r.indexIn(type->typedefStr) != -1);
                    userTypesBlock.append("typedef " + r.cap(2) + "* ptr" + type->name  + ";\n");
                }
                userTypesBlock.append("\n");
            }
        }

        utypeText.replace("<#userTypes>", userTypesBlock);

        //Создаем типы MPI
        QString mpiTypesBlock;
        foreach (const DataType *type, myTypeList) {
            if (!type->typedefStr.isEmpty()) {
                QRegExp r("(typedef\\s+(\\b.+\\b))");
                r.setMinimal(true);
                Q_ASSERT(r.indexIn(type->typedefStr) != -1);
                QString intType = r.cap(2);
                mpiTypesBlock.append("#define MPI_USER_TYPE_" + type->name.toUpper() + " " + (mpiTypes.contains(intType) ? mpiTypes[r.cap(2)] : "MPI_USER_TYPE_" + r.cap(2).toUpper()) + "\n");
            }
        }

        utypeText.replace("<#mpiTypes>", mpiTypesBlock);

        //Сохраняем файл
        QFile userTypes(myOutputDirectory + "/" + USER_TYPES_FILE_NAME + ".h");
        userTypes.open(QFile::WriteOnly);
        userTypes.write(utypeText.toUtf8());
        userTypes.close();
    }

    //Компиляруем класс POData
    {
        //Создаем файл podata.h
        QString poDataText = getTemplate(QString(PODATA_FILE_NAME) + ".h.template");
        Q_ASSERT(!poDataText.isEmpty());

        //Заполняем блок Id
        QString varIdBlock;
        for (int i = 0; i < varList.count(); i++)
            varIdBlock.append("int " + varList[i]->name + "_id;\n");
        poDataText.replace("<#varId>", varIdBlock);

        //Заполняем блок указателей на данные
        QString varPtrBlock;
        foreach (const Variable *var, varList)
            varPtrBlock.append(var->type->name + " *_" + var->name + ";\n");
        poDataText.replace("<#varPtr>", varPtrBlock);

        //Заполняем блок свойств
        QString varPropertyBlock;

        //Заполняем блок setter/getter
        QString setGetBlock;
        foreach (const Variable *var, varList) {
            const DataType* type = var->type;
            Q_ASSERT(type != NULL);
            //Если встертился массив
            if (type->typedefStr.contains("[")) {
                //Парсим тип элемента
                QRegExp r("(typedef\\s+(\\b.+\\b))");
                r.setMinimal(true);
                Q_ASSERT(r.indexIn(type->typedefStr) != -1);
                QString intType = r.cap(2);

                setGetBlock.append("ptr" + var->type->name + " get_" + var->name + "();\n");
                setGetBlock.append("ptr" + var->type->name + " set_" + var->name + "(const ptr" + var->type->name + "& value);\n");
                setGetBlock.append(intType + " get_" + var->name + "(const int& index);\n");
                setGetBlock.append(intType + " set_" + var->name + "(const int& index, const " + intType + "& value);\n");
                varPropertyBlock.append("__property_rw_indexed<" + intType + ", int, POData> " + var->name + ";\n");
            } else {
                setGetBlock.append(var->type->name + " get_" + var->name + "();\n");
                setGetBlock.append(var->type->name + " set_" + var->name + "(const " + var->type->name + "& value);\n");
                varPropertyBlock.append("__property_rw<" + var->type->name + ", POData> " + var->name + ";\n");
            }
        }
        poDataText.replace("<#setGet>", setGetBlock);
        poDataText.replace("<#varProperty>", varPropertyBlock);

        //Сохраняем файл
        QFile poDataH(myOutputDirectory + "/" + PODATA_FILE_NAME + ".h");
        poDataH.open(QFile::WriteOnly);
        poDataH.write(poDataText.toUtf8());
        poDataH.close();

        //Создаем файл podata.cpp
        poDataText = getTemplate(QString(PODATA_FILE_NAME) + ".cpp.template");

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
            QString intType = var->type->name;
            if (!type->typedefStr.isEmpty()) {
                //Парсим тип элемента
                QRegExp r("(typedef\\s+(\\b.+\\b))");
                r.setMinimal(true);
                Q_ASSERT(r.indexIn(type->typedefStr) != -1);
                intType = r.cap(2);
            }
            //Общее для всех типов
            initIdBlock.append(var->name + "_id = " + QString::number(i) + ";\n");
            getDataAddrBlock.append("case " + QString::number(i) + ":\n"
                                    "\tresult = _" + var->name + ";\n"
                                    "\tbreak;\n");
            getMpiTypeBlock.append("case " + QString::number(i) + ":\n"
                                    "\tresult = " + (mpiTypes.contains(var->type->name) ? mpiTypes[var->type->name] : "MPI_USER_TYPE_" + var->type->name.toUpper()) + ";\n"
                                    "\tbreak;\n");
            //Если встертился массив
            if (type->typedefStr.contains("[")) {
                assignSetterGetterBlock.append(var->name + ".Assign(this, &POData::set_" + var->name + ", &POData::get_" + var->name + ", &POData::set_" + var->name + ", &POData::get_" + var->name + ");\n");
                deleteBlock.append("delete[] _" + var->name + ";\n");
                initMemoryBlock.append("_" + var->name + " = (" + var->type->name + "*)" + "(new " + var->type->name + ");\n");
                getDataSizeBlock.append("case " + QString::number(i++) + ":\n"
                                        "\tresult = sizeof(" + var->type->name + ") / " + var->type->name + "_LENGTH;\n"
                                        "\tbreak;\n");
                //getter by index
                setGetBlock.append(intType + " POData::get_" + var->name + "(const int& index)\n{\n"
                                   "\tif (_" + var->name + " == NULL) _" + var->name + " = (" + var->type->name + "*)" + "(new " + var->type->name + ");\n" +
                                   "\tgetData(" + var->name + "_id, index, 1, &(*_" + var->name + ")[index]);\n"
                                   "\treturn (*_" + var->name + ")[index];\n}\n\n");
                //setter by index
                setGetBlock.append(intType + " POData::set_" + var->name + "(const int& index, const " + intType + " &value)\n{\n"
                                   "\tsetData(" + var->name + "_id, index, 1, &value);\n"
                                   "\treturn value;\n}\n\n");
                //getter all array
                setGetBlock.append("ptr" + var->type->name + " POData::get_" + var->name + "()\n{\n"
                                   "\tif (_" + var->name + " == NULL) _" + var->name + " = (" + var->type->name + "*)" + "(new " + var->type->name + ");\n" +
                                   "\tgetData(" + var->name + "_id, 0, " + var->type->name + "_LENGTH" + ", _" + var->name +");\n"
                                   "\treturn *_" + var->name + ";\n}\n\n");
                //setter all array
                setGetBlock.append("ptr" + var->type->name + " POData::set_" + var->name + "(const " + "ptr" + var->type->name + " &value)\n{\n"
                                   "\tsetData(" + var->name + "_id, 0, " + var->type->name + "_LENGTH" ", *(&value));\n"
                                   "\treturn (" + intType + " *)value;\n}\n\n");
            } else {
                assignSetterGetterBlock.append(var->name + ".Assign(this, &POData::set_" + var->name + ", &POData::get_" + var->name + ");\n");
                deleteBlock.append("delete _" + var->name + ";\n");
                initMemoryBlock.append("*(_" + var->name + " = new " + var->type->name + ")" + (var->initValue.isEmpty() ? ";\n" : " = " + var->initValue + ";\n"));
                getDataSizeBlock.append("case " + QString::number(i++) + ":\n"
                                        "\tresult = sizeof(" + var->type->name + ");\n"
                                        "\tbreak;\n");
                setGetBlock.append(var->type->name + " POData::get_" + var->name + "()\n{\n"
                                   "\tif (_" + var->name + " == NULL) *(_" + var->name + " = new " + var->type->name + ")" + (var->initValue.isEmpty() ? ";\n" : " = " + var->initValue + ";\n") +
                                   "\tgetData(" + var->name + "_id, 0, 1, _" + var->name +");\n"
                                   "\treturn *_" + var->name + ";\n}\n\n");
                setGetBlock.append(var->type->name + " POData::set_" + var->name + "(const " + var->type->name + " &value)\n{\n"
                                   "\tsetData(" + var->name + "_id, 0, 1, &value);\n"
                                   "\treturn value;\n}\n\n");
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

        foreach (const Variable* var, varList)
            initMemoryBlock.append("_" + var->name + " = NULL;\n");
        initMemoryBlock.append("}\n");
        poDataText.replace("<#initMemory>", initMemoryBlock);

        //Сохраняем файл
        QFile poDataCpp(myOutputDirectory + "/" + PODATA_FILE_NAME + ".cpp");
        poDataCpp.open(QFile::WriteOnly);
        poDataCpp.write(poDataText.toUtf8());
        poDataCpp.close();
    }
}

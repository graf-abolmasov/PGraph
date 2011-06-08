#include "compiler.h"
#include "datatype.h"
#include "variable.h"
#include "globalvariables.h"
#include <QtCore>
#include <QtGui>

Compiler::Compiler()
{
    QSettings settings("graph.ini", QSettings::IniFormat);
    myOutputDirectory = settings.value("Compi/outDir", QApplication::applicationDirPath() + "\\BaseDir\\").toString();
}

QString Compiler::getTemplate(const QString &type) const
{
    return QString();
}

void Compiler::compileData() const
{
    //Получаем (пока) все типы ПО
    QList<DataType *> typeList;
    globalDBManager->getDataTypeList(typeList);

    //Получаем (пока) все переменные ПО
    QList<Variable *> varList;
    globalDBManager->getVariableList(varList);

    //Предопределенные типы MPI
    QMap<QString, QString> mpiTypes;
    mpiTypes["int"] = "MPI_INT";
    mpiTypes["long"] = "MPI_LONG";
    mpiTypes["char"] = "MPI_CHAR";
    mpiTypes["float"] = "MPI_FLOAT";
    mpiTypes["double"] = "MPI_DOUBLE";
    mpiTypes["byte"] = "MPI_BYTE";

    //Создаем файл utype.h
    {
        QString utypeText = getTemplate(QString(USER_TYPES_FILE_NAME) + ".h.template");

        //Создаем типы
        QString userTypesBlock;

        foreach (DataType *type, typeList) {
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
                userTypesBlock.append("typedef " + type->name + "* ptr" + r.cap(2) + ";\n");
            }
            userTypesBlock.append("\n");
        }

        utypeText.replace("<#userTypes>", userTypesBlock);

        //Создаем типы MPI
        QString mpiTypesBlock;
        foreach (DataType *type, typeList) {
            QRegExp r("(typedef\\s+(\\b.+\\b))");
            Q_ASSERT(r.indexIn(type->typedefStr) != -1);
            mpiTypesBlock.append("#define MPI_USER_TYPE_" + type->name.toUpper() + " " + (mpiTypes.contains(r.cap(2)) ? mpiTypes[r.cap(2)] : "MPI_USER_TYPE_" + r.cap(2).toUpper()) + "\n");
        }

        utypeText.replace("<#mpiTypes>", mpiTypesBlock);

        //Сохраняем файл
        QFile userTypes(myOutputDirectory + "\\" + USER_TYPES_FILE_NAME + ".h");
        userTypes.open(QFile::WriteOnly);
        userTypes.write(utypeText.toUtf8());
        userTypes.close();
    }

    //Компиляруем класс POData
    {
        //Создаем файл podata.h
        QString poDataText = getTemplate(QString(PODATA_FILE_NAME) + ".h.template");

        //Заполняем блок Id
        QString varIdBlock;
        for (int i = 0; i < varList.count(); i++)
            varIdBlock.append("int " + varList[i]->name + "_id;\n");
        poDataText.replace("<#varId>", varIdBlock);

        //Заполняем блок указателей на данные
        QString varPtrBlock;
        foreach (Variable *var, varList)
            varPtrBlock.append(var->type + " *_" + var->name);
        poDataText.replace("<#varPtr>", varPtrBlock);

        //Заполняем блое свойств
        QString varPropertyBlock;
        foreach (Variable *var, varList)
            varPropertyBlock.append("__property_rw<" + var->type + ", POData> " + var->name);
        poDataText.replace("<#varProperty>", varPropertyBlock);

        //Заполняем блок setter/getter
        QString setGetBlock;
        foreach (Variable *var, varList) {
            int idx = typeList.indexOf(new DataType(var->type, ""));
            Q_ASSERT(idx != -1);
            DataType* type = typeList[idx];
            //Если встертился массив
            if (type->typedefStr.contains("[")) {

                //Парсим тип элемента
                QRegExp r("(typedef\\s+(\\b.+\\b))");
                Q_ASSERT(r.indexIn(type->typedefStr) != -1);
                QString intType = r.cap(2);

                setGetBlock.append("ptr" + var->type + " get_" + var->name + "();\n");
                setGetBlock.append("ptr" + var->type + " set_" + var->name + "(const ptr" + var->type + "& value);\n");
                setGetBlock.append(intType + " get_" + var->name + "(const int& index);\n");
                setGetBlock.append(intType + " set_" + var->name + "(const int& index, const " + intType + "& value);\n");
            } else {
                setGetBlock.append(var->type + " get_" + var->name + "();\n");
                setGetBlock.append(var->type + " set_" + var->name + "(const ptr" + var->type + "& value);\n");
            }
        }
        poDataText.replace("<#setGet>", setGetBlock);

        //Сохраняем файл
        QFile userTypes(myOutputDirectory + "\\" + PODATA_FILE_NAME + ".h");
        userTypes.open(QFile::WriteOnly);
        userTypes.write(poDataText.toUtf8());
        userTypes.close();

        //Создаем файл podata.cpp
        poDataText = getTemplate(QString(PODATA_FILE_NAME) + ".cpp.template");

        QString assignSetterGetterBlock;

        QString deleteBlock;

        QString initMemoryBlock;
        initMemoryBlock.append("if (myRank == 0) {\n");

        QString initIdBlock;

        QString getDataAddrBlock;
        getDataAddrBlock.append("void* result = NULL;\n"
                                "switch (id) {\n");

        QString getDataSizeBlock;
        getDataSizeBlock.append("int result = 0;\n"
                                "switch (id) {\n");

        QString getMpiTypeBlock;
        getMpiTypeBlock.append("MPI_Datatype result = MPI_INT;\n"
                                "switch (id) {\n");

        setGetBlock.clear();

        int i = 0;
        foreach (Variable* var, varList) {
            int idx = typeList.indexOf(new DataType(var->type, ""));
            Q_ASSERT(idx != -1);
            DataType* type = typeList[idx];
            //Парсим тип элемента
            QRegExp r("(typedef\\s+(\\b.+\\b))");
            Q_ASSERT(r.indexIn(type->typedefStr) != -1);
            QString intType = r.cap(2);
            //Общее для всех типов
            initIdBlock.append("_" + var->name + " = " + QString::number(i) + ";\n");
            getDataAddrBlock.append("case " + QString::number(i) + ":\n"
                                    "\tresult = _" + var->name + ";\n"
                                    "\tbreak;\n");
            getMpiTypeBlock.append("case " + QString::number(i) + ":\n"
                                    "\tresult = " + (mpiTypes.contains(var->type) ? mpiTypes[var->type] : "MPI_USER_TYPE_" + var->type.toUpper()) + ";\n"
                                    "\tbreak;\n");
            //Если встертился массив
            if (type->typedefStr.contains("[")) {
                assignSetterGetterBlock.append(var->name + ".Assign(this, &POData::set_" + var->name + ", &POData::get_" + var->name + ", &POData::set_" + var->name + ", &POData::get_" + var->name + ");\n");
                deleteBlock.append("delete[] " + var->name + ";\n");
                initMemoryBlock.append("_" + var->name + " = (" + var->type + "*)" + "(new " + var->type + ");\n");
                getDataSizeBlock.append("case " + QString::number(i) + ":\n"
                                        "\tresult = sizeof(" + var->type + ") / " + var->type + "_LENGTH;\n"
                                        "\tbreak;\n");
                setGetBlock.append(intType + " POData::get_" + var->name + "(const int& index)\n{\n"
                                   "\tif (_" + var->name + " == NULL) _" + var->name + " = (" + var->type + "*)" + "(new " + var->type + ");\n" +
                                   "\tgetData(" + var->name + "_id, index, 1, &(*_" + var->name + ")[index]);\n"
                                   "\treturn *_" + var->name + ";\n}\n\n");
                setGetBlock.append(intType + " POData::set_" + var->name + "(const int& index, const" + intType + " &value)\n{\n"
                                   "\tsetData(" + var->name + "_id, index, 1, &value);\n"
                                   "\treturn value;\n}\n\n");
                setGetBlock.append("ptr" + var->type + " POData::get_" + var->name + "()\n{\n"
                                   "\tif (_" + var->name + " == NULL) _" + var->name + " = (" + var->type + "*)" + "(new " + var->type + ");\n" +
                                   "\tgetData(" + var->name + "_id, 0, " + var->type + "_LENGTH" + ", _" + var->name +");\n"
                                   "\treturn *_" + var->name + ";\n}\n\n");
                setGetBlock.append("ptr" + var->type + " POData::set_" + var->name + "(const " + "ptr" + var->type + " &value)\n{\n"
                                   "\tsetData(" + var->name + "_id, 0, " + var->type + "_LENGTH" ", *(&value));\n"
                                   "\treturn (" + intType + " *)value;\n}\n\n");
            } else {
                assignSetterGetterBlock.append(var->name + ".Assign(this, &POData::set_" + var->name + ", &POData::get_" + var->name + ");\n");
                deleteBlock.append("delete " + var->name + ";\n");
                initMemoryBlock.append("*(_" + var->name + " = new " + var->type + ")" + (var->initValue.isEmpty() ? ";\n" : " = " + var->initValue + ";\n"));
                getDataSizeBlock.append("case " + QString::number(i) + ":\n"
                                        "\tresult = sizeof(" + var->type + ");\n"
                                        "\tbreak;\n");
                setGetBlock.append(var->type + " POData::get_" + var->name + "()\n{\n"
                                   "\tif (_" + var->name + " == NULL) *(_" + var->name + " = new " + var->type + ")" + (var->initValue.isEmpty() ? ";\n" : " = " + var->initValue + ";\n") +
                                   "\tgetData(" + var->name + "_id, 0, 1, _" + var->name +");\n"
                                   "\treturn *_" + var->name + ";\n}\n\n");
                setGetBlock.append(var->type + " POData::set_" + var->name + "(const " + var->type + " &value)\n{\n"
                                   "\tsetData(" + var->name + "_id, 0, 1, &value);\n"
                                   "\treturn value;\n}\n\n");
            }
        }
        poDataText.replace("<#assignSetterGetter>", assignSetterGetterBlock);
        poDataText.replace("<#delete>", deleteBlock);
        poDataText.replace("<#initId>", initIdBlock);
        poDataText.replace("<#setGetBlock>", setGetBlock);

        initMemoryBlock.append("} else {\n");
        getDataAddrBlock.append("}\nreturn result;\n");
        getDataSizeBlock.append("}\nreturn result;\n");
        getMpiTypeBlock.append("}\nreturn result;\n");
        poDataText.replace("<#getDataAddr>", getDataAddrBlock);
        poDataText.replace("<#getSizeAddr>", getDataSizeBlock);
        poDataText.replace("<#getMpiType>", getMpiTypeBlock);

        foreach (Variable* var, varList)
            initMemoryBlock.append("_" + var->name + " = NULL;\n");
        initMemoryBlock.append("}\n");
        poDataText.replace("<#initMemory>", initMemoryBlock);

    }


}

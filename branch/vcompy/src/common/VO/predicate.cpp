#include "predicate.h"
#include "../../src/common/errors.h"
#include "../../src/common/qgraphsettings.h"


Predicate::Predicate(QString name, QString extName, Type type, const BaseModule *baseModule, QList<const Variable *> variableList)
{
    this->extName = extName;
    this->name = name;
    this->type = type;
    this->variableList = variableList;
    this->baseModule = baseModule;
}

QStringList Predicate::validate() const
{
    QStringList msgs;
    if (type == NormalType && baseModule == NULL)
        msgs << QObject::tr(ERR_PREDICATE_NULL_BASEMODULE).arg(name);
    if (name.isEmpty())
        msgs << QObject::tr(ERR_PREDICATE_EMPTY_NAME).arg(name);
    if (extName.isEmpty())
        msgs << (type == NormalType ? QObject::tr(ERR_PREDICATE_EMPTY_EXTNAME).arg(name) : QObject::tr(ERR_PREDICATE_EMPTY_INLINE).arg(name));
    if (baseModule != NULL && variableList.count() != baseModule->parameterList.count())
        msgs << QObject::tr(ERR_PREDICATE_UNDEF_VAR).arg(name);
    return msgs;
}

void Predicate::build() const
{
    Q_ASSERT(validate().isEmpty());

    const QString myBaseDirectory = QGraphSettings::getBaseDirectory();
    QString outputData;
    QFile output;

    const BaseModule *currentBaseModule = this->baseModule;
    QStringList params;
    QStringList signature;
    // Создаем сpp файл
    outputData.append("#include \"tpodata.h\"\r\n");
    switch(this->type) {
    case Predicate::NormalType:
        Q_ASSERT(currentBaseModule);
        // Генерируем предикат
        foreach (BaseModuleParameter parameter, this->baseModule->parameterList) {
            signature << QString("%1 *%2")
                         .arg(parameter.type)
                         .arg(parameter.name);
        }
        outputData.append("extern int " + this->baseModule->uniqName + "(" + signature.join(", ") + ");\r\n");
        outputData.append("int " + this->name + "(TPOData *D)\r\n");
        outputData.append("{\r\n");
        outputData.append("//" + this->extName + "\r\n");
        // Инициализуем данные
        for (int i = 0; i < this->variableList.count(); i++) {
            BaseModuleParameter parameter = currentBaseModule->parameterList[i];
            outputData.append(this->variableList[i]->isGlobal ? QString("\t%1%2 _%3 = D->%4;\r\n") : QObject::tr("\tconst %1 *_%2 = D->%3;\r\n")
                              .arg(parameter.type)
                              .arg(parameter.name)
                              .arg(this->variableList[i]->name).toUtf8());
        }
        // Вызываем прототип
        outputData.append("\r\n\treturn ");
        outputData.append(this->baseModule->uniqName);
        outputData.append("(");
        for(int i = 0; i < this->variableList.count(); i++) {
            BaseModuleParameter parameter = currentBaseModule->parameterList[i];
            params << ((this->variableList[i]->isGlobal ? QObject::tr("&_") : QObject::tr("_")) + parameter.name);
        }
        outputData.append(params.join(", ").toUtf8());
        outputData.append(");\r\n\r\n}\r\n");
        break;
    case Predicate::InlineType:
        //генерируем с++ файл
        outputData.append("int " + this->name + "(TPOData *D)\r\n");
        outputData.append("{\r\n");
        QString code(this->extName);
        foreach (const Variable *variable, this->variableList) {
            QRegExp r("\\b" + variable->name + "\\b", Qt::CaseSensitive);
            r.setMinimal(true);
            code.replace(r, variable->isGlobal ? "D->" + variable->name : "(*(D->" + variable->name + "))");
        }
        outputData.append("  return (" + code + ");\r\n");
        outputData.append("}\r\n");
        break;
    }

    output.setFileName(myBaseDirectory + "/" + this->name + ".cpp");
    output.open(QFile::WriteOnly);
    output.write(outputData.toUtf8());
    output.close();
}

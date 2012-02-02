#include "actor.h"
#include "basemodule.h"
#include "qgraphsettings.h"
#include "QtCore"
#include "variable.h"
#include "globalvariables.h"

Actor::Actor(const QString &name, const QString &extName, const Type &type, const BaseModule *baseModule, const QList<const Variable *> &variableList, const QStringList &varAccModeList, const QPixmap &icon)
{
    this->extName = extName;
    this->name = name;
    this->type = type;
    this->variableList = variableList;
    this->baseModule = baseModule;
    this->varAccModeList = varAccModeList;
    this->icon = icon;
}

Actor::Actor(const Actor &actor)
{
    this->extName = actor.extName;
    this->name = actor.name;
    this->type = actor.type;
    this->baseModule = actor.baseModule;
    this->varAccModeList = actor.varAccModeList;
    this->icon = actor.icon;
    this->variableList = actor.variableList;
}

Actor::Actor(const QString &name, const QString &extName, const Type &type)
{
    this->name = name;
    this->extName = extName;
    this->type = type;
    this->baseModule = NULL;
}

QStringList Actor::validate() const
{
    QStringList msgs;   
    if (type == NormalType && baseModule == NULL)
        msgs << QObject::tr(ERR_ACTOR_NULL_BASEMODULE).arg(name);
    if (name.isEmpty())
        msgs << QObject::tr(ERR_ACTOR_EMPTY_NAME).arg(name);
    if (extName.isEmpty())
        msgs << (type == Actor::NormalType ? QObject::tr(ERR_ACTOR_EMPTY_EXTNAME).arg(name) : QObject::tr(ERR_ACTOR_EMPTY_INLINE).arg(name));
    if (baseModule != NULL && variableList.count() != baseModule->parameterList.count())
        msgs << QObject::tr(ERR_ACTOR_UNDEF_VAR).arg(name);
    if (variableList.count() != varAccModeList.count())
        msgs << QObject::tr(ERR_ACTOR_UNDEF_VAR_ACC).arg(name);
    return msgs;
}

void Actor::build() const
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
    case Actor::NormalType:
        Q_ASSERT(currentBaseModule);
        // Генерируем актор
        foreach (BaseModuleParameter parameter, this->baseModule->parameterList) {
            const QString constStr = parameter.accessMode == QObject::tr("Исходный") ? QObject::tr("const ") : "";
            signature << QString("%1%2 *%3").arg(constStr)
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
            const QString constStr = parameter.accessMode == QObject::tr("Исходный") ? QObject::tr("const ") : "";
//            outputData.append(QString("\t%1%2 _%3 = D->%4;\r\n").arg(constStr).arg(parameter[0]).arg(parameter[1]).arg(this->variableList[i]->name).toUtf8());
            //для простого TPOData на указателях
            outputData.append(QObject::tr("\t%1%2 *_%3 = D->%4;\r\n")
                              .arg(constStr)
                              .arg(parameter.type)
                              .arg(parameter.name)
                              .arg(this->variableList[i]->name).toUtf8());
        }
        // Вызываем прототип
        outputData.append("\r\n\tint result = ");
        outputData.append(this->baseModule->uniqName);
        outputData.append("(");
        for(int i = 0; i < this->variableList.count(); i++) {
            BaseModuleParameter parameter = currentBaseModule->parameterList[i];
//            params << QObject::tr("&_") + parameter[1];
            //для простого TPOData
            params << QObject::tr("_") + parameter.name;
        }
        outputData.append(params.join(", ").toUtf8());
        outputData.append(");\r\n\r\n");
        // сохраняем данные
        for(int i = 0; i < this->variableList.count(); i++) {
            BaseModuleParameter parameter = currentBaseModule->parameterList[i];
            if (parameter.accessMode != QObject::tr("Исходный"))
                outputData.append(QString(QObject::tr("\tD->%1 = _%2;\n"))
                                  .arg(this->variableList[i]->name)
                                  .arg(parameter.name).toUtf8());
        }
        // выход
        outputData.append("\r\n\r\n\treturn result;\r\n}");
        break;
    case Actor::InlineType:
        //генерируем с++ файл
        outputData.append("int " + this->name + "(TPOData *D)\r\n");
        outputData.append("{\r\n");
        QString code(this->extName);
        foreach (const Variable *variable, this->variableList) {
            QRegExp r("\\b" + variable->name + "\\b", Qt::CaseSensitive);
            r.setMinimal(true);
//            code.replace(r, "D->" + variable->name);
            //для простого TPOData
            code.replace(r, "(*(D->" + variable->name + "))");
        }
        outputData.append("  " + code + "\r\n");
        outputData.append("  return 1;\r\n");
        outputData.append("}\r\n");
        break;
    }

    output.setFileName(myBaseDirectory + "/" + this->name + ".cpp");
    output.open(QFile::WriteOnly);
    output.write(outputData.toUtf8());
    output.close();
}

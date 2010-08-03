#include "toppropertydialog.h"
#include "ui_toppropertydialog.h"
#include "databasemanager.h"

TopPropertyDialog::TopPropertyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TopPropertyDialog)
{
    ui->setupUi(this);
}

TopPropertyDialog::~TopPropertyDialog()
{
    delete ui;
}

void TopPropertyDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void TopPropertyDialog::prepareForm(QTop* top){
    myTop = top;

    globalDBManager->getActorList(myActorList);
    foreach (Actor* actor, myActorList){
        ui->actorsListWidget->addItem(actor->extName);
    }

    if (top->actor != NULL){
        int idx = -1;
        for (int i = 0; i < myActorList.count(); i++)
            if (top->actor->name == myActorList.at(i)->name &&
                top->actor->extName == myActorList.at(i)->extName) {
                idx = i;
                break;
        }
        if (idx != -1) ui->actorsListWidget->setCurrentRow(idx);
        else {
            QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("Вершина использует несуществующий объект"), QMessageBox::Ok).exec();
            top->actor = NULL;
        }
    }

    ui->spnBoxHeight->setProperty("value", top->polygon().boundingRect().height());
    ui->spnBoxWidth->setProperty("value", top->polygon().boundingRect().width());

    float minHeight = top->getMinHeight() > 10 ? top->getMinHeight() : 10;
    float minWidth = top->getMinWidth() > 10 ? top->getMinWidth() : 10;

    ui->spnBoxHeight->setProperty("minimum", minHeight);
    ui->spnBoxWidth->setProperty("minimum", minWidth);
}

void TopPropertyDialog::widthChanged(int w){
    int h = myTop->polygon().boundingRect().height();
    QPolygonF myPolygon;
    myPolygon << QPointF(-w/2, h/2) << QPointF(w/2, h/2)
            << QPointF(w/2,-h/2) << QPointF(-w/2, -h/2)
            << QPointF(-w/2, h/2);
    myTop->setPolygon(myPolygon);
}

void TopPropertyDialog::heightChanged(int h){
    int w = myTop->polygon().boundingRect().width();
    QPolygonF myPolygon;
    myPolygon << QPointF(-w/2, h/2) << QPointF(w/2, h/2)
            << QPointF(w/2,-h/2) << QPointF(-w/2, -h/2)
            << QPointF(-w/2, h/2);
    myTop->setPolygon(myPolygon);
}

QTop* TopPropertyDialog::getResult(){
    QPolygonF myPolygon;
    int w = ui->spnBoxWidth->value();
    int h = ui->spnBoxHeight->value();
    myPolygon << QPointF(-w/2, h/2) << QPointF(w/2, h/2)
            << QPointF(w/2,-h/2) << QPointF(-w/2, -h/2)
            << QPointF(-w/2, h/2);
    myTop->setPolygon(myPolygon);
    myTop->actor = myActorList.at(ui->actorsListWidget->currentRow());
    return myTop;
}

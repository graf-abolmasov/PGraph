#include "toppropertydialog.h"
#include "ui_toppropertydialog.h"

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


void TopPropertyDialog::prepareForm(TTop* top){
    myTop = top;

    ui->spnBoxHeight->setProperty("value", top->polygon().boundingRect().height());
    ui->spnBoxWidth->setProperty("value", top->polygon().boundingRect().width());

    float minHeight = top->getMinHeight() > 10 ? top->getMinHeight() : 10;
    float minWidth = top->getMinWidth() > 10 ? top->getMinWidth() : 10;

    ui->spnBoxHeight->setProperty("minimum", minHeight);
    ui->spnBoxWidth->setProperty("minimum", minWidth);
}

int TopPropertyDialog::getWidth(){
    return ui->spnBoxWidth->value();
};

int TopPropertyDialog::getHeight(){
    return ui->spnBoxHeight->value();
};

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


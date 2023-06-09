#include "menubuyer.h"
#include "ui_menubuyer.h"
#include "elementpeople.h"

MenuBuyer::MenuBuyer(BuyerModel* buyerModel, QWidget *parent) :
    QWidget(parent),
    buyerModel(buyerModel),
    ui(new Ui::MenuBuyer)
{
    ui->setupUi(this);
    ui->scrollAreaWidgetContents->setStyleSheet("background-color: transparent;");

    update();
}

void MenuBuyer::update()
{
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    for (ElementPeople* elementPeople : buyerModel->updateInfoBuyPeople(this))
        layout->insertWidget(0, elementPeople);

    ui->count->setText(buyerModel->getOneCell("select sum(listP_count) from listproduct"));
    ui->sum->setText(buyerModel->getOneCell("select sum(p_priceOne*listP_count) from product join listproduct using(id_product)"));
}

void MenuBuyer::search(const QString& text)
{
    for (ElementPeople* item : buyerModel->getList())
        if(!item->getTitle().contains(text, Qt::CaseInsensitive))
            item->hide();
        else
            item->show();
}

MenuBuyer::~MenuBuyer()
{
    delete ui;
}

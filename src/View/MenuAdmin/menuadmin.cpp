#include "menuadmin.h"
#include "ui_menuadmin.h"

#include "menueditproduct.h"
#include "productmodel.h"
#include "menueditproductviewmodel.h"
#include "addproductdialog.h"
#include "menuemployees.h"
#include "employeesmodel.h"
#include "addnewemployees.h"
#include "addemployeesviewmodel.h"
#include "suppliermodel.h"
#include "menusupplier.h"
#include "addnewsupplier.h"
#include "formwithbuttonback.h"
#include "menubuyer.h"
#include "buyermodel.h"
#include "menureport.h"
#include "iclose.h"
#include "addcharact.h"

//Класс где происходит контроль всех окон админа
//При желание добавить новое окно в область видимости окна администратора НЕОБХОДИМО вызвать метод freeMemory()

MenuAdmin::MenuAdmin(ConnectionHandler* connectionHandler, QLineEdit* lineSearch, QWidget *parent) :
    connectionHandler(connectionHandler),
    thisWindow(nullptr),
    thisViewModel(nullptr),
    thisModel(nullptr),
    lineSearch(lineSearch),
    QWidget(parent),
    ui(new Ui::MenuAdmin)
{
    ui->setupUi(this);
    connected();

    ui->frameButtonCheckInfo->installEventFilter(this);

    animation = new QPropertyAnimation(ui->frameButtonCheckInfo, "geometry");
    connect(animation, &QPropertyAnimation::finished, this, [&]()
    {
        ui->frameButtonCheckInfo->setMaximumWidth(ui->frameButtonCheckInfo->width());
    });
}

void MenuAdmin::connected()
{
    connect(ui->buttonEditProduct, &QPushButton::clicked, this, &MenuAdmin::buttonEditProduct_clicked);
    connect(ui->buttonInfoEmployees, &QPushButton::clicked, this, &MenuAdmin::buttonInfoEmployees_clicked);
    connect(ui->buttonInfoSupplier, &QPushButton::clicked, this, &MenuAdmin::buttonInfoSupplier_clicked);
    connect(ui->buttonInfoBuyer, &QPushButton::clicked, this, &MenuAdmin::buttonInfoBuyer_clicked);
    connect(ui->buttonReportCheck, &QPushButton::clicked, this, &MenuAdmin::buttonReportCheck_clicked);
}

void MenuAdmin::openAddNewProductDialog()
{
    ProductModel* productModel = new ProductModel(connectionHandler);
    AddProductDialogViewModel* addProductDialogViewModel = new AddProductDialogViewModel(productModel);
    FormWithButtonBack::pushToView({new AddProductDialog(addProductDialogViewModel, this)});
}

void MenuAdmin::openAddNewEmployees()
{
    EmployeesModel* employeesModel = new EmployeesModel(connectionHandler);
    AddEmployeesViewModel* addEmployeesViewModel = new AddEmployeesViewModel(employeesModel);
    FormWithButtonBack::pushToView({new AddNewEmployees(addEmployeesViewModel, this)});
}

void MenuAdmin::openAddCharact()
{
    ProductModel* productModel = new ProductModel(connectionHandler);
    FormWithButtonBack::pushToView({new AddCharact(productModel, this)});
}

void MenuAdmin::openAddSupplier()
{
    SupplierModel* supplierModel = new SupplierModel(connectionHandler);
    FormWithButtonBack::pushToView({new AddNewSupplier(supplierModel, this)});
}

void MenuAdmin::buttonEditProduct_clicked()
{
    if(qobject_cast<MenuEditProduct*>(thisWindow))
        return;

    if(!canClose())
        return;

    freeMemory();

    thisModel = new ProductModel(connectionHandler);
    thisViewModel = new MenuEditProductViewModel(static_cast<ProductModel*>(thisModel));
    thisWindow = new MenuEditProduct(static_cast<MenuEditProductViewModel*>(thisViewModel), connectionHandler, this);
    ui->widgetForWindowAdmin->layout()->addWidget(thisWindow);

    MenuEditProduct* menuEditProduct = qobject_cast<MenuEditProduct*>(thisWindow);
    connect(menuEditProduct, &MenuEditProduct::openAddNewProductDialogSignals, this, &MenuAdmin::openAddNewProductDialog);
    connect(menuEditProduct, &MenuEditProduct::openAddCharactSignals, this, &MenuAdmin::openAddCharact);

    colorButtonControl(qobject_cast<QPushButton*>(sender()));
}

void MenuAdmin::buttonInfoEmployees_clicked()
{
    if(qobject_cast<MenuEmployees*>(thisWindow))
        return;

    if(!canClose())
        return;

    freeMemory();

    thisModel = new EmployeesModel(connectionHandler);
    thisViewModel = nullptr;
    thisWindow = new MenuEmployees(static_cast<EmployeesModel*>(thisModel), this);
    ui->widgetForWindowAdmin->layout()->addWidget(thisWindow);
    connect(qobject_cast<MenuEmployees*>(thisWindow), &MenuEmployees::openAddNewEmployees, this, &MenuAdmin::openAddNewEmployees);
    colorButtonControl(qobject_cast<QPushButton*>(sender()));
}

void MenuAdmin::buttonInfoSupplier_clicked()
{
    if(qobject_cast<MenuSupplier*>(thisWindow))
        return;

    if(!canClose())
        return;

    freeMemory();

    thisModel = new SupplierModel(connectionHandler);
    thisWindow = new MenuSupplier(static_cast<SupplierModel*>(thisModel), this);
    ui->widgetForWindowAdmin->layout()->addWidget(thisWindow);
    connect(qobject_cast<MenuSupplier*>(thisWindow), &MenuSupplier::openAddSupplier, this, &MenuAdmin::openAddSupplier);
    colorButtonControl(qobject_cast<QPushButton*>(sender()));
}

void MenuAdmin::buttonInfoBuyer_clicked()
{
    if(qobject_cast<MenuBuyer*>(thisWindow))
        return;

    if(!canClose())
        return;

    freeMemory();

    thisModel = new BuyerModel(connectionHandler);
    thisViewModel = nullptr;
    thisWindow = new MenuBuyer(static_cast<BuyerModel*>(thisModel), this);
    ui->widgetForWindowAdmin->layout()->addWidget(thisWindow);
    colorButtonControl(qobject_cast<QPushButton*>(sender()));
}

void MenuAdmin::buttonReportCheck_clicked()
{
    if(qobject_cast<MenuReport*>(thisWindow))
        return;

    if(!canClose())
        return;

    freeMemory();

    thisModel = new ProductModel(connectionHandler);
    thisViewModel = nullptr;
    thisWindow = new MenuReport(static_cast<ProductModel*>(thisModel), this);
    ui->widgetForWindowAdmin->layout()->addWidget(thisWindow);
    colorButtonControl(qobject_cast<QPushButton*>(sender()));
}

bool MenuAdmin::canClose()
{
    if(IClose* iclose = dynamic_cast<IClose*>(thisWindow))
        return iclose->canClose();

    return true;
}

bool MenuAdmin::eventFilter(QObject *watched, QEvent *event)
{
    if(watched != ui->frameButtonCheckInfo)
        return false;

    if(!thisWindow)
        return false;

    if(event->type() == QEvent::Leave)
    {
        QWidget* frame = ui->frameButtonCheckInfo;

        animation->stop();
        animation->setDuration(400);
        animation->setStartValue(QRect(frame->x(), frame->y(), frame->width(), frame->height()));
        animation->setEndValue(QRect(frame->x(), frame->y(), 20, frame->height()));

        animation->start();
    }
    else if(event->type() == QEvent::Enter)
    {
        QWidget* frame = ui->frameButtonCheckInfo;
        frame->setMaximumWidth(270);

        animation->stop();
        animation->setDuration(400);
        animation->setStartValue(QRect(frame->x(), frame->y(), frame->width(), frame->height()));
        animation->setEndValue(QRect(frame->x(), frame->y(), 270, frame->height()));

        animation->start();
    }

    return false;
}

void MenuAdmin::show()
{
    if(IView* view = dynamic_cast<IView*>(thisWindow))
        view->show();

    QWidget::show();
}

void MenuAdmin::hide()
{
    QWidget::hide();
}

void MenuAdmin::colorButtonControl(QPushButton* sender)
{
    if(!sender)
        return;

    if(thisButton)
        thisButton->setStyleSheet("QWidget#frameButtonCheckInfo QPushButton{ background-color: transparent; }"
                                  " QWidget#frameButtonCheckInfo QPushButton::hover{ border-radius: 5px; background-color: rgba(255, 255, 255, 30); padding-left: 15px; }");

    thisButton = sender;
    sender->setStyleSheet("QWidget#frameButtonCheckInfo QPushButton{ margin-left: 11px; background-color: rgba(1, 176, 117, 200); }"
                          "QWidget#frameButtonCheckInfo QPushButton::hover{padding-left:4px;}");
}

void MenuAdmin::freeMemory()
{
    if(thisWindow)
    {
        delete thisWindow;
        thisWindow = nullptr;
    }

    if(thisViewModel)
    {
        delete thisViewModel;
        thisViewModel = nullptr;
    }

    if(thisModel)
    {
        delete thisModel;
        thisModel = nullptr;
    }

    //Очищаем поиск
    lineSearch->clear();
}

MenuAdmin::~MenuAdmin()
{
    ui->frameButtonCheckInfo->removeEventFilter(this);
    freeMemory();
    delete animation;
    delete ui;
}

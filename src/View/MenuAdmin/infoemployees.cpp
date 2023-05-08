#include "infoemployees.h"
#include "ui_infoemployees.h"
#include "formwithbuttonback.h"

const QRegularExpression InfoEmployees::regexNull = QRegularExpression("^\\s*$");

InfoEmployees::InfoEmployees(const QString& idEmployees, EmployeesModel* model, QWidget *parent) :
    QWidget(parent),
    idEmployees(idEmployees),
    model(model),
    ui(new Ui::InfoEmployees)
{
    ui->setupUi(this);
    ui->buttonAccept->hide();
    ui->comboBoxGender->addItems({"Чоловік", "Жінка"});

    notification = new CenteredNotification;

    QList<QLineEdit*> list = {ui->inputFullName, ui->inputPhoneNumber, ui->inputAddress};
    for (QLineEdit* item : list)
    {
        item->installEventFilter(this);
        connect(item, &QLineEdit::editingFinished, this, [=]()
        {
            if(item->text() != lastText)
                ui->buttonAccept->show();
        });
    }

    update();
    ui->saleCount->setText("Загальна кількість: " + model->getOneCell(QString("select sum(listP_count) from listproduct join sales using(id_sales) where id_worker = %1").arg(idEmployees)) + " од.");
    ui->saleSum->setText("Загальна сума: " + model->getOneCell(QString("select sum(listP_count*p_priceOne) from listproduct join sales using(id_sales) join product using(id_product) where id_worker = %1").arg(idEmployees)) + " грн.");
    ui->buyCount->setText("Загальна кількість: " + model->getOneCell(QString("select sum(listS_count) from listsupply join waybill using(id_waybill) where id_worker = %1").arg(idEmployees)) + " од.");
    ui->buySum->setText("Загальна сума: " + model->getOneCell(QString("select sum(listS_count*listS_priceCount) from listsupply join waybill using(id_waybill) where id_worker = %1").arg(idEmployees)) + " грн.");

    if(model->getOneCell(QString("select isDelete from worker where id_worker = %1").arg(idEmployees)) == "0")
    {
        ui->buttonReturn->hide();
        ui->inputPassword->hide();

        connect(ui->buttonChangePassword, &QPushButton::clicked, this, [=]() { ui->buttonChangePassword->hide(), ui->inputPassword->show(); ui->buttonAccept->show();});
        connect(ui->buttonDelete, &QPushButton::clicked, this, [=]()
        {
            model->requestBD(QString("UPDATE worker SET isDelete = 1 where id_worker = %1").arg(idEmployees));
            model->requestBD(QString("call deactivationUser(%1)").arg(idEmployees));
            FormWithButtonBack::clearStack();
        });
    }
    else
    {
        ui->buttonDelete->hide();
        ui->buttonChangePassword->hide();
        connect(ui->buttonReturn, &QPushButton::clicked, this, [=]()
        {
            if(regexNull.match(ui->inputPassword->text()).hasMatch())
                return notification->show("Введіть пароль!", 2);

            model->requestBD(QString("UPDATE worker SET isDelete = 0 where id_worker = %1").arg(idEmployees));

            if(model->getOneCell(QString("select position from worker where id_worker = %1").arg(idEmployees)) == "Адміністратор")
                model->requestBD(QString("call createAdmin((select w_full_name from worker where id_worker = %1), %2)").arg(idEmployees, ui->inputPassword->text()));
            else
                model->requestBD(QString("call createCasir((select w_full_name from worker where id_worker = %1), %2)").arg(idEmployees, ui->inputPassword->text()));

            FormWithButtonBack::clearStack();
        });
    }

    connect(ui->comboBoxGender, &QComboBox::currentIndexChanged, this, [&](const int& i)
    {
        if(i != idLastGender)
            ui->buttonAccept->show();
    });

    connect(ui->dateEdit, &QDateEdit::dateChanged, this, [&](const QDate&)
    {
        if(ui->dateEdit->text() != lastDate)
            ui->buttonAccept->show();
    });

    connect(ui->buttonAccept, &QPushButton::clicked, this, &InfoEmployees::change);
}

void InfoEmployees::update()
{
    ui->labelTop->setText(model->getOneCell(QString("select w_full_name from worker where id_worker = %1").arg(idEmployees)));
    ui->labelTopBottom->setText("Позиція: " + model->getOneCell(QString("select position from worker where id_worker = %1").arg(idEmployees)));

    model->updateModelViaQuery(QString("select w_full_name, w_phoneNum, w_address, gender, birthday from worker where id_worker = %1").arg(idEmployees));

    QSqlRelationalTableModel* modelData = model->getModelData();
    ui->inputFullName->setText(modelData->index(0, 0).data().toString());
    ui->inputPhoneNumber->setText(modelData->index(0, 1).data().toString());
    ui->inputAddress->setText(modelData->index(0, 2).data().toString());
    ui->comboBoxGender->setCurrentIndex(ui->comboBoxGender->findText(modelData->index(0, 3).data().toString()));
    ui->dateEdit->setDate(modelData->index(0, 4).data().toDate());

    idLastGender = ui->comboBoxGender->currentIndex();
    lastDate = ui->dateEdit->text();
}

bool InfoEmployees::eventFilter(QObject* watched, QEvent* event)
{
    QLineEdit* lineEdit = qobject_cast<QLineEdit*>(watched);
    if(lineEdit && event->type() == QEvent::FocusIn)
        lastText = lineEdit->text();

    return false;
}

void InfoEmployees::change()
{
    if(ui->buttonReturn->isHidden() && !ui->inputPassword->isHidden())
    {
        if(regexNull.match(ui->inputPassword->text()).hasMatch())
            return notification->show("Введіть новий пароль!", 2);
        //Не работает изменения пароля!!!
        model->requestBD(QString("SET PASSWORD FOR '%1'@'localhost'='%2'").arg(model->getOneCell(QString("select w_full_name from worker where id_worker = %1").arg(idEmployees)), ui->inputPassword->text()));
    }


    model->requestBD(QString("UPDATE worker SET w_full_name = '%1', w_phoneNum = '%2', w_address = '%3', gender = '%4', birthday = '%5' where id_worker = %6").arg(ui->inputFullName->text(), ui->inputPhoneNumber->text(), ui->inputAddress->text(), ui->comboBoxGender->currentText(), ui->dateEdit->date().toString("yyyy-MM-dd"), idEmployees));
    notification->show("Зміни внесено до БД!", 2);
    ui->buttonAccept->hide();

    update();
}

InfoEmployees::~InfoEmployees()
{
    delete ui;
}

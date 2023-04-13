#ifndef PRODUCTMODEL_H
#define PRODUCTMODEL_H

#include "basemodel.h"

#include <QSqlDatabase>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>

class ProductModel : public QObject, public BaseModel
{
    Q_OBJECT

public:
    ProductModel(QSqlDatabase* db);

public:
    QList<QString> getListCategory(){ return listCategory; }
    QList<QString> getListCountry(){ return listCountry; }
    QList<QString> getListBrand(){ return listBrand; }

    void updateListFilter();
    void updateList(QList<QString>& list, QString request);
    void createElementForDispleyCharact(QString request, bool lineEditIsReadOnly = true);
    QList<QCheckBox*> createCheckBox(const QList<QString>& list);

signals:
    void addInfoProductSignal(QLabel* label, QLineEdit* lineEdit);

private:
    void setHeaderModel() override;

private:
    QList<QString> listCategory;
    QList<QString> listCountry;
    QList<QString> listBrand;
};

#endif // PRODUCTMODEL_H

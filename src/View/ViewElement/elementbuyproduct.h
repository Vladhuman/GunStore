#ifndef ELEMENTBUYPRODUCT_H
#define ELEMENTBUYPRODUCT_H

#include <QWidget>
#include <QSqlTableModel>
#include <QRegularExpression>
#include <QMessageBox>

namespace Ui {
class ElementBuyProduct;
}

class ElementBuyProduct : public QWidget
{
    Q_OBJECT

public:
    explicit ElementBuyProduct(const QAbstractTableModel* model, QWidget *parent = nullptr);
    ~ElementBuyProduct();

public:
    const QString& getNameProduct();
    int getCount() { return count; }
    double getSumPrice() { return sumPrice; }
    const QString& getIdProduct() { return idProduct; }
    const QString getPrice() { return QString::number(price, 'f', 2); }
    QString getBrand();

signals:
    void deleteMe(ElementBuyProduct* element);
    void changedCountPriceSignals();

private:
    void setCount(int count);
    void connected();

private:
    Ui::ElementBuyProduct *ui;
    QString idProduct;
    QString nameProduct;
    int count;
    double price;
    double sumPrice;
};

#endif // ELEMENTBUYPRODUCT_H

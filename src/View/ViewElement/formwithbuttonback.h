#ifndef FORMWITHBUTTONBACK_H
#define FORMWITHBUTTONBACK_H

#include <QWidget>
#include <QLayout>
#include <QStack>

namespace Ui {
class FormWithButtonBack;
}

class FormWithButtonBack : public QWidget
{
    Q_OBJECT

public:
    static FormWithButtonBack* getInstance(QLayout *baseWindow);
    ~FormWithButtonBack();
    static void pushToView(QList<QWidget*> newDisplay);
    static void clearStack();

public slots:
    void closeMe();

signals:
    void openWindow();


//PRIVATE!!!
private:
    FormWithButtonBack();
    FormWithButtonBack(FormWithButtonBack &other) = delete;
    void operator=(const FormWithButtonBack&) = delete;
    FormWithButtonBack(QLayout *baseWindow);
//----------
    static void popView();
    static void popLast();

private:
    Ui::FormWithButtonBack *ui;
    static QLayout* baseCentalWidgetWindow;
    static QWidget* thisWidget;

    //Stack view element
    static QStack<QList<QWidget*>> stack;

    //Singleton
    static FormWithButtonBack* instance;
};

#endif // FORMWITHBUTTONBACK_H

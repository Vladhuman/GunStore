#ifndef MAINMENU_H
#define MAINMENU_H

#include "mainmenuviewmodel.h"
#include "LayoutState.h"
#include "centerednotification.h"
#include "iview.h"
#include "isearch.h"

#include <QWidget>

namespace Ui {
class MainMenu;
}

class MainMenu : public QWidget, public IView, public ISearch
{
    Q_OBJECT

public:
    explicit MainMenu(MainMenuViewModel* mainMenuViewModel, QWidget *parent = nullptr);
    ~MainMenu();

    void search(const QString& text) override;

public slots:
    void modelChangedSlots(QAbstractTableModel * modelData);
    void addCheckBoxSlots(QCheckBox* checkBox, const LayoutState& layoutName);
    void addInfoProductSlots(QWidget* widget = nullptr);
    void clearCheckBoxSlots();
    void clearLableSlots();
    void priceFilterChangedSlots();
    void show() override;
    void hide() override;
    void messageShow(const QString& message);

signals:
    void priceFilterChangedSignals(QLineEdit* inputTo, QLineEdit* inputDo, const QString& isDelete = "0");
    void updateCountForProduct(const int& newCount);

private:
    void connected();
    void deleteWidget(QLayout* layout);
    void buttonAddOrder_clicked();

private:
    Ui::MainMenu *ui;
    MainMenuViewModel* mainMenuViewModel;
    CenteredNotification* notification;
};

#endif // MAINMENU_H

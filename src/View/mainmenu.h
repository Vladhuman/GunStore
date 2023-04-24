#ifndef MAINMENU_H
#define MAINMENU_H

#include "mainmenuviewmodel.h"
#include "LayoutState.h"

#include <QWidget>

namespace Ui {
class MainMenu;
}

class MainMenu : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenu(MainMenuViewModel* mainMenuViewModel, QWidget *parent = nullptr);
    ~MainMenu();

public slots:
    void modelChangedSlots(QAbstractTableModel * modelData);
    void addCheckBoxSlots(QCheckBox* checkBox, const LayoutState& layoutName);
    void addInfoProductSlots(QWidget* widget = nullptr);
    void clearCheckBoxSlots();
    void clearLableSlots();
    void priceFilterChangedSlots();

signals:
    void priceFilterChangedSignals(QLineEdit* inputTo, QLineEdit* inputDo);

private:
    void connected();
    void deleteWidget(QLayout* layout);

private:
    Ui::MainMenu *ui;
    MainMenuViewModel* mainMenuViewModel;
};

#endif // MAINMENU_H
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

#include "database.h"

#include "weeklyview.h"
#include "monthlyview.h"
#include "settingsview.h"
#include "bottombar.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Database *db;

    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QStackedWidget *stackedWidget;

    WeeklyView *weeklyView;
    MonthlyView *monthlyView;
    SettingsView *settingsView;
    BottomBar *bottomBar;

private slots:
    void showWeeklyView();
    void showMonthlyView();
    void showSettingsView();

public slots:
    void budgetEdited(int);

public:
    MainWindow(Database *db = nullptr);
    ~MainWindow();

    QStackedWidget * getWidget();


};
#endif // MAINWINDOW_H

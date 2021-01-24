#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

#include "database.h"

#include "weeklyview.h"
#include "monthlyview.h"
#include "sidebar.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	Database *db;
	QStackedWidget *centralWidget;

	WeeklyView *weeklyView;
	MonthlyView *monthlyView;
	Sidebar *sidebar;

	QWidget *previous;

private slots:
	void showSideBar();
	void showPreviousWidget();
	void showWeeklyView();
	void showMonthlyView();

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	void setDatabase(Database *);

	QStackedWidget * getWidget();


};
#endif // MAINWINDOW_H

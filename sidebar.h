#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QToolBar>
#include <QAction>
#include <QHBoxLayout>

class Sidebar {

private:

	QWidget *widget;
	QHBoxLayout *layout;
	QToolBar *menu;
	QWidget* transparentWidget;

	QAction *weeklyViewAct;

public:
	Sidebar();
	~Sidebar();

	QWidget *getWidget();

	QAction *getWeeklyViewAction();

signals:
	void exit();
};

#endif // SIDEBAR_H

#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QToolBar>
#include <QAction>
#include <QHBoxLayout>

#include "transparentwidget.h"

class Sidebar : public QObject {

	Q_OBJECT

private:

	QWidget *widget;
	QHBoxLayout *layout;
	QToolBar *menu;
	TransparentWidget *transparentWidget;

	QAction *weeklyViewAct;
	QAction *monthlyViewAct;

public:
	Sidebar();
	~Sidebar();

	QWidget *getWidget();

	QAction *getWeeklyViewAction();
	QAction *getMonthlyViewAction();

private slots:
	void exitSlot();

signals:
	void exit();
};

#endif // SIDEBAR_H

#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QToolBar>
#include <QAction>
#include <QHBoxLayout>

#include "transparentwidget.h"

class BottomBar : public QObject {

	Q_OBJECT

private:

	QWidget *widget;
	QHBoxLayout *layout;
	QToolBar *menu;

	QAction *weeklyViewAct;
	QAction *monthlyViewAct;
	QAction *settingsViewAct;

public:
	BottomBar();
	~BottomBar();

	QWidget *getWidget();

	QAction *getWeeklyViewAction();
	QAction *getMonthlyViewAction();

	QAction *getSettingsViewAction();

private slots:
	void exitSlot();

signals:
	void exit();
};

#endif // SIDEBAR_H

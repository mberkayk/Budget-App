#include "bottombar.h"

BottomBar::BottomBar() {

	menu = new QToolBar();
	menu->setOrientation(Qt::Horizontal);
	menu->setContentsMargins(QMargins(0,0,0,0));
	menu->setIconSize(QSize(48, 48));

	weeklyViewAct = new QAction(QIcon(":icons/week_icon.svg"), "Weekly View");
	menu->addAction(weeklyViewAct);

	monthlyViewAct = new QAction(QIcon(":icons/month_icon.svg"), "Monthly View");
	menu->addAction(monthlyViewAct);

	settingsViewAct = new QAction(QIcon(":icons/settings_icon.svg"), "Settings View");
	menu->addAction(settingsViewAct);
}

BottomBar::~BottomBar(){
	delete menu;

	delete weeklyViewAct;
	delete monthlyViewAct;
	delete settingsViewAct;
}

QWidget *BottomBar::getWidget(){return menu;}

QAction *BottomBar::getWeeklyViewAction(){return weeklyViewAct;}
QAction *BottomBar::getMonthlyViewAction(){return monthlyViewAct;}
QAction *BottomBar::getSettingsViewAction(){return settingsViewAct;}


void BottomBar::exitSlot(){
	emit exit();
}

#include "bottombar.h"

BottomBar::BottomBar() {


	menu = new QWidget;

	layout = new QHBoxLayout;
	layout->setMargin(0);
	layout->setSpacing(0);
	menu->setLayout(layout);

	butts = new QButtonGroup;

	weeklyViewButt = new QPushButton;
	weeklyViewButt->setIcon(QIcon(":/icons/week_icon.png"));
	weeklyViewButt->setToolTip("Weekly View");
	weeklyViewButt->setFlat(true);
	weeklyViewButt->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
//	weeklyViewButt->setIconSize(QSize(64, 64));
	butts->addButton(weeklyViewButt);
	layout->addWidget(weeklyViewButt);

	monthlyViewButt = new QPushButton;
	monthlyViewButt->setIcon(QIcon(":/icons/month_icon.png"));
	monthlyViewButt->setToolTip("Monthly View");
	monthlyViewButt->setFlat(true);
	monthlyViewButt->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
//	monthlyViewButt->setIconSize(QSize(64, 64));
	butts->addButton(monthlyViewButt);
	layout->addWidget(monthlyViewButt);

	settingsViewButt = new QPushButton;
	settingsViewButt->setIcon(QIcon(":/icons/settings_icon.png"));
	settingsViewButt->setToolTip("Settings View");
	settingsViewButt->setFlat(true);
	settingsViewButt->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
//	settingsViewButt->setIconSize(QSize(64, 64));
	butts->addButton(settingsViewButt);
	layout->addWidget(settingsViewButt);


}

BottomBar::~BottomBar(){
	delete menu;
	delete layout;

	delete weeklyViewButt;
	delete monthlyViewButt;
	delete settingsViewButt;
}

QWidget *BottomBar::getWidget(){return menu;}

QWidget *BottomBar::getWeeklyViewAction(){return weeklyViewButt;}
QWidget *BottomBar::getMonthlyViewAction(){return monthlyViewButt;}
QWidget *BottomBar::getSettingsViewAction(){return settingsViewButt;}


void BottomBar::exitSlot(){
	emit exit();
}

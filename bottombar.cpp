#include "bottombar.h"

#include <QPalette>

BottomBar::BottomBar() {

	widget = new QWidget();

	layout = new QHBoxLayout();
	layout->setMargin(0);
	widget->setLayout(layout);

	menu = new QToolBar();
	menu->setOrientation(Qt::Horizontal);

	weeklyViewAct = new QAction("Week");
	menu->addAction(weeklyViewAct);

	monthlyViewAct = new QAction("Month");
	menu->addAction(monthlyViewAct);

	layout->addWidget(menu);

}

BottomBar::~BottomBar(){
	delete widget;
	delete layout;
	delete menu;

	delete weeklyViewAct;
	delete monthlyViewAct;
}

QWidget * BottomBar::getWidget(){return widget;}

QAction * BottomBar::getWeeklyViewAction(){return weeklyViewAct;}
QAction * BottomBar::getMonthlyViewAction(){return monthlyViewAct;}

void BottomBar::exitSlot(){
	emit exit();
}

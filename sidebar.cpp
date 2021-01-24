#include "sidebar.h"

#include <QPalette>

Sidebar::Sidebar() {

	widget = new QWidget();

	layout = new QHBoxLayout();
	layout->setMargin(0);
	widget->setLayout(layout);

	menu = new QToolBar();
	menu->setOrientation(Qt::Vertical);

	weeklyViewAct = new QAction("Weekly View");
	menu->addAction(weeklyViewAct);

	monthlyViewAct = new QAction("Monthly View");
	menu->addAction(monthlyViewAct);

	layout->addWidget(menu);
	layout->setStretchFactor(menu, 4);

	transparentWidget = new TransparentWidget();
	QObject::connect(transparentWidget, SIGNAL(exit()), this, SLOT(exitSlot()));
	layout->addWidget(transparentWidget);
	layout->setStretchFactor(transparentWidget, 1);

	QPalette p;
	p.setColor(QPalette::Background, QColor(100, 120, 100, 10));
	widget->setPalette(p);
	widget->setAutoFillBackground(true);
	menu->setAutoFillBackground(true);

}

Sidebar::~Sidebar(){
	delete widget;
	delete layout;
	delete menu;
	delete transparentWidget;

	delete weeklyViewAct;
	delete monthlyViewAct;
}

QWidget * Sidebar::getWidget(){return widget;}

QAction * Sidebar::getWeeklyViewAction(){return weeklyViewAct;}
QAction * Sidebar::getMonthlyViewAction(){return monthlyViewAct;}

void Sidebar::exitSlot(){
	emit exit();
}

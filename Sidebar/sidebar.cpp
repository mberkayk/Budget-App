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

	layout->addWidget(menu);
	layout->setStretchFactor(menu, 4);

	transparentWidget = new QWidget();
	layout->addWidget(transparentWidget);
	layout->setStretchFactor(transparentWidget, 1);

	QPalette p;
	p.setColor(QPalette::Background, QColor(100, 120, 100, 10));
	widget->setPalette(p);
	transparentWidget->setPalette(p);
	transparentWidget->setAutoFillBackground(true);
	widget->setAutoFillBackground(true);
	menu->setAutoFillBackground(true);

}

Sidebar::~Sidebar(){
	delete widget;
	delete layout;
	delete menu;
	delete transparentWidget;
}

QWidget * Sidebar::getWidget(){return widget;}

QAction * Sidebar::getWeeklyViewAction(){return weeklyViewAct;}

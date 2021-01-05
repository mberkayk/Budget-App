#include "monthlyview.h"

MonthlyView::MonthlyView() {

	widget = new QWidget();

	mainLayout = new QVBoxLayout();
	widget->setLayout(mainLayout);

	titleBarLayout = new QHBoxLayout();
	mainLayout->addLayout(titleBarLayout);

	menuBtn = new QPushButton("M");
	titleBarLayout->addWidget(menuBtn);

	titleLabel = new QLabel("This Month");
	titleBarLayout->addWidget(titleLabel);

	budgetInfoLayout = new QHBoxLayout();
	mainLayout->addLayout(budgetInfoLayout);

	entries = new EntryGroup("Total Montly Expenses: ");
	mainLayout->addWidget(entries->getWidget());

}

MonthlyView::~MonthlyView(){
	delete widget;
}

QWidget * MonthlyView::getWidget(){return widget;}

QPushButton * MonthlyView::getMenuBtn(){return menuBtn;}

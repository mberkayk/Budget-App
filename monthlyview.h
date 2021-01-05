#ifndef MONTHLYVIEW_H
#define MONTHLYVIEW_H

#include <QWidget>
#include <QBoxLayout>
#include <QPushButton>
#include "entrygroup.h"

class MonthlyView {

private:
	QWidget * widget;

	QVBoxLayout *mainLayout;
	QHBoxLayout *titleBarLayout;
	QHBoxLayout *budgetInfoLayout;

	EntryGroup *entries;

	QLabel *titleLabel;
	QPushButton *menuBtn;

public:
	MonthlyView();
	~MonthlyView();

	QWidget * getWidget();
	QPushButton * getMenuBtn();
};

#endif // MONTHLYVIEW_H

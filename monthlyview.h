#ifndef MONTHLYVIEW_H
#define MONTHLYVIEW_H

#include <QWidget>
#include <QBoxLayout>
#include <QPushButton>

#include "database.h"

#include "entrygroup.h"

class MonthlyView {

private:
	int budget;

	QDate *date;

	QWidget * widget;

	QVBoxLayout *mainLayout;
	QHBoxLayout *titleBarLayout;
	QHBoxLayout *budgetInfoLayout;

	QLabel *titleLabel;
	QPushButton *menuBtn;

	QLabel *budgetLabel;

	EntryGroup *entries;

public:
	MonthlyView();
	~MonthlyView();

	QWidget * getWidget();
	QPushButton * getMenuBtn();

	void loadFromDatabase(Database *db);
};

#endif // MONTHLYVIEW_H

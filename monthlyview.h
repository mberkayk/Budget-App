#ifndef MONTHLYVIEW_H
#define MONTHLYVIEW_H

#include <QDialog>

#include <QWidget>
#include <QBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QScrollArea>

#include "database.h"
#include "entrygroup.h"

class MonthlyEntryDialog : public QDialog {

	Q_OBJECT

private:
	QVBoxLayout *layout ;
	QLineEdit *descBox;
	QLineEdit *amtBox;

	QPushButton *xBtn;
	QPushButton *okBtn;

public:
	MonthlyEntryDialog(QWidget *);

	Entry * createEntry();
};

class MonthlyView : public QWidget {

	Q_OBJECT

private:
	Database *db;
	int budget;

	QDate *date;

	QVBoxLayout *mainLayout;
	QHBoxLayout *titleBarLayout;
	QHBoxLayout *budgetInfoLayout;

	QLabel *titleLabel;
	QPushButton *addBtn;

	QLabel *budgetLabel;

	QScrollArea *scrollArea;
	EntryGroup *entries;

	MonthlyEntryDialog *entryDialog;

	void setBudget(int);

private slots:
	void showEntryDialog();
	void addNewEntry();

public:
	MonthlyView(Database *db);
	~MonthlyView();

	void loadFromDatabase();
	void saveToDatabase();
};

#endif // MONTHLYVIEW_H

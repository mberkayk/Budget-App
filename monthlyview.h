#ifndef MONTHLYVIEW_H
#define MONTHLYVIEW_H

#include <QDialog>
#include <QInputDialog>
#include <QWidget>
#include <QBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QScrollArea>
#include <QMessageBox>

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
	QPushButton *editBudgetButton;

	QScrollArea *scrollArea;
	EntryGroup *entries;

	MonthlyEntryDialog *entryDialog;

	void setBudget(int);

signals:
	void budgetEdited(int);

private slots:
	void showEntryDialog();
	void addNewEntry();
	void showEditBudgetDialog();

	void entrySelectedSlot(EntryGroup *, int);

public:
	MonthlyView(Database *db);
	~MonthlyView();

	void loadFromDatabase();
	void saveToDatabase();
};

#endif // MONTHLYVIEW_H

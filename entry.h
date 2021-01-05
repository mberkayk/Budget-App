#ifndef ENTRY_H
#define ENTRY_H

#include <QString>

class Entry
{

private:
	int amount;
	QString desc;
public:
	Entry(int, QString = "Expense");

	int getAmount();
	QString getDesc();
};

#endif // ENTRY_H

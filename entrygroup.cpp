#include "entrygroup.h"

Entry::Entry(int a, QString s) : QWidget() {
	amount = a;
	desc = s;

	layout = new QHBoxLayout;
	layout->setMargin(0);

	descLabel = new QLabel(desc);
	amtLabel = new QLabel(QString::number(amount));

	layout->addWidget(descLabel);
	layout->addWidget(amtLabel);

	setLayout(layout);
}

int Entry::getAmount(){return amount;}

QString Entry::getDesc(){return desc;}


EntryGroup::EntryGroup(QString s) : QGroupBox(s) {
	collapsed = true;
	total = 0;

	stackedLayout = new QStackedLayout;
	collapsedWidget = new QWidget;
	expandedWidget = new QWidget;

	totalEntry = new Entry(total, "Total");

	collapsedLayout = new QHBoxLayout();
	collapsedLayout->addWidget(totalEntry);
	collapsedLayout->setMargin(0);
	collapsedWidget->setLayout(collapsedLayout);

	stackedLayout->addWidget(collapsedWidget);
	stackedLayout->addWidget(expandedWidget);

	stackedLayout->setCurrentWidget(collapsedWidget);
	setLayout(stackedLayout);
}

EntryGroup::~EntryGroup(){
	delete stackedLayout;
	delete collapsedLayout;
	delete collapsedWidget;
	delete expandedWidget;
}

void EntryGroup::updateTotal(){
	total = 0;
	foreach (Entry* entry, entries) {
		total += entry->getAmount();
	}
}

void EntryGroup::setEntries(QVector<Entry *> e){
	foreach (Entry* entry, e) {
		delete entry;
	}
	entries = e;
}

void EntryGroup::addEntry(Entry *e){
	entries.append(e);
}

void EntryGroup::removeEntry(int i){
	entries.removeAt(i);
}

void EntryGroup::collapse(){
	collapsed = true;
}

void EntryGroup::expand(){
	collapsed = false;
}

QVector<Entry *> EntryGroup::getEntries(){
	return entries;
}


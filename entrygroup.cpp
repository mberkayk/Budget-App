#include "entrygroup.h"

EntryGroup::EntryGroup(QString s) {

	total = 0;

	widget = new QWidget();

	mainLayout = new QHBoxLayout();
	widget->setLayout(mainLayout);

	descLayout = new QVBoxLayout();
	mainLayout->addLayout(descLayout);

	amountLayout = new QVBoxLayout();
	mainLayout->addLayout(amountLayout);

	titleLabel = new QLabel(s);
	descLayout->addWidget(titleLabel);

	totalLabel = new QLabel(QString::number(total));
	amountLayout->addWidget(totalLabel);

}

EntryGroup::~EntryGroup(){
	delete widget;
	delete mainLayout;
	delete descLayout;
	delete amountLayout;
	delete titleLabel;
	delete totalLabel;
	for(int i = 0; i < entries.size(); i++){
		delete entries.at(i);
	}
}

void EntryGroup::updateTotal(){
	total = 0;
	for(int i = 0; i < entries.size(); i++){
		total += entries.at(i)->getAmount();
	}
}

QWidget* EntryGroup::getWidget(){
	return widget;
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

int EntryGroup::getTotal(){return total;}

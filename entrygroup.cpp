#include "entrygroup.h"
#include "QDebug"

Entry::Entry(int a, QString s) : QWidget() {
	unsaved = false;

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

void Entry::setAmount(int a){
	amount = a;
	amtLabel->setText(QString::number(a));
}

void Entry::setUnsaved(bool b){ unsaved = b;}

int Entry::getAmount(){return amount;}

QString Entry::getDesc(){return desc;}

bool Entry::getUnsaved(){return unsaved;}

EntryGroup::EntryGroup(QString s) : QGroupBox(s), entries(){
	collapsed = true;
	total = 0;
	titleStr = s;
	updateTitle();

	stackedLayout = new QStackedLayout;
	setLayout(stackedLayout);

	collapsedWidget = new QWidget;
	stackedLayout->addWidget(collapsedWidget);
	stackedLayout->setCurrentWidget(collapsedWidget);

	collapsedLayout = new QHBoxLayout();
	collapsedLayout->setMargin(0);
	collapsedWidget->setLayout(collapsedLayout);

	expandedWidget = new QWidget;
	stackedLayout->addWidget(expandedWidget);

	expandedLayout = new QVBoxLayout;
	expandedLayout->setMargin(0);
	expandedWidget->setLayout(expandedLayout);

}

EntryGroup::~EntryGroup(){
	foreach (Entry* entry, entries) {
		delete entry;
	}
}

void EntryGroup::updateTotal(){
	total = 0;
	foreach (Entry* entry, entries) {
		total += entry->getAmount();
	}
	updateTitle();
}

void EntryGroup::updateTitle(){
	QString str;
	if(collapsed == true){
		str = "+";
	} else {
		str = "-";
	}
	setTitle(str + titleStr + " (" + QString::number(total) + ")");
}

void EntryGroup::setEntries(QVector<Entry *> e){
	entries = e;
	foreach(Entry * ent, entries){
		expandedLayout->addWidget(ent);
	}
	updateTotal();
}

void EntryGroup::addEntry(Entry *e){
	e->setUnsaved(true);
	entries.append(e);
	expandedLayout->addWidget(e);
	updateTotal();
}

void EntryGroup::removeEntry(int i){
	expandedLayout->removeWidget(entries.at(i));
	delete entries.at(i);
	entries.removeAt(i);
	updateTotal();
}

void EntryGroup::collapse(){
	stackedLayout->setCurrentWidget(collapsedWidget);
	collapsed = true;
	updateTitle();
}

void EntryGroup::expand(){
	stackedLayout->setCurrentWidget(expandedWidget);
	collapsed = false;
	updateTitle();
}

QVector<Entry *> EntryGroup::getEntries(){
	return entries;
}

QVector<Entry *> EntryGroup::getUnsavedEntries(){
	QVector<Entry*> result;
	foreach(Entry* e, entries){
		if(e->getUnsaved() == true){
			result.append(e);
			e->setUnsaved(false);
		}
	}
	return result;
}

void EntryGroup::mousePressEvent(QMouseEvent *event){
	if(collapsed) expand();
	else collapse();

	updateTitle();
}

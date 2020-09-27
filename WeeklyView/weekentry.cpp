#include "weekentry.h"

#include <QDebug>

WeekEntry::WeekEntry(QWidget * parent, QString s, QVector<double> v)
	: QWidget(parent),entries() {

	sum = 0;

	VLayout = new QVBoxLayout(this);
	setLayout(VLayout);

	nameLabel = new QLabel(s, this);
	VLayout->addWidget(nameLabel);
	nameLabel->show();

	HLayout = new QHBoxLayout();
	VLayout->addLayout(HLayout);

	doubleVal = new QDoubleValidator();
	doubleVal->setNotation(QDoubleValidator::StandardNotation);
	doubleVal->setBottom(0);

	if(v.isEmpty()){
		addEntryBox();
	}else{
		for(int i = 0; i < v.size(); i++){
			addEntryBox(QString::number(v.at(i)));
		}
	}

	sumLabel = new QLabel(" = 0", this);
	HLayout->addWidget(sumLabel);
	sumLabel->show();

	addEmptyLineEdit = false;

	updateSum();
}

WeekEntry::~WeekEntry(){
	delete VLayout;
	delete HLayout;
	foreach (QLineEdit * le, entries) {
		delete le;
	}
	delete nameLabel;
	delete sumLabel;
	delete doubleVal;
	delete sumLabel;
}

void WeekEntry::setAddEmptyLineEdit(bool b){
	addEmptyLineEdit = b;
}

void WeekEntry::addEntryBox(QString s){
	QLineEdit *qle = new QLineEdit(s, this);
	qle->setPlaceholderText("0");
	entries.append(qle);

	entries.back()->setValidator(doubleVal);

	if(entries.size() > 1){
		HLayout->insertWidget((entries.size()-1)*2-1, new QLabel("+",this));
	}
	HLayout->insertWidget((entries.size()-1)*2, entries.back());


//	QObject::connect(entries.last(), &QLineEdit::textEdited,
//					 this, &WeekEntry::checkIfEmpty);
	QObject::connect(entries.last(), &QLineEdit::returnPressed,
					 this, &WeekEntry::manageEntryBoxes);
}

void WeekEntry::manageEntryBoxes(){
	//create an empty box in today's week entry to indicate that user can
	// input more day entries
	if(entries.last()->text().toDouble() != 0 && addEmptyLineEdit == true){
		addEntryBox();
	}

	//remove the entries that have empty strings or 0 in them
	//except for the last one
	for(int i = 0; i < entries.size()-1; i++){
		QString str = entries.at(i)->text();
		if(str.toDouble() == 0 || str.isEmpty()){
			//remove and delete the QLineEdit
			HLayout->removeWidget(entries.at(i));
			delete entries.at(i);
			entries.remove(i);

			//remove and delete the + label
			QLayoutItem *item;
			if(i == 0){
				item = HLayout->takeAt(0);
			}else{
				item = HLayout->takeAt(2*i-1);
			}
			delete item->widget();
			delete item;
			//decrement the counter to not skip any elements
			i--;
		}
	}


	//if a day entry starts with 0 remove the 0 from the left of the number
	for(int i = 0; i < entries.size(); i++){
		while(entries[i]->text().startsWith("0")
			  && entries[i]->text().size() > 1){

			QString s = entries.at(i)->text();
			s.remove(0, 1);
			entries.at(i)->setText(s);
		}
	}

	//calculate and update the sum label
	updateSum();
	qDebug() << "managed entries";
}

void WeekEntry::updateSum(){

	sum = 0;
	foreach(QLineEdit *e, entries){
		sum += e->text().toDouble();
	}

	sumLabel->setText(" = " + QString::number(sum));
	emit sumChanged();
}

double WeekEntry::getSum(){
	return sum;
}

QVector<double> WeekEntry::getEntries(){
	QVector<double> result;
	foreach(QLineEdit *qle, entries){
		result.append(qle->text().toDouble());
	}
	return result;
}

void WeekEntry::setEntries(QVector<double> v){
	foreach (QLineEdit * le, entries) {
		delete le;
	}
	entries.clear();
	//remove the plus labels
	while (HLayout->count() > 1) {
		QLayoutItem *child = HLayout->takeAt(0);
		delete child->widget(); // delete the widget
		delete child;   // delete the layout item
	}
	foreach(double d, v){
		addEntryBox(QString::number(d));
	}
}

void WeekEntry::checkIfEmpty(){
	foreach(QLineEdit *le, entries){
		if(le->text().isEmpty()){
			le->setText("0");
		}
	}
}

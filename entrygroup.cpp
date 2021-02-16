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

	grabGesture(Qt::GestureType::TapAndHoldGesture);
}

void Entry::setAmount(int a){
	amount = a;
	amtLabel->setText(QString::number(a));
}

void Entry::setUnsaved(bool b){ unsaved = b;}

int Entry::getAmount(){return amount;}

QString Entry::getDesc(){return desc;}

bool Entry::getUnsaved(){return unsaved;}

bool Entry::event(QEvent *event){
	if(event->type() == QEvent::Gesture){
		QGestureEvent *gEvent = static_cast<QGestureEvent*>(event);
		if(QGesture *hold = gEvent->gesture(Qt::TapAndHoldGesture)){
			if(hold->state() == Qt::GestureFinished){
				emit entrySelectedSignal(this);
			}
		}
		return true;
	}
	return QWidget::event(event);
}



EntryGroup::EntryGroup(QString s) : QGroupBox(s),
	entries(), mouseClickTimer() {

	collapsed = true;
	total = 0;
	titleStr = s;
	updateTitle();
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

	stackedWidget = new QStackedWidget;
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setMargin(0);
	setLayout(layout);

	layout->addWidget(stackedWidget);

	collapsedWidget = new QWidget;
	collapsedWidget->setMaximumHeight(10);
	collapsedWidget->setMinimumSize(5,10);

	stackedWidget->addWidget(collapsedWidget);
	stackedWidget->setCurrentWidget(collapsedWidget);

	expandedWidget = new QWidget;
	stackedWidget->addWidget(expandedWidget);

	expandedLayout = new QVBoxLayout;
	expandedLayout->setMargin(0);
	expandedLayout->setSpacing(3);
	expandedWidget->setLayout(expandedLayout);

	mouseClickTimer.setSingleShot(true);
	mouseClickTimer.setInterval(200);
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
		bool b = QObject::connect(ent, &Entry::entrySelectedSignal,
						 this, &EntryGroup::entrySelectedSlot);
		Q_ASSERT(b);
	}
	updateTotal();
}

void EntryGroup::addEntry(Entry *e){
	e->setUnsaved(true);
	entries.append(e);
	expandedLayout->addWidget(e);
	bool b = QObject::connect(e, &Entry::entrySelectedSignal,
					 this, &EntryGroup::entrySelectedSlot);
	Q_ASSERT(b);
	updateTotal();
}

void EntryGroup::removeEntry(int i){
	expandedLayout->removeWidget(entries.at(i));
	delete entries.at(i);
	entries.removeAt(i);
	updateTotal();
}

void EntryGroup::collapse(){
	stackedWidget->setCurrentWidget(collapsedWidget);
	collapsed = true;
	updateTitle();
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
	// if the minimum size is larger than the value given to resize() underneath resizeEvent() expandes the widget to the minimum size
	setMinimumHeight(30);
	resize(collapsedWidget->size());
	updateGeometry();
}

void EntryGroup::expand(){
	stackedWidget->setCurrentWidget(expandedWidget);
	collapsed = false;
	updateTitle();
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setMinimumSize(0, 0); //0,0 is the default value of the minimumSize
}

QVector<Entry *> EntryGroup::getEntries(){
	return entries;
}

int EntryGroup::getTotal(){
	return total;
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
	mouseClickTimer.start();

	QWidget::mousePressEvent(event);
}

void EntryGroup::mouseReleaseEvent(QMouseEvent *event){

	if(mouseClickTimer.remainingTime() > 0){
		if(collapsed) expand();
		else collapse();

		updateTitle();
	}

	QWidget::mouseReleaseEvent(event);
}

void EntryGroup::entrySelectedSlot(Entry *entry){
	int id = entries.indexOf(entry);
	emit entrySelectedSignal(this, id);
}



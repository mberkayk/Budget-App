#include "weeklyview.h"
#include <QDebug>
#include <cmath>

SpinBoxView::SpinBoxView(QVector<QString> items) : QGraphicsView() {
    selectedItem = 0;
    fontSize = 20;
    spacing = 30;

    sceneUnstable = false;

    scene = new QGraphicsScene;
    for(int i = 0; i < items.size(); i++){
        QGraphicsTextItem *item = new QGraphicsTextItem(items[i]);
        QFont f = QFont();
        f.setPixelSize(20);
        item->setFont(f);
        scene->addItem(item);
        item->setPos(100 - item->boundingRect().width()/2 , spacing * i);
    }

    setScene(scene);
    setInteractive(false);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMaximumSize(200, 30);
    updateViewPort();

}

void SpinBoxView::mouseMoveEvent(QMouseEvent *event){
    QGraphicsView::mouseMoveEvent(event);
    if(event->buttons().testFlag(Qt::LeftButton)){
        sceneUnstable = true;
    }
}

void SpinBoxView::mouseReleaseEvent(QMouseEvent *event){
    QGraphicsView::mouseReleaseEvent(event);

    if(sceneUnstable == true){
        double d = mapToScene(0, 0).y() / spacing;
        selectedItem = std::round(d);
        verticalScrollBar()->setValue(selectedItem*spacing);
        sceneUnstable = false;
    }

}

void SpinBoxView::updateViewPort(){
    centerOn(scene->items(Qt::AscendingOrder).at(selectedItem));
}

void SpinBoxView::incrementSelectedDay(){
    if (selectedItem != 6){
        selectedItem++;
    }
    updateViewPort();
}

void SpinBoxView::decrementSelectedDay(){
    if (selectedItem != 0){
        selectedItem--;
    }
    updateViewPort();
}

void SpinBoxView::select(int i){
    selectedItem = i;
    updateViewPort();
}

int SpinBoxView::getSelectedItemIndex(){
    return selectedItem;
}




SpinBox::SpinBox(QVector<QString> items){

    layout = new QVBoxLayout;
    setLayout(layout);

    upBtn = new QToolButton;
    upBtn->setArrowType(Qt::UpArrow);
    upBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    layout->addWidget(upBtn);
    layout->setAlignment(upBtn, Qt::AlignHCenter);

    spinBoxView = new SpinBoxView(items);
    layout->addWidget(spinBoxView);

    downBtn = new QToolButton;
    downBtn->setArrowType(Qt::DownArrow);
    downBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    layout->addWidget(downBtn);
    layout->setAlignment(downBtn, Qt::AlignHCenter);

    layout->setStretchFactor(upBtn, 1);
    layout->setStretchFactor(downBtn, 1);
    layout->setStretchFactor(spinBoxView, 2);

    QObject::connect(upBtn, SIGNAL(pressed()), spinBoxView, SLOT(decrementSelectedDay()));
    QObject::connect(downBtn, SIGNAL(pressed()), spinBoxView, SLOT(incrementSelectedDay()));

}

void SpinBox::select(int i){
    spinBoxView->select(i);
}

int SpinBox::getSelectedItemIndex(){
    return spinBoxView->getSelectedItemIndex();
}



DailyEntryDialog::DailyEntryDialog(QWidget *parent) : QDialog(parent) {

    QSettings settings;

    layout = new QVBoxLayout;
    setLayout(layout);

    QVector<QString> days = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    spinBox = new SpinBox(days);
    spinBox->select(QDate::currentDate().dayOfWeek()-1);
    layout->addWidget(spinBox);

    descBox = new QLineEdit;
    descBox->setPlaceholderText(settings.value("default daily description").toString());
    layout->addWidget(descBox);

    layout->addWidget(new QLabel("Amount:"));

    amtBox = new QLineEdit;
    amtBox->setPlaceholderText("0");
    layout->addWidget(amtBox);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    layout->addLayout(btnLayout);

    xBtn = new QPushButton("X");
    xBtn->setAutoDefault(false);
    okBtn = new QPushButton("OK");
    okBtn->setAutoDefault(true);
    btnLayout->addWidget(xBtn);
    btnLayout->addWidget(okBtn);

    QObject::connect(xBtn, SIGNAL(pressed()), this, SLOT(reject()));
    QObject::connect(okBtn, SIGNAL(pressed()), this, SLOT(accept()));
}

int DailyEntryDialog::getSelectedDay(){
    return spinBox->getSelectedItemIndex();
}

Entry * DailyEntryDialog::createEntry(){
    Entry *entry;
    QString desc = descBox->text();
    if(desc == ""){
        QSettings settings;
        desc = settings.value("default daily description").toString();
    }
    entry = new Entry(amtBox->text().toInt(), desc);
    return entry;
}


WeeklyEntryDialog::WeeklyEntryDialog(QWidget *parent) : QDialog(parent) {

    layout= new QVBoxLayout;
    setLayout(layout);

    layout->addWidget(new QLabel("Description:"));

    descBox = new QLineEdit;
    QSettings settings;
    descBox->setPlaceholderText(settings.value("default weekly description").toString());
    layout->addWidget(descBox);

    layout->addWidget(new QLabel("Amount:"));

    amtBox = new QLineEdit;
    amtBox->setPlaceholderText("0");
    layout->addWidget(amtBox);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    layout->addLayout(btnLayout);

    xBtn = new QPushButton("X");
    xBtn->setAutoDefault(false);
    okBtn = new QPushButton("OK");
    okBtn->setAutoDefault(true);
    btnLayout->addWidget(xBtn);
    btnLayout->addWidget(okBtn);

    QObject::connect(xBtn, SIGNAL(pressed()), this, SLOT(reject()));
    QObject::connect(okBtn, SIGNAL(pressed()), this, SLOT(accept()));

}

Entry * WeeklyEntryDialog::createEntry(){
    Entry *entry;
    QString desc = descBox->text();
    if(desc == ""){
        QSettings settings;
        desc = settings.value("default weekly description").toString();
    }
    entry = new Entry(amtBox->text().toInt(), desc);
    return entry;
}



InfoWidget::InfoWidget(){

    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    QHBoxLayout *formContainer = new QHBoxLayout;
    mainLayout->addLayout(formContainer);

    QFormLayout *leftFormLayout = new QFormLayout;
    formContainer->addLayout(leftFormLayout);
    QFormLayout *rightFormLayout = new QFormLayout;
    formContainer->addLayout(rightFormLayout);

    QFormLayout *nextDaysBudgetContainer = new QFormLayout;
    mainLayout->addLayout(nextDaysBudgetContainer);

    weeklyBudgetLabel = new QLabel;
    weeklyRemainingLabel = new QLabel;
    todaysBudgetLabel = new QLabel;
    todaysRemainingLabel = new QLabel;
    nextDaysBudgetLabel = new QLabel;

    leftFormLayout->addRow(new QLabel("Weekly Budget: "), weeklyBudgetLabel);
    rightFormLayout->addRow(new QLabel("Remaining:"), weeklyRemainingLabel);

    leftFormLayout->addRow(new QLabel("Today's Budget:"), todaysBudgetLabel);
    rightFormLayout->addRow(new QLabel("Remaining:"), todaysRemainingLabel);

    nextDaysBudgetContainer->addRow(new QLabel("Budget for the next days:"),
                                    nextDaysBudgetLabel);

}

void InfoWidget::updateInfo(int b, int br, int t, int tr, int nd){
    weeklyBudgetLabel->setText(QString::number(b));
    weeklyRemainingLabel->setText(QString::number(br));
    todaysBudgetLabel->setText(QString::number(t));
    todaysRemainingLabel->setText(QString::number(tr));
    nextDaysBudgetLabel->setText(QString::number(nd));
}


WeeklyView::WeeklyView(Database *database) : QWidget(), date() {

    db = database;


    mainLayout = new QVBoxLayout();
    setLayout(mainLayout);

    titleBarLayout = new QHBoxLayout();
    mainLayout->addLayout(titleBarLayout);

    infoWidget = new InfoWidget;
    mainLayout->addWidget(infoWidget);

    buttonsLayout = new QHBoxLayout;
    mainLayout->addLayout(buttonsLayout);

    dailyEntryGroupsScrollArea = new QScrollArea;
    dailyEntryGroupsScrollArea->setWidgetResizable(true);
    mainLayout->addWidget(dailyEntryGroupsScrollArea);

    weekEntryGroupLayout = new QVBoxLayout;
    mainLayout->addLayout(weekEntryGroupLayout);



    titleLabel = new QLabel("This Week");
    titleBarLayout->addWidget(titleLabel);

    addDailyEntryButton = new QPushButton("Add Daily Expense");
    buttonsLayout->addWidget(addDailyEntryButton);
    QObject::connect(addDailyEntryButton, SIGNAL(pressed()), this, SLOT(showDailyEntryDialog()));

    addWeeklyEntryButton = new QPushButton("Add Weekly Expense");
    buttonsLayout->addWidget(addWeeklyEntryButton);
    QObject::connect(addWeeklyEntryButton, SIGNAL(pressed()), this, SLOT(showWeeklyEntryDialog()));


    dailyGroupsStackedWidget = new QStackedWidget;

    noEntriesLabel = new QLabel("No Entries for this week yet");
    dailyGroupsStackedWidget->addWidget(noEntriesLabel);

    dailyGroupsListWidget = new QWidget;
    dailyEntryGroupsListLayout = new QVBoxLayout();
    dailyEntryGroupsListLayout->setSpacing(1);
    dailyGroupsListWidget->setLayout(dailyEntryGroupsListLayout);
    dailyGroupsListWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    dailyGroupsStackedWidget->addWidget(dailyGroupsListWidget);

    for(int i = 0; i < 7; i++){
        dailyEntryGroupsListLayout->addWidget(groups[i]);
    }

    //scrollArea widget has to be set after everything is added to the layouts
    dailyEntryGroupsScrollArea->setWidget(dailyGroupsStackedWidget);

    weekEntryGroupLayout->addWidget(groups[7]);

    //expand week group and today's group by default
    groups[7]->expand();
    groups[QDate::currentDate().dayOfWeek() - 1]->expand();



    date = QDate::currentDate().addDays(-QDate::currentDate().dayOfWeek() + 1);

    loadFromDatabase();
    //If an entry group doesn't have any entries don't display it
    bool allEmpty = true;
    for(int i = 0; i < 7; i++){
        if(groups[i]->getEntries().size() == 0){
            groups[i]->setVisible(false);
        }else {
            allEmpty = false;
        }
    }
    if(allEmpty){
        dailyGroupsStackedWidget->setCurrentWidget(noEntriesLabel);
    }else{
        dailyGroupsStackedWidget->setCurrentWidget(dailyGroupsListWidget);
    }

    for(int i = 0; i < 8; i++){
        bool s = QObject::connect(groups[i], &EntryGroup::entrySelectedSignal,
                         this, &WeeklyView::entrySelectedSlot);
        Q_ASSERT(s);
    }
}

void WeeklyView::loadFromDatabase(){
    for(int i = 0; i < 7; i++){
        QDate d = date.addDays(i);
        groups[i]->setEntries(db->getDayEntries(d));
    }
    groups[7]->setEntries(db->getWeekEntries(date));
    setBudget(db->getWeeklyBudget(date));
}

void WeeklyView::saveToDatabase(){
    for(int i = 0; i < 7; i++){
        QDate d = date.addDays(i);
        db->appendDayEntries(d, groups[i]->getUnsavedEntries());
    }
    db->appendWeekEntries(date, groups[7]->getUnsavedEntries());
    db->setWeeklyBudget(date, budget);

    db->saveDayDataToFile();
    db->saveWeekDataToFile();
}

void WeeklyView::showDailyEntryDialog(){
    dailyEntryDialog = new DailyEntryDialog(this);
    QObject::connect(dailyEntryDialog, SIGNAL(accepted()), this, SLOT(addNewDailyEntry()));
    dailyEntryDialog->exec();
    delete dailyEntryDialog;
}

void WeeklyView::showWeeklyEntryDialog(){
    weeklyEntryDialog = new WeeklyEntryDialog(this);
    QObject::connect(weeklyEntryDialog, SIGNAL(accepted()), this, SLOT(addNewWeeklyEntry()));
    weeklyEntryDialog->exec();
    delete weeklyEntryDialog;
}


void WeeklyView::addNewDailyEntry(){
    EntryGroup *g = groups[dailyEntryDialog->getSelectedDay()];

    g->addEntry(dailyEntryDialog->createEntry());
    saveToDatabase();

    dailyGroupsStackedWidget->setCurrentWidget(dailyGroupsListWidget);

    for(int i = 0; i < 7; i++){
        EntryGroup *e = groups[i];
        if(e->getEntries().size() > 0 && e->isVisible() == false){
            e->expand();
            e->setVisible(true);
        }
    }
    calculateNumbers();
}

void WeeklyView::addNewWeeklyEntry(){
    groups[7]->addEntry(weeklyEntryDialog->createEntry());
    saveToDatabase();
    calculateNumbers();
}

void WeeklyView::entrySelectedSlot(EntryGroup *group, int id){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "", "Remove entry?",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::No){
        return;
    }

    group->removeEntry(id);
    if(group == groups[7]){
        db->removeWeeklyEntry(date ,id);
    }
    else{
        int dayOfTheEntryGroup;
        for(int i = 0; i < 7; i++){
            if(groups[i] == group){
                dayOfTheEntryGroup = i;
            }
        }
        QDate d = date.addDays(dayOfTheEntryGroup);
        db->removeDailyEntry(d, id);
        for(int i = 0; i < 7; i++){
            EntryGroup *e = groups[i];
            if(e->getEntries().size() == 0 && e->isVisible() == true){
                e->collapse();
                e->setVisible(false);
            }
        }

        bool allEmpty = true;
        for(int i = 0; i < 7; i++){
            if(groups[i]->isVisible()){
                allEmpty = false;
                break;
            }

        }
        if(allEmpty){
            dailyGroupsStackedWidget->setCurrentWidget(noEntriesLabel);
        }
    }
    saveToDatabase();
}

void WeeklyView::setBudget(int b){
    budget = b;
    saveToDatabase();
    calculateNumbers();
}

void WeeklyView::calculateNumbers(){
    //TODO: update info widget
    int weekTotal = 0;
    for(int i = 0; i < 8; i++){
        weekTotal += groups[i]->getTotal();
    }
    //remaining amount and the budget for the WEEk
    remaining = budget - weekTotal;


    int currentWeekDay = date.currentDate().dayOfWeek()-1;
    int previousDaysTotal = 0; //how much was spent on previous days
    for(int i = 0; i < currentWeekDay; i++){
        previousDaysTotal += groups[i]->getTotal();
    }
    //when setting the today's budget instead of dividing 'remaining' by the number of
    //days left in the week, divide the 'budget - previousDaysTotal'. In other words
    //don't take today's spendings into account when calculating today's budget so that
    //today's budget doesn't keep changing with every entry.
    todaysBudget = (budget - previousDaysTotal) / (8 - QDate::currentDate().dayOfWeek());
    todaysRemaining = todaysBudget
            - groups[QDate::currentDate().dayOfWeek()-1]->getTotal();

    if(QDate::currentDate().dayOfWeek() == 7)
        dailyBudgetForTheWeek = todaysBudget;
    else
        dailyBudgetForTheWeek = remaining / (7 - QDate::currentDate().dayOfWeek());

    infoWidget->updateInfo(budget, remaining,
                           todaysBudget, todaysRemaining, dailyBudgetForTheWeek);
}









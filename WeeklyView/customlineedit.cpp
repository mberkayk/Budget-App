#include "customlineedit.h"

#include <QDebug>

CustomLineEdit::CustomLineEdit(QString s, QWidget* p) : QLineEdit(s, p) {

}

CustomLineEdit::~CustomLineEdit(){}

void CustomLineEdit::focusOutEvent(QFocusEvent *e){
	QLineEdit::focusOutEvent(e);
	emit focusOut();
}


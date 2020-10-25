#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H

#include <QObject>
#include <QLineEdit>

class CustomLineEdit : public QLineEdit {

Q_OBJECT

signals:
	void focusOut();

public:
	CustomLineEdit(QString, QWidget* p);
	~CustomLineEdit();

	void focusOutEvent(QFocusEvent *) override;
};

#endif // CUSTOMLINEEDIT_H

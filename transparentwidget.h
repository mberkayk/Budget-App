#ifndef TRANSPARENTWIDGET_H
#define TRANSPARENTWIDGET_H

#include <QWidget>

class TransparentWidget : public QWidget {

	Q_OBJECT

public:
	TransparentWidget();
	~TransparentWidget();



	void mousePressEvent(QMouseEvent *event) override;
	void paintEvent(QPaintEvent *event) override;

signals:
	void exit();
};

#endif // TRANSPARENTWIDGET_H

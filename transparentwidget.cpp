#include "transparentwidget.h"

TransparentWidget::TransparentWidget() : QWidget() {}

TransparentWidget::~TransparentWidget(){}

void TransparentWidget::mousePressEvent(QMouseEvent *event){
	emit exit();
}

void TransparentWidget::paintEvent(QPaintEvent *event){
 //do nothing
}

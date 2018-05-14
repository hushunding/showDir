#include "myscrollarea.h"

MyScrollArea::MyScrollArea(QWidget *parent) :
    QScrollArea(parent)
{
}

void MyScrollArea::wheelEvent(QWheelEvent *event)
{
    event->ignore();
}

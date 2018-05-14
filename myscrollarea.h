#ifndef MYSCROLLAREA_H
#define MYSCROLLAREA_H

#include <QScrollArea>
#include <QWheelEvent>

class MyScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    explicit MyScrollArea(QWidget *parent = 0);

signals:

public slots:
protected:
    virtual void wheelEvent(QWheelEvent *event);
};

#endif // MYSCROLLAREA_H

#ifndef MYPICLIST_H
#define MYPICLIST_H

#include <QFileInfoList>
#include <QWidget>
#include "mypiclabel.h"
#include <QWheelEvent>

#define MAX_PIC_IN_LOAD 5

namespace Ui {
class MyPicList;
}

class MyPicList : public QWidget
{
    Q_OBJECT

public:
    explicit MyPicList(QWidget *parent = 0);
    ~MyPicList();
public slots:
    void LoadImages(const QString &fileName);
    void ShowImges(const int newCurPic = 0);

private slots:
    void on_PageUpBtn_clicked();
    void on_PageDownBtn_clicked();

    void on_goBtn_clicked();
    void on_HScroller_valueChanged(int value);
protected:
    virtual void wheelEvent(QWheelEvent *event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);
    virtual void mouseMoveEvent(QMouseEvent * event);
private:
    void ScrollToNextPic(int newCurPic);
    Ui::MyPicList *ui;
    MyPicLabel *pLable[MAX_PIC_IN_LOAD];
    QFileInfoList fileList;
    QString filepath;
    int pageNum;
    int curPage;
    int imagesNum;
    int curPic;
    bool isPress;
    int mouseY;
    int mouseX;
};

#endif // MYPICLIST_H

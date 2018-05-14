#include "mypiclist.h"
#include "ui_mypiclist.h"
#include <QDir>
#include <QDebug>
#include <QScrollBar>


MyPicList::MyPicList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyPicList)
{
    ui->setupUi(this);
    for(int i=0; i<MAX_PIC_IN_LOAD; i++)
    {
        pLable[i] = new MyPicLabel(this);
        ui->picListLayout->insertWidget(i,pLable[i]);
    }
    connect(ui->scrollArea->horizontalScrollBar(),&QScrollBar::valueChanged, this, &MyPicList::on_HScroller_valueChanged);
}

MyPicList::~MyPicList()
{
    delete ui;
}

void MyPicList::LoadImages(const QString &fileName)
{

    if(filepath != fileName)
    {
        filepath = fileName;

        QDir d(fileName);
        QStringList namefilters;
        namefilters << "*.jpg"<<"*.JPG"<<"*.png"<<"*.PNG"<<"*.bmp"<<"*.BMP"<<"*.gif"<<"*.GIF";
        fileList.clear();
        fileList= d.entryInfoList(namefilters);

        imagesNum = fileList.length();
        pageNum = (imagesNum+MAX_PIC_IN_LOAD-1)/MAX_PIC_IN_LOAD;
        curPage = 0;
        ShowImges();
        this->setWindowTitle(fileName);
    }
    show();
    this->setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
}

void MyPicList::ShowImges(const int newCurPic)
{

     ui->totalPage->setText(QString("%1").arg(pageNum));
     ui->curPage->setMaximum(pageNum);
     ui->curPage->setMinimum(1);
     ui->curPage->setValue(curPage+1);
     ui->PageUpBtn->setEnabled(!(curPage == 0));
     ui->PageDownBtn->setEnabled(!(curPage+1 == pageNum));

    int start = MAX_PIC_IN_LOAD*curPage;
    int end = qMin(start+MAX_PIC_IN_LOAD, imagesNum);

    for(int i=start; i<end; i++)
    {
        //qDebug()<<fileList[i].absoluteFilePath();
        pLable[i-start]->LoadImage(fileList[i].absoluteFilePath());
    }
    for(int i = end; i< start+MAX_PIC_IN_LOAD; i++)
    {
         pLable[i-start]->LoadImage(QString());
    }
    curPic = 0;
    int newpos = ui->picListLayout->itemAt(newCurPic)->geometry().x()-ui->picListLayout->itemAt(0)->geometry().x();
    ui->scrollArea->horizontalScrollBar()->setValue(newpos);
}

void MyPicList::on_PageUpBtn_clicked()
{
    curPage--;
    ShowImges();
}

void MyPicList::on_PageDownBtn_clicked()
{
    curPage++;
    ShowImges();
}


void MyPicList::on_goBtn_clicked()
{
    curPage = qMin(ui->curPage->value(), pageNum)-1;
    ShowImges();
}

void MyPicList::on_HScroller_valueChanged(int value)
{
    int maxPic =  imagesNum - MAX_PIC_IN_LOAD*curPage;
    for(int i=0; i<maxPic; i++)
    {
        int diff = ui->picListLayout->itemAt(i)->geometry().x()-ui->picListLayout->itemAt(0)->geometry().x();
        if(diff > value)
        {
            break;
        }
        curPic = i;
    }
     int start = MAX_PIC_IN_LOAD*curPage;

     this->setWindowTitle(fileList[start+curPic].absoluteFilePath());

}

void MyPicList::wheelEvent(QWheelEvent *event)
{
    int newCurPic = curPic - (event->angleDelta().y() > 0 ?1:-1);
    ScrollToNextPic(newCurPic);
}

void MyPicList::mousePressEvent(QMouseEvent *event)
{
#ifndef Q_OS_ANDROID
    if(event->button() == Qt::MidButton)
#endif
    {
        this->setCursor(Qt::ClosedHandCursor);
        isPress = true;
        mouseY = event->y();
        mouseX = event->x();
    }
    event->ignore();
}

void MyPicList::mouseReleaseEvent(QMouseEvent *event)
{
#ifndef Q_OS_ANDROID
    if(event->button() == Qt::MidButton)
#endif
    {
        this->setCursor(Qt::ArrowCursor);
        isPress = false;
    }
    event->ignore();
}

void MyPicList::mouseMoveEvent(QMouseEvent * event)
{
#ifndef Q_OS_ANDROID
    if(isPress)
    {
        ui->scrollArea->verticalScrollBar()->setValue(
                   ui->scrollArea->verticalScrollBar()->value()+mouseY-event->y());

        ui->scrollArea->horizontalScrollBar()->setValue(
                   ui->scrollArea->horizontalScrollBar()->value()+mouseX-event->x());
        qDebug()<<mouseX-event->x();
        mouseY = event->y();
        mouseX = event->x();

    }
#else
    if(isPress)
    {
        ui->scrollArea->verticalScrollBar()->setValue(
                   ui->scrollArea->verticalScrollBar()->value()+mouseY-event->y());

        ui->scrollArea->horizontalScrollBar()->setValue(
                   ui->scrollArea->horizontalScrollBar()->value()+mouseX-event->x());
        mouseY = event->y();
        mouseX = event->x();
    }
#endif

}

void MyPicList::ScrollToNextPic(int newCurPic)
{
    int maxPic =  imagesNum - MAX_PIC_IN_LOAD*curPage;
    if(maxPic > MAX_PIC_IN_LOAD)
    {
        maxPic = MAX_PIC_IN_LOAD-1;
    }

    if(newCurPic < 0)
    {
        if(curPage > 0)
        {
            curPage--;

            maxPic =  imagesNum - MAX_PIC_IN_LOAD*curPage;
                if(maxPic > MAX_PIC_IN_LOAD)
                {
                    maxPic = MAX_PIC_IN_LOAD-1;
                }
            ShowImges(maxPic);
        }
    }
    else if(newCurPic > maxPic)
    {
        if(curPage < pageNum-1)
        {
            curPage++;
            ShowImges();
        }
    }
    else
    {
        curPic = newCurPic;
        int newpos = ui->picListLayout->itemAt(newCurPic)->geometry().x()-ui->picListLayout->itemAt(0)->geometry().x();
        ui->scrollArea->horizontalScrollBar()->setValue(newpos);
    }
}

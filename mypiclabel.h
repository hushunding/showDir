#ifndef MYPICLABEL_H
#define MYPICLABEL_H

#include <QLabel>
#include <QThread>
#include <QDebug>
#include <QMovie>

#define DEFALUT_PIC_WITH 1024

class MyPicLabelThread;

class MyPicLabel : public QLabel
{
	Q_OBJECT
public:
    friend class  MyPicLabelThread;
	MyPicLabel(QWidget *parent);
    virtual ~MyPicLabel()
    {}

public slots:
    void LoadImage(const QString &fileName, QImage *image = nullptr);
    void LoadFinished();
protected:
	virtual void wheelEvent(QWheelEvent *event);

private:
    QImage m_image;
    QMovie m_movie;
    QString m_fileName;
	int m_curWidth;
    bool isImageLoad;
    MyPicLabelThread *myThread;
};

class MyPicLabelThread: public QThread
{
    Q_OBJECT
public:
    MyPicLabelThread(QWidget *parent, MyPicLabel& picLabel);
    virtual ~MyPicLabelThread()
    {

    }
protected:
     virtual void run();
private:
     MyPicLabel& m_picLabel;
};
#endif // MYPICLABEL_H

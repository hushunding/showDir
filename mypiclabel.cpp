#include "mypiclabel.h"
#include <QPoint>
#include <QWheelEvent>
#include <QFileInfo>
#include <QMovie>

MyPicLabel::MyPicLabel(QWidget *parent)
	: QLabel(parent)
{
    isImageLoad = false;
    myThread = new MyPicLabelThread(this, *this);
    connect(myThread,&MyPicLabelThread::finished, this, &MyPicLabel::LoadFinished);
    this->setAlignment(Qt::AlignTop);
}

void MyPicLabel::wheelEvent(QWheelEvent *event )
{
    if(this->parent() == 0)
    {
        if(m_curWidth != 0)
        {
            int aNum = event->angleDelta().y()/4;
            if ((aNum+m_curWidth > 0) &&(aNum+m_curWidth < DEFALUT_PIC_WITH*2))
            {
                m_curWidth = aNum+m_curWidth;
                setPixmap(QPixmap::fromImage(m_image.scaledToWidth(m_curWidth)));
            }
        }
    }
    else
    {
        event->ignore();
    }
}

void MyPicLabel::LoadImage( const QString &fileName, QImage *image )
{
    m_movie.stop();
    setMovie(0);
    if(!fileName.isEmpty())
    {
        m_fileName = fileName;
        QString ext = QFileInfo(fileName).suffix().toLower();

        if(ext == "gif")
        {
            setPixmap(QPixmap());
            m_movie.setFileName(fileName);
            setMovie(&m_movie);
            m_movie.start();
        }
        else if(image == nullptr)
        {
            myThread->start();
            setText("Processing...");
        }
        else
        {
            m_curWidth = image->width()>DEFALUT_PIC_WITH?DEFALUT_PIC_WITH:image->width();
            setPixmap(QPixmap::fromImage(image->scaledToWidth(m_curWidth)));
            setText("");
        }
        setWindowTitle(fileName);
        if(this->parent() == 0)
        {
            show();
        }
    }
    else
    {
        setMovie(0);
        setPixmap(QPixmap());
        setText("");
        qDebug()<<"Clear...";
    }
}

void MyPicLabel::LoadFinished()
{
    if(isImageLoad)
    {
        m_curWidth = m_image.width()>DEFALUT_PIC_WITH?DEFALUT_PIC_WITH:m_image.width();
        setPixmap(QPixmap::fromImage(m_image.scaledToWidth(m_curWidth)));
        setText("");
        if(this->parent() == 0)
        {
            this->resize(this->pixmap()->size());
        }
    }
    else
    {
        m_curWidth =0;
        setPixmap(QPixmap());
        setText("Load Failed");
    }
}

MyPicLabelThread::MyPicLabelThread(QWidget *parent, MyPicLabel &picLabel)
    :QThread(parent),m_picLabel(picLabel)
{

}

void MyPicLabelThread::run()
{
    //qDebug()<<"Load "<<m_picLabel.m_fileName;
    m_picLabel.isImageLoad = m_picLabel.m_image.load(m_picLabel.m_fileName);
}

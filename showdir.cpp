#include "showdir.h"
#ifdef HAS_WEBVIEW
#include <QWebEngineView>
#endif
#include <QImage>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QDebug>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QUrl>


ShowDir::ShowDir(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
    ui.stackedWidget->setCurrentIndex(0);
	ui.treeView->setModel(&dirModeTree);
	ui.listView->setModel(&dirModeList);
	ui.treeView->setColumnHidden(1,true);
	ui.treeView->setColumnHidden(2,true);
	ui.treeView->setColumnHidden(3,true);
	dirModeTree.setFilter(QDir::Dirs);
	//dirModeList.setFilter(QDir::Dirs);
	dirModeList.setFilter(QDir::AllDirs|QDir::Files);
	//connect(ui.treeView,SIGNAL(clicked(QModelIndex)),&dirMode,SLOT(refresh(QModelIndex)));
	setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    webView =  new QWebEngineView();
#ifdef HAS_WEBVIEW
    webView->setBaseSize(1024,768);
	connect(webView,SIGNAL(loadFinished(bool)), this, SLOT(SetTile(bool)));
#endif
	picLabel = new MyPicLabel(0);
	picLabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	this->move(10,200);

	connect(ui.listView, SIGNAL(selectChange(QModelIndex)),this,SLOT(showFile(QModelIndex)));
    QString startPath;
#ifdef Q_OS_ANDROID
    startPath = "/mnt/sdcard/pemp1/pic/";
#else
    startPath = QDir::currentPath();
#endif
    TreeClick(dirModeTree.index(startPath));
    ListClick(dirModeList.index(startPath));
    db = QSqlDatabase::addDatabase("QSQLITE");
    model = new QSqlTableModel(ui.picTable, db);

//    connect(ui.picTable->horizontalHeader(), &QHeaderView::sortIndicatorChanged,
//            this,&ShowDir::sortPicTable);
    picList = new MyPicList();
    IsUseList = true;


}

ShowDir::~ShowDir()
{
#ifndef HAS_WEBVIEW
    delete webView;
#endif
    delete picList;
    delete picLabel;
}
void ShowDir::SetTile(bool ok)
{
#if HAS_WEBVIEW
    webView->setWindowTitle(webView->page()->title());
#endif
}

void ShowDir::sortPicTable(int index, Qt::SortOrder ord)
{
    model->sort(index,ord);

}
bool isPicFile(const QString &ext)
{
    if( ext == QLatin1String("jpg") || ext == QLatin1String("JPG")
            ||ext == QLatin1String("gif") || ext == QLatin1String("GIF")
            ||ext == QLatin1String("bmp") || ext == QLatin1String("BMP"))
   {
       return true;
   }
   else
   {
       return false;
   }
}
void ShowDir::showFile(const QModelIndex & index)
{
	QFileInfo fi = dirModeList.fileInfo(index);
	QString fileName;
	int type = 0;
	if(!fi.isFile())
	{
        ui.entryBtn->setEnabled(true);
        QVariant data = dirModeList.data(index);
        //qDebug()<<data.typeName()<<data.toString();
        if(data.toString() == ".." || data.toString() == ".")
        {
            qDebug()<<data.toString();
        }
        else
        {
            QDir d(dirModeList.filePath(index));
            QStringList namefilters;
            namefilters << "page_s.html";
            QFileInfoList fielList = d.entryInfoList(namefilters);
            if (!fielList.empty() && !IsUseList)
            {
                fileName = fielList[0].absoluteFilePath();
                type =1;

            }
            else
            {
                namefilters.clear();
                namefilters << "*.jpg"<<"*.JPG"<<"*.png"<<"*.PNG"<<"*.bmp"<<"*.BMP"<<"*.gif"<<"*.GIF";
                fielList= d.entryInfoList(namefilters);
                if (!fielList.empty() )
                {
                    fileName = fi.absoluteFilePath();
                    type =4;
                }
            }
        }

	}
	else
	{
        ui.entryBtn->setEnabled(false);
		if (fi.suffix() == QLatin1String("html"))
		{
			fileName = fi.absoluteFilePath();
			type =1;
		}
        else if(isPicFile(fi.suffix()))
		{

			fileName = fi.absoluteFilePath();
			type =2;
		}
        else if(fi.suffix() == QLatin1String("db3"))
        {
            fileName = fi.absoluteFilePath();
            type =3;
        }
	}
	switch(type)
	{
	case 1: 
		{
			QUrl url =QUrl::fromUserInput(fileName);

			webView->load(url);
            webView->show();

			picLabel->close();
            picList->close();
            picBrower= webView;
		}
		break;
	case 2:
		{
			picLabel->LoadImage(fileName);
			webView->close();
            picList->close();
            picBrower = picLabel;
		}
		break;
    case 4:
        {
           picList->LoadImages(fileName);
           webView->close();
           picLabel->close();
           picBrower = picList;
        }
        break;
    case 3:
        {
            ui.stackedWidget->setCurrentIndex(1);
#ifndef Q_OS_ANDROID
            this->resize(500,500);
#endif
            LoadDbFile(fileName);
        }

	default:
		webView->close();
		picLabel->close();
        picList->close();
		break;
	}
	this->activateWindow();
}

	void ShowDir::TreeClick(const QModelIndex & index)
	{
		QModelIndex listIndex = dirModeList.index(dirModeTree.filePath(index));
		ui.listView->setRootIndex(listIndex);
        dirModeList.refresh(listIndex);
	}
	void ShowDir::ListClick(const QModelIndex & index)
	{

        QFileInfo fi = dirModeList.fileInfo(index);
        if(fi.isDir())
        {
            ui.listView->setRootIndex(index);
        }

        //ui.treeView->expand(indext);
	}
void ShowDir::on_exitBtn_clicked()
{
	webView->close();
	picLabel->close();
    ui.stackedWidget->setCurrentIndex(0);
#ifndef Q_OS_ANDROID
    this->resize(250,500);
#endif
	//setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
}

void ShowDir::LoadDbFile(QString filename)
{
    //setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);

    if(!db.isOpen())
    {
        db.setDatabaseName(filename);
        if(!db.open())
        {
            //错误处理
        }
        //model->setQuery(QString("select * from pictrues;"));
        model->setTable(QString("pictrues"));
//        model->setHeaderData(0,Qt::Horizontal,QObject::tr("date"));
//        model->setHeaderData(1,Qt::Horizontal,QObject::tr("name"));
//        model->setHeaderData(2,Qt::Horizontal,QObject::tr("title"));
        model->select();
        ui.picTable->setModel(model);
#ifdef Q_OS_ANDROID
        ui.picTable->setColumnHidden(3,true);
        ui.picTable->setColumnHidden(4,true);
#endif
        ui.picTable->setColumnWidth(0,80);
        ui.picTable->setColumnWidth(1,80);
        ui.picTable->setColumnWidth(2,400);
        ui.picTable->setToolTip(filename);

        QSqlQueryModel *cmodel = new QSqlQueryModel(this);
        cmodel->setQuery(QString("select name from pictrues group by name;"));
        ui.picFilterName->addItems(QStringList()<<"date"<<"name");
        ui.picFilterValue->setModel(cmodel);
       // ui.picFilterValue->setCurrentIndex(-1);
        ui.picFilterValue->hidePopup();
    }
}

void ShowDir::on_picTable_clicked(const QModelIndex &index)
{
    QSqlRecord re = model->record(index.row());

    QString path = re.value(3).toString();
    //qDebug() << path;
    QFileInfo fi(ui.picTable->toolTip());
    QString picPath =  fi.absolutePath()+QDir::separator()+path;
    //qDebug() << picPath;

    showFile(dirModeList.index(picPath));
}

void ShowDir::OnDestroy()
{
	webView->close();
	picLabel->close();
    picList->close();
}

void ShowDir::closeEvent( QCloseEvent * event )
{
    OnDestroy();
	 event->accept();
}

void ShowDir::on_picFilterValue_editTextChanged(const QString &arg1)
{
//    if(arg1.length()>0)
//    {
//        QSqlQueryModel *cmodel = dynamic_cast<QSqlQueryModel*>(ui.picFilterValue->model());
//        cmodel->setQuery(QString("select name from pictrues where name =%1* group by name;").arg(arg1));
//        ui.picFilterValue->showPopup();
//        ui.picFilterValue->setFocus();
//    }
    if(arg1.length()>0)
    {
    }
    else
    {
        model->setFilter(QString());
        model->select();
    }

}

void ShowDir::on_picFilterValue_currentIndexChanged(const QString &arg1)
{
//    if(arg1.length()>0)
//    {
//    model->setFilter(QString("name=%1").arg(arg1));
//    qDebug()<< QString("name=%1").arg(arg1);
//    model->select();
//    }
//    else
//    {
//        model->setFilter(QString());
//        model->select();
//    }
}

void ShowDir::on_picFilterBtn_clicked()
{
    const QString &arg1 = ui.picFilterValue->currentText();

    if(arg1.length()>0)
    {
        QString filter;
        if(arg1[0] == '/')
        {
            filter = arg1.mid(1);
        }
        else
        {
            filter = QString("name=\"%1\"").arg(arg1);
        }


        model->setFilter(filter);
        qDebug()<< filter;
        model->select();
    }
    else
    {
        model->setFilter(QString());
        model->select();
    }
}

void ShowDir::on_UseList_stateChanged(int arg1)
{
    IsUseList = (arg1==Qt::Checked);
}

void ShowDir::on_entryBtn_clicked()
{
    ui.listView->setRootIndex(ui.listView->selectionModel()->currentIndex());
}

void ShowDir::on_picTable_customContextMenuRequested(const QPoint &pos)
{
   QModelIndex index =  ui.picTable->indexAt(pos);
   QSqlRecord re = model->record(index.row());
   QString path = re.value(3).toString();
   QFileInfo fi(ui.picTable->toolTip());
   QString picPath =  fi.absolutePath()+QDir::separator()+path;

   QString ulrstr = QString("file:///")+picPath.replace(QDir::separator(), "/");

   QDesktopServices::openUrl(QUrl(ulrstr));

}

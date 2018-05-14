#ifndef SHOWDIR_H
#define SHOWDIR_H

#include <QtWidgets/QMainWindow>
#include "ui_showdir.h"
#include "qdirmodel.h"
#ifdef HAS_WEBVIEW
#include <QWebEngineView>
#endif

#include <QFileSystemModel>
#include <QSqlTableModel>
#include "mypiclabel.h"
#include "mypiclist.h"

#ifndef HAS_WEBVIEW
class QWebEngineView:public QWidget
{
public:
    QWebEngineView()
    {}
    ~QWebEngineView()
    {}
    void show()
    {}
    void close()
    {}
    void load(QUrl)
    {}
};

#endif
class ShowDir : public QMainWindow
{
	Q_OBJECT

public:
	ShowDir(QWidget *parent = 0);
	~ShowDir();
public slots:
	void showFile(const QModelIndex & index);
	void TreeClick(const QModelIndex & index);
	void ListClick(const QModelIndex & index);
	void SetTile(bool ok);
    void sortPicTable(int index,Qt::SortOrder ord);

protected:
	virtual void closeEvent(QCloseEvent * event);
private slots:
    void on_exitBtn_clicked();

    void on_picTable_clicked(const QModelIndex &index);
	void OnDestroy();

    void on_picFilterValue_editTextChanged(const QString &arg1);

    void on_picFilterValue_currentIndexChanged(const QString &arg1);

    void on_picFilterBtn_clicked();

    void on_UseList_stateChanged(int arg1);

    void on_entryBtn_clicked();

    void on_picTable_customContextMenuRequested(const QPoint &pos);

private:
    void LoadDbFile(QString filename);

	Ui::ShowDirClass ui;
	QDirModel dirModeTree;
	QDirModel dirModeList;
    QWebEngineView *webView;
	MyPicLabel *picLabel;
	QWidget *picBrower;
    QSqlTableModel *model;
    QSqlDatabase db;
    MyPicList *picList;
    bool IsUseList;
};

#endif // SHOWDIR_H

#ifndef MYLISTVIEW_H
#define MYLISTVIEW_H

#include <QListView>

class MyListView : public QListView
{
	Q_OBJECT

public:
	MyListView(QWidget *parent);
	~MyListView();
signals:
	void selectChange(const QModelIndex & current);
private:
	virtual void keyPressEvent(QKeyEvent * event);
	QModelIndex const *curIndex;
};

#endif // MYLISTVIEW_H

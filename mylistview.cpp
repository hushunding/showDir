#include "mylistview.h"

MyListView::MyListView(QWidget *parent)
	: QListView(parent)
{
	curIndex = NULL;
}

MyListView::~MyListView()
{

}

void MyListView::keyPressEvent(QKeyEvent * event)
{
	QListView::keyPressEvent(event);
	if(!selectedIndexes().isEmpty())
	{
		if (curIndex != &selectedIndexes().first())
		{
			curIndex = &selectedIndexes().first();
			emit selectChange(selectedIndexes().first());
		}

	}
}

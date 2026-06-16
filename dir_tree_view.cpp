#include "dir_tree_view.h"
#include <QScroller>

DirTreeView::DirTreeView(QWidget *parent)
    : QTreeView(parent)
{

}

void DirTreeView::setup()
{
    this->setAnimated(false);
    this->setIndentation(20);

    this->setSortingEnabled(true); // TODO: custom sorting further

    QScroller::grabGesture(this, QScroller::TouchGesture);
}

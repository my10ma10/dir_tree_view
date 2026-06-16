#ifndef DIRTREEVIEW_H
#define DIRTREEVIEW_H

#include <QWidget>
#include <QTreeView>

class DirTreeView : public QTreeView
{
public:
    DirTreeView(QWidget *parent = nullptr);

    void setup();

};

#endif // DIRTREEVIEW_H

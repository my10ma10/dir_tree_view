#ifndef DIRTREEMODEL_H
#define DIRTREEMODEL_H

#include <QObject>
#include <QFileSystemModel>

class DirTreeModel : public QFileSystemModel
{
public:
    DirTreeModel(QObject *parent = nullptr);

private:
    // QHash for dirs sizes
};

#endif // DIRTREEMODEL_H

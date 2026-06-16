#ifndef DIRTREEMODEL_H
#define DIRTREEMODEL_H

#include <QObject>
#include <QFileSystemModel>

class DirTreeModel : public QObject
{
public:
    DirTreeModel(QObject *parent = nullptr);

    QFileSystemModel* getModelPtr();

    void setOption(QFileSystemModel::Option option, bool on = true);
    QModelIndex index(const QString& path) const;

private:
    QFileSystemModel model_;

    // QHash for dirs sizes
};

#endif // DIRTREEMODEL_H

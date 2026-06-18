#ifndef DIRFILTERMODEL_H
#define DIRFILTERMODEL_H

#include <QSortFilterProxyModel>
#include <QWidget>

enum class SearchDepth { Shallow, Deep };

class DirFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit DirFilterModel(QObject *parent = nullptr);

    void setRootPath(const QString& path);
    void setSearchDepth(SearchDepth depth);

    SearchDepth getSearchDepth() const;

protected:
    bool filterAcceptsRow(int sourceRow,
                const QModelIndex &sourceParent) const override;

    bool matches(const QModelIndex& index, const QString& elemName) const;

//    bool hasMatchingAncestor(const QModelIndex& index) const;
    bool hasMatchingDescendant(const QModelIndex& index) const;


    QString rootPath_;
    SearchDepth searchDepth_ = SearchDepth::Shallow;
};

#endif // DIRFILTERMODEL_H

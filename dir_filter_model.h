#ifndef DIRFILTERMODEL_H
#define DIRFILTERMODEL_H

#include <QSortFilterProxyModel>
#include <QWidget>
#include <QSet>

enum class SearchDepth { Shallow, Deep };

class DirFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit DirFilterModel(QObject *parent = nullptr);

    void setRootPath(const QString& path);
    void setSearchDepth(SearchDepth depth);
    void setMatchedPaths(const QSet<QString> paths);
    void setSearchText(const QString& text);

    SearchDepth getSearchDepth() const;

protected:
    bool filterAcceptsRow(int sourceRow,
                const QModelIndex &sourceParent) const override;


    QSet<QString> matchedPaths_;
    QString rootPath_;
    SearchDepth searchDepth_ = SearchDepth::Shallow;
    QString currentSearchPattern_;
};

#endif // DIRFILTERMODEL_H

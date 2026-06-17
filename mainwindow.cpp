#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDir>
#include <QFileIconProvider>
#include <QScreen>
#include <QCommandLineOption>


MainWindow::MainWindow(const QApplication& app, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dir_model_ = new DirTreeModel(this);
    dir_view_ = new DirTreeView(this);
    sort_filter_model_ = new QSortFilterProxyModel(this);


    QCommandLineParser parser;
    parser.process(app);
    parseCommandLine(parser);

    sort_filter_model_->setSourceModel(dir_model_);
    dir_view_->setModel(sort_filter_model_);

    const QString path = parser.positionalArguments().isEmpty()
        ? QString(QDir::homePath()) : parser.positionalArguments().first();

    if (!path.isEmpty()) {
        const QModelIndex sourceIndex = dir_model_->index(QDir::cleanPath(path));
        if (sourceIndex.isValid()) {
            const QModelIndex proxyIndex = sort_filter_model_->mapFromSource(sourceIndex);
            dir_view_->setRootIndex(proxyIndex);
        }
    }

    dir_view_->setup();

    const QSize availableSize = dir_view_->screen()->availableGeometry().size();
    dir_view_->resize(availableSize / 2);
    dir_view_->setColumnWidth(0, dir_view_->width() / 3);


    dir_view_->setWindowTitle("Директории");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::parseCommandLine(QCommandLineParser& parser)
{
    parser.setApplicationDescription("Утилита отображения дерева файловой системы");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption dontUseCustomDirectoryIconsOption("c", "Отключить отображение пользовательских иконок.");
    parser.addOption(dontUseCustomDirectoryIconsOption);

    QCommandLineOption dontWatchOption("w", "Отключить отслеживание изменений.");
    parser.addOption(dontWatchOption);

    parser.addPositionalArgument("директория", "Начальная директория.");

    if (parser.isSet(dontUseCustomDirectoryIconsOption)) {
        dir_model_->setOption(QFileSystemModel::DontUseCustomDirectoryIcons);
    }
    if (parser.isSet(dontWatchOption)) {
        dir_model_->setOption(QFileSystemModel::DontWatchForChanges);
    }

}


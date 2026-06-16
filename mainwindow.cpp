#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDir>
#include <QFileIconProvider>
#include <QScreen>
#include <QCommandLineParser>
#include <QCommandLineOption>


MainWindow::MainWindow(const QApplication& app, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model_ = new DirTreeModel(this);
    view_ = new DirTreeView(this);



    QCommandLineParser parser;
    parser.setApplicationDescription("Утилита отображения дерева файловой системы");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption dontUseCustomDirectoryIconsOption("c", "Отключить отображение пользовательских иконок.");
    parser.addOption(dontUseCustomDirectoryIconsOption);

    QCommandLineOption dontWatchOption("w", "Отключить отслеживание изменений.");
    parser.addOption(dontWatchOption);

    parser.addPositionalArgument("директория", "Начальная директория.");
    parser.process(app);

    const QString rootPath = parser.positionalArguments().isEmpty()
        ? QString(QDir::homePath()) : parser.positionalArguments().first();



    if (parser.isSet(dontUseCustomDirectoryIconsOption)) {
        model_->setOption(QFileSystemModel::DontUseCustomDirectoryIcons);
    }
    if (parser.isSet(dontWatchOption)) {
        model_->setOption(QFileSystemModel::DontWatchForChanges);
    }


    view_->setModel(model_->getModelPtr());

    if (!rootPath.isEmpty()) {
        const QModelIndex rootIndex = model_->index(QDir::cleanPath(rootPath));
        if (rootIndex.isValid()) {
            view_->setRootIndex(rootIndex);
        }
    }

    view_->setup();

    const QSize availableSize = view_->screen()->availableGeometry().size();
    view_->resize(availableSize / 2);
    view_->setColumnWidth(0, view_->width() / 3);


    view_->setWindowTitle(QObject::tr("Dir View"));
}

MainWindow::~MainWindow()
{
    delete ui;
}


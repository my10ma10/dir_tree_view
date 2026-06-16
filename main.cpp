#include "mainwindow.h"

#include <QApplication>
#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QScreen>
#include <QScroller>
#include <QTreeView>
#include <QCommandLineParser>
#include <QCommandLineOption>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

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


    QFileSystemModel model;
    model.setRootPath("");

    if (parser.isSet(dontUseCustomDirectoryIconsOption)) {
        model.setOption(QFileSystemModel::DontUseCustomDirectoryIcons);
    }
    if (parser.isSet(dontWatchOption)) {
        model.setOption(QFileSystemModel::DontWatchForChanges);
    }

    QTreeView tree;
    tree.setModel(&model);

    if (!rootPath.isEmpty()) {
        const QModelIndex rootIndex = model.index(QDir::cleanPath(rootPath));
        if (rootIndex.isValid()) {
            tree.setRootIndex(rootIndex);
        }
    }

    // Demonstrating look and feel features
//    tree.setAnimated(false);
    tree.setIndentation(20);
    tree.setSortingEnabled(true);

    const QSize availableSize = tree.screen()->availableGeometry().size();
    tree.resize(availableSize / 2);
    tree.setColumnWidth(0, tree.width() / 3);

    // Make it flickable on touchscreens
    QScroller::grabGesture(&tree, QScroller::TouchGesture);

    tree.setWindowTitle(QObject::tr("Dir View"));
    tree.show();

    return app.exec();
}

#include "mainwindow.hpp"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile styleFile(":/neutral/stylesheets/darkorange.qss");

    styleFile.open(QFile::ReadOnly);

    const QString style(styleFile.readAll());

    app.setStyleSheet(style);
    app.setApplicationDisplayName("Pushup - An graphical shell for the Up language");
    app.setApplicationName("Pushup");

    QIcon appIcon(":/neutral/icons/app.png");

    app.setWindowIcon(appIcon);

    MainWindow win;

    win.showMaximized();

    return app.exec();
}

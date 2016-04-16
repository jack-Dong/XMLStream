#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QXmlStreamReader>
#include <QStringList>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <cassert>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool readFile(const QString &fileName);

private:
    void readBookindexElemnt();
    void readEntryElement(QTreeWidgetItem *parent);
    void readPageElement(QTreeWidgetItem *parent);
    void skipeUnknownElement();

    QTreeWidget *treeWiget;
    QXmlStreamReader reader;
};

#endif // MAINWINDOW_H

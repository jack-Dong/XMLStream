#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("XML Reader"));
    
    treeWiget = new QTreeWidget(this);
    QStringList header;
    header <<"Iterms"<<"Pages";
    treeWiget->setHeaderLabels(header);
    setCentralWidget(treeWiget);
}

MainWindow::~MainWindow()
{

}

bool MainWindow::readFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Cannot read file %1").arg(fileName)
                              );
        return false;
    }
    reader.setDevice(&file);
    while (!reader.atEnd())
    {
        if(reader.isStartElement())
        {
//            qDebug() <<reader.name();
            if(reader.name() == "bookindex")
            {
                readBookindexElemnt();
            } else
            {
                reader.raiseError("Not a valid book file");
            }
        } else
        {
            reader.readNext();
        }
        
//        qDebug() << reader.name();
//        reader.readNext();
//        qDebug() <<"name:"<<reader.name() <<"  "
//                 <<"text:"<<reader.text();
//        if(reader.isStartDocument())
//        {
//            qDebug() << "StartDocument";
//        }else if(reader.isEndDocument())
//        {
//            qDebug() << "EndDocument";
//        }else if(reader.isStartElement())
//        {
//            qDebug() << "StartElement";
//        }else if( reader.isEndElement())
//        {
//             qDebug() << "EndElement";
//        }else if( reader.isCDATA())
//        {
//            qDebug() << "CDATA";
//        }
        
//        reader.readNext();
        
    }
    file.close();
    
    if(reader.hasError())
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Faileed to parse file %1\n info:%2").arg(fileName,
                                                                           reader.errorString())
                              );
        return false;
    }
    return true;
}

void MainWindow::readBookindexElemnt()
{
    assert(reader.isStartElement() && reader.name() == "bookindex");
    reader.readNext();
    while(!reader.atEnd())
    {
        if(reader.isEndElement())
        {
            reader.readNext();
            break;
        }
        
        if(reader.isStartElement())
        {
            if(reader.name() == "entry")
            {
                readEntryElement(treeWiget->invisibleRootItem());
            }else
            {
                skipeUnknownElement();
            }
        }else
        {
            reader.readNext();
        }
    }
}

void MainWindow::readEntryElement(QTreeWidgetItem *parent)
{
    QTreeWidgetItem *iterm = new QTreeWidgetItem(parent);
    iterm->setText(0,reader.attributes().value("term").toString());
    
    reader.readNext();    
    while(!reader.atEnd())
    {
        if(reader.isEndElement())
        {
            reader.readNext();
            break;
        }
        
        if(reader.isStartElement())
        {
            if(reader.name() == "entry")
            {
                readEntryElement(iterm);
            }else if(reader.name() == "page")
            {
                readPageElement(iterm);
            }else
            {
                skipeUnknownElement();
            }
        }else
        {
            reader.readNext();
        }
    }
}

void MainWindow::readPageElement(QTreeWidgetItem *parent)
{
    QString page = reader.readElementText();
    if(reader.isEndElement())
    {
        reader.readNext();
    }
    
    QString allpages = parent->text(1);
    if(!allpages.isEmpty())
    {
        allpages+=" ,";
    }
    allpages += page;
    parent->setText(1,allpages);
}

void MainWindow::skipeUnknownElement()
{
    reader.readNext();
    while(!reader.atEnd())
    {
        if(reader.isEndElement())
        {
            reader.readNext();
            break;      
        }
        
        if(reader.isStartElement())
        {
            skipeUnknownElement();
        }else
        {
            reader.readNext();
        }
    }
}

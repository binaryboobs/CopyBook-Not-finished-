#include "copybook.h"
#include <QCoreApplication>
#include <QDebug>
#include <QRegExp>
#include <QMessageBox>
#include <QFileDialog>
#include <QPixmap>

void CopyBook::slotOpen()
{
    globPath = QFileDialog::getOpenFileName(nullptr, "", "C://", "*.json");
    file.setFileName(globPath);
    slotLoad();
    lcdNum1->display(pages[0]);
    lcdNum2->display(pages[1]);

}

void CopyBook::slotSave()
{
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QVariantMap map;

        map.insert( "Page", pageCounter);
        map.insert( "TextFirst", pageLeft->toPlainText());
        map.insert( "TextSecond", pageRight->toPlainText());

        QJsonObject json = QJsonObject::fromVariantMap(map);

        QJsonArray docToWrite = doc.object().value("pages").toArray();

        if(docToWrite.size() >= pageCounter-1 && docToWrite.size() > 0)
        {
            file.seek(0);

            docToWrite.removeAt(pageCounter-1);
            docToWrite.insert(pageCounter-1,json);

            doc.setArray(docToWrite);

            file.write("{\"pages\":" + doc.toJson() + "}");
            file.close();

        }
        else
        {
            docToWrite.append(json);
            doc.setArray(docToWrite);

            file.write("{\"pages\":" + doc.toJson() + "}");
            file.close();
        }

    }
}

void CopyBook::slotLoad()
{
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        doc = QJsonDocument::fromJson(QByteArray(file.readAll()), &docError);
        file.close();
    }
    else{qCritical()<<"error" << Qt::endl;}

    if(docError.errorString().toInt() == QJsonParseError::NoError)
    {
        docAr = QJsonValue(doc.object().value("pages")).toArray();
        pageLeft->setPlainText(docAr.at(pageCounter - 1).toObject().value("TextFirst").toString());
        pageRight->setPlainText(docAr.at(pageCounter - 1).toObject().value("TextSecond").toString());
    }
    else
        qCritical() << "PASHEL";

    file.close();

}

void CopyBook::slotPrev()
{
    pageCounter--;
    if(pageCounter == 0)
    {
       QMessageBox::information(this, "", "This is the first page.");
       pageCounter++;
       return;
    }
    pageLeft->clear();
    pageRight->clear();
    slotLoad();

    pages[0]-=2;
    pages[1]-=2;

    lcdNum1->display(pages[0]);
    lcdNum2->display(pages[1]);
}

void CopyBook::slotNext()
{
    pageCounter++;
    pageLeft->clear();
    pageRight->clear();
    slotLoad();

    pages[0]+=2;
    pages[1]+=2;

    lcdNum1->display(pages[0]);
    lcdNum2->display(pages[1]);

}

void CopyBook::createCopyBook()
{   

    QHBoxLayout* lHorLayout   = new QHBoxLayout;
    QHBoxLayout* lHorlayout_2 = new QHBoxLayout;
    QHBoxLayout* lHorLayout_3 = new QHBoxLayout;
    QVBoxLayout* lVertLayout  = new QVBoxLayout;

    pushButtonPrev = new QPushButton("Prev");
    pushButtonSave = new QPushButton("Save");
    pushButtonNext = new QPushButton("Next");
    pushButtonLoad = new QPushButton("Load");
    pushButtonOpen = new QPushButton("Open");

    lcdNum1 = new QLCDNumber;
    lcdNum1->setStyleSheet("border: 0");
    lcdNum1->setPalette(Qt::yellow);

    lcdNum2 = new QLCDNumber;
    lcdNum2->setStyleSheet("border: 0");
    lcdNum2->setPalette(Qt::yellow);

    pageLeft  = new QPlainTextEdit;
    pageLeft->setStyleSheet(QString::fromUtf8("font-size:20px;"));

    pageRight = new QPlainTextEdit;
    pageRight->setStyleSheet(QString::fromUtf8("font-size:20px;"));

    lHorLayout->addWidget   (pushButtonPrev);
    lHorLayout->addWidget   (pushButtonSave);
    lHorLayout->addWidget   (pushButtonLoad);
    lHorLayout->addWidget   (pushButtonNext);

    lHorlayout_2->addWidget(pageLeft);
    lHorlayout_2->addWidget(pageRight);

    lHorLayout_3->addWidget(lcdNum1);
    lHorLayout_3->addWidget (pushButtonOpen);
    lHorLayout_3->addWidget(lcdNum2);

    lVertLayout->addLayout(lHorLayout_3);
    lVertLayout->addLayout(lHorlayout_2);
    lVertLayout->addLayout(lHorLayout);

    setLayout(lVertLayout);

    connect(pushButtonSave, SIGNAL(clicked()), this, SLOT(slotSave()));
    connect(pushButtonLoad, SIGNAL(clicked()), this, SLOT(slotLoad()));
    connect(pushButtonPrev, SIGNAL(clicked()), this, SLOT(slotPrev()));
    connect(pushButtonNext, SIGNAL(clicked()), this, SLOT(slotNext()));
    connect(pushButtonOpen, SIGNAL(clicked()), this, SLOT(slotOpen()));
}

CopyBook::CopyBook(QWidget *parent):QWidget(parent)
{
    resize(300,150);
    setWindowTitle("CopyBook");
    createCopyBook();
}

CopyBook::~CopyBook()
{
}


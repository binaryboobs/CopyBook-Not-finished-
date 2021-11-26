#ifndef COPYBOOK_H
#define COPYBOOK_H

#include <QMainWindow>
#include <QDialog>
#include <QTextEdit>
#include <QPushButton>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QPlainTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMenuBar>
#include <QMenu>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QLCDNumber>

class CopyBook : public QWidget
{
    Q_OBJECT

    QJsonDocument   doc;
    QJsonArray      docAr;
    QJsonParseError docError;
    QFile           file;
    QString         globPath;
public:
    explicit CopyBook(QWidget* parent = 0);
    ~CopyBook();

public:
    void createCopyBook();
    int pageCounter = 1;
    int pages[2] = {1,2};

private:
    QPlainTextEdit *pageLeft;
    QPlainTextEdit *pageRight;

    QPushButton* pushButtonNext;
    QPushButton* pushButtonPrev;
    QPushButton* pushButtonSave;
    QPushButton* pushButtonLoad;

    QLCDNumber* lcdNum1;
    QLCDNumber* lcdNum2;

    QTextEdit*   leftSideCounter;
    QTextEdit*   rightSideCounter;

    QPushButton* pushButtonOpen;

    bool pageBusy(QString str);

private slots:
    void slotSave();
    void slotLoad();
    void slotPrev();
    void slotNext();
    void slotOpen();
};
#endif // COPYBOOK_H

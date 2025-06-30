#ifndef PRINTERMANAGE_H
#define PRINTERMANAGE_H

#include <QObject>
#include <QTextDocument>
#include <QtPrintSupport/QPrinter>

class PrinterManage : public QObject
{
    Q_OBJECT
public:
    PrinterManage();
    Q_INVOKABLE void imprimerAbsenceSeance();
private:
    void print();
    QPrinter m_printer;
    QTextDocument m_doc;
    QString m_html;
};

#endif // PRINTERMANAGE_H

#ifndef PRINTERMANAGE_H
#define PRINTERMANAGE_H
#include "misc.h"
#include "absencemodel.h"
#include <QObject>
#include <QTextDocument>
#include <QtPrintSupport/QPrinter>

class PrinterManage : public QObject
{
    Q_OBJECT
public:
    PrinterManage();
    Q_INVOKABLE void startPrinting(const QString &);
    void imprimerAbsenceSeance();
    void setAbsenceModel(QObject *);
signals:
    void s_printAbsence();
private:
    void print();
    QPrinter m_printer;
    QTextDocument m_doc;
    QString m_html;
    QObject* m_model = nullptr;
};

#endif // PRINTERMANAGE_H

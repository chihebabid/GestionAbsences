#ifndef PRINTERMANAGE_H
#define PRINTERMANAGE_H
#include "misc.h"
#include "absencemodel.h"
#include <QObject>
#include <QTextDocument>
#include <QtPrintSupport/QPrinter>
#include <QPainter>

class PrinterManage : public QObject
{
    Q_OBJECT
public:
    PrinterManage();
    Q_INVOKABLE void startPrinting(const QString &);
    void imprimerAbsenceSeance();
    void imprimerSynthese();
    void setModel(QObject *);
signals:
    void s_printAbsence();
    void s_printSynthese();
private:
    bool preprint(const QString &);
    QPrinter m_printer;
    QPainter m_painter;
    QObject* m_model = nullptr;
};

#endif // PRINTERMANAGE_H

// New canonical header: PrinterManager
#ifndef PRINTERMANAGER_H
#define PRINTERMANAGER_H
#include "misc.h"
#include "absencemodel.h"
#include <QObject>
#include <QTextDocument>
#include <QtPrintSupport/QPrinter>
#include <QPainter>

class PrinterManager : public QObject
{
    Q_OBJECT
public:
    PrinterManager();
    Q_INVOKABLE void startPrinting(const QString &);
    void imprimerAbsenceSeance();
    void imprimerSynthese();
    void setModel(QObject *);
signals:
    void s_printAbsence();
    void s_printSynthese();
private:
    template<typename... Args>
    void drawFormattedText(int x, int y, const QString& format, const Args&... args);
    bool preprint(const QString &);
    QPrinter m_printer;
    QPainter m_painter;
    QObject* m_model = nullptr;
};

#endif // PRINTERMANAGER_H



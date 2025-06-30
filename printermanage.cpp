#include "printermanage.h"
#include <QPrintDialog>
#include <QFileDialog>

PrinterManage::PrinterManage() {}


void PrinterManage::imprimerAbsenceSeance() {
    print();
}

void PrinterManage::print() {
    m_html="Bonjour";
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Exporter en PDF", "Absences.pdf", "Fichiers PDF (*.pdf)");
    if (filePath.isEmpty())
        return;

    if (!filePath.endsWith(".pdf", Qt::CaseInsensitive))
        filePath += ".pdf";

    // Configurer le QPrinter pour PDF
    //QPrinter printer(QPrinter::HighResolution);
    m_printer.setOutputFormat(QPrinter::PdfFormat);
    m_printer.setOutputFileName(filePath);

    QPrintDialog printDialog(&m_printer);
    if (printDialog.exec() == QDialog::Accepted) {
        m_doc.setHtml(m_html);
        m_doc.print(&m_printer);
    }

}

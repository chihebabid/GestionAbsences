#include "presencemodel.h"
#include "printermanage.h"
#include <QPrintDialog>
#include <QFileDialog>

PrinterManage::PrinterManage() {}


void PrinterManage::imprimerAbsenceSeance() {
    AbsenceModel* model {dynamic_cast<AbsenceModel*>(m_model)};
    if (!model) {
        qDebug()<<"Erreur de conversion dynamique...";
    }
    m_html.clear();
    // En-tête HTML
    m_html += "<html><head><meta charset='utf-8'></head><body>";
    m_html += "<center><h2>Liste des absences</h2></center><br><br>";
    m_html+= "<b>Section : </b><br>";
    // Début du tableau
    m_html += "<table border='1' cellspacing='0' cellpadding='5'>";
    m_html += "<thead><tr style='background-color:#f0f0f0'>";
    m_html += "<th>N° Inscription</th>";
    m_html += "<th>Nom</th>";
    m_html += "<th>Prénom</th>";
    m_html += "<th>Présence</th>";
    m_html += "</tr></thead><tbody>";

    // Lignes des étudiants
    const auto& liste = model->getListeEtudiants();
    for (const auto& e : liste) {
        m_html += "<tr>";
        m_html += "<td>" + e.inscri + "</td>";
        m_html += "<td>" + e.nom + "</td>";
        m_html += "<td>" + e.prenom + "</td>";

        PresenceModel presence; // Interprétation de l'ID présence
        presence.loadFromDatabase();
        m_html += "<td>" + presence.getLabel(e.presence) + "</td>";
        m_html += "</tr>";
    }

    m_html += "</tbody></table>";
    m_html += "</body></html>";
    print();
}

void PrinterManage::print() {    
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Exporter en PDF", "Absences.pdf", "Fichiers PDF (*.pdf)");
    if (filePath.isEmpty())
        return;

    if (!filePath.endsWith(".pdf", Qt::CaseInsensitive))
        filePath += ".pdf";

    // Configurer le QPrinter pour PDF
    //QPrinter printer(QPrinter::HighResolution);
    m_printer.setOutputFormat(QPrinter::PdfFormat);
    m_printer.setOutputFileName(filePath);

   // QPrintDialog printDialog(&m_printer);
    //if (printDialog.exec() == QDialog::Accepted) {
        m_doc.setHtml(m_html);
        m_doc.print(&m_printer);
    //}
}

void PrinterManage::setAbsenceModel(QObject *m) {
    m_model=m;
}



void PrinterManage::startPrinting(const QString &s) {
    if (s=="absence")
        emit s_printAbsence();
}

#include "presencemodel.h"
#include "printermanage.h"
#include <QPrintDialog>
#include <QFileDialog>
#include <QPainter>

PrinterManage::PrinterManage() {}

/*
void PrinterManage::imprimerAbsenceSeance() {
    AbsenceModel* model {dynamic_cast<AbsenceModel*>(m_model)};
    if (!model) {
        qDebug()<<"Erreur de conversion dynamique...";
    }
    const Seance & seance {model->getSeance()->getSelectedSeance()};
    QTime debut = QTime::fromString(seance.debut, "hh:mm");
    QTime delta = QTime::fromString(seance.duree, "hh:mm");



    int minutes = seance.duree.toInt();
    QTime resultat = debut.addSecs(minutes * 60);

    QString heure_fin {resultat.toString("hh:mm")};
    m_html.clear();
    // En-tête HTML
    m_html += "<html><head><meta charset='utf-8'></head><body>";
    m_html += "<center><h2>Liste des absences</h2></center><br><br>";
    m_html+= "<b>Section : </b>"+seance.section;
    m_html+= "<br><b>Module : </b>"+seance.module+ " ("+seance.type+")";
    m_html+= "<br><b>Date : </b>"+seance.date+" de "+seance.debut+" à "+heure_fin;
    // Début du tableau
    m_html += "<br><table border='1' cellspacing='0' cellpadding='5'>";
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
}*/

void PrinterManage::imprimerAbsenceSeance() {
    AbsenceModel* model = qobject_cast<AbsenceModel*>(m_model);
    if (!model) {
        qDebug() << "Erreur de conversion dynamique...";
        return;
    }

    const Seance& seance = model->getSeance()->getSelectedSeance();
    QTime debut = QTime::fromString(seance.debut, "hh:mm");
    int minutes = seance.duree.toInt();
    QTime fin = debut.addSecs(minutes * 60);
    QString heureFin = fin.toString("hh:mm");

    PresenceModel presence;
    presence.loadFromDatabase();

    QString filePath = QFileDialog::getSaveFileName(nullptr, "Exporter en PDF", "Absences.pdf", "Fichiers PDF (*.pdf)");
    if (filePath.isEmpty())
        return;
    if (!filePath.endsWith(".pdf", Qt::CaseInsensitive))
        filePath += ".pdf";

    // Configuration de l'imprimante
    m_printer.setOutputFormat(QPrinter::PdfFormat);
    m_printer.setOutputFileName(filePath);
    m_printer.setPageSize(QPageSize::A4);
    m_printer.setPageOrientation(QPageLayout::Portrait);
    m_printer.setFullPage(false); // Important pour respecter les marges

    // Définition des marges (en millimètres)
    QPageLayout layout = m_printer.pageLayout();
    layout.setUnits(QPageLayout::Millimeter);
    layout.setMargins(QMarginsF(15, 15, 15, 15)); // 1.5 cm de marge sur tous les côtés
    m_printer.setPageLayout(layout);

    // Polices
    const QFont normalFont("Times", 10);
    const QFont boldFont("Times", 10, QFont::Bold);
    const QFont titleFont("Times", 16, QFont::Bold);

    // Métriques
    QFontMetrics titleMetrics(titleFont);
    QFontMetrics normalMetrics(normalFont);
    const int lineHeight = normalMetrics.height() + 5;
    const int titleHeight = titleMetrics.height() + 10;

    // Dimensions des colonnes (en pixels)
    const QVector<int> colWidths = {120, 180, 180, 100};
    const QStringList headers = {"N° Inscription", "Nom", "Prénom", "Présence"};
    const auto& liste = model->getListeEtudiants();

    // Obtenir les dimensions réelles de la zone imprimable
    QRectF pageRect = m_printer.pageRect(QPrinter::DevicePixel);
    const int pageWidth = pageRect.width();
    const int pageHeight = pageRect.height();
    const int marginLeft = pageRect.left();
    const int marginTop = pageRect.top();

    // Calculer l'espace utilisable
    const int headerHeight = titleHeight + (4 * lineHeight); // Titre + 4 lignes d'info
    const int footerHeight = lineHeight;
    const int usableHeight = pageHeight - headerHeight - footerHeight;
    const int linesPerPage = usableHeight / lineHeight;
    const int totalPages = std::ceil(static_cast<double>(liste.size()) / linesPerPage);

    QPainter painter;
    if (!painter.begin(&m_printer)) {
        qDebug() << "Erreur lors de l'ouverture de l'imprimante";
        return;
    }

    int page = 1;
    int y = marginTop;

    auto printHeader = [&]() {
        y = marginTop;

        // Titre centré
        painter.setFont(titleFont);
        QString titre = "Liste des absences";
        int titreWidth = titleMetrics.horizontalAdvance(titre);
        painter.drawText((pageWidth - titreWidth)/2, y + titleMetrics.ascent(), titre);
        y += titleHeight;

        // Infos séance
        painter.setFont(boldFont);
        painter.drawText(marginLeft, y + normalMetrics.ascent(), "Section : ");
        painter.setFont(normalFont);
        painter.drawText(marginLeft + 70, y + normalMetrics.ascent(), seance.section);
        y += lineHeight;

        painter.setFont(boldFont);
        painter.drawText(marginLeft, y + normalMetrics.ascent(), "Module : ");
        painter.setFont(normalFont);
        painter.drawText(marginLeft + 70, y + normalMetrics.ascent(), seance.module + " (" + seance.type + ")");
        y += lineHeight;

        painter.setFont(boldFont);
        painter.drawText(marginLeft, y + normalMetrics.ascent(), "Date : ");
        painter.setFont(normalFont);
        painter.drawText(marginLeft + 70, y + normalMetrics.ascent(), seance.date + " de " + seance.debut + " à " + heureFin);
        y += 2 * lineHeight;

        // En-tête tableau avec alignement gauche pour les 3 premières colonnes
        painter.setFont(boldFont);
        int x = marginLeft;
        for (int i = 0; i < headers.size(); ++i) {
            QRect cellRect(x, y, colWidths[i], lineHeight);
            painter.fillRect(cellRect, QColor("#f0f0f0"));
            painter.drawRect(cellRect);

            // Alignement différent selon la colonne
            Qt::Alignment align = (i < 3) ? Qt::AlignLeft | Qt::AlignVCenter : Qt::AlignCenter;
            painter.drawText(cellRect.adjusted(5, 0, -5, 0), align, headers[i]);
            x += colWidths[i];
        }
        y += lineHeight;
    };

    printHeader();

    for (int i = 0; i < liste.size(); ++i) {
        if (i > 0 && i % linesPerPage == 0) {
            // Pied de page
            QString footer = QString("Page %1 / %2").arg(page).arg(totalPages);
            painter.drawText((pageWidth - normalMetrics.horizontalAdvance(footer))/2,
                             pageHeight - 10,
                             footer);

            m_printer.newPage();
            page++;
            y = marginTop;
            printHeader();
        }

        const auto& e = liste[i];
        QStringList values = {e.inscri, e.nom, e.prenom, presence.getLabel(e.presence)};
        int x = marginLeft;
        for (int j = 0; j < values.size(); ++j) {
            QRect cellRect(x, y, colWidths[j], lineHeight);
            painter.drawRect(cellRect);

            Qt::Alignment align = Qt::AlignCenter;
            if (j == 1 || j == 2)
                align = Qt::AlignVCenter | Qt::AlignLeft;

            painter.drawText(cellRect.adjusted(5, 0, -5, 0), align, values[j]);
            x += colWidths[j];
        }
        y += lineHeight;
    }

    // Dernier pied de page
    QString footer = QString("Page %1 / %2").arg(page).arg(totalPages);
    painter.drawText((pageWidth - normalMetrics.horizontalAdvance(footer))/2,
                     pageHeight - 10,
                     footer);

    painter.end();
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

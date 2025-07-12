#include "presencemodel.h"
#include "synthesetablemodel.h"
#include "printermanage.h"
#include <QPrintDialog>
#include <QFileDialog>
#include <QPainter>
#include <QRegularExpression>

PrinterManage::PrinterManage() {}



void PrinterManage::imprimerAbsenceSeance() {
    AbsenceModel* model = qobject_cast<AbsenceModel*>(m_model);
    if (!model) {
        qDebug() << "Erreur de conversion dynamique...";
        return;
    }

    const Seance& seance = model->getSeance()->getSelectedSeance();
    QString heureFin {am::calculateEndTime(seance.debut,seance.duree.toInt())};


    PresenceModel presence;
    presence.loadFromDatabase();

    if (!preprint("Absences.pdf")) {
        return;
    }

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
    const int linesPerPage = (usableHeight / lineHeight) - 4;
    const int totalPages = std::ceil(static_cast<double>(liste.size()) / linesPerPage);


    if (!m_painter.begin(&m_printer)) {
        qDebug() << "Erreur lors de l'ouverture de l'imprimante";
        return;
    }

    int page = 1;
    int y = marginTop;

    auto printHeader = [&]() {
        y = marginTop;

        // Titre centré
        m_painter.setFont(titleFont);
        QString titre = "Liste des absences";
        int titreWidth = titleMetrics.horizontalAdvance(titre);
        m_painter.drawText((pageWidth - titreWidth)/2, y + titleMetrics.ascent(), titre);
        y += titleHeight;

        // Infos séance
        m_painter.setFont(boldFont);
        m_painter.drawText(marginLeft, y + normalMetrics.ascent(), "Section : ");
        m_painter.setFont(normalFont);
        m_painter.drawText(marginLeft + 70, y + normalMetrics.ascent(), seance.section);
        y += lineHeight;

        m_painter.setFont(boldFont);
        m_painter.drawText(marginLeft, y + normalMetrics.ascent(), "Module : ");
        m_painter.setFont(normalFont);
        m_painter.drawText(marginLeft + 70, y + normalMetrics.ascent(), seance.module + " (" + seance.type + ")");
        y += lineHeight;

        m_painter.setFont(boldFont);
        m_painter.drawText(marginLeft, y + normalMetrics.ascent(), "Date : ");
        m_painter.setFont(normalFont);
        m_painter.drawText(marginLeft + 70, y + normalMetrics.ascent(), seance.date + " de " + seance.debut + " à " + heureFin);
        y += 2 * lineHeight;

        // En-tête tableau avec alignement gauche pour les 3 premières colonnes
        m_painter.setFont(boldFont);
        int x = marginLeft;
        for (int i = 0; i < headers.size(); ++i) {
            QRect cellRect(x, y, colWidths[i], lineHeight);
            m_painter.fillRect(cellRect, QColor("#f0f0f0"));
            m_painter.drawRect(cellRect);

            // Alignement différent selon la colonne
            Qt::Alignment align = (i < 3) ? Qt::AlignLeft | Qt::AlignVCenter : Qt::AlignCenter;
            m_painter.drawText(cellRect.adjusted(5, 0, -5, 0), align, headers[i]);
            x += colWidths[i];
        }
        y += lineHeight;
    };

    printHeader();

    for (int i = 0; i < liste.size(); ++i) {
        if (i > 0 && i % linesPerPage == 0) {
            // Pied de page
            QString footer = QString("Page %1 / %2").arg(page).arg(totalPages);
            m_painter.drawText((pageWidth - normalMetrics.horizontalAdvance(footer))/2,
                             pageHeight - 10,
                             footer);

            m_printer.newPage();
            page++;
            y = marginTop;
            printHeader();
        }

        const auto& e = liste[i];
        QStringList values = {e.inscri, e.nom, e.prenom, presence.getLabelById(e.presence)};
        int x = marginLeft;
        for (int j = 0; j < values.size(); ++j) {
            QRect cellRect(x, y, colWidths[j], lineHeight);
            m_painter.drawRect(cellRect);

            Qt::Alignment align = Qt::AlignCenter;
            if (j == 1 || j == 2)
                align = Qt::AlignVCenter | Qt::AlignLeft;

            m_painter.drawText(cellRect.adjusted(5, 0, -5, 0), align, values[j]);
            x += colWidths[j];
        }
        y += lineHeight;
    }

    // Dernier pied de page
    QString footer = QString("Page %1 / %2").arg(page).arg(totalPages);
    m_painter.drawText((pageWidth - normalMetrics.horizontalAdvance(footer))/2,
                     pageHeight - 10,
                     footer);

    m_painter.end();
}




bool PrinterManage::preprint(const QString &file_name) {
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Exporter en PDF",file_name, "Fichiers PDF (*.pdf)");
    if (filePath.isEmpty())
        return false;
    if (!filePath.endsWith(".pdf", Qt::CaseInsensitive))
        filePath += ".pdf";

    // Configuration de l'imprimante
    m_printer.setOutputFormat(QPrinter::PdfFormat);
    m_printer.setOutputFileName(filePath);
    m_printer.setPageSize(QPageSize::A4);
    m_printer.setPageOrientation(QPageLayout::Portrait);
    m_printer.setFullPage(false); // Important pour respecter les marges


    QPageLayout layout = m_printer.pageLayout();
    layout.setUnits(QPageLayout::Point);
    layout.setMargins(QMarginsF(20, 20, 20, 20));
    m_printer.setPageLayout(layout);
    return true;

}

void PrinterManage::setModel(QObject *m) {
    m_model=m;
}



void PrinterManage::startPrinting(const QString &s) {
    if (s=="absence")
        emit s_printAbsence();
    else if (s=="synthese")
        emit s_printSynthese();
}

void PrinterManage::imprimerSynthese() {
    SyntheseTableModel * model = qobject_cast<SyntheseTableModel*>(m_model);
    if (!model) {
        qDebug() << "Erreur de conversion dynamique...";
        return;
    }
    if (!preprint("Synthese.pdf")) {
        return;
    }
    const am::Info_t _info {model->getCurrentInfo()};

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
    const QVector<int> colWidths = {90, 160, 160, 115,115};
    const QStringList headers = {"N° Inscription", "Nom", "Prénom", "# des absences","% des absences"};
    const auto& liste = model->getListeEtudiants();


    QRectF pageRect = m_printer.pageRect(QPrinter::DevicePixel);
    const int pageWidth = pageRect.width();
    const int pageHeight = pageRect.height();
    const int marginLeft = pageRect.left();
    const int marginTop = pageRect.top();

    // Calculer l'espace utilisable
    const int headerHeight = titleHeight + (4 * lineHeight); // Titre + 4 lignes d'info
    const int footerHeight = lineHeight;
    const int usableHeight = pageHeight - headerHeight - footerHeight;
    const int linesPerPage = (usableHeight / lineHeight) - 4;
    const int totalPages = std::ceil(static_cast<double>(liste.size()) / linesPerPage);


    if (!m_painter.begin(&m_printer)) {
        qDebug() << "Erreur lors de l'ouverture de l'imprimante";
        return;
    }

    int page = 1;
    int y = marginTop;


    auto printHeader = [&]() {
        y = marginTop;
        m_painter.setFont(normalFont);
        drawFormattedText(marginLeft, y + normalMetrics.ascent(),"%B%N", "Date d'impression : ",QDate::currentDate().toString("dd/MM/yyyy"));
        y +=  2*lineHeight;

        m_painter.setFont(titleFont);
        QString titre = "Synthèse des absences";
        int titreWidth = titleMetrics.horizontalAdvance(titre);
        m_painter.drawText((pageWidth - titreWidth)/2, y + titleMetrics.ascent(), titre);
        y += titleHeight;

        m_painter.setFont(boldFont);
        m_painter.drawText(marginLeft, y + normalMetrics.ascent(), "Semestre : ");
        m_painter.setFont(normalFont);
        m_painter.drawText(marginLeft + 70, y + normalMetrics.ascent(), QString::number(_info.semestre));
        y +=  lineHeight;

        // Infos séance
        m_painter.setFont(boldFont);
        m_painter.drawText(marginLeft, y + normalMetrics.ascent(), "Section : ");
        m_painter.setFont(normalFont);
        m_painter.drawText(marginLeft + 70, y + normalMetrics.ascent(), _info.section);
        y += lineHeight;

        m_painter.setFont(boldFont);
        m_painter.drawText(marginLeft, y + normalMetrics.ascent(), "Module : ");
        m_painter.setFont(normalFont);
        m_painter.drawText(marginLeft + 70, y + normalMetrics.ascent(), _info.module+" ( "+model->getListeTypesModule()+ " )" );
        y += lineHeight;

        drawFormattedText(marginLeft + 70, y + normalMetrics.ascent(),"%B%N%B%N", QString::number(model->nbSeances()), " séance(s) effectuée(s) / ",QString::number(model->nbTotalSeances()), " séance(s) planifiée(s)");
        y += 2 *lineHeight;


        m_painter.setFont(boldFont);
        int x = marginLeft;
        for (int i {}; i < headers.size(); ++i) {
            QRect cellRect(x, y, colWidths[i], lineHeight);
            m_painter.fillRect(cellRect, QColor("#f0f0f0"));
            m_painter.drawRect(cellRect);

            // Alignement différent selon la colonne
            Qt::Alignment align = (i < 3) ? Qt::AlignLeft | Qt::AlignVCenter : Qt::AlignCenter;
            m_painter.drawText(cellRect.adjusted(5, 0, -5, 0), align, headers[i]);
            x += colWidths[i];
        }
        y += lineHeight;
    };

    printHeader();

    for (int i {}; i < liste.size(); ++i) {
        if (i > 0 && i % linesPerPage == 0) {
            // Pied de page
            QString footer = QString("Page %1 / %2").arg(page).arg(totalPages);
            m_painter.drawText((pageWidth - normalMetrics.horizontalAdvance(footer))/2,
                               pageHeight - 10,
                               footer);

            m_printer.newPage();
            page++;
            y = marginTop;
            printHeader();
        }

        const auto& e = liste[i];
        QStringList values = {e.inscri, e.nom, e.prenom, QString::number(e.nbAbsences),QString::number(e.nbAbsences*100.0f/model->nbSeances(),'f',2)+" \%"};
        int x = marginLeft;
        for (int j = 0; j < values.size(); ++j) {
            QRect cellRect(x, y, colWidths[j], lineHeight);
            m_painter.drawRect(cellRect);

            Qt::Alignment align = Qt::AlignCenter;
            if (j == 1 || j == 2)
                align = Qt::AlignVCenter | Qt::AlignLeft;

            m_painter.drawText(cellRect.adjusted(5, 0, -5, 0), align, values[j]);
            x += colWidths[j];
        }
        y += lineHeight;
    }

    // Dernier pied de page
    QString footer = QString("Page %1 / %2").arg(page).arg(totalPages);
    m_painter.drawText((pageWidth - normalMetrics.horizontalAdvance(footer))/2,
                       pageHeight - 10,
                       footer);


    m_painter.end();
}

template<typename... Args>
void PrinterManage::drawFormattedText(int x, int y, const QString& format, const Args&... args) {
    QStringList argStrings = { QString("%1").arg(args)... };

    QFont normalFont = m_painter.font();
    QFont boldFont = normalFont;
    boldFont.setBold(true);

    QFontMetrics normalMetrics(normalFont);
    QFontMetrics boldMetrics(boldFont);


    QStringList segments = format.split(QRegularExpression("%[BN]"), Qt::KeepEmptyParts);
    QStringList placeholders;
    QRegularExpression placeholderRegex("%[BN]");
    QRegularExpressionMatchIterator it = placeholderRegex.globalMatch(format);
    while (it.hasNext()) {
        placeholders << it.next().captured(0);
    }


    if (placeholders.size() != argStrings.size()) {
        qWarning() << "drawFormattedText: Mismatch between placeholders (" << placeholders.size()
        << ") and arguments (" << argStrings.size() << ")";
        return;
    }

    // Draw each segment
    int currentX = x;

    int argIndex = 0;

    for (const QString segment : segments) {
        if (!segment.isEmpty()) {
            m_painter.setFont(normalFont);
            m_painter.drawText(currentX, y , segment);
            currentX += normalMetrics.horizontalAdvance(segment);
        }

        if (argIndex < placeholders.size()) {
            const QString& placeholder = placeholders[argIndex];
            const QString& argText = argStrings[argIndex];
            if (placeholder == "%B") {
                m_painter.setFont(boldFont);
                m_painter.drawText(currentX, y , argText);
                currentX += boldMetrics.horizontalAdvance(argText);
            } else if (placeholder == "%N") {
                m_painter.setFont(normalFont);
                m_painter.drawText(currentX, y, argText);
                currentX += normalMetrics.horizontalAdvance(argText);
            }
            argIndex++;
        }
    }
}

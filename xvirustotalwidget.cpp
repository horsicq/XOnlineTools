/* Copyright (c) 2022-2025 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "xvirustotalwidget.h"

#include "ui_xvirustotalwidget.h"

XVirusTotalWidget::XVirusTotalWidget(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::XVirusTotalWidget)
{
    ui->setupUi(this);

    XOptions::adjustToolButton(ui->toolButtonReload, XOptions::ICONTYPE_RELOAD);
    XOptions::adjustToolButton(ui->toolButtonSave, XOptions::ICONTYPE_SAVE);
    XOptions::adjustToolButton(ui->toolButtonRescan, XOptions::ICONTYPE_RELOAD);
    XOptions::adjustToolButton(ui->toolButtonWebsite, XOptions::ICONTYPE_VIRUSTOTAL);

    ui->toolButtonReload->setToolTip(tr("Reload"));
    ui->toolButtonSave->setToolTip(tr("Save"));
    ui->toolButtonRescan->setToolTip(tr("Rescan"));
    ui->toolButtonWebsite->setToolTip(tr("Website"));
    ui->checkBoxShowDetects->setToolTip(tr("Show detects"));
    ui->lineEditFirst->setToolTip(tr("First"));
    ui->lineEditLast->setToolTip(tr("Last"));

    g_pDevice = nullptr;
    g_mode = MODE_UNKNOWN;
}

XVirusTotalWidget::~XVirusTotalWidget()
{
    delete ui;
}

void XVirusTotalWidget::setData(QIODevice *pDevice)
{
    // TODO Check API key
    g_pDevice = pDevice;

    g_sMD5 = XBinary::getHash(XBinary::HASH_MD5, pDevice);

    reload(false);
}

void XVirusTotalWidget::reload(bool bRescanFile)
{
    g_mode = MODE_UNKNOWN;

    QString sApiKey = getGlobalOptions()->getValue(XOptions::ID_ONLINETOOLS_VIRUSTOTAL_APIKEY).toString();

    if (sApiKey == "") {
        g_mode = MODE_NOAPIKEY;
    }

    if (g_mode != MODE_NOAPIKEY) {
        g_jsonDocument = QJsonDocument();

        XVirusTotal virusTotal;

        connect(&virusTotal, SIGNAL(errorMessage(QString)), this, SLOT(errorMessageSlot(QString)));

        virusTotal.setApiKey(sApiKey);

        bool bIsNotFound = false;

        if (bRescanFile) {
            XVirusTotal _virusTotal;
            _virusTotal.setApiKey(sApiKey);
            _virusTotal.setDevice(g_pDevice);
            _virusTotal.setParameter(g_sMD5);
            _virusTotal.setMode(XOnlineTools::MODE_UPLOAD);

            XDialogProcess xotdp(this, &_virusTotal);
            xotdp.setGlobal(getShortcuts(), getGlobalOptions());
            xotdp.start();
            xotdp.showDialogDelay();

            g_jsonDocument = virusTotal.getFileInfo(g_sMD5, &bIsNotFound);  // mb TODO
        } else {
            g_jsonDocument = virusTotal.getFileInfo(g_sMD5, &bIsNotFound);

            if (bIsNotFound) {
                // TODO upload
                if (QMessageBox::question(XOptions::getMainWidget(this), tr("Information"), tr("Upload the file for analyze?")) == QMessageBox::Yes) {
                    XVirusTotal _virusTotal;

                    connect(&_virusTotal, SIGNAL(errorMessage(QString)), this, SLOT(errorMessageSlot(QString)));

                    _virusTotal.setApiKey(sApiKey);
                    _virusTotal.setDevice(g_pDevice);
                    _virusTotal.setParameter(g_sMD5);
                    _virusTotal.setMode(XOnlineTools::MODE_UPLOAD);

                    XDialogProcess xotdp(this, &_virusTotal);
                    xotdp.setGlobal(getShortcuts(), getGlobalOptions());
                    xotdp.start();
                    xotdp.showDialogDelay();

                    g_jsonDocument = virusTotal.getFileInfo(g_sMD5, &bIsNotFound);  // mb TODO
                }
            }
        }

        if (!bIsNotFound) {
            g_mode = MODE_EXISTS;

            showRecords();
        } else {
            g_mode = MODE_NOTFOUND;
        }

        ui->toolButtonSave->setEnabled(true);
    } else {
        ui->toolButtonSave->setEnabled(false);
    }

    //    if(g_mode==MODE_EXISTS)
    //    {
    //        ui->pushButtonRescan->setEnabled(true);
    //    }
    //    else
    //    {
    //        ui->pushButtonRescan->setEnabled(false);
    //    }
}

void XVirusTotalWidget::showRecords()
{
    bool bShowDetected = ui->checkBoxShowDetects->isChecked();

    QList<XVirusTotal::SCAN_RESULT> listRecords = XVirusTotal::getScanInfo(&g_jsonDocument, bShowDetected).listScanResult;

    qint32 nNumberOfRecords = listRecords.count();

    QStandardItemModel *pModel = new QStandardItemModel(nNumberOfRecords, 4);

    pModel->setHeaderData(0, Qt::Horizontal, tr("Scan"));
    pModel->setHeaderData(1, Qt::Horizontal, tr("Version"));
    pModel->setHeaderData(2, Qt::Horizontal, tr("Date"));
    pModel->setHeaderData(3, Qt::Horizontal, tr("Result"));

    for (qint32 i = 0; i < nNumberOfRecords; i++) {
        QStandardItem *pItemScan = new QStandardItem;
        pItemScan->setText(listRecords.at(i).engine_name);
        pModel->setItem(i, 0, pItemScan);

        QStandardItem *pItemVersion = new QStandardItem;
        pItemVersion->setText(listRecords.at(i).engine_version);
        pModel->setItem(i, 1, pItemVersion);

        QStandardItem *pItemDate = new QStandardItem;
        pItemDate->setText(listRecords.at(i).engine_update);
        pModel->setItem(i, 2, pItemDate);

        QStandardItem *pItemResult = new QStandardItem;
        pItemResult->setText(listRecords.at(i).result);
        pModel->setItem(i, 3, pItemResult);
    }

    ui->tableViewScanResult->setCustomModel(pModel, true);

    ui->tableViewScanResult->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->tableViewScanResult->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    ui->tableViewScanResult->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    ui->tableViewScanResult->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);

    ui->tableViewScanResult->setColumnWidth(0, 150);  // TODO consts
    ui->tableViewScanResult->setColumnWidth(1, 100);
    ui->tableViewScanResult->setColumnWidth(2, 100);

    XVirusTotal::SCAN_INFO scanInfo = XVirusTotal::getScanInfo(&g_jsonDocument, false);

    if (scanInfo.dtFirstScan.isValid()) {
        ui->lineEditFirst->setText(scanInfo.dtFirstScan.toString("yyyy-MM-dd hh:mm:ss"));
    }

    if (scanInfo.dtLastScan.isValid()) {
        ui->lineEditLast->setText(scanInfo.dtLastScan.toString("yyyy-MM-dd hh:mm:ss"));
    }
}

bool XVirusTotalWidget::checkVirusTotalKey(XOptions *pOptions, QWidget *pParent)
{
    bool bResult = false;

    QString sApiKey = pOptions->getValue(XOptions::ID_ONLINETOOLS_VIRUSTOTAL_APIKEY).toString();

    if (sApiKey != "") {
        bResult = true;
    }

    if (!bResult) {
        QString sInfo = tr("Please use valid API key");

        sInfo += QString("\n\n %1 -> %2 -> %3").arg(tr("Options"), tr("Online tools"), QString("VirusTotal API key"));

        QMessageBox::critical(pParent, tr("Error"), sInfo);
    }

    return bResult;
}

void XVirusTotalWidget::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
    // TODO
}

void XVirusTotalWidget::on_toolButtonReload_clicked()
{
    if (checkVirusTotalKey(getGlobalOptions(), XOptions::getMainWidget(this))) {
        reload(false);
    }
}

void XVirusTotalWidget::on_toolButtonSave_clicked()
{
    XShortcutsWidget::saveTableModel(ui->tableViewScanResult->getProxyModel(), XBinary::getResultFileName(g_pDevice, QString("%1.txt").arg(QString("VirusTotal"))));
}

void XVirusTotalWidget::on_toolButtonRescan_clicked()
{
    if (checkVirusTotalKey(getGlobalOptions(), XOptions::getMainWidget(this))) {
        reload(true);
    }
}

void XVirusTotalWidget::on_checkBoxShowDetects_stateChanged(int nValue)
{
    Q_UNUSED(nValue)

    showRecords();
}

void XVirusTotalWidget::on_toolButtonWebsite_clicked()
{
    showInBrowser();
}

bool XVirusTotalWidget::showInBrowser()
{
    return showInBrowser(g_sMD5);
}

bool XVirusTotalWidget::showInBrowser(const QString &sHash)
{
    return QDesktopServices::openUrl(QUrl(XVirusTotal::getFileLink(sHash)));
}

void XVirusTotalWidget::adjustView()
{
    getGlobalOptions()->adjustWidget(this, XOptions::ID_VIEW_FONT_CONTROLS);
    getGlobalOptions()->adjustTableView(ui->tableViewScanResult, XOptions::ID_VIEW_FONT_TABLEVIEWS);
    ui->tableViewScanResult->adjust();
}

void XVirusTotalWidget::reloadData(bool bSaveSelection)
{
    Q_UNUSED(bSaveSelection)

    reload(false);
}

/* Copyright (c) 2022 hors<horsicq@gmail.com>
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

XVirusTotalWidget::XVirusTotalWidget(QWidget *pParent) :
    XShortcutsWidget(pParent),
    ui(new Ui::XVirusTotalWidget)
{
    ui->setupUi(this);

    g_pDevice=nullptr;
}

XVirusTotalWidget::~XVirusTotalWidget()
{
    delete ui;
}

void XVirusTotalWidget::setData(QIODevice *pDevice)
{
    // TODO Check API key
    g_pDevice=pDevice;

    g_sMD5=XBinary::getHash(XBinary::HASH_MD5,pDevice);

    reload();
}

void XVirusTotalWidget::reload()
{
    QString sApiKey=getGlobalOptions()->getValue(XOptions::ID_ONLINETOOLS_VIRUSTOTAL_APIKEY).toString();

    XVirusTotal virusTotal;

    virusTotal.setApiKey(sApiKey);

    bool bIsNotFound=false;

    QJsonDocument jsonDocument=virusTotal.getFileInfo(g_sMD5,&bIsNotFound);

    if(bIsNotFound)
    {
        // TODO upload
        if(QMessageBox::question(XOptions::getMainWidget(this),tr("Information"),tr("Upload the file to VirusTotal for analyze?"))==QMessageBox::Yes)
        {
            XVirusTotal _virusTotal;
            _virusTotal.setApiKey(sApiKey);
            _virusTotal.setDevice(g_pDevice);
            _virusTotal.setParameter(g_sMD5);
            _virusTotal.setMode(XOnlineTools::MODE_UPLOAD);

            XOnlineToolsDialogProcess xotdp(this,&_virusTotal);

            xotdp.showDialogDelay(1000);
            // TODO wait
        }
    }

    if(!bIsNotFound)
    {
        QList<XVirusTotal::SCAN_RESULT> listRecords=XVirusTotal::getScanResults(&jsonDocument);;

        QAbstractItemModel *pOldModel=ui->tableViewScanResult->model();

        qint32 nNumberOfRecords=listRecords.count();

        QStandardItemModel *pModel=new QStandardItemModel(nNumberOfRecords,4,this);

        pModel->setHeaderData(0,Qt::Horizontal,tr("Scan"));
        pModel->setHeaderData(1,Qt::Horizontal,tr("Version"));
        pModel->setHeaderData(2,Qt::Horizontal,tr("Date"));
        pModel->setHeaderData(3,Qt::Horizontal,tr("Result"));

        for(qint32 i=0;i<nNumberOfRecords;i++)
        {
            QStandardItem *pItemScan=new QStandardItem;
            pItemScan->setText(listRecords.at(i).engine_name);
            pModel->setItem(i,0,pItemScan);

            QStandardItem *pItemVersion=new QStandardItem;
            pItemVersion->setText(listRecords.at(i).engine_version);
            pModel->setItem(i,1,pItemVersion);

            QStandardItem *pItemDate=new QStandardItem;
            pItemDate->setText(listRecords.at(i).engine_update);
            pModel->setItem(i,2,pItemDate);

            QStandardItem *pItemResult=new QStandardItem;
            pItemResult->setText(listRecords.at(i).result);
            pModel->setItem(i,3,pItemResult);
        }

        ui->tableViewScanResult->setModel(pModel);

        delete pOldModel;

        ui->tableViewScanResult->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Interactive);
        ui->tableViewScanResult->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Interactive);
        ui->tableViewScanResult->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Interactive);
        ui->tableViewScanResult->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);

        ui->tableViewScanResult->setColumnWidth(0,150);
        ui->tableViewScanResult->setColumnWidth(1,100);
        ui->tableViewScanResult->setColumnWidth(2,100);
    }
}

void XVirusTotalWidget::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
    // TODO
}

void XVirusTotalWidget::on_pushButtonReload_clicked()
{
    reload();
}

void XVirusTotalWidget::on_pushButtonSave_clicked()
{
    // TODO
}


void XVirusTotalWidget::on_pushButtonRescan_clicked()
{

}


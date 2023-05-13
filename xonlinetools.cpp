/* Copyright (c) 2022-2023 hors<horsicq@gmail.com>
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
#include "xonlinetools.h"

XOnlineTools::XOnlineTools(QObject *pParent) : QObject(pParent)
{
    g_pdStructEmpty = XBinary::createPdStruct();
    g_pPdStruct = &g_pdStructEmpty;
    g_mode = MODE_UNKNOWN;
    g_pDevice = nullptr;
    g_nFreeIndex = -1;
}

void XOnlineTools::setApiKey(const QString &sApiKey)
{
    g_sApiKey = sApiKey;
}

QString XOnlineTools::getApiKey()
{
    return g_sApiKey;
}

void XOnlineTools::setPdStruct(XBinary::PDSTRUCT *pPdStruct)
{
    g_pPdStruct = pPdStruct;
}

XBinary::PDSTRUCT *XOnlineTools::getPdStruct()
{
    return g_pPdStruct;
}

void XOnlineTools::setParameter(const QString &sParameter)
{
    g_sParameter = sParameter;
}

QString XOnlineTools::getParameter()
{
    return g_sParameter;
}

void XOnlineTools::setMode(MODE mode)
{
    g_mode = mode;
}

XOnlineTools::MODE XOnlineTools::getMode()
{
    return g_mode;
}

void XOnlineTools::setDevice(QIODevice *pDevice)
{
    g_pDevice = pDevice;
}

QIODevice *XOnlineTools::getDevice()
{
    return g_pDevice;
}

bool XOnlineTools::isPagePresent(const QString &sUrl)
{
    bool bResult;

    QNetworkAccessManager networkAccessManager;
    QNetworkRequest networkRequest;

    connect(&networkAccessManager, SIGNAL(sslErrors(QNetworkReply *, const QList<QSslError> &)), this,
            SLOT(handleSslErrors(QNetworkReply *, const QList<QSslError> &)));  // TODO Check connect

    networkRequest.setUrl(QUrl(sUrl));
    QNetworkReply *pReply = networkAccessManager.get(networkRequest);
    QEventLoop loop;
    QObject::connect(pReply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    if (pReply->bytesAvailable()) {
        bResult = true;
    } else {
        bResult = false;
    }

    return bResult;
}

QString XOnlineTools::getPageContent(const QString &sUrl)
{
    QString sResult;

    QNetworkAccessManager networkAccessManager;
    QNetworkRequest networkRequest;

    connect(&networkAccessManager, SIGNAL(sslErrors(QNetworkReply *, const QList<QSslError> &)), this, SLOT(handleSslErrors(QNetworkReply *, const QList<QSslError> &)));

    networkRequest.setUrl(QUrl(sUrl));

    QNetworkReply *pReply = networkAccessManager.get(networkRequest);
    QEventLoop loop;
    QObject::connect(pReply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    if (pReply->bytesAvailable()) {
        sResult = pReply->readAll();
    }

    return sResult;
}

QString XOnlineTools::getSslVersion()
{
    QString sResult;

    sResult = QSslSocket::sslLibraryVersionString();

    return sResult;
}

bool XOnlineTools::_process()
{
    return false;
}

void XOnlineTools::process()
{
    QElapsedTimer scanTimer;
    scanTimer.start();

    //    getPdStruct()->pdRecordOpt.bIsValid=true;

    //    if(!(getPdStruct()->bIsStop))
    //    {
    //        getPdStruct()->pdRecordOpt.bSuccess=_process();
    //    }

    //    getPdStruct()->pdRecordOpt.bFinished=true;

    g_nFreeIndex = XBinary::getFreeIndex(g_pPdStruct);
    XBinary::setPdStructInit(g_pPdStruct, g_nFreeIndex, 0);

    bool bResult = _process();

    g_pPdStruct->bIsStop = !(bResult);

    XBinary::setPdStructFinished(g_pPdStruct, g_nFreeIndex);

    emit completed(scanTimer.elapsed());
}

void XOnlineTools::_uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    XBinary::setPdStructCurrent(g_pPdStruct, g_nFreeIndex, bytesSent);
    XBinary::setPdStructTotal(g_pPdStruct, g_nFreeIndex, bytesTotal);

    QNetworkReply *pReply = qobject_cast<QNetworkReply *>(sender());

    if (pReply) {
        if (g_pPdStruct->bIsStop) {
            pReply->abort();
        }
    }
}

void XOnlineTools::_downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    XBinary::setPdStructCurrent(g_pPdStruct, g_nFreeIndex, bytesReceived);
    XBinary::setPdStructTotal(g_pPdStruct, g_nFreeIndex, bytesTotal);

    QNetworkReply *pReply = qobject_cast<QNetworkReply *>(sender());

    if (pReply) {
        if (g_pPdStruct->bIsStop) {
            pReply->abort();
        }
    }
}

void XOnlineTools::_finished()
{
    XBinary::setPdStructFinished(g_pPdStruct, g_nFreeIndex);
}

void XOnlineTools::handleSslErrors(QNetworkReply *pReply, const QList<QSslError> &listErrors)
{
    Q_UNUSED(pReply)

    QString sError;

    if (listErrors.count()) {
        sError = listErrors.at(0).errorString();  // The first error
    }

    emit errorMessage(QString("%1(%2)").arg(sError, XOnlineTools::getSslVersion()));
}

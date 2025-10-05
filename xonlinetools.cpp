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
#include "xonlinetools.h"

XOnlineTools::XOnlineTools(QObject *pParent) : XThreadObject(pParent)
{
    g_pdStructEmpty = XBinary::createPdStruct();
    m_pPdStruct = &g_pdStructEmpty;
    m_mode = MODE_UNKNOWN;
    m_pDevice = nullptr;
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
    m_pPdStruct = pPdStruct;
}

XBinary::PDSTRUCT *XOnlineTools::getPdStruct()
{
    return m_pPdStruct;
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
    m_mode = mode;
}

XOnlineTools::MODE XOnlineTools::getMode()
{
    return m_mode;
}

void XOnlineTools::setDevice(QIODevice *pDevice)
{
    m_pDevice = pDevice;
}

QIODevice *XOnlineTools::getDevice()
{
    return m_pDevice;
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

bool XOnlineTools::handleProcess()
{
    return false;  // This method should be overridden in derived classes
}

void XOnlineTools::process()
{
    //    getPdStruct()->pdRecordOpt.bIsValid=true;

    //    if(!(getPdStruct()->bIsStop))
    //    {
    //        getPdStruct()->pdRecordOpt.bSuccess=_process();
    //    }

    //    getPdStruct()->pdRecordOpt.bFinished=true;

    g_nFreeIndex = XBinary::getFreeIndex(m_pPdStruct);
    XBinary::setPdStructInit(m_pPdStruct, g_nFreeIndex, 0);

    bool bResult = handleProcess();

    if (!(bResult)) {
        XBinary::setPdStructStopped(m_pPdStruct);
    }

    XBinary::setPdStructFinished(m_pPdStruct, g_nFreeIndex);
}

void XOnlineTools::_uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    XBinary::setPdStructCurrent(m_pPdStruct, g_nFreeIndex, bytesSent);
    XBinary::setPdStructTotal(m_pPdStruct, g_nFreeIndex, bytesTotal);

    QNetworkReply *pReply = qobject_cast<QNetworkReply *>(sender());

    if (pReply) {
        if (XBinary::isPdStructStopped(m_pPdStruct)) {
            pReply->abort();
        }
    }
}

void XOnlineTools::_downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    XBinary::setPdStructCurrent(m_pPdStruct, g_nFreeIndex, bytesReceived);
    XBinary::setPdStructTotal(m_pPdStruct, g_nFreeIndex, bytesTotal);

    QNetworkReply *pReply = qobject_cast<QNetworkReply *>(sender());

    if (pReply) {
        if (XBinary::isPdStructStopped(m_pPdStruct)) {
            pReply->abort();
        }
    }
}

void XOnlineTools::_finished()
{
    XBinary::setPdStructFinished(m_pPdStruct, g_nFreeIndex);  // Check mb remove
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

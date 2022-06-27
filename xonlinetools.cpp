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
#include "xonlinetools.h"

XOnlineTools::XOnlineTools(QObject *pParent)
    : QObject(pParent)
{
    pdStructEmpty={};
    g_pPdStruct=&pdStructEmpty;
}

void XOnlineTools::setApiKey(QString sApiKey)
{
    g_sApiKey=sApiKey;
}

QString XOnlineTools::getApiKey()
{
    return g_sApiKey;
}

void XOnlineTools::setPdStruct(XBinary::PDSTRUCT *pPdStruct)
{
    g_pPdStruct=pPdStruct;
}

XBinary::PDSTRUCT *XOnlineTools::getPdStruct()
{
    return g_pPdStruct;
}

void XOnlineTools::_uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    QNetworkReply *pReply=qobject_cast<QNetworkReply *>(sender());

    g_pPdStruct->pdRecord.nCurrent=bytesSent;
    g_pPdStruct->pdRecord.nTotal=bytesTotal;

    if(pReply)
    {
        if(g_pPdStruct->bIsStop)
        {
            pReply->abort();
        }
    }
}

void XOnlineTools::_downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    QNetworkReply *pReply=qobject_cast<QNetworkReply *>(sender());

    g_pPdStruct->pdRecord.nCurrent=bytesReceived;
    g_pPdStruct->pdRecord.nTotal=bytesTotal;

    pReply->abort();

    if(pReply)
    {
        if(g_pPdStruct->bIsStop)
        {
            pReply->abort();
        }
    }
}

void XOnlineTools::_finished()
{
    if(!(g_pPdStruct->bIsStop))
    {
        g_pPdStruct->pdRecord.bSuccess=true;
    }

    g_pPdStruct->pdRecord.bFinished=true;

    emit completed();
}


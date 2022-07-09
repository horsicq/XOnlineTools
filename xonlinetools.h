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
#ifndef XONLINETOOLS_H
#define XONLINETOOLS_H

#include "xbinary.h"
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValueRef>
#include <QJsonArray>
#include <QHttpMultiPart>

class XOnlineTools : public QObject
{
    Q_OBJECT

public:
    enum MODE
    {
        MODE_UNKNOWN=0,
        MODE_DOWNLOAD,
        MODE_UPLOAD,
        MODE_RESCAN
    };

    explicit XOnlineTools(QObject *pParent=nullptr);

    void setApiKey(QString sApiKey);
    QString getApiKey();
    void setPdStruct(XBinary::PDSTRUCT *pPdStruct);
    XBinary::PDSTRUCT *getPdStruct();

    void setParameter(QString sParameter);
    QString getParameter();
    void setMode(MODE mode);
    MODE getMode();
    void setDevice(QIODevice *pDevice);
    QIODevice *getDevice();

protected:
    virtual bool _process();

public slots:
    void process();
    void _uploadProgress(qint64 bytesSent,qint64 bytesTotal);
    void _downloadProgress(qint64 bytesReceived,qint64 bytesTotal);
    void _finished();

signals:
    void errorMessage(QString sErrorMessage);
    void completed(qint64 nElapsed);

private:
    QString g_sApiKey;
    XBinary::PDSTRUCT *g_pPdStruct;
    XBinary::PDSTRUCT pdStructEmpty;
    MODE g_mode;
    QIODevice *g_pDevice;
    QString g_sParameter;
};

#endif // XONLINETOOLS_H

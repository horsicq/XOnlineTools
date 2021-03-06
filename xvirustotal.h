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
#ifndef XVIRUSTOTAL_H
#define XVIRUSTOTAL_H

#include "xonlinetools.h"
#include <QThread>

class XVirusTotal : public XOnlineTools
{
    Q_OBJECT
public:
    enum RTYPE
    {
        RTYPE_UNKNOWN=0,
        RTYPE_GETFILEINFO,
        RTYPE_UPLOADFILE,
        RTYPE_GETFILEANALYSES,
        RTYPE_RESCANFILE,
        RTYPE_GETUPLOADLINK
    };

    struct SCAN_RESULT
    {
        QString category;
        QString engine_name;
        QString engine_version;
        QString result;
        QString method;
        QString engine_update;
    };

    struct SCAN_INFO
    {
        QDateTime dtFirstScan;
        QDateTime dtLastScan;
    };

    explicit XVirusTotal(QObject *pParent=nullptr);

    QJsonDocument getFileInfo(QString sHash,bool *pBNotFound=nullptr);
    QJsonDocument getFileAnalyses(QString sId);
    QString uploadFile(QIODevice *pDevice,QString sName="");
    QString uploadFile(QString sFileName);
    QString rescanFile(QString sHash);
    QList<SCAN_RESULT> getScanResults(QString sHash,bool bShowDetected);
    static QList<SCAN_RESULT> getScanResults(QJsonDocument *pJsonDoc,bool bShowDetected);
    SCAN_INFO getScanInfo(QString sHash);
    static SCAN_INFO getScanInfo(QJsonDocument *pJsonDoc);
    static QString getFileLink(QString sHash);
    bool isFilePresent(QString sHash);

protected:
    virtual bool _process();

private:
    QByteArray sendRequest(RTYPE rtype,QString sParameter,QIODevice *pDevice=nullptr,bool *pBNotFound=nullptr);
};

#endif // XVIRUSTOTAL_H

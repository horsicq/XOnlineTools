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
#include "xvirustotal.h"

XVirusTotal::XVirusTotal(QObject *pParent)
    : XOnlineTools(pParent)
{
    g_pDevice=nullptr;
}

QJsonDocument XVirusTotal::getFileInfo(QString sMD5, bool *pBNotFound)
{
    return QJsonDocument::fromJson(sendRequest(RTYPE_GETFILEINFO,sMD5,nullptr,pBNotFound));
}

QJsonDocument XVirusTotal::getFileAnalyses(QString sId)
{
    return QJsonDocument::fromJson(sendRequest(RTYPE_GETFILEANALYSES,sId));
}

bool XVirusTotal::uploadFile(QIODevice *pDevice, QString sName)
{
    QJsonDocument jsDoc=QJsonDocument::fromJson(sendRequest(RTYPE_UPLOADFILE,sName,pDevice));

    return true; // TODO
}

bool XVirusTotal::uploadFile(QString sFileName)
{
    bool bResult=false;

    QFile file;
    file.setFileName(sFileName);

    if(file.open(QIODevice::ReadOnly))
    {
        bResult=uploadFile(&file);

        file.close();
    }

    return bResult;
}

QList<XVirusTotal::SCAN_RESULT> XVirusTotal::getScanResults(QString sMD5)
{
    QJsonDocument jsonDoc=getFileInfo(sMD5);

    return getScanResults(&jsonDoc);
}

QList<XVirusTotal::SCAN_RESULT> XVirusTotal::getScanResults(QJsonDocument *pJsonDoc)
{
    QList<SCAN_RESULT> listResult;

    if(pJsonDoc->isObject())
    {
        QJsonObject jsonObject=pJsonDoc->object()["data"].toObject()["attributes"].toObject()["last_analysis_results"].toObject();

        QStringList slList=jsonObject.keys();
        qint32 nCount=slList.count();

        for(qint32 i=0;i<nCount;i++)
        {
            SCAN_RESULT record={};

            record.category=jsonObject[slList.at(i)].toObject()["category"].toString();
            record.engine_name=jsonObject[slList.at(i)].toObject()["engine_name"].toString();
            record.engine_version=jsonObject[slList.at(i)].toObject()["engine_version"].toString();
            record.result=jsonObject[slList.at(i)].toObject()["result"].toString();
            record.method=jsonObject[slList.at(i)].toObject()["method"].toString();
            record.engine_update=jsonObject[slList.at(i)].toObject()["engine_update"].toString();

            listResult.append(record);
        }
    }

    return listResult;
}

QString XVirusTotal::getScanInfo(QString sMD5)
{
    QJsonDocument jsonDoc=getFileInfo(sMD5);

    return getScanInfo(&jsonDoc);
}

QString XVirusTotal::getScanInfo(QJsonDocument *pJsonDoc)
{
    QString sResult;

    if(pJsonDoc->isObject())
    {
        QJsonObject jsonDataObject=pJsonDoc->object()["data"].toObject();

        QStringList slDataList=jsonDataObject.keys();
        qint32 nDataCount=slDataList.count();

        for(qint32 i=0;i<nDataCount;i++)
        {

        }
    }

    return sResult;
}

void XVirusTotal::setDataUpload(QIODevice *pDevice, QString sName, XBinary::PDSTRUCT *pPdStruct)
{
    g_pDevice=pDevice;
    g_sName=sName;

    setPdStruct(pPdStruct);
}

void XVirusTotal::upload()
{
    getPdStruct()->pdRecordOpt.bIsValid=true;

    getPdStruct()->pdRecordOpt.bSuccess=uploadFile(g_pDevice,g_sName);

    getPdStruct()->pdRecordOpt.bFinished=true;

    // TODO emit
}

QByteArray XVirusTotal::sendRequest(RTYPE rtype, QString sParameter, QIODevice *pDevice, bool *pBNotFound)
{
    QByteArray baResult;

    QNetworkAccessManager *pNetworkAccessManager=new QNetworkAccessManager(this);

    QNetworkRequest networkRequest;

    QUrl url=QUrl();
    url.setScheme("https");
    url.setHost("www.virustotal.com");

    QString sUrlPath;

    if(rtype==RTYPE_GETFILEINFO)
    {
        sUrlPath="/api/v3/files/"+sParameter;
    }
    else if(rtype==RTYPE_UPLOADFILE)
    {
        sUrlPath="/api/v3/files";
    }
    else if(rtype==RTYPE_GETFILEANALYSES)
    {
        sUrlPath="/api/v3/analyses/"+sParameter;
    }

    url.setPath(sUrlPath);

    networkRequest.setUrl(url);
    networkRequest.setRawHeader("x-apikey",getApiKey().toLatin1());

    QNetworkReply *pReply=nullptr;
    QHttpMultiPart *pMultiPart=nullptr;

    if( (rtype==RTYPE_GETFILEINFO)||
        (rtype==RTYPE_GETFILEANALYSES))
    {
        pReply=pNetworkAccessManager->get(networkRequest);
    }
    else if(rtype==RTYPE_UPLOADFILE)
    {
        pMultiPart=new QHttpMultiPart(QHttpMultiPart::FormDataType);

        QHttpPart filePart;

        QString sFileName=XBinary::getDeviceFileName(pDevice);

        if(sFileName=="")
        {
            if(sParameter!="")
            {
                sFileName=sParameter;
            }
            else
            {
                // We use MD5
                sFileName=XBinary::getHash(XBinary::HASH_MD5,pDevice);
            }
        }

        filePart.setHeader(QNetworkRequest::ContentTypeHeader,"application/octet-stream");
        filePart.setHeader(QNetworkRequest::ContentDispositionHeader,QVariant("form-data; name=\"file\"; filename=\"" + sFileName + "\""));

        filePart.setBodyDevice(pDevice);

        pMultiPart->append(filePart);

        pReply=pNetworkAccessManager->post(networkRequest,pMultiPart);
    }

    if(pReply)
    {
        QEventLoop loop;

        connect(pReply,&QNetworkReply::finished,&loop,&QEventLoop::quit);
        connect(pReply,&QNetworkReply::downloadProgress,this,&XOnlineTools::_downloadProgress,Qt::DirectConnection);
        connect(pReply,&QNetworkReply::uploadProgress,this,&XOnlineTools::_uploadProgress,Qt::DirectConnection);
        connect(pReply,&QNetworkReply::finished,this,&XOnlineTools::_finished,Qt::DirectConnection);

        loop.exec();

        if(pReply->error()==QNetworkReply::NoError)
        {
            baResult=pReply->readAll();
        }
        else if(pReply->error()==QNetworkReply::ContentNotFoundError)
        {
            if(pBNotFound)
            {
                *pBNotFound=true;
            }
        }
        else
        {
            emit errorMessage(pReply->errorString());
        #ifdef QT_DEBUG
            qDebug("%s",pReply->errorString().toLatin1().data());
        #endif
        }
    }

    delete pNetworkAccessManager;

    if(pMultiPart)
    {
        delete pMultiPart;
    }

//#ifdef QT_DEBUG
//    qDebug(baResult.data());
//#endif

    return baResult;
}

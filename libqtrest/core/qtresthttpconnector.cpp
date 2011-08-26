/***************************************************************************
 *   Copyright (C) 2011 Fernando Jiménez Moreno <ferjmoreno@gmail.com>     *
 *                                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QDebug>
#include <QNetworkAccessManager>
#include <QEventLoop>

#include "qtresthttpconnector.h"

HttpConnectorException::HttpConnectorException(const QString &what) : _what(what)
{
}

const char* HttpConnectorException::what() const throw()
{
    QString retval = "HttpConnectorException: " + _what;
    return retval.toAscii();
}

HttpConnector::HttpConnector()
{
    manager = new QNetworkAccessManager();
}

HttpConnector::~HttpConnector()
{
    delete manager;
}

QByteArray HttpConnector::httpRequest(const HttpRequest *request)
{
    replyData.clear();
#ifdef QTREST_DEBUG
    qDebug() << "Url: " << request->getRequestEndpoint().toString();
    qDebug() << "Headers: ";
#endif
    QNetworkRequest networkRequest(request->getRequestEndpoint());
    QList<QPair<QByteArray,QByteArray> > httpHeaders = request->getHttpHeaders();
    QList<QPair<QByteArray,QByteArray> >::const_iterator it = httpHeaders.begin();
    for(;it != httpHeaders.end(); it++)
    {
#ifdef QTREST_DEBUG
        qDebug() << (*it).first << ": " << (*it).second;
#endif
        networkRequest.setRawHeader((*it).first,(*it).second);
    }
    {
        QEventLoop *loop = new QEventLoop;
        HttpRequest::RequestHttpMethod httpMethod = request->getHttpMethod();
        switch(httpMethod)
        {
        case HttpRequest::GET:
    #ifdef QTREST_DEBUG
            qDebug() << "GET\n";
    #endif
            reply = manager->get(networkRequest);
            break;
        case HttpRequest::POST: {
            const QByteArray body = request->getRequestBody();
    #ifdef QTREST_DEBUG
            qDebug() << "POST\n";
            qDebug() << "Body: " << body;
    #endif
            reply = manager->post(networkRequest,body);
            break; }
        case HttpRequest::PUT: {
            const QByteArray body = request->getRequestBody();
    #ifdef QTREST_DEBUG
            qDebug() << "PUT\n";
            qDebug() << "Body: " << body;
    #endif
            reply = manager->put(networkRequest,body);
            break; }
        case HttpRequest::HEAD:
    #ifdef QTREST_DEBUG
            qDebug() << "PUT\n";
    #endif
            reply = manager->head(networkRequest);
            break;
        case HttpRequest::DELETE:
    #ifdef QTREST_DEBUG
            qDebug() << "DELETE\n";
    #endif
            reply = manager->deleteResource(networkRequest);
            break;
        }
        loop->connect(reply, SIGNAL(readyRead()), this, SLOT(readyRead()));
        loop->connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
        loop->connect(manager, SIGNAL(finished(QNetworkReply *)),loop, SLOT(quit()));
        loop->connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
        loop->exec();
    }
    return replyData;
}

void HttpConnector::finished(QNetworkReply *mReply)
{
    mReply->deleteLater();
}

void HttpConnector::readyRead()
{
    replyData = reply->readAll();
#ifdef QTREST_DEBUG
    qDebug() << "HTTP Reply: \n\t" << replyData;
#endif
    //TODO: parse replyData and return IEntity body + header
    emit requestFinished(replyData);
}

void HttpConnector::error(QNetworkReply::NetworkError errorCode)
{
    QString errorCodeStr;
    switch(errorCode)
    {
    case QNetworkReply::NoError:
        break;
    case QNetworkReply::ConnectionRefusedError:
    case QNetworkReply::RemoteHostClosedError:
    case QNetworkReply::HostNotFoundError:
    case QNetworkReply::TimeoutError:
    case QNetworkReply::OperationCanceledError:
    case QNetworkReply::SslHandshakeFailedError:
    case QNetworkReply::TemporaryNetworkFailureError:
    case QNetworkReply::UnknownNetworkError:
        errorCodeStr = "Network layer error [relating to the destination server]";
        break;
    case QNetworkReply::ProxyConnectionRefusedError:
    case QNetworkReply::ProxyConnectionClosedError:
    case QNetworkReply::ProxyNotFoundError:
    case QNetworkReply::ProxyTimeoutError:
    case QNetworkReply::ProxyAuthenticationRequiredError:
    case QNetworkReply::UnknownProxyError:
        errorCodeStr = "Proxy error";
        break;
    case QNetworkReply::ContentAccessDenied:
    case QNetworkReply::ContentOperationNotPermittedError:
    case QNetworkReply::ContentNotFoundError:
    case QNetworkReply::AuthenticationRequiredError:
    case QNetworkReply::ContentReSendError:
    case QNetworkReply::UnknownContentError:
        errorCodeStr = "Content error";
        break;
    case QNetworkReply::ProtocolUnknownError:
    case QNetworkReply::ProtocolInvalidOperationError:
    case QNetworkReply::ProtocolFailure:
        errorCodeStr = "Protocol error";
        break;
    }
#ifdef QTREST_DEBUG
    qDebug() << "NetworkError: " << errorCodeStr;
#endif
    this->lastError = QString("NetworkError: " + errorCodeStr);
}

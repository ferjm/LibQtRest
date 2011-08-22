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

#include "qtresthttprequest.h"

HttpRequest::HttpRequest(QObject *parent) :
    QObject(parent)
{
    httpMethod = HttpRequest::POST;
}

void HttpRequest::setHttpMethod(HttpRequest::RequestHttpMethod httpMethod)
{
    QString requestHttpMethodString;
    switch (httpMethod) {
    case HttpRequest::GET:
        requestHttpMethodString = "GET";
        break;
    case HttpRequest::POST:
        requestHttpMethodString = "POST";
        break;
    case HttpRequest::PUT:
        requestHttpMethodString = "PUT";
        break;
    case HttpRequest::HEAD:
        requestHttpMethodString = "HEAD";
        break;
    case HttpRequest::DELETE:
        requestHttpMethodString = "DELETE";
        break;
    default:
        qWarning() << "Invalid HTTP method set.";
        break;
    }
    this->httpMethod = httpMethod;
    this->httpMethodString = requestHttpMethodString;
}

HttpRequest::RequestHttpMethod HttpRequest::getHttpMethod() const
{
    return this->httpMethod;
}

void HttpRequest::setRequestEndpoint(QUrl requestEndpoint)
{
    this->requestEndpoint = requestEndpoint;
}

QUrl HttpRequest::getRequestEndpoint() const
{
    return this->requestEndpoint;
}

void HttpRequest::setRequestBody(QByteArray body)
{
    this->requestBody = body;
}

void HttpRequest::setRequestBody(QMultiMap<QString, QString> body)
{
    QList<QString> postBodyKeys = body.keys();
    QList<QString> postBodyValues = body.values();

    requestBody.clear();
    bool first = true;
    for(int i=0; i<postBodyKeys.size(); i++)
    {
        if(!first)
        {
            requestBody.append("&");
        }
        else
        {
            first = false;
        }
        QString key = postBodyKeys.at(i);
        QString value = postBodyValues.at(i);
        requestBody.append(QUrl::toPercentEncoding(key) + QString("=").toUtf8() +
                               QUrl::toPercentEncoding(value));
    }
}

QByteArray HttpRequest::getRequestBody() const
{
    return requestBody;
}

bool HttpRequest::isValid() const
{
    return(requestEndpoint.isValid());

}

void HttpRequest::setHeader(const QByteArray & headerName, const QByteArray & headerValue)
{
    this->requestHeaders.append(QPair<QByteArray,QByteArray>(headerName,headerValue));
}

QList<QPair<QByteArray,QByteArray> > HttpRequest::getHttpHeaders() const
{
    return requestHeaders;
}

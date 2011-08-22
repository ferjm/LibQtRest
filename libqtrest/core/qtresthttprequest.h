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

#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QObject>
#include <QMultiMap>
#include <QUrl>
#include <QPair>
#include <QNetworkRequest>

class HttpRequest : public QObject
{
    Q_OBJECT

public:
    explicit HttpRequest(QObject *parent = 0);
    virtual ~HttpRequest(){}

    enum RequestHttpMethod { GET, POST, PUT, HEAD, DELETE };

    virtual bool isValid() const;

    void setHttpMethod(HttpRequest::RequestHttpMethod httpMethod = HttpRequest::POST);
    void setRequestEndpoint(QUrl requestEndpoint);
    void setRequestBody(QByteArray body);
    void setRequestBody(QMultiMap<QString, QString> body);
    void setHeader(const QByteArray & headerName, const QByteArray & headerValue);

    HttpRequest::RequestHttpMethod getHttpMethod() const;
    QUrl getRequestEndpoint() const;
    QByteArray getRequestBody() const;
    QList<QPair<QByteArray,QByteArray> > getHttpHeaders() const;
    virtual void setAuthHeader() {}

protected:
    QUrl requestEndpoint;
    HttpRequest::RequestHttpMethod httpMethod;
    QString httpMethodString;
    QByteArray requestBody;
    QList<QPair<QByteArray,QByteArray> > requestHeaders;
};

#endif // HTTPREQUEST_H

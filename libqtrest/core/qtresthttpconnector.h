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

#ifndef HTTPCONNECTOR_H
#define HTTPCONNECTOR_H

#include <exception>

#include <QtNetwork>

#include "qtrestientity.h"
#include "qtresthttprequest.h"

class HttpConnectorException : public std::exception
{
public:
    /**
     * Constructor "HttpConnectorException": Creates a new HttpConnectorException.
     * Default constructor.
     *
     * @param [in] what The reason or info/error clause informing about the error.
     *
     */
    HttpConnectorException(const QString& what);
    /**
     * Destructor "~HttpConnectorException": Virtual destructor
     *
     */
    virtual ~HttpConnectorException() throw () {
    }
    /**
     * Operation "What": It retrieves the reason or info/error clause informing
     * about the error.
     *
     * @return const char*. The reason or info/error clause informing about the error.
     *
     */
    virtual const char* what() const throw ();

protected:
    QString _what; /**< The reason or info/error clause informing about the error. */
};

class HttpConnector : public QObject
{
    Q_OBJECT

    QNetworkReply *reply;
    QNetworkAccessManager *manager;
    QByteArray replyData;
    QString lastError;
public:
    HttpConnector();
    ~HttpConnector();
    QByteArray httpRequest(const HttpRequest *request);

public slots:
    void finished(QNetworkReply *);
    void readyRead();
    void error(QNetworkReply::NetworkError);

signals:
    void requestFinished(QByteArray reply);
    void headersRetrieved(QList<QNetworkReply::RawHeaderPair> headers);
};

#endif // HTTPCONNECTOR_H

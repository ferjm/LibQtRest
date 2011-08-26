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


#ifndef OAUTH_H
#define OAUTH_H

#include <QObject>

#include "qtrestoauthrequest.h"
#include "qtresthttpconnector.h"

class OAuth : public QObject
{
    Q_OBJECT
public:
    explicit OAuth(QString consumerKey = "anonymous",QString consumerSecret = "anonymous",QObject *parent = 0);
    ~OAuth();
    //TODO: add additional params
    void getRequestToken(QUrl requestTokenUrl, QUrl authorizationUrl,HttpRequest::RequestHttpMethod httpMethod = HttpRequest::GET);
    void getAccessToken(QString requestToken, QString requestTokenSecret, QString verifier,QUrl accessTokenUrl,HttpRequest::RequestHttpMethod httpMethod =  HttpRequest::GET);
    static void prepareRequest(OAuthRequest &request);

private slots:
    void onRequestTokenReceived(QByteArray response);
    void onAccessTokenReceived(QByteArray response);

private:
    HttpConnector *connector;
    QString consumerKey;
    QString consumerSecret;
    QUrl authorizationUrl;

    void initRequest(OAuthRequest &request,QUrl url,HttpRequest::RequestHttpMethod httpMethod);
signals:
    // This signal is emited when temporary tokens are returned from the service.
    // Note that this signal is also emited in case temporary tokens are not available.
    void temporaryTokenReceived(QString oauthToken, QString oauthTokenSecret, QUrl authorizationUrl);
    void accessTokenReceived(QString oauthToken, QString oauthTokenSecret);
    void error(QString errorMsg);
};

#endif // OAUTH_H

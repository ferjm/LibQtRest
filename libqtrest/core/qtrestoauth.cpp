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

#include "qtrestoauth.h"
#include "qtresthttpconnector.h"

#include <QDebug>

//#define GOOGLE_AUTHORIZATION_URL "https://www.google.com/accounts/OAuthAuthorizeToken?oauth_token="

OAuth::OAuth(QString consumerKey,QString consumerSecret,QObject *parent)
{
    connector = new HttpConnector();
    this->consumerKey = consumerKey;
    this->consumerSecret = consumerSecret;
}

OAuth::~OAuth()
{
    delete connector;
}

void OAuth::initRequest(OAuthRequest &request,QUrl url)
{
    request.setRequestEndpoint(url);
    request.setConsumerKey(this->consumerKey);
    request.setConsumerSecretKey(this->consumerSecret);
    request.setCallbackUrl(QUrl("oob"));
    request.setHttpMethod(HttpRequest::GET);
}

void OAuth::getRequestToken(QList<QUrl> scope, QUrl requestTokenUrl, QUrl authorizationUrl)
{
    this->authorizationUrl = authorizationUrl;
    OAuthRequest request(OAuthRequest::TemporaryCredentials,this);
    initRequest(request,requestTokenUrl);
    QUrl scopeUrl;
    QString scopes;
    foreach(scopeUrl,scope)
        scopes += scopeUrl.toString();
    QMultiMap<QString,QString> additionalParams;
    additionalParams.insert("scope",scopes);
    request.setAdditionalParameters(additionalParams);
    request.setAuthHeader();
    connect(connector, SIGNAL(requestFinished(QByteArray)), this, SLOT(onRequestTokenReceived(QByteArray)));
    connector->httpRequest(&request);
}

void OAuth::getAccessToken(QString requestToken, QString requestTokenSecret, QString verifier, QUrl accessTokenUrl)
{
    OAuthRequest request(OAuthRequest::AccessToken,this);
    initRequest(request,accessTokenUrl);
    request.setToken(requestToken);
    request.setTokenSecret(requestTokenSecret);
    request.setVerifier(verifier);
    request.setAuthHeader();
    connect(connector,SIGNAL(requestFinished(QByteArray)), this, SLOT(onAccessTokenReceived(QByteArray)));
    connector->httpRequest(&request);
}

QMultiMap<QString, QString> formatResult(QByteArray response)
{
    QMultiMap<QString, QString> result;
    QString replyString(response);

    QStringList parameterPairs = replyString.split('&', QString::SkipEmptyParts);
    foreach (const QString &parameterPair, parameterPairs) {
        QStringList parameter = parameterPair.split('=');
#ifdef QTGDATA_DEBUG
        qDebug() << parameter.value(0) << " = " << parameter.value(1) << "\n";
#endif
        result.insert(parameter.value(0), parameter.value(1));
    }
    return result;
}

void OAuth::onRequestTokenReceived(QByteArray response)
{
    QMultiMap<QString, QString> result = formatResult(response);
    QString requestToken = QUrl::fromPercentEncoding( QString(result.value("oauth_token")).toLocal8Bit() );
    QString requestTokenSecret =  QUrl::fromPercentEncoding( QString(result.value("oauth_token_secret")).toLocal8Bit() );
    QUrl authorizationUrl = QUrl(this->authorizationUrl.toString()+requestToken);
    emit temporaryTokenReceived(requestToken,requestTokenSecret,authorizationUrl);
}

void OAuth::onAccessTokenReceived(QByteArray response)
{
    QMultiMap<QString, QString> result = formatResult(response);
    QString accessToken = QUrl::fromPercentEncoding( QString(result.value("oauth_token")).toLocal8Bit() );
    QString accessTokenSecret =  QUrl::fromPercentEncoding( QString(result.value("oauth_token_secret")).toLocal8Bit() );
    emit accessTokenReceived(accessToken,accessTokenSecret);
}

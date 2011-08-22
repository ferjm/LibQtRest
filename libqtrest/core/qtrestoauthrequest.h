/***************************************************************************
 *   Copyright (C) 2011 Fernando Jiménez Moreno <ferjmoreno@gmail.com>     *
 *                                                                         *
 *   This class is based on KQOAuthRequest from KQOAuth Library            *
 *   by Johan Paul (johan.paul@d-pointer.com)                              *
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

#ifndef QTRESTOAUTHREQUEST_H
#define QTRESTOAUTHREQUEST_H

#include "qtresthttprequest.h"

class OAuthRequest : public HttpRequest
{
    Q_OBJECT
public:
    enum OAuthRequestType {
        TemporaryCredentials = 0,
        AccessToken,
        AuthorizedRequest
    };

    enum OAuthRequestSignatureMethod {
        PLAINTEXT = 0,
        HMAC_SHA1,
        RSA_SHA1
    };
    explicit OAuthRequest(OAuthRequest::OAuthRequestType requestType = OAuthRequest::AuthorizedRequest, QObject *parent = 0);

    void setConsumerKey(const QString &consumerKey);
    void setConsumerSecretKey(const QString &consumerSecretKey);
    void setToken(const QString &token);
    void setTokenSecret(const QString &tokenSecret);
    void setSignatureMethod(OAuthRequest::OAuthRequestSignatureMethod = OAuthRequest::HMAC_SHA1);
    void setCallbackUrl(const QUrl &callback);
    void setVerifier(const QString &verifier);
    void setAdditionalParameters(const QMultiMap<QString,QString> &additionalParams);
    QList<QByteArray> getRequestParameters();
    void signRequest();
    void prepareRequest();
    void setAuthHeader();

private:
    OAuthRequest::OAuthRequestType requestType;
    QString oauthConsumerKey;
    QString oauthConsumerSecretKey;
    QString oauthToken;
    QString oauthTokenSecret;
    QString oauthSignatureMethod;
    QUrl oauthCallbackUrl;
    QString oauthVersion;
    QString oauthVerifier;
    QString oauthTimestamp_;
    QString oauthNonce_;
    QList<QPair<QString,QString> > requestParameters;
    QMultiMap<QString,QString> additionalParams;

    QString oauthTimestamp() const;
    QString oauthNonce() const;
    QString oauthSignature();
    QByteArray requestBaseString();
    QByteArray encodedParameterList(const QList<QPair<QString,QString> > &requestParameters);
    void insertAdditionalParams(QList<QPair<QString,QString> > &requestParams);
    void insertPostBody();
    bool validateRequest() const;
    QMultiMap<QString,QString> getAdditionalParameters();
    OAuthRequest::OAuthRequestType getRequestType();
};

#endif // QTRESTOAUTHREQUEST_H

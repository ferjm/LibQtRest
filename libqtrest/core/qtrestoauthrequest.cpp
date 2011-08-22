/***************************************************************************
 *   Copyright (C) 2011 Fernando Jiménez Moreno <ferjmoreno@gmail.com>     *
 *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  *
 *   This class is based on KQOAuthRequest from KQOAuth Library            *
 *   by Johan Paul (johan.paul@d-pointer.com)                              *
 *!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
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

#include <QCryptographicHash>
#include <QDebug>
#include <QDateTime>

#include "qtrestoauthrequest.h"
#include "qtrestdefs.h"

OAuthRequest::OAuthRequest(OAuthRequest::OAuthRequestType requestType, QObject *parent) : HttpRequest(parent)
{
    this->requestType = requestType;
    oauthTimestamp_ = oauthTimestamp();
    oauthNonce_ = oauthNonce();
    oauthVersion = "1.0";
    oauthSignatureMethod = "HMAC-SHA1";
    qsrand(QTime::currentTime().msec());
}

void OAuthRequest::prepareRequest()
{
    if( !requestParameters.isEmpty() ) {
        return;
    }

    switch(requestType) {
    case OAuthRequest::TemporaryCredentials:
        requestParameters.append( qMakePair( OAUTH_KEY_CALLBACK, oauthCallbackUrl.toString()) );
        requestParameters.append( qMakePair( OAUTH_KEY_SIGNATURE_METHOD, oauthSignatureMethod) );
        requestParameters.append( qMakePair( OAUTH_KEY_CONSUMER_KEY, oauthConsumerKey ));
        requestParameters.append( qMakePair( OAUTH_KEY_VERSION, oauthVersion ));
        requestParameters.append( qMakePair( OAUTH_KEY_TIMESTAMP, this->oauthTimestamp() ));
        requestParameters.append( qMakePair( OAUTH_KEY_NONCE, this->oauthNonce() ));
        insertAdditionalParams(requestParameters);
        break;

    case OAuthRequest::AccessToken:
        requestParameters.append( qMakePair( OAUTH_KEY_SIGNATURE_METHOD, oauthSignatureMethod ));
        requestParameters.append( qMakePair( OAUTH_KEY_CONSUMER_KEY, oauthConsumerKey ));
        requestParameters.append( qMakePair( OAUTH_KEY_VERSION, oauthVersion ));
        requestParameters.append( qMakePair( OAUTH_KEY_TIMESTAMP, this->oauthTimestamp() ));
        requestParameters.append( qMakePair( OAUTH_KEY_NONCE, this->oauthNonce() ));
        requestParameters.append( qMakePair( OAUTH_KEY_VERIFIER, oauthVerifier ));
        requestParameters.append( qMakePair( OAUTH_KEY_TOKEN, oauthToken ));
        insertAdditionalParams(requestParameters);
        break;

    case OAuthRequest::AuthorizedRequest:
        requestParameters.append( qMakePair( OAUTH_KEY_SIGNATURE_METHOD, oauthSignatureMethod ));
        requestParameters.append( qMakePair( OAUTH_KEY_CONSUMER_KEY, oauthConsumerKey ));
        requestParameters.append( qMakePair( OAUTH_KEY_VERSION, oauthVersion ));
        requestParameters.append( qMakePair( OAUTH_KEY_TIMESTAMP, this->oauthTimestamp() ));
        requestParameters.append( qMakePair( OAUTH_KEY_NONCE, this->oauthNonce() ));
        requestParameters.append( qMakePair( OAUTH_KEY_TOKEN, oauthToken ));
        insertAdditionalParams(requestParameters);
        break;

    default:
        break;
    }
}

void OAuthRequest::setAuthHeader()
{
    QList<QByteArray> reqHeaders = this->getRequestParameters();
    QByteArray authHeader;
    bool first = true;
    foreach (const QByteArray header, reqHeaders) {
       if (!first) {
           authHeader.append(", ");
       } else {
           authHeader.append("OAuth ");
           first = false;
       }
       authHeader.append(header);
    }
    this->setHeader("Authorization",authHeader);
    prepareRequest();
}

void OAuthRequest::insertAdditionalParams(QList<QPair<QString,QString> > &requestParams)
{
    QList<QString> additionalKeys = this->additionalParams.keys();
    QList<QString> additionalValues = this->additionalParams.values();
    for(int i=0; i<additionalKeys.size(); i++)
        requestParams.append( qMakePair(QString(additionalKeys.at(i)),
                                            QString(additionalValues.at(i))));
    if(this->httpMethod == HttpRequest::POST) {
        insertPostBody();
    }
}

void OAuthRequest::insertPostBody()
{
//TODO: ...
}

QString hmac_sha1(const QString &message, const QString &key)
{
    QByteArray keyBytes = key.toAscii();
    int keyLength;              // Lenght of key word
    const int blockSize = 64;   // Both MD5 and SHA-1 have a block size of 64.

    keyLength = keyBytes.size();
    // If key is longer than block size, we need to hash the key
    if (keyLength > blockSize) {
        QCryptographicHash hash(QCryptographicHash::Sha1);
        hash.addData(keyBytes);
        keyBytes = hash.result();
    }

    /* http://tools.ietf.org/html/rfc2104  - (1) */
    // Create the opad and ipad for the hash function.
    QByteArray ipad;
    QByteArray opad;

    ipad.fill( 0, blockSize);
    opad.fill( 0, blockSize);

    ipad.replace(0, keyBytes.length(), keyBytes);
    opad.replace(0, keyBytes.length(), keyBytes);

    /* http://tools.ietf.org/html/rfc2104 - (2) & (5) */
    for (int i=0; i<64; i++) {
        ipad[i] = ipad[i] ^ 0x36;
        opad[i] = opad[i] ^ 0x5c;
    }

    QByteArray workArray;
    workArray.clear();

    workArray.append(ipad, 64);
    /* http://tools.ietf.org/html/rfc2104 - (3) */
    workArray.append(message.toAscii());


    /* http://tools.ietf.org/html/rfc2104 - (4) */
    QByteArray sha1 = QCryptographicHash::hash(workArray, QCryptographicHash::Sha1);

    /* http://tools.ietf.org/html/rfc2104 - (6) */
    workArray.clear();
    workArray.append(opad, 64);
    workArray.append(sha1);

    sha1.clear();

    /* http://tools.ietf.org/html/rfc2104 - (7) */
    sha1 = QCryptographicHash::hash(workArray, QCryptographicHash::Sha1);
    return QString(sha1.toBase64());
}

void OAuthRequest::signRequest()
{
    QString signature = this->oauthSignature();
    requestParameters.append( qMakePair( OAUTH_KEY_SIGNATURE, signature) );
}

QString OAuthRequest::oauthSignature()
{
    QByteArray baseString = this->requestBaseString();

    QString secret = QString(QUrl::toPercentEncoding(oauthConsumerSecretKey)) + "&" + QString(QUrl::toPercentEncoding(oauthTokenSecret));
    QString signature = hmac_sha1(baseString, secret);

#ifdef QTREST_DEBUG
    qDebug() << " * Signature : " << QUrl::toPercentEncoding(signature) << "\n";
#endif
    return QString( QUrl::toPercentEncoding(signature) );
}

bool normalizedParameterSort(const QPair<QString, QString> &left, const QPair<QString, QString> &right)
{
    QString keyLeft = left.first;
    QString valueLeft = left.second;
    QString keyRight = right.first;
    QString valueRight = right.second;

    if(keyLeft == keyRight)
        return (valueLeft < valueRight);
    else
        return (keyLeft < keyRight);
}

QByteArray OAuthRequest::requestBaseString()
{
    QByteArray baseString;
    // Every request has these as the commont parameters.
    baseString.append( httpMethodString.toUtf8() + "&");
    baseString.append( QUrl::toPercentEncoding( requestEndpoint.toString(QUrl::RemoveQuery) ) + "&" );
    QList< QPair<QString, QString> > baseStringParameters;
    baseStringParameters.append(requestParameters);
    // Sort the request parameters. These parameters have been
    // initialized earlier.
    qSort(baseStringParameters.begin(),
          baseStringParameters.end(),
          normalizedParameterSort);
    // Last append the request parameters correctly encoded.
    baseString.append( encodedParameterList(baseStringParameters) );
#ifdef QTREST_DEBUG
    qDebug() << "========== OAuthRequest has the following base string:";
    qDebug() << baseString << "\n";
#endif
    return baseString;
}

QByteArray OAuthRequest::encodedParameterList(const QList< QPair<QString, QString> > &parameters)
{
    QByteArray resultList;

    bool first = true;
    QPair<QString, QString> parameter;

    // Do the debug output.
#ifdef QTREST_DEBUG
    qDebug() << "========== OAuthRequest has the following parameters:";
#endif
    foreach (parameter, parameters)
    {
        if(!first)
            resultList.append( "&" );
        else
            first = false;

        // Here we don't need to explicitely encode the strings to UTF-8 since
        // QUrl::toPercentEncoding() takes care of that for us.
        resultList.append( QUrl::toPercentEncoding(parameter.first)     // Parameter key
                           + "="
                           + QUrl::toPercentEncoding(parameter.second)  // Parameter value
                          );
#ifdef QTREST_DEBUG
            qDebug() << " * "
                     << parameter.first
                     << " : "
                     << parameter.second;
#endif
    }
#ifdef QTREST_DEBUG
        qDebug() << "\n";
#endif
    return QUrl::toPercentEncoding(resultList);
}

QString OAuthRequest::oauthTimestamp() const
{
    if (!oauthTimestamp_.isEmpty())
        return oauthTimestamp_;
#if QT_VERSION >= 0x040700
    return QString::number(QDateTime::currentDateTimeUtc().toTime_t());
#else
   return QString::number(QDateTime::currentDateTime().toUTC().toTime_t());
#endif
}

QString OAuthRequest::oauthNonce() const
{
    if (!oauthNonce_.isEmpty())
        return oauthNonce_;

    return QString::number(qrand());
}

bool OAuthRequest::validateRequest() const
{
    switch (requestType)
    {
    case OAuthRequest::TemporaryCredentials:
        if (requestEndpoint.isEmpty()
            || oauthConsumerKey.isEmpty()
            || oauthNonce_.isEmpty()
            || oauthSignatureMethod.isEmpty()
            || oauthTimestamp_.isEmpty()
            || oauthVersion.isEmpty())
            return false;
        return true;

    case OAuthRequest::AccessToken:
        if (requestEndpoint.isEmpty()
            || oauthVerifier.isEmpty()
            || oauthConsumerKey.isEmpty()
            || oauthNonce_.isEmpty()
            || oauthSignatureMethod.isEmpty()
            || oauthTimestamp_.isEmpty()
            || oauthToken.isEmpty()
            || oauthTokenSecret.isEmpty()
            || oauthVersion.isEmpty())
            return false;
        return true;

    case OAuthRequest::AuthorizedRequest:
        if (requestEndpoint.isEmpty()
            || oauthConsumerKey.isEmpty()
            || oauthNonce_.isEmpty()
            || oauthSignatureMethod.isEmpty()
            || oauthTimestamp_.isEmpty()
            || oauthToken.isEmpty()
            || oauthTokenSecret.isEmpty()
            || oauthVersion.isEmpty())
            return false;
        return true;

    default:
        return false;
    }
    return false;
}

void OAuthRequest::setConsumerKey(const QString &consumerKey)
{
    oauthConsumerKey = consumerKey;
}

void OAuthRequest::setConsumerSecretKey(const QString &consumerSecretKey)
{
    oauthConsumerSecretKey = consumerSecretKey;
}

void OAuthRequest::setCallbackUrl(const QUrl &callback)
{
    oauthCallbackUrl = callback;
}

void OAuthRequest::setSignatureMethod(OAuthRequest::OAuthRequestSignatureMethod signatureMethod)
{
    switch(signatureMethod)
    {
    case OAuthRequest::PLAINTEXT:
        oauthSignatureMethod = "PLAINTEXT";
        break;
    case OAuthRequest::HMAC_SHA1:
        oauthSignatureMethod = "HMAC-SHA1";
        break;
    case OAuthRequest::RSA_SHA1:
        oauthSignatureMethod = "RSA-SHA1";
        break;
    default:
        qWarning() << "Invalid signature method\n";
        break;
    }
}

void OAuthRequest::setTokenSecret(const QString &tokenSecret)
{
    oauthTokenSecret = tokenSecret;
}

void OAuthRequest::setToken(const QString &token)
{
    oauthToken = token;
}

void OAuthRequest::setVerifier(const QString &verifier)
{
    oauthVerifier = verifier;
}

void OAuthRequest::setAdditionalParameters(const QMultiMap<QString,QString> &additionalParams)
{
    this->additionalParams = additionalParams;
}

QMultiMap<QString,QString> OAuthRequest::getAdditionalParameters()
{
    return this->additionalParams;
}

OAuthRequest::OAuthRequestType OAuthRequest::getRequestType()
{
    return this->requestType;
}

QList<QByteArray> OAuthRequest::getRequestParameters()
{
    QList<QByteArray> requestParamList;

    prepareRequest();
    if (!isValid() ) {
       qWarning() << "Request is not valid! I will still sign it, but it will probably not work.";
    }
    signRequest();

    QPair<QString, QString> requestParam;
    QString param;
    QString value;
    foreach (requestParam, this->requestParameters) {
       param = requestParam.first;
       value = requestParam.second;
       requestParamList.append(QString(param + "=\"" + value +"\"").toUtf8());
    }

    return requestParamList;
}


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

#include "qtrestoauthdata.h"

#define CONSUMER_KEY "consumerkey"
#define CONSUMER_SECRET "consumersecret"
#define TOKEN "token"
#define TOKEN_SECRET "tokensecret"

OAuthData::OAuthData()
{
}

void OAuthData::setConsumerKey(QString consumerKey)
{
    data.insert(CONSUMER_KEY,consumerKey);
}

void OAuthData::setConsumerSecret(QString consumerSecret)
{
    data.insert(CONSUMER_SECRET,consumerSecret);
}

void OAuthData::setToken(QString token)
{
    data.insert(TOKEN,token);
}

void OAuthData::setTokenSecret(QString tokenSecret)
{
    data.insert(TOKEN_SECRET,tokenSecret);
}

QString OAuthData::getConsumerKey()
{
    return data.value(CONSUMER_KEY).toString();
}

QString OAuthData::getConsumerSecret()
{
    return data.value(CONSUMER_SECRET).toString();
}

QString OAuthData::getToken()
{
    return data.value(TOKEN).toString();
}

QString OAuthData::getTokenSecret()
{
    return data.value(TOKEN_SECRET).toString();
}

QVariant OAuthData::getAuthentication()
{
    return data;
}

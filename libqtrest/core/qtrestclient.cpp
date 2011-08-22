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

#include "qtrestclient.h"
#include "qtrestoauthdata.h"
#include "qtrestoauthrequest.h"
#include "qtrestxmlparser.h"

QtRestClient::QtRestClient(int version, QObject *parent) : version(version)
{
    ID = BASE;
}

QtRestClient::~QtRestClient()
{
    if(authenticationData) delete authenticationData;
    authenticationData = NULL;
}

void QtRestClient::setAuthenticationData(IAuthentication *authenticationData)
{
    this->authenticationData = authenticationData;
    if(dynamic_cast<OAuthData*>(authenticationData) != NULL)
        authenticationMode = OAUTH_1_0;
}

HttpRequest* QtRestClient::authenticatedRequest()
{
    switch(authenticationMode)
    {
    case OAUTH_1_0: {
            OAuthRequest *request = new OAuthRequest();
            OAuthData *oAuthData = dynamic_cast<OAuthData*>(authenticationData);
            request->setConsumerKey(oAuthData->getConsumerKey());
            request->setConsumerSecretKey(oAuthData->getConsumerSecret());
            request->setToken(oAuthData->getToken());
            request->setTokenSecret(oAuthData->getTokenSecret());
            return dynamic_cast<HttpRequest*>(request);
            break;
        }
    default:
        return NULL;
    }
}

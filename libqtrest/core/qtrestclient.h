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

#ifndef QTRESTCLIENT_H
#define QTRESTCLIENT_H

#include <QObject>

#include "qtresthttpconnector.h"
#include "qtrestiauthentication.h"

/**
  \class QtRestClient
  */
class QtRestClient : public QObject
{
    Q_OBJECT

public:
    /*!
      Creates a new QtRestClient object
      */
    QtRestClient(int version, QObject *parent = 0);
    /**
      Destructor
      */
    virtual ~QtRestClient();

    //! Sets the client authentication data.
    /*!
      \param authenticationData Client authentication parameters
    */
    void setAuthenticationData(IAuthentication *authenticationData);

protected:
    // now the only authentication supported is oauth v1.0
    enum AuthenticationMode { OAUTH_1_0 } authenticationMode;
    enum ClientID { BASE } ID;

    /*! Instance of the HttpConnector used to send
     * and receive clients requests to the servers */
    HttpConnector httpConnector;
    /*! Client authentication data */
    IAuthentication *authenticationData;
    /*! Rest Protocol version */
    int version;

    /*! \returns instance of an authenticated http request object*/
    HttpRequest* authenticatedRequest();    
};

#endif // QTRESTCLIENT_H

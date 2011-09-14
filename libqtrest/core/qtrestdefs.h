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

#ifndef QTRESTDEFS_H
#define QTRESTDEFS_H

#include <QString>

#define Ids(...) typedef enum {__VA_ARGS__} tipo;             \
  const QString sAux(#__VA_ARGS__);

namespace Id
{
    Ids
    (
        NULLID,
        mockId,
        mockId2,
        mockId3,
        mockId4,
        smsText,
        address,
        message,
        originAddress,
        phoneNumber,
        alias
    )
}

namespace AttributeId
{
    Ids
    (
        NULLID,
        mockId,
        mockId1,
        mockId2,
        mockId3
    )
}

namespace NamespaceId {
    Ids
    (
        NULLID,
        mockId,
        tns,
        tns1,
        tns2,
        tns3
    )
}

/* OAuth strings */
const QString OAUTH_KEY_CONSUMER("oauth_consumer");
const QString OAUTH_KEY_CONSUMER_KEY("oauth_consumer_key");
const QString OAUTH_KEY_TOKEN("oauth_token");
const QString OAUTH_KEY_TOKEN_SECRET("oauth_token_secret");
const QString OAUTH_KEY_SIGNATURE_METHOD("oauth_signature_method");
const QString OAUTH_KEY_TIMESTAMP("oauth_timestamp");
const QString OAUTH_KEY_NONCE("oauth_nonce");
const QString OAUTH_KEY_SIGNATURE("oauth_signature");
const QString OAUTH_KEY_CALLBACK("oauth_callback");
const QString OAUTH_KEY_VERIFIER("oauth_verifier");
const QString OAUTH_KEY_VERSION("oauth_version");

#endif // QTRESTDEFS_H

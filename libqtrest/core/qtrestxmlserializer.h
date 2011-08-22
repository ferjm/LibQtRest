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


#ifndef XMLSERIALIZER_H
#define XMLSERIALIZER_H

#include <exception>
#include <QStringList>
#include <QtXml/QXmlStreamWriter>
#include <QMultiMap>

#include "qtrestiserializer.h"

class XMLSerializerException : public std::exception
{
public:
    /**
     * Constructor "XMLSerializerException": Creates a new XMLSerializerException.
     * Default constructor.
     *
     * @param [in] what The reason or info/error clause informing about the error.
     *
     */
    XMLSerializerException(const QString& what);
    /**
     * Destructor "~XMLSerializerException": Virtual destructor
     *
     */
    virtual ~XMLSerializerException() throw () {
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


class XMLSerializer : public ISerializer
{
private:
    QMultiMap<int,QString> _additionalNamespaces;
    QString _namespace;

    void serialize(const IEntity *obj, QXmlStreamWriter *stream);

public:
    /*!
     *
     * \param defaultNamespace Default XML namespace
     * \param additionalNamespace Additional namespaces
     *
     * XMLSerializer constructor
     */
    XMLSerializer(QString defaultNamespace = QString(),
                  QMultiMap<int,QString> additionalNamespaces = QMultiMap<int,QString>());

    /**
     * serialize:
     * @obj: IEntity object to serialize
     *
     * Recursive procedure to serialize a IEntity object
     *
     * Returns the QString containing the serialized entity
     *
     */
    QString serialize(const IEntity *obj);
};

#endif // XMLSERIALIZER_H

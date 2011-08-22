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


#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <exception>
#include <QDomNode>

#include "qtrestiparser.h"

class XMLParserException : public std::exception
{
public:
    /**
     * Constructor "XMLParserException": Creates a new XMLParserException.
     * Default constructor.
     *
     * @param [in] what The reason or info/error clause informing about the error.
     *
     */
    XMLParserException(const QString& what);
    /**
     * Destructor "~XMLParserException": Virtual destructor
     *
     */
    virtual ~XMLParserException() throw () {
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

class XMLParser : public IParser
{
public:
    XMLParser();
    IEntity* parse(void *data) {
        return(parse((QString*)data));
    }
    IEntity* parse(QString *data);
    IEntity* parse(QByteArray buff,int size);
private:
    void parseNode(IEntity *entity,QDomNode *node);
};

#endif // XMLPARSER_H

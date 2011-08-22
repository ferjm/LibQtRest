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

#include <QDomDocument>
#include <QDomElement>
#include <iostream>
#include <QDebug>
#include <QDomNamedNodeMap>
#include <QStringList>

#include "qtrestxmlparser.h"

XMLParserException::XMLParserException(const QString &what) : _what(what)
{
}

const char* XMLParserException::what() const throw()
{
    QString retval = "XMLParserException: " + _what;
    return retval.toAscii();
}

XMLParser::XMLParser()
{
}

IEntity* XMLParser::parse(QString *data)
{
    IEntity *res = NULL;
    if(!data->isEmpty())
        res = parse(data->toAscii(),data->size());
    return res;
}

/* DOM implementation */
void splitNode(QString node, QString &namesp, QString &name)
{
    if(!node.isEmpty())
    {
        QStringList strings = node.split(":");
        if(!strings.isEmpty())
        {
            if(strings.length() == 2)
            {
                namesp = strings[0];
                name = strings[1];
            } else
            {
                name = strings[0];
            }
        }
    }
}

void XMLParser::parseNode(IEntity *entity,QDomNode *node)
{
    while(!node->isNull())
    {
        QString namesp;
        QString name;
        IEntity *auxent = NULL;
        QDomElement child = node->toElement();
        if(!child.isNull())
        {
#ifdef PARSER_DEBUG
            qDebug() << child.tagName();
#endif
            splitNode(child.tagName(),namesp,name);
            if(name.isEmpty())
                throw new XMLParserException("Unable to parse node\n");
            auxent = new IEntity(namesp,name);
        }
        if(node->hasAttributes())
        {
            QDomNamedNodeMap attributes = node->attributes();
            for(unsigned int i=0;i<=attributes.length();i++)
            {
                QDomNode n = attributes.item(i);
                if(n.isAttr())
                {
#ifdef PARSER_DEBUG
                    qDebug() << n.toAttr().name()<< "=" << n.toAttr().value();
#endif
                    QString attnamesp;
                    QString attname;
                    splitNode(n.toAttr().name(),attnamesp,attname);
                    //TODO: add attribute with namespace
                    auxent->addAttribute(attname,n.toAttr().value());
                }
            }
        }
        if(node->isText())
        {
#ifdef PARSER_DEBUG
            qDebug() << node->toText().data();
#endif
            entity->addValue(node->toText().data());
        } else
            entity->addEntity(auxent);
        if (node->hasChildNodes())
            parseNode(auxent,&node->firstChild());
        node = &(node->nextSibling());
    }
}

IEntity* XMLParser::parse(QByteArray buff,int size)
{
    IEntity *ent = NULL;
    if ((buff.isNull()) || (size == 0))
        return ent;
    QDomDocument domDocument;
    if(domDocument.setContent(buff))
    {
        QDomElement root = domDocument.documentElement();
        QString namesp;
        QString name;
        splitNode(root.tagName(),namesp,name);
        if(!name.isEmpty())
            ent = new IEntity(namesp,name);
#ifdef PARSER_DEBUG
        qDebug() << root.tagName();
#endif
        QDomNode node = root.firstChild();
        parseNode(ent,&node);
    }
#ifdef QTREST_DEBUG
    if(ent!=NULL)
        qDebug() << ent->toString();
#endif
    return ent;
}

//TODO: SAX implementation

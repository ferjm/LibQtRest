#include "qtrest.h"
#include "qtrestdefs.h"

QtRest QtRest::qtrest;

QtRest::QtRest()
{
}

QtRest::~QtRest()
{
}

QStringList& QtRest::getAttributes()
{
    if (attributes.isEmpty())
        attributes = AttributeId::sAux.split(QString(", "));
    return attributes;
}

QStringList& QtRest::getProperties()
{
    if (properties.isEmpty())
        properties = Id::sAux.split(QString(", "));
    return properties;
}

QStringList& QtRest::getNamespaces()
{
    if (namespaces.isEmpty())
        namespaces = NamespaceId::sAux.split(QString(", "));
    return namespaces;
}

QtRest* QtRest::getInstance()
{
    return &qtrest;
}

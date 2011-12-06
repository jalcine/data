#include "query.hpp"
#include <QStringList>

namespace Wintermute {
namespace Data {
namespace Ontology {

Query::Query(QObject *parent) :
    QObject(parent), blockOpened(false), base("")
{
}

QString Query::getContents()
{
    return base + contents;
}

void Query::setBase(const QUrl &baseIRI)
{
    base = baseIRI.toString();
}

void Query::addPrefix(const QString &pxName, const QUrl &pxIRI)
{
    contents += "PREFIX " + pxName + ": <" + pxIRI.toString() + ">\n";
}

void Query::addEndpoint(const QUrl &eURL)
{
    contents += "FROM <" + eURL.toString() + ">\n";
}

void Query::addVariables(QString form, const QStringList &vars)
{
    contents += form + QString(" ");
    foreach(QString var, vars)
    {
        contents += "?" + var + " ";
    }
    contents += "\n";
}

void Query::addTriple(QString subject, QString predicate, QString object)
{
    if (!blockOpened)
    {
        contents += "WHERE {\n";
        blockOpened = true;
    }
    contents += subject + " ";
    contents += predicate + " ";
    contents += object + " .\n";
}

void Query::closeQuery()
{
    if (blockOpened)
    {
        contents += "}\n";
    }
}

}}} // namespaces

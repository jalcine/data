/**
 * @file ontology.hpp
 * @author Wintermute Developers <wintermute-devel@lists.launchpad.net>
 * @date March 29, 2011, 2:25 PM
 * @namespace Wintermute::Data::Ontology
 * @legalese
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 * @endlegalese
 *
 */

#ifndef ONTOLOGY_HPP
#define	ONTOLOGY_HPP

#include <QString>
#include <QVector>
#include <Soprano/Model>

using namespace std;

using std::map;
using std::pair;
using std::string;

namespace Wintermute {
    namespace Data {
        namespace Ontology {
            struct System;
            struct Repository;
            struct Resource;

            /**
             * @brief Configuration class for the ontology section.
             *
             *
             *
             * @class Configuration ontology.hpp "include/wntr/data/ontology.hpp"
             */
            class System : public QObject {
                Q_OBJECT

                public:
                    /**
                     * @brief Initializes the ontology system.
                     * Runs all of the necessary initialziation code to get the ontology system on its toes.
                     * @fn Initialize
                     */
                    Q_INVOKABLE static void load();
                    /**
                     * @brief Deinitializes the ontology system/
                     * Runs all of the necessary deinitialization code to have the ontology system be safely shut down.
                     * @fn Deinitialize
                     */
                    Q_INVOKABLE static void unload();
            };

            /**
             * @brief Represents an object being described.
             *
             * According to the definition of resources in the RDF syntax documentation
             * <http://www.w3.org/TR/REC-rdf-syntax> it's defined as,
             * <blockquote>
             * All things being described by RDF expressions are called resources.
             * A resource may be an entire Web page; such as the HTML document "http://www.w3.org/Overview.html"
             * for example. A resource may be a part of a Web page; e.g. a specific HTML or XML element within
             * the document source. A resource may also be a whole collection of pages; e.g. an entire Web site.
             * A resource may also be an object that is not directly accessible via the Web; e.g. a printed book.
             * Resources are always named by URIs plus optional anchor ids. Anything can have a URI; the extensibility
             * of URIs allows the introduction of identifiers for any entity imaginable.
             * </blockquote>
             *
             * This class provides a means of convience over RDF-XML (mainly SPARQL and RDFQL) so that
             * properties of resources can be properly queried.
             *
             * @class Resource ontology.hpp "include/wntr/data/ontology.hpp"
             */
            class Resource : public QObject {
                public:
                    /**
                     * @brief Default constructor.
                     *
                     * @fn Resource
                     * @param p_parent The parent object.
                     */
                    explicit Resource( const QObject* = NULL );

                    /**
                     * @brief Initializating constructor.
                     *
                     * Creates a new Resource from its parent Repository and
                     * its corresponding Node within the ontology.
                     *
                     * @fn Resource
                     * @param p_repo The parent repository.
                     * @param p_node The Soprano::Node that this Resource represents.
                     */
                    Resource( const Repository* , const Soprano::Node& );

                    /**
                     * @brief Copy constructor.
                     *
                     * @fn Resource
                     * @param p_resource The Resource to be copied.
                     */
                    Resource( const Resource& );

                    /**
                     * @brief Deconstructor.
                     *
                     * @fn ~Resource
                     */
                    virtual ~Resource();

                    /**
                     * @brief
                     *
                     * @fn object
                     * @param
                     */
                    const Resource* object(const QString& ) const;

                    /**
                     * @brief
                     *
                     * @fn object
                     * @param
                     */
                    const Resource* object(const Resource& ) const;

                    /**
                     * @brief
                     *
                     * @fn setObject
                     * @param
                     */
                    void setObject(const QString& );

                    /**
                     * @brief
                     *
                     * @fn setObject
                     * @param
                     */
                    void setObject(const Resource& );

                private:
                    const Repository* m_repo;
                    Soprano::Node m_node;

            };

            /**
             * @brief Represents an entire ontological store.
             *
             * Repostiories represent the entire encompassing set of Resources
             * in their native format (that being RDF-XML, for now).
             *
             * @todo Obtain the specified Resource from the internal ontology when requested.
             *
             * @class Repository ontology.hpp "include/wntr/data/ontology.hpp"
             */
            class Repository : public QObject {
                Q_OBJECT
                signals:
                    void loaded() const;

                public:
                    explicit Repository(const QObject* = NULL);
                    Repository(const QString&);
                    Repository(const Repository&);
                    virtual ~Repository();
                    const Resource* obtainResource(const QString &) const;

                    static const Repository* obtainRepository(const QString&);
                    static const Resource* obtainResource(const QString&, const QString&);

                private:
                    void load(const QString& = "") const;
                    const QString getPath() const;
                    mutable QString m_repo;
                    mutable Soprano::Model* m_model;
            };
        }
    }
}

#endif	/* ONTOLOGY_HPP */

// kate: indent-mode cstyle; space-indent on; indent-width 4;

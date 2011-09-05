/**
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
 * @file ontology.cpp
 * @author Wintermute Developers <wintermute-devel@lists.launchpad.net>
 * @date March 29, 2011, 2:25 PM
 * @todo Determine a means of using Soprano to interpret RDF/XML (OWL) files into semantic information.
 * @todo Determine how and where UUIDs will be obtained. (Boost::UUID anyone? :])
 */

#include "config.hpp"
#include "ontology.hpp"
#include "wntrdata.hpp"
#include "unctrl.h"
#include <QtDebug>
#include <Soprano/Soprano>
#include <boost/progress.hpp>

using namespace Soprano;

namespace Wintermute {
    namespace Data {
        namespace Ontology {

            void Configuration::Initialize() {
                //const Repository* l_repo = Repository::obtainRepository("COSMO");
                qDebug() << "(data) [Ontology::Configuration] Loaded.";
            }

            void Configuration::Deinitialize() {
                qDebug() << "(data) [Ontology::Configuration] Unloaded.";
            }

            Repository::Repository(const QString &p_str) : m_repo(p_str) { load(); }

            Repository::Repository(const Repository &p_repo) : m_repo(p_repo.m_repo), m_model(p_repo.m_model) { }

            const Repository* Repository::obtainRepository(const QString& p_repoName){
                Repository* l_repo = new Repository(p_repoName);
                //Repository::s_repos.insert(&p_repoName,l_repo);
                return l_repo;
            }

            const QString Repository::getPath() const {
                return QUrl::fromLocalFile (QString::fromStdString (Data::Configuration::directory ()) + QString("/")
                        + QString(WNTRDATA_ONTO_DIR) + QString("/") + m_repo + QString(".owl")).toString ();
            }

            void Repository::load(const QString& p_repoName) const {
                qDebug() << "(data) [Repository] Loading ontology" << m_repo << "...";
                if (!p_repoName.isEmpty ())
                    m_repo = p_repoName;

                m_model = Soprano::createModel ();
                const QUrl l_url = getPath();
                const Parser* l_rdfPrsr = PluginManager::instance()->discoverParserForSerialization( SerializationRdfXml );
                StatementIterator l_itr = l_rdfPrsr->parseFile( l_url.toLocalFile (), l_url, SerializationRdfXml );
                m_model->addStatements (l_itr.allStatements ());
                qDebug() << "(data) [Repository] Loaded ontology" << m_repo << "with" << m_model->listStatements ().allStatements ().size () << "statements.";
                emit loaded();
                this->obtainResource ("Boy");
           }

            /// @todo Figure out how to use SPARQL to obtain a resource. I (Jacky A.) am having no luck whatsoever.
            const Resource* Repository::obtainResource(const QString& p_res) const {
                return NULL;
            }

            const Resource* Repository::obtainResource(const QString& p_repository, const QString& p_resource){
                Repository* l_repo = new Repository(p_repository);
                return l_repo->obtainResource(p_resource);
            }

            Repository::~Repository() { qDebug() << "Destroyed repository" << m_repo << "."; }
        }
    }
}
// kate: indent-mode cstyle; space-indent on; indent-width 4;

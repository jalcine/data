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
#include "query.hpp"
#include "wntrdata.hpp"
#include <QtDebug>
#include <Soprano/Soprano>
#include <boost/progress.hpp>

using namespace Soprano;

namespace Wintermute {
    namespace Data {
        namespace Ontology {
            QMap<QString, Repository*> Repository::s_repos;

            QUrl System::getSystemOntology() {
                return QUrl(QString(WNTRDATA_ONTO_DIR)+QString("/COSMO.owl"));
            }

            void System::load() {
                const Repository* l_repo = Repository::obtainRepository("COSMO");
                qDebug() << "(data) [System] # ontology # Loaded.";
            }

            void System::unload() {
                qDebug() << "(data) [System] # ontology # Unloaded.";
            }

            const int Resource::countConcepts () { return 0; }

            Repository::Repository(const QString &p_str) : m_repoName(p_str) { load(); }

            Repository::Repository(const Repository &p_repo) : m_repoName(p_repo.m_repoName), m_model(p_repo.m_model) { }

            Repository* Repository::obtainRepository(const QString& p_repoName) {
                Repository* l_repo = NULL;
                
                if (Repository::s_repos.count(p_repoName) == 0) {
                    l_repo = new Repository(p_repoName);
                    Repository::s_repos.insert(p_repoName,l_repo);
                } else 
                    l_repo = Repository::s_repos.value(p_repoName);
                    
                return l_repo;
            }

            const int Repository::countOntologies () { return 0; }

            const QString Repository::getPath() const {
                return QUrl::fromLocalFile (Data::System::directory () + QString("/")
                        + QString(WNTRDATA_ONTO_DIR) + QString("/") + m_repoName + QString(".owl")).toString ();
            }

            void Repository::load(const QString& p_repoName) const {
                qDebug() << "(data) [Repository] Loading ontology" << m_repoName << "...";
                if (!p_repoName.isEmpty ())
                    m_repoName = p_repoName;
           }

            Resource* Repository::obtainResource(const Concept& concept) const {
                Query query;
                query.setBase(QUrl("http://micra.com/COSMO/COSMO.owl#"));
                query.addPrefix("rdf", QUrl("http://www.w3.org/1999/02/22-rdf-syntax-ns#"));
                query.addPrefix("owl", QUrl("http://www.w3.org/2002/07/owl#"));

                QStringList variables;
                variables.append("x");
                query.addVariables("SELECT", variables);

                query.addTriple("?x", "owl:Class", "?res");
                query.addTriple("?res", "rdf:ID", concept);

                QueryResultIterator it = m_model->executeQuery(query.getContents());
                QList<BindingSet> bindingSets = it.allBindings();
                return new Resource(bindingSets[0]["x"], this);
            }

            Resource* Repository::obtainResource(const QString& p_repository, const QString& p_res){                
                Repository* l_repo = Repository::obtainRepository(p_repository);
                if (l_repo == NULL) return NULL;
                else return l_repo->obtainResource(p_res);
            }

            Repository::~Repository() { qDebug() << "Destroyed repository" << m_repoName << "."; }
        }
    }
}
// kate: indent-mode cstyle; space-indent on; indent-width 4;

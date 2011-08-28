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
 * @author Jacky Alcine
 * @date March 29, 2011, 2:25 PM
 * @todo Determine a means of using Soprano to interpret RDF/XML (OWL) files into semantic information.
 * @todo Determine how and where UUIDs will be obtained. (Boost::UUID anyone? :])
 */

#include "config.hpp"
#include "ontology.hpp"
#include "wntrdata.hpp"
#include "unctrl.h"
#include <QtDebug>
#include <boost/progress.hpp>

namespace Wintermute {
    namespace Data {
        namespace Ontology {
            QMap<const QString*, Repository*> Repository::s_repos;
            void Configuration::Initialize() {
                Repository* l_repo = Repository::obtain("COSMO");
                qDebug() << "(data) [Ontology::Configuration] Loaded.";
            }

            void Configuration::Deinitialize() {
                QList<Repository*> l_repos = Repository::s_repos.values ();

                foreach (Repository* l_repo, l_repos)
                    delete l_repo;

                qDebug() << "(data) [Ontology::Configuration] Unloaded.";
            }

            Repository::Repository( QObject* parent ) : QObject(parent) { }

            Repository::Repository( const Repository& p_repo ) : m_repoName(p_repo.m_repoName), m_model(p_repo.m_model) { }

            Repository::Repository( QString p_repoName, QObject* parent ) : m_repoName(p_repoName), QObject(parent) { load(); }

            Repository* Repository::obtain(QString p_repoName){
                Repository* l_repo = new Repository(p_repoName);
                Repository::s_repos.insert(&p_repoName,l_repo);
                return l_repo;
            }

            const QString Repository::getPath() const {
                return QString::fromStdString (Data::Configuration::getDirectory ()) + QString("/")
                        + QString(WNTRDATA_ONTO_DIR) + QString("/") + m_repoName + QString(".owl");
            }

            void Repository::load(const QString p_repoName){
                if (!p_repoName.isEmpty ())
                    m_repoName = p_repoName;

                const QString l_url = getPath();
                m_model = Soprano::createModel ();
                const Soprano::Parser* l_rdfPrsr = Soprano::PluginManager::instance()->discoverParserForSerialization( Soprano::SerializationRdfXml );
                Soprano::StatementIterator l_itr = l_rdfPrsr->parseFile( l_url , l_url, Soprano::SerializationRdfXml );
                QList<Soprano::Statement> l_stats = l_itr.allStatements ();
                m_model->addStatements (l_itr.allStatements ());

                qDebug() << "(data) [Repository] Loading ontology" << m_repoName << "...";
                emit loading();

                {
                    boost::progress_display show_progress( l_stats.length () );
                    boost::progress_timer timer;
                    foreach(Soprano::Statement l_st, l_stats){
                        emit loadingProgress((double) show_progress.count() / (double) l_stats.length () );
                        ++show_progress;
                    }
                }

                emit loaded();
                qDebug() << "(data) [Repository] Loading ontology" << m_repoName << ".";
            }

            Repository::~Repository() { qDebug() << "repo"; }
        }
    }
}
// kate: indent-mode cstyle; space-indent on; indent-width 4;

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
#include <boost/timer.hpp>
#include <boost/progress.hpp>
#include <QUrl>
#include <QtXml>

using namespace Soprano;
using namespace boost;

using boost::progress_display;

namespace Wintermute {
    namespace Data {
        namespace Ontology {
            LinkList* Link::allLinks = new LinkList;
            StoreList* Store::allStores = new StoreList;
            ConceptList* Concept::allConcepts = new ConceptList;
            InstanceList* Instance::allInstances = new InstanceList;

            /* begin class Configuration */
            void Configuration::Initialize(){
                cout << "(database) [Ontology::Configuration] Loaded." << endl;
            }

            void Configuration::Deinitialize(){
                cout << "(database) [Ontology::Configuration] Unloaded." << endl;
            }

            /* end class Configuration */

            /* begin class Store */

            Store::Store() : _uuid(""), _model(NULL) {}

            /// @todo Load every single Statement as a Concept in the background.
            /*
              Approach!
              ---------------
              Make a Soprano::Model and load all of the statements from the parser into the model.
              Iteratively query for each statement and pull out the extras (use those to form a link and its respective concepts).

              Tada?
            */
            Store::Store( const string& storeUUID ) : _uuid(storeUUID), _model(NULL){
                cout << "(database) [Store] (UUID: '" << storeUUID << "') Initializing.. " << endl;
                Model *theModel = createModel();
                const QString theUrl((string(Data::Configuration::getDirectory() + string("/ontology/") + storeUUID + string(".owl"))).c_str());
                const QUrl ontologyUrl(theUrl);
                cout <<theUrl.toStdString ()<< endl;

                const Soprano::Parser* parser = Soprano::PluginManager::instance()->discoverParserForSerialization( Soprano::SerializationRdfXml );
                StatementIterator itr = parser->parseFile (theUrl, ontologyUrl, Soprano::SerializationRdfXml);
                cout << "(database) [Store] (UUID: '" << storeUUID << "') Loading from '" << ontologyUrl.toString ().toStdString () << "'.." << endl;
                QList<Statement> statementList = itr.allStatements ();

                {
                    progress_display theStatus(statementList.count (),cout," "," "," ");
                    progress_timer theTimer;

                    Q_FOREACH(Statement s, statementList){
                        if (theModel->addStatement (s) != Error::ErrorNone){
                            cout << "(database) [Store] (UUID: '" << storeUUID << "') Parse error: "
                                 << theModel->lastError ().message ().toStdString () << endl;
                        }
                        ++theStatus;
                    }

                    cout << "(database) [Store] (UUID: '" << storeUUID << "') Parsing elapsed in ";
                }

                cout << "(database) [Store] (UUID: '" << storeUUID << "') " << statementList.count() << " concepts recognized. " << endl;
            }

            void Store::saveInstance( const Instance& theInstance, const bool& canOverwrite ) {}
            void Store::saveConcept( const Concept& theConcept, const bool& canOverwrite ) {}
            void Store::saveLink ( const Link& theLink, const bool& canOverwrite ) {}
            //void Store::save();

            Store* Store::obtain( const string& storeUUID ) {
                Store *theStore = NULL;
                StoreList::iterator itr = allStores->find (storeUUID);
                if (itr == allStores->end ()){
                    theStore = new Store(storeUUID);
                    allStores->insert (StoreList::value_type(storeUUID,theStore));
                } else { theStore = itr->second; }

                return theStore;
            }

            const Concept* Store::obtainConcept( const string& storeUUID, const string& conceptUUID ) { return NULL; }

            Instance* Store::obtainInstance( const string& storeUUID, const string& instanceUUID ) { return NULL; }

            Store::~Store() {}

            /* end class Store */

            /* begin class Concept */

            Concept::Concept() {}

            /// @todo Copy only the links and UUID information.
            Concept::Concept( const Concept& copiedConcept ) {
                this->_uuid = copiedConcept._uuid;
                this->_links = copiedConcept._links;
            }

            /// @todo Determine a means of loading ontology information into this class. COSMO is in OWL, so Soprano or Raptor (in the repos) would work.
            Concept::Concept( const string& conceptUUID ) {
                this->_uuid = conceptUUID;
            }

            Concept::~Concept() {}

            /// @todo: Add code to load specific information about this concept.
            /// @todo: Add code to load attributes about this concept.
            void Concept::initialize(){
                _links = NULL;
            }

            const string& Concept::getUUID() const { return this->_uuid; }

            template<class Archive>
            void Concept::serialize(Archive & ar, const unsigned int version){
                ar & _uuid;
                ar & _links;
            }

			const Concept* Concept::obtain( const string& conceptUUID ) {
				if ( allConcepts->find(conceptUUID) == allConcepts->end()){
					const Concept* aNewConcept = new Concept(conceptUUID);
					allConcepts->insert(ConceptList::value_type(conceptUUID,aNewConcept));
					return aNewConcept;
				} else
					return allConcepts->find(conceptUUID)->second;
			}

			const LinkList* Concept::getLinks() const { return _links; }

			const Link* Concept::getLink( const string& linkUUID ) const {
				const LinkList::iterator result = _links->find(linkUUID);
				if ( result == _links->end() )
					return NULL;
				else
					return result->second;
			}

			/* end class Concept */
			/* begin class Link */

            Link::Link() : _relation(NULL),
                _parent(NULL),
                _child(NULL),
                _uuid("") { this->initialize();	}

            Link::Link( const Link& copiedLink ) : _parent(copiedLink._parent),
                _relation(copiedLink._relation),
                _uuid(copiedLink._uuid),
                _child(copiedLink._child) { this->initialize(); }

            Link::Link( const Concept* parentConcept, const Concept* childConcept, const Concept* relationalConcept ) : _uuid(""),
                _relation(relationalConcept),
                _parent(parentConcept),
                _child(childConcept) { this->initialize(); }

            Link::Link( const string& savedUUID ) : _uuid(savedUUID),
                _parent(NULL),
                _relation(NULL),
                _child(NULL) { this->initialize(); }

            Link::~Link() {}

            template<class Archive>
            void Link::serialize(Archive & ar, const unsigned int version){
                ar & _parent;
                ar & _child;
                ar & _relation;
                ar & _uuid;
            }

            void Link::initialize() {
                /// @todo: Add code on how to load link information (or copy pre-existing information into this.)
                /// @todo: Add code on finding the parent concept and child concept (ie: load if not existing, nullify if not existing, etc. )
            }

            const Concept* Link::getParentConcept() const { return _parent; }

            const Concept* Link::getChildConcept() const { return _child; }

            const Concept* Link::getRelationship() const { return _relation; }

            const Link* Link::setParentConcept( const Concept* newParent ) { return new Link(newParent,_child,_relation); }

            const Link* Link::setChildConcept( const Concept* newChild ) { return new Link(_parent,newChild,_relation); }

            const Link* Link::setRelationship( const Concept* newRelation ) { return new Link(_parent,_child,newRelation); }

			const Link* Link::obtain( const string& linkUUID ) {
				if ( allLinks->find(linkUUID) == allLinks->end() ){
					const Link* aNewLink = new Link(linkUUID);
					allLinks->insert(LinkList::value_type(linkUUID,aNewLink));
					return aNewLink;
				} else
					return allLinks->find(linkUUID)->second;
			}

			/* end class Link */
			/* begin class Instance */

			Instance::Instance() : Concept() {}

			Instance::Instance( const Concept& baseConcept ) : Concept(baseConcept) {
				this->_links = baseConcept._links;
			}

			Instance::Instance( const string& baseConceptUUID ) : Concept(baseConceptUUID) {
				const Concept* baseConcept = Concept::obtain(baseConceptUUID);
				this->_links = baseConcept->_links;
			}

			const Instance* Instance::obtain(const string& uuid){ return NULL; }

			void Instance::initialize (){}

			void Instance::setLink( const string& linkUUID, const Link* newLink ) {
				if (this->_links->find(linkUUID) != this->_links->end()){
					/// @todo: Swap this link for a new one.
				}
				else
					this->_links->insert(LinkList::value_type(linkUUID,newLink));
			}

			void Instance::setLinks( const LinkList* linkList ) {
				for (LinkList::const_iterator itr = linkList->begin(); itr != linkList->end(); itr++){
					const LinkList::const_iterator query = linkList->find(itr->first);
					if (query == this->_links->end()) // not found; so add!
						this->_links->insert(LinkList::value_type(itr->first,itr->second));
					else
						this->_links->find(itr->first)->second = itr->second;
				}
			}

            /* end class Instance */
        }
    }
}

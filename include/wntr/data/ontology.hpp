/**
 * @file ontology.hpp
 * @author Jacky Alcine
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
 */

#ifndef ONTOLOGY_HPP
#define	ONTOLOGY_HPP

#include <map>
#include <string>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <Soprano/Soprano>

using namespace std;
using namespace Soprano;

using std::map;
using std::pair;
using std::string;
using Soprano::Model;

namespace Wintermute {
    namespace Data {
        namespace Ontology {
            struct Store;
            struct Concept;
            struct Instance;
            struct Link;
            struct Configuration;
            typedef map<const string, Store*> StoreList;
            typedef map<const string, const Concept*> ConceptList;
            typedef map<const string, const Link*> LinkList;
            typedef map<const string, Instance*> InstanceList;

            /**
             * @brief Management class for the ontologies.
             * This class manages every and anything having to deal with ontologies.
             * @class Configuration ontology.hpp "include/wntr/data/ontology.hpp"
             */
            class Configuration {
            public:
                /**
                 * @brief
                 *
                 * @fn Initialize
                 */
                static void Initialize();
                /**
                 * @brief
                 *
                 * @fn Deinitialize
                 */
                static void Deinitialize();
            };

            /**
             * @brief Maintainer class for manipulating Concepts, Instances and Links.
             * Represents the storage of all concepts. This provides a data store of
             * sorts to hold concepts and instances; consider concepts books and this
             * a bookshelf.
             * @class Store ontology.hpp "include/wntr/data/ontology.hpp"
             * @see Concept
             * @see Instance
             * @see Link
             * @note This class should be able to be serialized and deserialized at whim (consider doing this asynchronously as well?)
             * @todo Apply serialization and deserialization of this class.
             */
            class Store {
                friend struct Concept;
                friend struct Instance;
                friend struct Link;
                private:
                    string _uuid;
                    Model* _model;

                protected:
                    static StoreList* allStores;
                    /**
                     * @brief Default constructor.
                     * Creates a new empty Store.
                     * @fn Store
                     */
                    explicit Store();
                    /**
                     * @brief Constructor, loads from UUID.
                     * Creates a Store by loading it from a specified location by its designated UUID.
                     * @fn Store
                     * @param  uuid The UUID of the Store to load.
                     */
                    Store( const string& );

                    ~Store();
                public:
                    /**
                     * @brief
                     *
                     * @fn hasConcept
                     * @param
                     */
                    void hasConcept(const string& );
                    /**
                     * @brief
                     *
                     * @fn hasLink
                     * @param
                     */
                    void hasLink(const string& );
                    /**
                     * @brief
                     *
                     * @fn hasInstance
                     * @param
                     */
                    void hasInstance(const string& );
                    /**
                     * @brief Save instance to store.
                     * Saves a reference of an Instance to this Store; overwriting it if necessary.
                     * @fn saveInstance
                     * @param Instance|instance The instance to save.
                     * @param bool|canOverwrite Determines whether or not this can be overwritten (defaults to true, can be omitted).
                     */
                    void saveInstance( const Instance&, const bool& = true );
                    /**
                     * @brief
                     *
                     * @fn saveLink
                     * @param
                     * @param
                     */
                    void saveLink( const Link&, const bool& = true );
                    /**
                     * @brief
                     *
                     * @fn saveConcept
                     * @param
                     * @param
                     */
                    void saveConcept( const Concept&, const bool& = true );
                    //void save();
                    /**
                     * @brief
                     *
                     * @fn obtain
                     * @param
                     */
                    static Store* obtain( const string& );
                    /**
                     * @brief
                     *
                     * @fn obtainConcept
                     * @param
                     * @param
                     */
                    static const Concept* obtainConcept( const string&, const string& );
                    /**
                     * @brief
                     *
                     * @fn obtainInstance
                     * @param
                     * @param
                     */
                    static Instance* obtainInstance( const string&, const string& );
            };

            /**
             * @brief Individual topic.
             * Represents a bit of information; an isolated idea. A concept is purely
             * a lone nueron in a mind, without a link, only with its own meaning.
             * @class Concept ontology.hpp "include/wntr/data/ontology.hpp"
             * @todo Wrap this class around Soprano::Statement and see how it can purely represent a concept.
             */
            class Concept {
                protected:
                    friend class Instance;
                    friend class boost::serialization::access;

                    static ConceptList* allConcepts;
                    string _uuid;
                    LinkList* _links;

					template<class Archive>
					void serialize( Archive&, const unsigned int );
					/**
					 * @brief Initializes concept.
					 * Does initialization work for the Concept.
					 * @fn initialize
					 */
					virtual void initialize();
					/**
					 * @brief Default constructor.
					 * Creates a new empty Concept.
					 * @fn Concept
					 */
					explicit Concept();
					/**
					 * @brief Default copy constructor.
					 * Creates a copy of a previously existing Concept.
					 * @fn Concept
					 * @param Concept|copiedConcept The concept to be copied.
					 */
					Concept( const Concept& );
					/**
					 * @brief Constructs from deserialization.
					 * Creates a new Concept from an UUID.
					 * @fn Concept
					 * @param conceptUUID The UUID of the Concept to load.
					 */
					Concept( const string& );
					~Concept();
				public:
					/**
					 * @brief Return UUID.
					 * Obtains the UUID of this Concept.
					 * @fn getUUID
					 * @return string The UUID.
					 */
					virtual const string& getUUID() const;
					/**
					 * @brief Obtain link.
					 * Obtains the Link determined by the specified UUID.
					 * @fn getLink
					 * @param  The UUID of the link to obtained.
					 * @return Link* A pointer to the found link or NULL.
					 */
					const Link* getLink( const string& ) const;
					/**
					 * Obtains all of the Links that this Concept has.
					 * @return LinkList* The links of this Concept or NULL.
					 */
					const LinkList* getLinks() const;
					/**
					 * Obtains a Concept; creates it if not already created.
					 * @return Concept* The Concept found (or created).
					 */
					static const Concept* obtain( const string& );
			};

            /**
             * Represents the bond formed by linking a parent concept to a child concept, by means of
             * a relationship concept. This is considered knowledge. Links are practically concept triples.
             */
            class Link {
                private:
                    friend class boost::serialization::access;
                    static LinkList* allLinks;

					string _uuid;
					const Concept* _parent;
					const Concept* _relation;
					const Concept* _child;

					template<class Archive>
					void serialize(Archive& , const unsigned int );
					/**
					 * @brief
					 * Does the initialization work for the Link.
					 */
					virtual void initialize();
					/**
					 * @brief
					 * Creates an empty Link.
					 */
					Link();
					/**
					 * @brief
					 * Creates a Link by copying a previously existing Link.
					 * @param Link& The Link to be copied.
					 */
					Link( const Link& );
					/**
					 * @brief
					 * Binds two node concepts by a relationship concept and returns the result in the form of a Link.
					 * @param Concept* The l-concept.
					 * @param Concept* The r-concept.
					 * @param Concept* The relating concept.
					 */
					Link( const Concept*, const Concept*, const Concept* );
					/**
					 * @brief
					 * Obtains a Link from disk by its UUID.
					 */
					Link( const string& );
					~Link();
				public:

					/**
					 * @brief
					 *
					 * @fn getParentConcept
					 */
					const Concept* getParentConcept() const;

					/**
					 * @brief
					 *
					 * @fn getChildConcept
					 */
					const Concept* getChildConcept() const;

					/**
					 * @brief
					 *
					 * @fn getRelationship
					 */
					const Concept* getRelationship() const;

					/**
					 * @brief
					 *
					 * @fn getUUID
					 */
					const string& getUUID() const;

					/**
					 * @brief
					 *
					 * @fn setParentConcept
					 * @param
					 */
					const Link* setParentConcept( const Concept* );

					/**
					 * @brief
					 *
					 * @fn setChildConcept
					 * @param
					 */
					const Link* setChildConcept( const Concept* );

					/**
					 * @brief
					 *
					 * @fn setRelationship
					 * @param
					 */
					const Link* setRelationship( const Concept* );

					/**
					 * @brief
					 *
					 * @fn obtain
					 * @param
					 */
					static const Link* obtain( const string& );

					/**
					 * @brief
					 *
					 * @fn form
					 * @param
					 * @param
					 * @param
					 */
					static const Link* form( const Concept*, const Concept*, const Concept* );

					/**
					 * @brief
					 *
					 * @fn form
					 * @param
					 * @param
					 * @param
					 */
					static const Link* form( const string&, const string&, const string& );
			};

            /**
             * @brief An editable instance of an Concept.
             * Represents a concept that's been modified via means of changing the
             * bonds of one or more of its links. The special thing about this class
             * is that it's allowed to modify the links it has; unlike a typical Concept.
             * The changes are not saved to the base concept and thus allows a specialized
             * version of such a class.
             * @class Instance ontology.hpp "include/wntr/data/ontology.hpp"
             */
            class Instance : public Concept {
                private:
                    static InstanceList* allInstances;
                    friend class boost::serialization::access;
                    template<class Archive>
                    void serialize(Archive& , const unsigned int );

                    /**
                     * @brief
                     *
                     * @fn initialize
                     */
                    virtual void initialize();

                public:

                    /**
                     * @brief
                     *
                     * @fn Instance
                     */
                    Instance();

                    /**
                     * @brief
                     *
                     * @fn Instance
                     * @param
                     */
                    Instance( const Concept& );

                    /**
                     * @brief
                     *
                     * @fn Instance
                     * @param
                     */
                    Instance( const string& );

                    /**
                     * @brief
                     *
                     * @fn setLink
                     * @param
                     * @param
                     */
                    void setLink( const string&, const Link* );

                    /**
                     * @brief
                     *
                     * @fn setLinks
                     * @param
                     */
                    void setLinks( const LinkList* );

                    /**
                     * @brief
                     *
                     * @fn obtain
                     * @param
                     */
                    static const Instance* obtain( const string& );

                    /**
                     * @brief
                     *
                     * @fn create
                     * @param
                     */
                    static const Instance* create( const Concept* );

					/**
					 * @brief
					 *
					 * @fn create
					 * @param
					 */
					static const Instance* create( const string& );
			};
		}
	}
}

#endif	/* ONTOLOGY_HPP */


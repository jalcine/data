/**
 * @file models.hpp
 * @author Adrian Borucki <gentoolx@gmail.com>
 * @date Fri, 13 May 21:54:16
 *
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
 * @todo When the plugin system is up-and-functional, remove all backends and storage systems (except for the local system) and implement each type of data in its own plugin.
 */

#ifndef MODELS_HPP
#define MODELS_HPP

#include "linguistics.hpp"

#include <map>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/map.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/signals2/slot.hpp>
#include <QtXml/QDomElement>

using namespace std;
using namespace boost::serialization;

using std::string;
using std::multimap;
using boost::serialization::access;

namespace Wintermute {
    namespace Data {
        namespace Linguistics {
            struct Model;
            struct SaveModel;
            struct LoadModel;
            struct Storage;

            struct LocalBackend;
            struct LocalSaveModel;
            struct LocalLoadModel;
            struct LocalStorage;

            // Experimental ideas: the former is not necessearily needed, the latter will be reviewed later.
            struct XMLBackend;
            struct XMLSaveModel;
            struct XMLLoadModel;
            struct XMLStorage;

            struct Serializer; /// What's this for?
            struct Lexidata;

            /**
             * @todo Implement iterator class with the next() method to allow convenient iterating over containers.
             * @typedef LLexicalVector
             */
            typedef vector<LocalStorage> LLexicalVector;

            /**
             * @typedef LLexicalIterator
             */
            typedef vector<LocalStorage>::const_iterator LLexicalIterator;

            /**
             * @todo Implement iterator class with the next() method to allow convenient iterating over containers.
             * @typedef XLexicalVector
             */
            typedef vector<XMLStorage> XLexicalVector;

            /**
             * @typedef LLexicalIterator
             */
            typedef vector<XMLStorage>::const_iterator XLexicalIterator;

            /**
             * @typedef LexicalMap
             */
            typedef multimap<const string, const string> Leximap;

            /**
             * @brief Wrapper class for lexical data.
             * This class represents the lexical information such as locale, lexical ID, lexical symbol and
             * the syntactic flags that represents the semantic tenses of a word.
             * @class LexicalInformation models.hpp "include/wntr/data/models.hpp"
             */
            class Lexidata {
                friend class boost::serialization::access;
                private:
                    string m_id;
                    string m_lcl;
                    string m_sym;
                    Leximap m_flgs;

                    template<class Archive>
                    void serialize ( Archive &ar, const unsigned int version ) {
                        ar & m_sym;
                        ar & m_flgs;
                    }

                public:
                    ~Lexidata() {}
                    /**
                     * @brief Obtains lexical symbol.
                     * @fn getSymbol
                     */
                    const string symbol() const;
                    /**
                     * @brief Obtains lexical ID.
                     * @fn getID
                     */
                    const string id() const;
                    /**
                     * @brief Obtains locale.
                     * @fn getLocale
                     */
                    const string locale() const;
                    /**
                     * @brief Obtains syntactic flags.
                     * @fn getFlags
                     */
                    const Leximap* flags() const;
                    /**
                     * @brief Default constructor.
                     * @fn LexicalInformation
                     */
                    Lexidata ( const string& id = string(),
                               const string& locale = Configuration::locale (),
                               const string& symbol = string(),
                               const Leximap& flags = Leximap() ) : m_lcl ( locale ),
                            m_sym ( symbol ),
                            m_flgs ( flags ),
                            m_id ( id ) {}

                    const bool operator == ( const Lexidata* lxin ) {
                        return ( lxin->m_id == m_id && lxin->m_lcl == m_lcl );
                    }
            };

            /**
             * @brief Foundational class of I/O handles for lexical information.
             * @class Model models.hpp "include/wntr/data/models.hpp"
             */
            class Model {
                    friend class boost::serialization::access;
                protected:
                    Lexidata m_lxdata;
                    /**
                     * @brief Null constructor.
                     * @fn Model
                     */
                    Model() {}
                    /**
                     * @brief Constructor, using lexical data.
                     * @fn Model
                     * @param info The data to fill itself with.
                     */
                    Model ( const Lexidata& p_info ) : m_lxdata ( p_info ) { }
                    /**
                     * @brief
                     * @fn Model
                     * @param
                     */
                    Model ( const Model& p_mdl ) : m_lxdata ( p_mdl.m_lxdata ) { }

                private:
                    template<class Archive>
                    void serialize ( Archive& p_ar, const unsigned int p_version ) {
                        p_ar & m_lxdata;
                    }
            };

            /**
             * @brief Model used to save lexical information.
             */
            class SaveModel : public Model {
                    friend class boost::serialization::access;
                protected:
                    /**
                     * @brief
                     *
                     * @fn SaveModel
                     */
                    SaveModel() : Model() {}
                    /**
                     * @brief
                     *
                     * @fn SaveModel
                     * @param p_lxin
                     */
                    SaveModel ( const Lexidata& p_lxin ) : Model ( p_lxin ) {}
                    /**
                     * @brief
                     *
                     * @fn SaveModel
                     * @param p_mod
                     */
                    SaveModel ( const Model& p_mod ) : Model ( p_mod ) {}
                    /**
                     * @brief
                     *
                     * @fn SaveModel
                     * @param p_smod
                     */
                    SaveModel ( const SaveModel& p_smod ) : Model ( p_smod ) {}

                public:
                    /**
                     * @brief Saves information.
                     * @todo Have this output to stdout.
                     */
                    virtual void save() = 0;

                private:
                    template<class Archive>
                    void serialize ( Archive& ar, const unsigned int version ) {
                        ar &  boost::serialization::base_object<Model,SaveModel> ( *this );
                    }
            };

            /**
             * @brief Model used to load lexical information.
             * @class LoadModel models.hpp "include/wntr/data/models.hpp"
             */
            class LoadModel : public Model {
                    friend class boost::serialization::access;
                protected:
                    /**
                     * @brief
                     *
                     * @fn LoadModel
                     */
                    LoadModel() : Model() {}
                    /**
                     * @brief
                     *
                     * @fn LoadModel
                     * @param map
                     */
                    LoadModel ( const Lexidata& p_in ) : Model ( p_in ) { }
                    /**
                     * @brief
                     *
                     * @fn LoadModel
                     * @param loadModel
                     */
                    LoadModel ( const LoadModel& loadModel ) : Model ( loadModel ) { }
                    /**
                     * @brief

                     * @fn LoadModel
                     * @param model
                     */
                    LoadModel ( const Model& model ) : Model ( model ) { }

                public:
                    /**
                     * @brief
                     * @todo Have this get input from stdin.
                     */
                    virtual void load() = 0;

                    /**
                     * @brief
                     * @fn getLexicalMap
                     */
                    Lexidata* const lexicalData() {
                        return &this->Model::m_lxdata;
                    }

                private:
                    template<class Archive>
                    void serialize ( Archive& ar, const unsigned int version ) {
                        ar &  boost::serialization::base_object<Model,LoadModel> ( *this );
                    }
            };

            /**
             * @brief Model used to load and save lexical information.
             * @todo When defining copy constructor; you'll run into ambigiousity for _map for SaveModel & LoadModel...
             * @class StorageModel models.hpp "include/wntr/data/models.hpp"
             */
            class Storage : virtual public SaveModel, virtual public LoadModel {
                    friend class boost::serialization::access;
                public:
                    /**
                     * @brief
                     *
                     * @fn showLexicalInformation
                     * @return LexicalInformation*
                     */

                protected:
                    /**
                     * @brief
                     *
                     * @fn StorageModel
                     */
                    Storage() : SaveModel(), LoadModel() {}

                    /**
                     * @brief
                     * @fn StorageModel
                     * @param map
                     */
                    Storage ( const Lexidata& map ) : SaveModel ( map ), LoadModel ( map ) {}

                    /**
                     * @brief
                     * @fn StorageModel
                     * @param model
                     */
                    Storage ( const Model& model ) : SaveModel ( model ), LoadModel ( model ) {}

                private:
                    template<class Archive>
                    void serialize ( Archive& ar, const unsigned int version ) {
                        ar &  boost::serialization::base_object<SaveModel> ( *this );
                    }

                    /**
                      * @brief Used to agggregate the return value of the StorageObtainSignal and StorageExistsSignal. It dumps any NULL/false values and brings back the first non-NULL value, or returns NULL if all are NULL or if there's nothing there.
                      * @class solo_value linguistics.hpp "include/wntr/data/linguistics.hpp"
                      */
                    template<typename T>
                    struct solo_value {
                        typedef T result_type;

                        template<typename InputIterator>
                        result_type operator() ( InputIterator p_fst, InputIterator p_lst ) {
                            if ( p_fst == p_lst ) return NULL;
                            InputIterator cur = p_fst;
                            while ( cur != p_lst ) {
                                if ( ( *cur ) ) {
                                    return *cur;
                                    break;
                                } else
                                    ++cur;
                            }

                            return NULL;
                        }

                    };

                    /**
                     * @brief
                     * @typedef LexicalSignalObtainer
                     */
                    typedef boost::signals2::signal<Storage* ( const Lexidata& ), solo_value<Storage*> > StorageObtainSignal;

                    /**
                     * @brief
                     * @typedef StorageExistsSignal
                     */
                    typedef boost::signals2::signal<bool ( const Lexidata& ), solo_value<bool> > StorageExistsSignal;

                    static StorageObtainSignal s_sigObtain;
                    static StorageExistsSignal s_sigExists;

                public:

                    /**
                    * @brief Generates a Storage object.
                    * @fn obtain
                    * @param id The ID of the Storage object.
                    * @param locale The locale of the Storage object.
                    * @return Return a pointer to a formed Storage object or NULL if it cannot be found.
                    */
                    static Storage* obtain ( const Lexidata& );

                    /**
                     * @brief Determines existence of a Storage object.
                     * @fn exists
                     * @param id The ID of the Storage object.
                     * @param locale The locale of the Storage object.
                     */
                    static const bool exists ( const Lexidata& );

                    /**
                     * @brief Adds source of data.
                     * Attaches a obtaining and existing qualifier to the slots.
                     * @fn addSource
                     * @param slotObtain The slot to obtain a new Storage object.
                     * @param slotExists The slot to determine the existence of a possible Storage object.
                     */
                    static void addDataSource ( const StorageObtainSignal::slot_type &, const StorageExistsSignal::slot_type & );

            };

            /**
             * @brief Abstract class defining interface for every object representing local data.
             * The purpose of this class is to standarize interface for all classes
             * intended for using as representations of data placed locally, e.g node files.
             * @todo|closed Should this inherit from Model? (Hmm, good question; if it does, the methods in LocalSaveModel and LocalReadModel could be moved into this then..) [No, it shouldn't.]
             * @todo think about replacing this by more network-transparent interfaces in serializer/lexical classes/models. (If it's local; then why have a transparency for network resources? Shouldn't they have network capabilities in the background?)
             */
            class LocalBackend {
                    friend class LocalStorage;
                protected:
                    /**
                     * @brief Null constructor.
                     * Creates an empty local model.
                     */
                    LocalBackend() {}

                    /**
                     * @brief Default constructor
                     * Constructor with path initialization.
                     * @param string Path of the represented local data.
                     */
                    explicit LocalBackend ( string const& path ) : m_url ( path ) {}

                    /**
                     * @brief Obtains url of data.
                     * Return path of the represented local data.
                     * @return Path of the represented local data.
                     */
                    string url() {
                        return m_url;
                    }

                    string m_url;
            };

            /**
             * @brief Model used to save lexical information locally
             * @see SaveModel
             * @see LocalModel
             */
            class LocalSaveModel : public SaveModel, public LocalBackend {
                    friend class LocalStorage;
                    friend class boost::serialization::access;
                protected:
                    /**
                     * @brief
                     *
                     * @fn LocalSaveModel
                     */
                    LocalSaveModel() : SaveModel(), LocalBackend() {}

                    /**
                     * @brief
                     *
                     * @fn LocalSaveModel
                     * @param path
                     */
                    LocalSaveModel ( const string& m_pth ) : SaveModel(), LocalBackend ( m_pth ) {}

                    /**
                     * @brief
                     *
                     * @fn LocalSaveModel
                     * @param path
                     * @param info
                     */
                    LocalSaveModel ( const string& m_pth, const Lexidata& p_in ) : SaveModel ( p_in ), LocalBackend ( m_pth ) {}

                    /**
                     * @brief
                     *
                     * @fn LocalSaveModel
                     * @param info
                     */
                    LocalSaveModel ( const Lexidata& );

                    /**
                     * @brief
                     *
                     * @fn LocalSaveModel
                     * @param model
                     */
                    LocalSaveModel ( const Model& );

                public:
                    /**
                     * @brief
                     */
                    void save();

                private:
                    template<class Archive>
                    void serialize ( Archive& ar, const unsigned int version ) {
                        ar &  boost::serialization::base_object<SaveModel> ( *this );
                    }
            };

            /**
             * @brief Model used to load lexical information from local repositiories.
             * This model permits the loading of lexical data from the more readily avialable data location;
             * this location is configured upon installation of WntrData.
             * @class LocalLoadModel models.hpp "include/wntr/data/models.hpp"
             */
            class LocalLoadModel : public LoadModel, public LocalBackend {
                    friend class boost::serialization::access;
                protected:
                    /**
                     * @brief
                     *
                     * @fn LocalLoadModel
                     */
                    LocalLoadModel() : LoadModel(), LocalBackend() {}
                    /**
                     * @brief
                     *
                     * @fn LocalLoadModel
                     * @param path
                     */
                    LocalLoadModel ( const string& m_pth ) : LoadModel(), LocalBackend ( m_pth ) {
                        this->LocalLoadModel::load();
                    }
                    /**
                     * @brief
                     *
                     * @fn LocalLoadModel
                     * @param path
                     * @param info
                     */
                    LocalLoadModel ( const string& m_pth, const Lexidata& p_in ) : LoadModel ( p_in ), LocalBackend ( m_pth ) {
                        this->LocalLoadModel::load();
                    }
                    /**
                     * @brief
                     *
                     * @fn LocalLoadModel
                     * @param info
                     */
                    LocalLoadModel ( const Lexidata& );

                    /**
                     * @brief
                     *
                     * @fn LocalLoadModel
                     * @param model
                     */
                    LocalLoadModel ( const Model& );

                public:
                    /**
                     * @brief
                     * @todo Override the base method...
                     */
                    void load();

                private:
                    template<class Archive>
                    void serialize ( Archive& ar, const unsigned int version ) {
                        ar &  boost::serialization::base_object<LoadModel> ( *this );
                    }
            };

            /**
             * @brief Model used to load and save lexical information from local repositiories.
             * @class LocalStorageModel models.hpp "include/wntr/data/models.hpp"
             * @todo Implement type-casting to/from LocalLoadModel, LocalSaveModel, and Storage.
             */
            class LocalStorage : public virtual Storage,
                        public virtual LocalLoadModel,
                        public virtual LocalSaveModel {
                    friend class boost::serialization::access;
                protected:
                    /**
                     * @brief
                     *
                     * @fn LocalStorageModel
                     */
                    LocalStorage() : Storage() {}

                    /**
                     * @brief
                     *
                     * @fn LocalStorageModel
                     * @param map
                     */
                    LocalStorage ( const Lexidata& p_in ) : Storage ( p_in ),
                            LocalLoadModel ( ( LocalStorage::formPath ( p_in ) ), p_in ),
                            LocalSaveModel ( ( LocalStorage::formPath ( p_in ) ) ,p_in ) {  }

                    /**
                     * @brief
                     *
                     * @fn LocalStorageModel
                     * @param model
                     */
                    LocalStorage ( const Model& p_mod ) : Storage ( p_mod ),
                            LocalLoadModel ( p_mod ),
                            LocalSaveModel ( p_mod ) { }

                    /**
                     * @brief
                     *
                     * @fn LocalStorageModel
                     * @param path
                     */
                    LocalStorage ( const string& m_pth ) : Storage(),
                            LocalLoadModel ( m_pth ),
                            LocalSaveModel ( m_pth ) { }

                public:
                    void save() {
                        this->LocalSaveModel::save ();
                    }

                    void load() {
                        this->LocalLoadModel::load ();
                    }

                    /**
                     * @brief Generates a local storage.
                     * Gets you a local storage system that can be used to load and save information for a specific bit of lexical information.
                     * @fn create
                     * @param
                     */
                    static Storage* create ( const Lexidata& );

                    /**
                     * @brief Determines local lexical existence.
                     * Finds out if specific lexical information is avialable locally.
                     * @fn exists
                     * @param
                     */
                    static const bool exists ( const Lexidata & );

                    /**
                     * @brief
                     *
                     * @fn serializeToDisk
                     * @param
                     */
                    static void serializeToDisk ( const Lexidata& );

                    /**
                     * @brief
                     *
                     * @fn spawn
                     */
                    static void spawn();

                    /**
                     * @brief
                     *
                     * @fn formPath
                     * @param
                     */
                    static const string formPath ( const Lexidata& );

                private:
                    template<class Archive>
                    void serialize ( Archive& ar, const unsigned int version ) {
                        ar &  boost::serialization::base_object<Model,LocalStorage> ( *this );
                    }
            };

            /**
             * @brief
             * @class XMLModel models.hpp "include/wntr/data/models.hpp"
             */
            class XMLBackend {
                private:
                    QDomElement *m_nod;

                public:
                    /**
                     * @brief
                     *
                     * @fn XMLModel
                     */
                    XMLBackend();
                    /**
                     * @brief
                     *
                     * @fn XMLModel
                     * @param node
                     */
                    XMLBackend ( QDomElement* p_nod ) : m_nod ( p_nod ) {}
            };

            /**
             * @brief
             * @class XMLSaveModel models.hpp "include/wntr/data/models.hpp"
             */
            class XMLSaveModel : public SaveModel, public XMLBackend {
                    friend class boost::serialization::access;
                protected:
                    /**
                     * @brief
                     *
                     * @fn XMLSaveModel
                     */
                    XMLSaveModel() : SaveModel(), XMLBackend() {}
                    /**
                     * @brief
                     *
                     * @fn XMLSaveModel
                     * @param map
                     */
                    XMLSaveModel ( const Lexidata& p_in ) : SaveModel ( p_in ), XMLBackend() {}
                    /**
                     * @brief
                     *
                     * @fn XMLSaveModel
                     * @param node
                     */
                    XMLSaveModel ( QDomElement* p_nod ) : SaveModel(), XMLBackend ( p_nod ) {}
                    /**
                     * @brief
                     *
                     * @fn XMLSaveModel
                     * @param node
                     * @param map
                     */
                    XMLSaveModel ( QDomElement *p_nod, const Lexidata& p_in ) : SaveModel ( p_in ), XMLBackend ( p_nod ) {}

                public:
                    /**
                     * @brief
                     *
                     * @fn save
                     * @todo Override this method.
                     */
                    void save();

                private:
                    template<class Archive>
                    void serialize ( Archive& ar, const unsigned int version ) {
                        ar &  boost::serialization::base_object<SaveModel,XMLSaveModel> ( *this );
                    }
            };

            /**
             * @brief
             * @class XMLLoadModel models.hpp "include/wntr/data/models.hpp"
             */
            class XMLLoadModel : public LoadModel, public XMLBackend {
                    friend class boost::serialization::access;
                protected:
                    /**
                     * @brief
                     *
                     * @fn XMLLoadModel
                     */
                    XMLLoadModel() : LoadModel(), XMLBackend() {}

                    /**
                     * @brief
                     *
                     * @fn XMLLoadModel
                     * @param map
                     */
                    XMLLoadModel ( const Lexidata& p_in ) : LoadModel ( p_in ), XMLBackend() {}

                    /**
                     * @brief
                     *
                     * @fn XMLLoadModel
                     * @param node
                     */
                    XMLLoadModel ( QDomElement* p_nod ) : LoadModel(), XMLBackend ( p_nod ) {}

                    /**
                     * @brief
                     *
                     * @fn XMLLoadModel
                     * @param node
                     * @param map
                     */
                    XMLLoadModel ( QDomElement *p_nod, const Lexidata& p_in ) : LoadModel ( p_in ), XMLBackend ( p_nod ) {}

                public:
                    /**
                     * @brief
                     * @fn load
                     * @todo Override this method.
                     */
                    void load();

                private:
                    template<class Archive>
                    void serialize ( Archive& ar, const unsigned int version ) {
                        ar &  boost::serialization::base_object<LoadModel,XMLLoadModel> ( *this );
                    }
            };

            /**
             * @brief
             * @class XMLStorageModel models.hpp "include/wntr/data/models.hpp"
             */
            class XMLStorage : public virtual XMLLoadModel,
                        public virtual XMLSaveModel,
                        public Storage {
                    friend class boost::serialization::access;
                protected:
                    /**
                     * @brief
                     *
                     * @fn XMLStorage
                     */
                    XMLStorage() : Storage() {}
                    /**
                     * @brief
                     *
                     * @fn XMLStorage
                     * @param info
                     */
                    XMLStorage ( const Lexidata& info ) : Storage ( info ) {}
                    /**
                     * @brief
                     *
                     * @fn XMLStorage
                     * @param model
                     */
                    XMLStorage ( const Model& model ) : Storage ( model ) {}
                    /**
                     * @brief
                     *
                     * @fn XMLStorage
                     * @param node
                     */
                    XMLStorage ( QDomElement* node ) : XMLLoadModel ( node ), XMLSaveModel ( node ), Storage() {}
                    static vector<QDomDocument> s_allDocs;

                public:
                    /**
                     * @brief
                     *
                     * @fn create
                     * @param id
                     * @param locale
                     * @return Lexical *
                     */
                    static Storage* create ( const Lexidata& );

                    /**
                     * @brief
                     *
                     * @fn exists
                     * @param
                     */
                    static const bool exists ( const Lexidata & );

                    /**
                     * @brief Adds document to system.
                     * Loads a XML document from disc by the specified URI and if found, adds it to the system.
                     * @fn addDocument
                     * @param url
                     */
                    static void addDocument ( const string& );

                    /**
                     * @brief
                     *
                     * @fn spawn
                     */
                    static void spawn();

                private:
                    template<class Archive>
                    void serialize ( Archive& ar, const unsigned int version ) {
                        ar &  boost::serialization::base_object<XMLSaveModel> ( *this );
                    }
            };

            /// @todo Define these classes for the Binding, Rule, and RuleSet classes. This'll add some dynamic means of implementing a back-end.
            namespace Rules {
                struct Model;
                struct SaveModel;
                struct LoadModel;
                struct Storage;

                struct LocalBackend;
                struct LocalLoadModel;
                struct LocalSaveModel;
                struct LocalStorage;

                struct XMLBackend;
                struct XMLLoadModel;
                struct XMLSaveModel;
                struct XMLStorage;
            }
        }
    }
}

#endif /* MODELS_HPP */
// kate: indent-mode cstyle; space-indent on; indent-width 4;

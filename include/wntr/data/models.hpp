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
#include "md5.hpp"

#include <map>
#include <boost/signals2/signal.hpp>
#include <boost/signals2/slot.hpp>
#include <QtDebug>
#include <QtXml/QDomElement>

using namespace std;
using std::multimap;

namespace Wintermute {
    namespace Data {
        namespace Linguistics {
            namespace Lexical {
                struct Model;
                struct SaveModel;
                struct LoadModel;
                struct Storage;

                struct LocalBackend;
                struct LocalSaveModel;
                struct LocalLoadModel;
                struct LocalStorage;

                struct XMLBackend;
                struct XMLSaveModel;
                struct XMLLoadModel;
                struct XMLStorage;

                struct Lexidata;

                /**
                 * @typedef Leximap
                 */
                typedef multimap<const string, const string> Leximap;

                /**
                 * @brief Wrapper class for lexical data.
                 * This class represents the lexical information such as locale, lexical ID, lexical symbol and
                 * the syntactic flags that represents the semantic tenses of a word.
                 * @class Lexidata models.hpp "include/wntr/data/models.hpp"
                 */
                class Lexidata : public QObject {
                    Q_OBJECT
                    Q_PROPERTY(const string symbol READ symbol)
                    Q_PROPERTY(const string id READ id)
                    Q_PROPERTY(const string locale READ locale)
                    Q_PROPERTY(const Leximap flags READ flags)

                    private:
                        string m_id;
                        string m_lcl;
                        string m_sym;
                        Leximap m_map;

                    public:
                        /**
                         * @brief Obtains lexical symbol.
                         * @fn getSymbol
                         */
                        const string* symbol() const { return &m_sym; }

                        /**
                         * @brief Obtains lexical ID.
                         * @fn getID
                         */
                        const string* id() const { return &m_id; }

                        /**
                         * @brief Obtains locale.
                         * @fn getLocale
                         */
                        const string* locale() const { return &m_lcl; }

                        /**
                         * @brief Obtains syntactic flags.
                         * @fn getFlags
                         */
                        const Leximap flags() const { return m_map; }

                        /**
                         * @brief
                         * @fn operator =
                         * @param p_lxdt
                         */
                        void operator= (const Lexidata& p_lxdt) {
                            this->m_id = p_lxdt.m_id;
                            this->m_lcl = p_lxdt.m_lcl;
                            this->m_map = p_lxdt.m_map;
                            this->m_sym = p_lxdt.m_sym;
                        }

                        /**
                         * @brief Default constructor.
                         * @fn LexicalInformation
                         */
                        explicit Lexidata(const string *p_id, const string *p_lcl, const string *p_sym = new string, Leximap p_map = Leximap() ) : m_lcl(*p_lcl),
                            m_id(*p_id), m_sym(*p_sym), m_map(p_map.begin (),p_map.end ()) {
                        }

                        Lexidata(const string &p_id, const string &p_lcl, const string &p_sym = string(""), Leximap p_map = Leximap() ) : m_lcl(p_lcl),
                            m_id(p_id), m_sym(p_sym), m_map(p_map.begin (),p_map.end ()) {
                        }

                        /**
                         * @brief
                         * @fn Lexidata
                         * @param p_lxdt
                         */
                        Lexidata(const Lexidata& p_lxdt) : m_id(p_lxdt.m_id), m_lcl(p_lxdt.m_lcl), m_sym(p_lxdt.m_sym), m_map(p_lxdt.m_map) { }

                        /**
                         * @brief
                         * @fn Lexidata
                         */
                        Lexidata() { }

                        /**
                         * @brief
                         * @fn ~Lexidata
                         */
                        ~Lexidata() { }
                };


                /**
                 * @brief Foundational class of I/O handles for lexical information.
                 * @class Model models.hpp "include/wntr/data/models.hpp"
                 */
                class Model : public QObject {
                    Q_OBJECT

                    protected:
                        Lexidata* m_lxdt;

                    public:
                        /**
                         * @brief
                         * @fn ~Model
                         */
                        virtual ~Model() { }
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
                        Model ( Lexidata* p_info ) : m_lxdt ( p_info ) { }
                        /**
                         * @brief
                         * @fn Model
                         * @param
                         */
                        Model ( const Model& p_mdl ) : m_lxdt ( p_mdl.m_lxdt ) { }

                        /**
                         * @brief
                         * @fn getLexicalMap
                         */
                        Lexidata* lexicalData() {
                            return m_lxdt;
                        }
                };

                /**
                 * @brief Model used to save lexical information.
                 */
                class SaveModel : public Model {
                    Q_OBJECT
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
                        SaveModel ( Lexidata* p_lxin ) : Model ( p_lxin ) {}

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
                };

                /**
                 * @brief Model used to load lexical information.
                 * @class LoadModel models.hpp "include/wntr/data/models.hpp"
                 */
                class LoadModel : public Model{
                    Q_OBJECT
                    Q_PROPERTY(Lexidata* lexicalData READ lexicalData)

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
                        LoadModel ( Lexidata* p_in ) : Model ( p_in ) { }

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
                        virtual Lexidata* load() = 0;

                };

                /**
                 * @brief Model used to load and save lexical information.
                 * @todo When defining copy constructor; you'll run into ambigiousity for _map for SaveModel & LoadModel...
                 * @class StorageModel models.hpp "include/wntr/data/models.hpp"
                 */
                class Storage : virtual public SaveModel, virtual public LoadModel {
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
                        Storage ( Lexidata* map ) : SaveModel ( map ), LoadModel ( map ) {}

                        /**
                         * @brief
                         * @fn StorageModel
                         * @param model
                         */
                        Storage ( const Model& model ) : SaveModel ( model ), LoadModel ( model ) {}

                    private:
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
                        typedef boost::signals2::signal<Storage* ( Lexidata* ), solo_value<Storage*> > StorageObtainSignal;

                        /**
                         * @brief
                         * @typedef StorageExistsSignal
                         */
                        typedef boost::signals2::signal<bool ( Lexidata* ), solo_value<bool> > StorageExistsSignal;

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
                        static Storage* obtain ( const Lexidata* );

                        /**
                         * @brief Determines existence of a Storage object.
                         * @fn exists
                         * @param id The ID of the Storage object.
                         * @param locale The locale of the Storage object.
                         */
                        static const bool exists ( const Lexidata* );

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
                 * @brief Framing class defining interface for every object representing local data.
                 * The purpose of this class is to standarize interface for all classes
                 * intended for using as representations of data placed locally, e.g node files.
                 */
                class LocalBackend {
                    friend class LocalStorage;
                    public:
                        ~LocalBackend() { }
                    protected:
                        /**
                         * @brief Null constructor.
                         * Creates an empty local model.
                         */
                        LocalBackend() : m_file(NULL) {}

                        /**
                         * @brief Default constructor
                         * Constructor with path initialization.
                         * @param path Path of the represented local data.
                         */
                        explicit LocalBackend ( string const& p_path ) : m_file( new QFile(QString::fromStdString (p_path)) ) { }


                        /**
                         * @brief Build backend from a QFile.
                         * @fn LocalBackend
                         * @param file The QFile to obtain data from.
                         */
                        LocalBackend( QFile& p_file ) : m_file( &p_file ) { }

                        /**
                         * @brief Obtains url of data.
                         * Return path of the represented local data.
                         * @return Path of the represented local data.
                         */
                        string url() {
                            if (m_file) return m_file->fileName ().toStdString ();
                            else return "";
                        }

                        QFile* m_file;
                };

                /**
                 * @brief Model used to save lexical information locally.
                 * @see SaveModel
                 * @see LocalModel
                 */
                class LocalSaveModel : public SaveModel, public LocalBackend {
                    friend class LocalStorage;
                    Q_OBJECT
                    Q_PROPERTY(string url READ url);
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
                        LocalSaveModel ( const string& m_pth, Lexidata* p_in ) : SaveModel ( p_in ), LocalBackend ( m_pth ) {}

                        /**
                         * @brief
                         *
                         * @fn LocalSaveModel
                         * @param info
                         */
                        LocalSaveModel ( Lexidata * );

                        /**
                         * @brief
                         *
                         * @fn LocalSaveModel
                         * @param model
                         */
                        LocalSaveModel ( const Model & );

                    public:
                        /**
                         * @brief
                         */
                        Q_INVOKABLE void save();
                };

                /**
                 * @brief Model used to load lexical information from local repositiories.
                 * This model permits the loading of lexical data from the more readily avialable data location;
                 * this location is configured upon installation of WntrData.
                 * @class LocalLoadModel models.hpp "include/wntr/data/models.hpp"
                 */
                class LocalLoadModel : public LoadModel, public LocalBackend {
                    friend class LocalStorage;
                    Q_OBJECT
                    Q_PROPERTY(string url READ url);
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
                        LocalLoadModel ( const string& m_pth ) : LoadModel(), LocalBackend ( m_pth ) { this->LocalLoadModel::load(); }
                        /**
                         * @brief
                         *
                         * @fn LocalLoadModel
                         * @param path
                         * @param info
                         */
                        LocalLoadModel ( const string& m_pth, Lexidata* p_in ) : LoadModel ( p_in ), LocalBackend ( m_pth ) { this->LocalLoadModel::load(); }

                        /**
                         * @brief
                         * @fn LocalLoadModel
                         * @param p_info
                         */
                        LocalLoadModel ( Lexidata * );
                        /**
                         * @brief
                         *
                         * @fn LocalLoadModel
                         * @param model
                         */
                        LocalLoadModel ( const Model & );

                    public:
                        /**
                         * @brief
                         */
                        Q_INVOKABLE Lexidata* load();
                };

                /**
                 * @brief Model used to load and save lexical information from local repositiories.
                 * @class LocalStorageModel models.hpp "include/wntr/data/models.hpp"
                 */
                class LocalStorage : public virtual Storage,
                            public virtual LocalLoadModel,
                            public virtual LocalSaveModel {
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
                        LocalStorage ( Lexidata* p_in ) : Storage ( p_in ),
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
                        Q_INVOKABLE inline void save() { this->LocalSaveModel::save (); }

                        Q_INVOKABLE inline Lexidata* load() { this->LocalLoadModel::load (); }

                        /**
                         * @brief Generates a local storage.
                         * Gets you a local storage system that can be used to load and save information for a specific bit of lexical information.
                         * @fn create
                         * @param
                         */
                        static Storage* create ( const Lexidata* );

                        /**
                         * @brief Determines local lexical existence.
                         * Finds out if specific lexical information is avialable locally.
                         * @fn exists
                         * @param
                         */
                        static const bool exists ( const Lexidata* );

                        /**
                         * @brief
                         *
                         * @fn serializeToDisk
                         * @param
                         */
                        static void serializeToDisk ( const Lexidata* );

                        /**
                         * @brief
                         *
                         * @fn spawn
                         */
                        static void spawn( const string & = Configuration::locale() );

                        /**
                         * @brief
                         *
                         * @fn formPath
                         * @param
                         */
                        static const string formPath ( const Lexidata* );
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
                        ~XMLBackend() { }
                };

                /**
                 * @brief
                 * @class XMLSaveModel models.hpp "include/wntr/data/models.hpp"
                 */
                class XMLSaveModel : public SaveModel, public XMLBackend {
                    Q_OBJECT
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
                        XMLSaveModel ( Lexidata* p_in ) : SaveModel ( p_in ), XMLBackend() {}
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
                        XMLSaveModel ( QDomElement *p_nod, Lexidata* p_in ) : SaveModel ( p_in ), XMLBackend ( p_nod ) {}

                    public:
                        /**
                         * @brief
                         *
                         * @fn save
                         * @todo Override this method.
                         */
                        void save();
                };

                /**
                 * @brief
                 * @class XMLLoadModel models.hpp "include/wntr/data/models.hpp"
                 */
                class XMLLoadModel : public LoadModel, public XMLBackend {
                    Q_OBJECT
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
                        XMLLoadModel ( Lexidata* p_in ) : LoadModel ( p_in ), XMLBackend() {}

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
                        XMLLoadModel ( QDomElement *p_nod, Lexidata* p_in ) : LoadModel ( p_in ), XMLBackend ( p_nod ) {}

                    public:
                        /**
                         * @brief
                         * @fn load
                         * @todo Override this method.
                         */
                        Lexidata* load();
                };

                /**
                 * @brief
                 * @class XMLStorageModel models.hpp "include/wntr/data/models.hpp"
                 */
                class XMLStorage : public virtual XMLLoadModel, public virtual XMLSaveModel, public Storage {
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
                        XMLStorage ( Lexidata* info ) : Storage ( info ) {}
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
                        static Storage* create ( const Lexidata* );

                        /**
                         * @brief
                         *
                         * @fn exists
                         * @param
                         */
                        static const bool exists ( const Lexidata* );

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
                        static void spawn( const string & = Configuration::locale() );
                };
            }

            /// @todo Define these classes for the Binding, Rule, and RuleSet classes. This'll add some dynamic means of implementing a back-end.
            namespace Rules {
                struct Syntax;
                struct Model;
                struct LoadModel;
                struct SaveModel;
                struct Storage;

                /// @todo The means of implementing this should be done with a set of locale documents (~/en.lcl); and within it, contains a list of the name of the binding files.
                struct LocalBackend;
                struct LocalLoadModel;
                struct LocalSaveModel;
                struct LocalStorage;

                /// @todo This should reimplement the traditional means of discovering Bindings.
                struct XMLBackend;
                struct XMLLoadModel;
                struct XMLSaveModel;
                struct XMLStorage;

                typedef QList<Syntax*> SyntaxList;

                class Syntax : public QObject {
                    Q_OBJECT
                    Q_PROPERTY(string locale READ locale WRITE setLocale)
                    Q_PROPERTY(string ruleText READ ruleText WRITE setRuleText)
                    Q_PROPERTY(string linkText READ linkText WRITE setLinkText)

                    private:
                        string m_lcl;
                        string m_rlTxt;
                        string m_lnkTxt;

                    public:
                        Syntax() : QObject(NULL), m_lcl(), m_rlTxt(), m_lnkTxt() { }
                        Syntax(const Syntax& p_syntx) : QObject(NULL), m_lcl(p_syntx.m_lcl), m_rlTxt(p_syntx.m_rlTxt), m_lnkTxt(p_syntx.m_lnkTxt) { }
                        Syntax& operator= (const Syntax&);
                        ~Syntax() { }
                        void setLocale(string const);
                        void setRuleText(string const);
                        void setLinkText(string const);
                        const string locale() const;
                        const string ruleText() const;
                        const string linkText() const;
                };

                /**
                 * @brief
                 * @class Model models.hpp "include/wntr/data/models.hpp"
                 * @todo This should provide the practical methods of obtaining Bindings.
                 */
                class Model : public QObject {
                    Q_OBJECT
                    //Q_PROPERTY(Syntax syntax READ syntax WRITE setSyntax)

                    private:
                        Syntax m_syntx;

                    protected:
                        Model(const Syntax& p_syntx) : QObject(NULL), m_syntx(p_syntx) { }

                    public:
                        Model(const Model& p_1) : QObject(NULL), m_syntx(p_1.m_syntx) { }
                        Model() : QObject(NULL) { }
                        virtual ~Model() { }
                        //void setSyntax(Syntax p_syntx) { m_syntx = p_syntx; }
                        Syntax syntax() const { return m_syntx; }
                };

                /**
                 * @brief
                 * @class SaveModel models.hpp "include/wntr/data/models.hpp"
                 */
                class SaveModel : public Model {
                    Q_OBJECT

                    public slots:
                        /**
                         * @brief
                         * @fn saved
                         */
                        void saved();

                    public:
                        /**
                         * @brief
                         * @fn save
                         */
                        virtual void save() = 0;

                    protected:
                        /**
                         * @brief
                         * @fn SaveModel
                         */
                        SaveModel() : Model() { }
                        /**
                         * @brief
                         * @fn SaveModel
                         * @param p_model
                         */
                        SaveModel(const Model& p_1) : Model(p_1) { }
                        /**
                         * @brief
                         * @fn SaveModel
                         * @param p_saveModel
                         */
                        SaveModel(const SaveModel& p_1) : Model(p_1) { }
                };

                /**
                 * @brief
                 * @class LoadModel models.hpp "include/wntr/data/models.hpp"
                 */
                class LoadModel : public Model {
                    Q_OBJECT

                    public slots:
                        /**
                         * @brief
                         * @fn loaded
                         */
                        void loaded();

                    public:
                        /**
                         * @brief
                         * @fn load
                         */
                        virtual void load() = 0;

                    protected:
                        /**
                         * @brief
                         * @fn LoadModel
                         */
                        LoadModel() : Model() { }
                        /**
                         * @brief
                         * @fn LoadModel
                         * @param p_model
                         */
                        LoadModel(const Model& p_model) : Model(p_model) { }
                        /**
                         * @brief
                         * @fn LoadModel
                         * @param p_model
                         */
                        LoadModel(const LoadModel& p_model) : Model(p_model) { }
                };

                class Storage : virtual public SaveModel, virtual public LoadModel {
                    public:
                    protected:
                    private:
                };
            }
        }
    }
}

#endif /* MODELS_HPP */
// kate: indent-mode cstyle; space-indent on; indent-width 4;

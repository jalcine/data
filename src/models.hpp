/**
 * @file models.hpp
 * @author Wintermute Developers <wintermute-devel@lists.launchpad.net>
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
 */

#ifndef MODELS_HPP
#define MODELS_HPP

#define DOMSTORAGE_MAXSTR 1.0

#include <map>
#include <QHash>
#include <QDebug>
#include <QMultiHash>
#include <QStringList>
#include <QMetaType>
#include <QtDBus/QDBusMetaType>
#include <QtXml/QDomDocument>
#include "linguistics.hpp"

using namespace std;

using std::map;

namespace Wintermute {
    namespace Data {
        struct NodeAdaptor;
        struct RuleAdaptor;
        namespace Linguistics {
            namespace Lexical {
                struct Data;

                struct Model;
                struct SaveModel;
                struct LoadModel;
                struct Storage;
                struct Backend;
                struct Cache;


                struct DomLoadModel;
                struct DomSaveModel;
                struct DomStorage;
                struct DomBackend;

                /**
                 * @brief A collection of flags.
                 *
                 * This typedef encapsulates a QMultiMap that holds a one-to-many
                 * mapping of values. This flag collection is used by the parser
                 * to hold the vital ontological value up to the lexicosyntactical
                 * binding ID.
                 *
                 * @typedef FlagMapping
                 */
                typedef QMultiMap<QString, QString> FlagMapping;

                /**
                 * @brief The lexical POD format of linguistics parsing.
                 *
                 * The Data object represents the internal workings of the lexical
                 * bindings. Data objects hold information about the locale of the
                 * potential parent node, the ID of the node, the symbol and the
                 * flags of the node. All of this information can be used to assist
                 * interoperability in moving it from a literal symbol ("you") to
                 * its internal, workable type ("Aeon1#~" [en]).
                 *
                 * @note This class can be considered this a POD (<b>p</b>lain <b>o</b>l' <b>data format) of Wintermute.
                 * @class Data models.hpp "include/wntr/data/models.hpp"
                 * @see FlagMapping
                 */
                class Data : public QObject {
                    friend QDBusArgument& operator<< (QDBusArgument&, const Data&);
                    friend const QDBusArgument& operator>> (const QDBusArgument&, Data&);
                    friend QDebug operator<<(QDebug dbg, const Data*);

                    Q_OBJECT
                    Q_PROPERTY(QString Locale READ locale)
                    Q_PROPERTY(QString Symbol READ symbol WRITE setSymbol)
                    Q_PROPERTY(QString ID READ id)
                    Q_PROPERTY(FlagMapping Flags READ flags WRITE setFlags)

                    private:
                        QString m_id;
                        QString m_lcl;
                        QString m_sym;
                        FlagMapping m_flg;

                    public:
                        /**
                         * @brief Default constructor.
                         * @fn Data
                         * @param string The ID of the Data.
                         * @param string The locale of the Data.
                         * @param string The symbol of the Data.
                         * @param DataFlagMap The flags of the Data.
                         */
                        explicit Data(const QString , const QString , const QString = "" , const FlagMapping = FlagMapping());

                        /**
                         * @brief Null constructor.
                         * @fn Data
                         */
                        Data();

                        /**
                         * @brief Copy constructor.
                         * @fn Data
                         * @param Data The Data to be copied.
                         */
                        Data(const Data&);

                        /**
                         * @brief Equality operator.
                         * @fn operator==
                         * @param The Data to be equated against.
                         */
                        bool operator==(const Data&) const;

                        /**
                         * @brief Assignment operator.
                         * @fn operator=
                         * @param The Data to be copied.
                         */
                        void operator=(const Data&);

                        /**
                         * @brief Deconstructor.
                         * @fn ~Data
                         */
                        virtual ~Data();

                        /**
                         * @brief Returns the ID of the node.
                         * @fn id
                         */
                        const QString id() const;

                        /**
                         * @brief Returns the locale of the Data.
                         * @fn locale
                         */
                        const QString locale() const;

                        /**
                         * @brief Returns the symbol of the Data.
                         * @fn symbol
                         */
                        const QString symbol() const;

                        /**
                         * @brief Returns the flags of the Data.
                         * @fn flags
                         */
                        const FlagMapping flags() const;

                        /**
                         * @brief Changes the symbol of the Data to p_dt.
                         * @fn setSymbol
                         * @param p_dt The symbol for the Data to hold now.
                         * @note When the symbol is changed, the ID is changed as well. This
                         *       is why there's no setID() method. The ID value is actually a
                         *       MD5 hash of a lower-case representation of the symbol string.
                         * @see idFromString(const QString)
                         */
                        void setSymbol( const QString& );

                        /**
                         * @brief Sets the flags of the Data.
                         * @fn setFlags
                         * @param p_flg The flags for the Data to hold now.
                         */
                        void setFlags( const FlagMapping& );

                        /**
                         * @brief Determines if this Data is equivalent to a null Data object.
                         * @fn isEmpty
                         */
                        const bool isEmpty() const;

                        /**
                         * @brief Obtains the ID from a said QString.
                         * @fn idFromString
                         * @param QString The text to be transformed into its proper ID.
                         */
                        static const QString idFromString(const QString);

                        QDBusArgument &operator<< (QDBusArgument &p_arg) {
                            p_arg.beginStructure();
                            p_arg << m_id << m_lcl << m_sym << m_flg;
                            p_arg.endStructure();
                            return p_arg;
                        }

                        const QDBusArgument &operator>> (const QDBusArgument &p_arg) {
                            p_arg.beginStructure();
                            p_arg >> m_id >> m_lcl >> m_sym >> m_flg;
                            p_arg.endStructure();
                            return p_arg;
                        }

                        static const Data Null; /**< Represents an empty set of data. */
                };

                /**
                 * @brief The most basic model for data obtaining for lexical information
                 * storage.
                 *
                 * The Model class provides the most simplest means of loading and
                 * saving lexical information for Wintermute. This class merely serves
                 * as a base of the more defined classes (LoadModel and SaveModel, typically
                 * used instead of this) for storage purposes.
                 *
                 * @see LoadModel, SaveModel
                 * @class Model models.hpp "include/wntr/data/models.hpp"
                 */
                class Model : public QObject {
                    Q_OBJECT
                    Q_PROPERTY(Data data READ data WRITE setData)

                    protected:
                        mutable Data m_dt; /**< The variable holding the internal Data. */

                    public:
                        /**
                         * @brief Default constructor.
                         * @fn ~Model
                         */
                        virtual ~Model();

                        /**
                         * @brief Null constructor.
                         * @fn Model
                         */
                        Model();

                        /**
                         * @brief Constructor, using lexical data.
                         * @fn Model
                         * @param p_info The data to fill itself with.
                         */
                        Model ( Data& );

                        /**
                         * @brief Copy constructor.
                         * @fn Model
                         * @param p_mdl The Model to be copied.
                         */
                        Model ( const Model& );

                        /**
                         * @brief Obtains the data stored in this Model.
                         * @fn getLexicalMap
                         * @return Data
                         */
                        const Data& data();

                        /**
                         * @brief Changes the internal Data to p_dt.
                         * @fn setData
                         * @param p_dt The Data to be used, or typically Data::Null.
                         */
                        void setData(const Data& = Data::Null );
                };

                /**
                 * @brief Represents a model for saving data to the lexical information
                 * storage.
                 *
                 * The SaveModel class is typically dervived for the simplest of
                 * saving information to whatever it's dervied source of lexical
                 * information may be. Typically, you'd use this class if you
                 * don't know where your information is being saved to; but the
                 * way that the system is built; you'll never need to programatically
                 * use this base.
                 *
                 * @see DomSaveModel
                 * @class SaveModel models.hpp "include/wntr/data/models.hpp"
                 */
                class SaveModel : public Model {
                    Q_OBJECT
                    protected:
                        /**
                         * @brief Default constructor.
                         * @fn SaveModel
                         */
                        SaveModel();

                        /**
                         * @brief Constructor.
                         * @fn SaveModel
                         * @param p_lxin The Data to be saved.
                         */
                        SaveModel ( Data& );

                        /**
                         * @brief Base copy constructor.
                         * @fn SaveModel
                         * @param p_mod The Model to be copied.
                         */
                        SaveModel ( const Model& );

                        /**
                         * @brief Copy constructor.
                         * @fn SaveModel
                         * @param p_smod The SaveModel to be copied.
                         */
                        SaveModel ( const SaveModel& );

                        /**
                         * @brief Deconstructor.
                         * @fn ~SaveModel
                         */
                        ~SaveModel();

                    public:
                        /**
                         * @brief Saves the data in this SaveModel to the specified
                         *        lexical information storage.
                         * @fn save
                         */
                        virtual void save() = 0;

                        /**
                         * @brief Saves p_dt to the specified lexical information
                         *        storage.
                         * @fn saveFrom
                         * @param p_dt The Data to be saved.
                         */
                        virtual void saveFrom(const Data& ) = 0;

                    signals:

                        /**
                         * @brief Emitted when a save operation has been completed
                         *        successfully. Typically emitted if the operation
                         *        is asynchronous.
                         * @fn saved
                         */
                        void saved();
                };

                /**
                 * @brief Represents a model for saving data to the lexical information
                 * storage.
                 *
                 * The LoadModel class is typically dervived for the simplest of
                 * loading information to whatever it's dervied source of lexical
                 * information may be. Typically, you'd use this class if you
                 * don't know where your information is being loaded from; but the
                 * way that the system is built; you'll never need to programatically
                 * use this base.
                 *
                 * @class LoadModel models.hpp "include/wntr/data/models.hpp"
                 */
                class LoadModel : public Model {
                    Q_OBJECT

                    protected:
                        /**
                         * @brief Null constructor.
                         * @fn LoadModel
                         */
                        LoadModel();

                        /**
                         * @brief Copy constructor.
                         * @fn LoadModel
                         * @param p_mdl The LoadModel to be copied.
                         */
                        LoadModel ( const LoadModel& );

                        /**
                         * @brief Base copy constructor.
                         * @fn LoadModel
                         * @param p_mdl The Model to be copied.
                         */
                        LoadModel ( const Model& );

                        /**
                         * @brief Deconstructor.
                         * @fn ~LoadModel
                         */
                        ~LoadModel();

                    public:

                        /**
                         * @brief Loads the Data from its specified lexical
                         *        information storage.
                         * @fn load
                         * @return The Data obtained from disk, or Data::Null.
                         */
                        virtual const Data* load( ) const = 0;

                        /**
                         * @brief Loads the lexical information storage to p_dt.
                         * @fn loadTo
                         * @param p_dt The Data to load the information to.
                         */
                        virtual bool loadTo(Data& ) const = 0;

                    signals:
                        /**
                         * @brief Emitted when the load operation behind this
                                  LoadModel has completed. Typically, this is used
                                  for asynchronous operations.
                         * @fn loaded
                         */
                        void loaded() const;
                };

                /**
                 * @brief Represents the infrastructual backend of all storage classes.
                 * @class Backend models.hpp "src/models.hpp"
                 */
                class Backend { };

                /**
                 * @brief Represents the foundational front-end means of loading
                 *        and saving lexical information from any storage.
                 *
                 * The Storage class is the <b>recommended</b> class to use for
                 *
                 * @class Storage models.hpp "include/wntr/data/models.hpp"
                 */
                class Storage : public virtual Backend {
                    public:
                        /**
                         * @brief
                         *
                         * @fn GenericStorage
                         */
                        Storage();
                        /**
                         * @brief
                         *
                         * @fn Storage
                         * @param
                         */
                        Storage(const Storage&);
                        /**
                         * @brief
                         *
                         * @fn Storage
                         * @param
                         */
                        Storage(const Backend&);
                        /**
                         * @brief
                         *
                         * @fn operator ==
                         * @param
                         */
                        bool operator==(const Storage&) const;
                        /**
                         * @brief
                         *
                         * @fn type
                         */
                        virtual const QString type() const = 0;
                        /**
                         * @brief
                         *
                         * @fn exists
                         * @param
                         */
                        virtual const bool exists(const Data& ) const = 0;
                        /**
                         * @brief
                         *
                         * @fn loadTo
                         * @param
                         */
                        virtual void loadTo(Data&) const = 0;
                        /**
                         * @brief
                         *
                         * @fn saveFrom
                         * @param
                         */
                        virtual void saveFrom(const Data&) = 0;
                        /**
                         * @brief
                         *
                         * @fn generate
                         */
                        virtual void generate() = 0;
                        /**
                         * @brief
                         *
                         * @fn hasPseudo
                         * @param
                         */
                        virtual const bool hasPseudo(const Data&) const = 0;
                        /**
                         * @brief
                         *
                         * @fn loadPseudo
                         * @param
                         */
                        virtual void loadPseudo(Data&) const = 0;

                        /**
                         * @brief
                         *
                         * @fn obtainFullSuffix
                         * @param
                         * @param
                         */
                        virtual const QString obtainFullSuffix(const QString&, const QString&) const = 0;
                };

                /**
                 * @brief
                 *
                 * @class Cache models.hpp "include/wntr/data/models.hpp"
                 */
                class Cache {
                    friend class Storage;
                    friend class Wintermute::Data::NodeAdaptor;
                    friend class Wintermute::Data::Linguistics::System;
                    typedef QList<Storage*> StorageList;

                    private:
                        static StorageList s_stores; /**< TODO */

                        /**
                         * @brief
                         *
                         * @fn addStorage
                         * @param
                         */
                        static Storage* addStorage(Storage*);
                        /**
                         * @brief
                         *
                         * @fn clearStorage
                         */
                        static void clearStorage();
                        /**
                         * @brief
                         *
                         * @fn hasStorage
                         * @param
                         */
                        static const bool hasStorage(const QString&);
                        /**
                         * @brief
                         *
                         * @fn storage
                         * @param
                         */
                        static Storage* storage(const QString&);

                    public:
                        ~Cache();
                        /**
                         * @brief
                         *
                         * @fn read
                         * @param
                         */
                        static const bool read( Data & );
                        /**
                         * @brief
                         *
                         * @fn write
                         * @param
                         */
                        static void write( const Data & );
                        /**
                         * @brief
                         *
                         * @fn exists
                         * @param
                         */
                        static const bool exists( const Data& );
                        /**
                         * @brief
                         *
                         * @fn pseudo
                         * @param
                         */
                        static void pseudo( Data & );
                        /**
                         * @brief
                         *
                         * @fn isPseudo
                         * @param
                         */
                        static const bool isPseudo(const Data &);
                        /**
                         * @brief
                         *
                         * @fn generate
                         */
                        static void generate();

                        /**
                         * @brief
                         *
                         * @fn obtainFullSuffix
                         * @param
                         * @param
                         */
                        static const QString obtainFullSuffix(const QString&, const QString&);
                };

                /**
                 * @brief
                 * @class DomBackend models.hpp "include/wntr/data/models.hpp"
                 */
                class DomBackend : public Backend {
                    public:
                        /**
                         * @brief
                         *
                         * @fn DomBackend
                         */
                        DomBackend();
                        /**
                         * @brief
                         *
                         * @fn DomBackend
                         * @param
                         */
                        DomBackend(QDomElement* );

                    protected:
                        mutable QDomElement* m_ele; /**< Represents the Dom data of the object. */
                };

                /**
                 * @brief
                 *
                 * @class DomLoadModel models.hpp "include/wntr/data/models.hpp"
                 */
                class DomLoadModel : public LoadModel, public DomBackend {
                    public:
                        /**
                         * @brief
                         *
                         * @fn DomLoadModel
                         */
                        DomLoadModel();
                        /**
                         * @brief
                         *
                         * @fn DomLoadModel
                         * @param
                         */
                        DomLoadModel(QDomElement* );
                        /**
                         * @brief
                         *
                         * @fn DomLoadModel
                         * @param
                         */
                        DomLoadModel(const DomLoadModel&);
                        /**
                         * @brief
                         *
                         * @fn ~DomLoadModel
                         */
                        virtual ~DomLoadModel();
                        /**
                         * @brief
                         *
                         * @fn load
                         * @param
                         */
                        virtual const Data* load() const;
                        /**
                         * @brief
                         *
                         * @fn loadTo
                         * @param
                         */
                        virtual bool loadTo(Data&) const;

                };

                /**
                 * @brief
                 *
                 * @class DomSaveModel models.hpp "include/wntr/data/models.hpp"
                 */
                class DomSaveModel : public SaveModel, public DomBackend {
                    public:
                        /**
                         * @brief
                         *
                         * @fn DomSaveModel
                         */
                        DomSaveModel();
                        /**
                         * @brief
                         *
                         * @fn DomSaveModel
                         * @param
                         */
                        DomSaveModel(QDomElement* );
                        /**
                         * @brief
                         *
                         * @fn DomSaveModel
                         * @param
                         */
                        DomSaveModel(const DomSaveModel&);
                        /**
                         * @brief
                         *
                         * @fn ~DomSaveModel
                         */
                        virtual ~DomSaveModel();
                        /**
                         * @brief
                         *
                         * @fn save
                         * @param
                         */
                        virtual void save();
                        /**
                         * @brief
                         *
                         * @fn saveFrom
                         * @param
                         */
                        virtual void saveFrom(const Data&);
                };

                /**
                 * @brief
                 *
                 * @class DomStorage models.hpp "include/wntr/data/models.hpp"
                 */
                class DomStorage : public Storage {
                    private:
                        /**
                         * @brief
                         *
                         * @fn getPath
                         * @param
                         */
                        static const QString getPath(const Data&);
                        /**
                         * @brief
                         *
                         * @fn spawn
                         * @param
                         */
                        static void spawn(const QDomDocument&);

                    public:
                        /**
                         * @brief
                         *
                         * @fn DomStorage
                         */
                        DomStorage();
                        /**
                         * @brief
                         *
                         * @fn DomStorage
                         * @param
                         */
                        DomStorage(const DomStorage& );
                        /**
                         * @brief
                         *
                         * @fn ~DomStorage
                         */
                        virtual ~DomStorage();
                        /**
                         * @brief
                         *
                         * @fn exists
                         * @param
                         */
                        virtual const bool exists (const Data &) const;
                        /**
                         * @brief
                         *
                         * @fn type
                         */
                        virtual const QString type () const;
                        /**
                         * @brief
                         *
                         * @fn loadTo
                         * @param
                         */
                        virtual void loadTo (Data &) const;
                        /**
                         * @brief
                         *
                         * @fn saveFrom
                         * @param
                         */
                        virtual void saveFrom (const Data &);
                        /**
                         * @brief
                         *
                         * @fn generate
                         */
                        virtual void generate ();
                        /**
                         * @brief
                         *
                         * @fn hasPseudo
                         * @param
                         */
                        virtual const bool hasPseudo(const Data &) const;
                        /**
                         * @brief
                         *
                         * @fn loadPseduo
                         * @param
                         */
                        virtual void loadPseudo(Data&) const;

                        /**
                         * @brief
                         *
                         * @fn obtainFullSuffix
                         * @param
                         * @param
                         */
                        virtual const QString obtainFullSuffix (const QString &, const QString &) const;

                        /**
                         * @brief
                         *
                         * @fn spawnDoc
                         * @param
                         */
                        QDomDocument* getSpawnDoc(const Data&) const;
                };

            }

            namespace Rules {
                struct Bond;
                struct Chain;

                struct Model;
                struct LoadModel;
                struct SaveModel;
                struct Storage;
                struct Backend;
                struct Cache;

                struct DomLoadModel;
                struct DomSaveModel;
                struct DomStorage;
                struct DomBackend;

                /**
                 * @brief Represents a key-value list of strings.
                 * @typedef StringMap
                 */
                typedef QMap<QString, QString> StringMap;

                /**
                 * @brief Represents a list of Bonds.
                 * @typedef BondVector
                 */
                typedef QList<Bond> BondList;

                /**
                 * @brief Represents the syntactical data and rules needed to form a syntactic link.
                 *
                 * Bonds are used by the Parser to determine whether or not a word (of a specified parent rule)
                 * can efficiently and properly bond with other words. Specificially, the Binding object uses
                 * the Bond object to form specialized links:
                 *
                 * @code
                 * const Link* l_lnk = Binding::obtain(p_nd, p_nd2);
                 * @endcode
                 *
                 * @see Binding
                 * @see Cache
                 * @class Bond models.hpp "include/wntr/data/models.hpp"
                 */
                class Bond : public QObject {
                    Q_OBJECT
                    Q_PROPERTY(QString With READ with WRITE setWith)
                    Q_PROPERTY(StringMap Attributes READ attributes WRITE setAttributes)
                    friend QDBusArgument& operator<< (QDBusArgument& , const Bond& );
                    friend const QDBusArgument& operator>> (const QDBusArgument& , Bond& );

                    public:
                        /**
                         * @brief Empty constructor.
                         * @fn Bond
                         */
                        Bond();

                        /**
                         * @brief Copy constructor.
                         * @fn Bond
                         * @param Bond The source Bond to be copied.
                         */
                        Bond(const Bond& );

                        /**
                         * @brief Assignment operator.
                         * @fn operator =
                         * @param Bond The source Bond to be copied.
                         */
                        void operator=(const Bond&);

                        /**
                         * @brief
                         *
                         * @fn operator ==
                         * @param
                         */
                        const bool operator==(const Bond&) const;
                        /**
                         * @brief
                         *
                         * @fn ~Bond
                         */
                        virtual ~Bond();
                        /**
                         * @brief
                         *
                         * @fn with
                         */
                        const QString with() const;
                        /**
                         * @brief
                         *
                         * @fn attribute
                         * @param
                         */
                        const QString attribute(const QString& ) const;
                        /**
                         * @brief
                         *
                         * @fn attributes
                         */
                        const StringMap attributes() const;
                        /**
                         * @brief
                         *
                         * @fn hasAttribute
                         * @param
                         */
                        const bool hasAttribute(const QString& ) const;
                        /**
                         * @brief
                         *
                         * @fn setWith
                         * @param
                         */
                        void setWith( QString& );
                        /**
                         * @brief
                         *
                         * @fn setAttribute
                         * @param
                         * @param
                         */
                        void setAttribute(const QString& , QString& );
                        /**
                         * @brief
                         *
                         * @fn setAttributes
                         * @param
                         */
                        void setAttributes(const StringMap& );
                        /**
                         * @brief
                         *
                         * @fn matches
                         * @param
                         * @param
                         */
                        static const double matches(const QString& , const QString& );

                    private:
                        StringMap m_props; /**< Holds all of the attributes. */
                };

                /**
                 * @brief
                 *
                 * @class Chain models.hpp "include/wntr/data/models.hpp"
                 */
                class Chain : public QObject {
                    friend QDBusArgument& operator<< (QDBusArgument&, const Chain&);
                    friend const QDBusArgument& operator>> (const QDBusArgument&, Chain&);
                    Q_OBJECT
                    Q_PROPERTY(BondList Bonds READ bonds WRITE setBonds)
                    Q_PROPERTY(QString Type READ type WRITE setType)
                    Q_PROPERTY(QString Locale READ locale)

                    public:
                        /**
                         * @brief
                         *
                         * @fn Chain
                         */
                        Chain();
                        /**
                         * @brief
                         *
                         * @fn Chain
                         * @param
                         */
                        Chain(const Chain&);
                        /**
                         * @brief
                         *
                         * @fn Chain
                         * @param
                         * @param
                         */
                        explicit Chain(const QString&, const QString& = QString::null, const BondList& = BondList());
                        /**
                         * @brief
                         *
                         * @fn operator =
                         * @param
                         */
                        void operator=(const Chain&);
                        /**
                         * @brief
                         *
                         * @fn ~Chain
                         */
                        virtual ~Chain();
                        /**
                         * @brief
                         *
                         * @fn setBonds
                         * @param
                         */
                        void setBonds(const BondList&);
                        /**
                         * @brief
                         *
                         * @fn setType
                         */
                        void setType(const QString&);
                        /**
                         * @brief
                         *
                         * @fn bonds
                         */
                        const BondList bonds() const;
                        /**
                         * @brief
                         *
                         * @fn locale
                         */
                        const QString locale() const;
                        /**
                         * @brief
                         *
                         * @fn type
                         */
                        const QString type() const;
                        /**
                         * @brief
                         *
                         * @fn operator []
                         * @param
                         */
                        Bond operator[](const int&) const;
                private:
                        BondList m_bndVtr; /**< Holds the bonds */
                        QString m_lcl;
                        QString m_typ;
                };


                /**
                 * @brief
                 * @class Model models.hpp "include/wntr/data/models.hpp"
                 * @todo This should provide the practical methods of obtaining Bindings.
                 */
                class Model : public QObject {
                    Q_OBJECT
                    Q_PROPERTY(Chain Chain READ chain WRITE setChain)

                    protected:
                        Chain m_chn; /**< Represents the information used to form a binding. */
                        /**
                         * @brief
                         *
                         * @fn Model
                         * @param
                         */
                        Model(const Bond& );

                    public:
                        /**
                         * @brief
                         *
                         * @fn Model
                         */
                        Model();
                        /**
                         * @brief
                         *
                         * @fn Model
                         * @param
                         */
                        Model(const Model& );
                        /**
                         * @brief
                         *
                         * @fn ~Model
                         */
                        virtual ~Model();
                        /**
                         * @brief
                         *
                         * @fn bond
                         */
                        const Chain chain() const;
                        /**
                         * @brief
                         *
                         * @fn setBond
                         * @param
                         */
                        void setChain(const Chain& );
                };

                /**
                 * @brief
                 * @class SaveModel models.hpp "include/wntr/data/models.hpp"
                 */
                class SaveModel : public Model {
                    Q_OBJECT

                    signals:
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
                        /**
                         * @brief
                         *
                         * @fn saveFrom
                         * @param
                         */
                        virtual void saveFrom(const Chain&) = 0;

                    protected:
                        /**
                         * @brief
                         * @fn SaveModel
                         */
                        SaveModel();
                        /**
                         * @brief
                         * @fn SaveModel
                         * @param p_model
                         */
                        SaveModel(const Model& );
                        /**
                         * @brief
                         * @fn ~SaveModel
                         */
                        virtual ~SaveModel() = 0;
                };

                /**
                 * @brief
                 * @class LoadModel models.hpp "include/wntr/data/models.hpp"
                 */
                class LoadModel : public Model {
                    Q_OBJECT

                    signals:
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
                        virtual const Chain* load() const = 0;
                        /**
                         * @brief
                         *
                         * @fn loadTo
                         * @param
                         */
                        virtual void loadTo(Chain&) const = 0;

                    protected:
                        /**
                         * @brief
                         * @fn LoadModel
                         */
                        LoadModel();
                        /**
                         * @brief
                         * @fn LoadModel
                         * @param p_model
                         */
                        LoadModel(const Model& );
                        /**
                         * @brief
                         *
                         * @fn ~LoadModel
                         */
                        virtual ~LoadModel() = 0;
                };

                class Backend {

                };

                /**
                 * @brief
                 * @class Storage models.hpp "include/wntr/data/models.hpp"
                 */
                class Storage : public virtual Backend {
                    public:
                        /**
                         * @brief
                         *
                         * @fn Storage
                         */
                        Storage();
                        /**
                         * @brief
                         *
                         * @fn Storage
                         * @param
                         */
                        Storage(const Storage&);
                        /**
                         * @brief
                         *
                         * @fn Storage
                         * @param
                         */
                        Storage(const Backend&);
                        /**
                         * @brief
                         *
                         * @fn Storage
                         * @param
                         * @param
                         */
                        Storage(const QString&, const QString&);
                        /**
                         * @brief
                         *
                         * @fn operator ==
                         * @param
                         */
                        bool operator==(const Storage&);
                        /**
                         * @brief
                         *
                         * @fn loadTo
                         * @param
                         */
                        virtual void loadTo(Chain&) const = 0;
                        /**
                         * @brief
                         *
                         * @fn saveFrom
                         * @param
                         */
                        virtual void saveFrom(const Chain&) = 0;
                        /**
                         * @brief
                         *
                         * @fn exists
                         * @param
                         */
                        virtual const bool exists(const QString, const QString) const = 0;
                        /**
                         * @brief
                         *
                         * @fn type
                         */
                        virtual const QString type() const = 0;
                        /**
                         * @brief
                         *
                         * @fn flag
                         */
                        virtual const QString flag() const;
                        /**
                         * @brief
                         *
                         * @fn locale
                         */
                        virtual const QString locale() const;
                        /**
                         * @brief
                         *
                         * @fn ~Storage
                         */
                        virtual ~Storage();
                    private:
                        QString m_flg;
                        QString m_lcl;
                };


                /**
                 * @brief
                 *
                 * @class DomBackend models.hpp "include/wntr/data/models.hpp"
                 */
                class DomBackend {
                    public:
                        /**
                         * @brief
                         *
                         * @fn DomBackend
                         */
                        DomBackend();
                        /**
                         * @brief
                         *
                         * @fn DomBackend
                         * @param
                         */
                        DomBackend(const DomBackend&);
                        /**
                         * @brief
                         *
                         * @fn DomBackend
                         * @param
                         */
                        explicit DomBackend(QDomElement* );
                        /**
                         * @brief
                         *
                         * @fn ~DomBackend
                         */
                        ~DomBackend();

                    protected:
                        QDomElement* m_elem;
                };

                /**
                 * @brief
                 *
                 * @class DomLoadModel models.hpp "include/wntr/data/models.hpp"
                 */
                class DomLoadModel : public LoadModel, public DomBackend {
                    Q_OBJECT
                    public:
                        /**
                         * @brief
                         *
                         * @fn DomLoadModel
                         */
                        DomLoadModel();
                        /**
                         * @brief
                         *
                         * @fn DomLoadModel
                         * @param
                         */
                        DomLoadModel(const DomLoadModel&);
                        /**
                         * @brief
                         *
                         * @fn DomLoadModel
                         * @param
                         */
                        DomLoadModel(QDomElement* );
                        /**
                         * @brief
                         *
                         * @fn ~DomLoadModel
                         */
                        virtual ~DomLoadModel();
                        /**
                         * @brief
                         *
                         * @fn load
                         */
                        virtual const Chain* load () const;
                        /**
                         * @brief
                         *
                         * @fn loadTo
                         * @param
                         */
                        virtual void loadTo (Chain &) const;
                    private:
                        /**
                         * @brief
                         *
                         * @fn obtainType
                         * @param QDomElement
                         */
                        const QString obtainType(QDomElement ) const;
                        /**
                         * @brief
                         *
                         * @fn obtainBonds
                         * @param
                         * @param QDomElement
                         */
                        void obtainBonds(BondList&, QDomElement) const;
                };

                /**
                 * @brief
                 *
                 * @class DomSaveModel models.hpp "include/wntr/data/models.hpp"
                 */
                class DomSaveModel : public SaveModel, public DomBackend {
                    Q_OBJECT
                    public:
                        /**
                         * @brief
                         *
                         * @fn DomSaveModel
                         */
                        DomSaveModel();
                        /**
                         * @brief
                         *
                         * @fn DomSaveModel
                         * @param
                         */
                        DomSaveModel(const DomSaveModel&);
                        /**
                         * @brief
                         *
                         * @fn DomSaveModel
                         * @param
                         */
                        DomSaveModel(QDomElement* );
                        /**
                         * @brief
                         *
                         * @fn ~DomSaveModel
                         */
                        virtual ~DomSaveModel();
                        /**
                         * @brief
                         *
                         * @fn save
                         */
                        virtual void save ();
                        /**
                         * @brief
                         *
                         * @fn saveFrom
                         * @param
                         */
                        virtual void saveFrom(const Chain &);

                };

                /**
                 * @brief
                 *
                 * @class DomStorage models.hpp "include/wntr/data/models.hpp"
                 */
                class DomStorage : public Storage {
                    public:
                        /**
                         * @brief
                         *
                         * @fn DomStorage
                         */
                        DomStorage();
                        /**
                         * @brief
                         *
                         * @fn DomStorage
                         * @param
                         */
                        DomStorage(const Storage&);
                        /**
                         * @brief
                         *
                         * @fn ~DomStorage
                         */
                        virtual ~DomStorage();
                        /**
                         * @brief
                         *
                         * @fn loadTo
                         * @param
                         */
                        virtual void loadTo (Chain &) const;
                        /**
                         * @brief
                         *
                         * @fn saveFrom
                         * @param
                         */
                        virtual void saveFrom (const Chain &);
                        /**
                         * @brief
                         *
                         * @fn exists
                         * @param
                         */
                        virtual const bool exists (const QString , const QString ) const;
                        /**
                         * @brief
                         *
                         * @fn type
                         */
                        virtual const QString type () const;
                    private:
                        mutable double m_min; /**< Represents the strength of matching. */
                        /**
                         * @brief
                         *
                         * @fn matches
                         * @param
                         * @param
                         */
                        static const double matches(const QString&, const QString&);
                        /**
                         * @brief
                         *
                         * @fn findElement
                         * @param
                         * @param QDomElement
                         */
                        QDomElement findElement(const Chain&, QDomElement ) const;
                        /**
                         * @brief
                         *
                         * @fn findElement
                         * @param
                         * @param QString
                         */
                        QDomElement findElement(const Chain&, QDomElement , QString) const;
                        /**
                         * @brief
                         *
                         * @fn getPath
                         * @param
                         */
                        static const QString getPath(const Chain&);
                        /**
                         * @brief
                         *
                         * @fn loadDom
                         * @param
                         */
                        static QDomDocument* loadDom(const Chain&);
                };

                /**
                 * @brief
                 *
                 * @class Cache models.hpp "include/wntr/data/models.hpp"
                 */
                class Cache {
                    /**
                     * @brief
                     *
                     * @typedef StorageList
                     */
                    typedef QList<Storage*> StorageList;
                    friend class Wintermute::Data::RuleAdaptor;
                    friend class Wintermute::Data::Linguistics::System;

                    private:
                        Cache();
                        static StorageList s_stores; /**< Holds the storage. */
                        /**
                         * @brief
                         *
                         * @fn addStorage
                         * @param
                         */
                        static Storage* addStorage(Storage* );
                        /**
                         * @brief
                         *
                         * @fn storage
                         * @param
                         */
                        static Storage* storage(const QString&);
                        /**
                         * @brief
                         *
                         * @fn hasStorage
                         * @param
                         */
                        static const bool hasStorage(const QString&);
                        /**
                         * @brief
                         *
                         * @fn clearStorage
                         */
                        static void clearStorage();

                    public:
                        /**
                         * @brief
                         *
                         * @fn write
                         * @param
                         */
                        static void write(const Chain&);
                        /**
                         * @brief
                         *
                         * @fn exists
                         * @param
                         */
                        static const bool exists(const QString&, const QString&);
                        /**
                         * @brief
                         *
                         * @fn read
                         * @param
                         */
                        static const bool read(Chain&);
                };
            }
        }
    }
}

Q_DECLARE_METATYPE(Wintermute::Data::Linguistics::Lexical::Data)
Q_DECLARE_METATYPE(Wintermute::Data::Linguistics::Rules::Bond)
Q_DECLARE_METATYPE(Wintermute::Data::Linguistics::Rules::Chain)

#endif /* MODELS_HPP */
// kate: indent-mode cstyle; space-indent on; indent-width 4;

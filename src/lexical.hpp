/**
 * @file lexical.hpp
 * @author Wintermute Developers <wintermute-devel@lists.launchpad.net>
 * @date Sun, 30 Oct 2011 21:54:16
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
 
#ifndef LEXICAL_HPP
#define LEXICAL_HPP
 
#include <QObject>
#include <QList>
#include <QMultiMap>
#include <QDebug>
#include <QtXml/QDomDocument>
#include <QtDBus/QDBusMetaType>
#include <QMetaType>
#include "linguistics.hpp"

namespace Wintermute {
    namespace Data {
        struct NodeAdaptor;
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
                 * @class Data models.hpp "src/models.hpp"
                 * @see FlagMapping
                 */
                class Data : public QObject {
                    friend QDBusArgument& operator<< (QDBusArgument&, const Data&);
                    friend const QDBusArgument& operator>> (const QDBusArgument&, Data&);
                    friend QDebug operator<<(QDebug dbg, const Data&);

                    Q_OBJECT
                    Q_PROPERTY(QString Locale READ locale WRITE setLocale)
                    Q_PROPERTY(QString Symbol READ symbol WRITE setSymbol)
                    Q_PROPERTY(QString ID READ id WRITE setID)
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

                        void setLocale(const QString&);
                        void setID(const QString&);

                        /**
                         * @brief Determines if this Data is equivalent to a null Data object.
                         * @fn isNull
                         */
                        const bool isNull() const;

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
                 * @class Model models.hpp "src/models.hpp"
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
                 * @class SaveModel models.hpp "src/models.hpp"
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
                 * @class LoadModel models.hpp "src/models.hpp"
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
                 * @class Storage models.hpp "src/models.hpp"
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
                 * @class Cache models.hpp "src/models.hpp"
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
                         * @fn countFlags
                         */
                        static const int countFlags();

                        /**
                         * @brief
                         *
                         * @fn countSymbols
                         */
                        static const int countSymbols();

                        /**
                         * @brief
                         *
                         * @fn allNodes
                         * @param
                         */
                        static const QStringList allNodes(const QString& = Wintermute::Data::Linguistics::System::locale ());

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
                 * @class DomBackend models.hpp "src/models.hpp"
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
                 * @class DomLoadModel models.hpp "src/models.hpp"
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
                 * @class DomSaveModel models.hpp "src/models.hpp"
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
                 * @class DomStorage models.hpp "src/models.hpp"
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
                         * @fn spawnDoc
                         * @param
                         */
                        static QDomDocument* getSpawnDoc(const Data&);
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
                         * @fn countFlags
                         */
                        static const int countFlags();

                        /**
                         * @brief
                         *
                         * @fn countSymbols
                         */
                        static const int countSymbols();
                };

            }
        }
    }
}

Q_DECLARE_TYPEINFO(Wintermute::Data::Linguistics::Lexical::Data, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(Wintermute::Data::Linguistics::Lexical::Data)

#endif

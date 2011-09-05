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
#include <QtDebug>
#include <QMultiHash>
#include <QStringList>
#include <QtXml/QDomDocument>
#include "linguistics.hpp"

using namespace std;

using std::map;

namespace Wintermute {
    namespace Data {
        namespace Linguistics {
            namespace Lexical {
                struct Data;

                struct Model;
                struct SaveModel;
                struct LoadModel;
                struct Storage;
                struct Cache;

                /**
                 * @brief
                 *
                 * @typedef DataMap
                 */
                typedef QMultiHash<const QString, const QString> DataFlagMap;

                /**
                 * @brief
                 * @note This class can be considered this a POD of Wintermute; as it is used extensively.
                 * @class Data models.hpp "include/wntr/data/models.hpp"
                 */
                class Data : public QObject {
                    Q_OBJECT
                    Q_PROPERTY(QString Locale READ locale)
                    Q_PROPERTY(QString Symbol READ symbol WRITE setSymbol)
                    Q_PROPERTY(QString ID READ id)
                    Q_PROPERTY(DataFlagMap Flags READ flags WRITE setFlags)

                    private:
                        QString m_id;
                        QString m_lcl;
                        QString m_sym;
                        DataFlagMap m_flg;

                    protected:
                        /**
                         * @brief
                         *
                         * @fn Data
                         * @param string
                         * @param string
                         * @param string
                         * @param DataFlagMap
                         */
                        Data(const QString , const QString , const QString = "" , const DataFlagMap = DataFlagMap());

                    public:
                        /**
                         * @brief
                         *
                         * @fn Data
                         */
                        Data();
                        /**
                         * @brief
                         *
                         * @fn Data
                         * @param
                         */
                        Data(const Data&);
                        /**
                         * @brief
                         *
                         * @fn operator ==
                         * @param
                         */
                        bool operator==(const Data&) const;
                        /**
                         * @brief
                         *
                         * @fn operator =
                         * @param
                         */
                        void operator=(const Data&);
                        /**
                         * @brief
                         *
                         * @fn ~Data
                         */
                        virtual ~Data();
                        /**
                         * @brief
                         *
                         * @fn id
                         */
                        const QString id() const;
                        /**
                         * @brief
                         *
                         * @fn locale
                         */
                        const QString locale() const;
                        /**
                         * @brief
                         *
                         * @fn symbol
                         */
                        const QString symbol() const;
                        /**
                         * @brief
                         *
                         * @fn flags
                         */
                        const DataFlagMap flags() const;
                        /**
                         * @brief
                         *
                         * @fn setSymbol
                         * @param
                         */
                        void setSymbol( const QString& );
                        /**
                         * @brief
                         *
                         * @fn setFlags
                         * @param
                         */
                        void setFlags( const DataFlagMap& );
                        /**
                         * @brief
                         *
                         * @fn isEmpty
                         */
                        const bool isEmpty() const;
                        /**
                         * @brief
                         *
                         * @fn createData
                         * @param string
                         * @param string
                         * @param string
                         * @param DataFlagMap
                         */
                        static Data createData(const QString, const QString, const QString = "", const DataFlagMap = DataFlagMap());
                        /**
                         * @brief
                         *
                         * @fn idFromString
                         * @param string
                         */
                        static const QString idFromString(const QString);

                        static const Data Null; /**< Represents an empty set of data. */
                };

                /**
                 * @brief
                 *
                 * @class Model models.hpp "include/wntr/data/models.hpp"
                 */
                class Model : public QObject {
                    Q_OBJECT
                    Q_PROPERTY(Data data READ data WRITE setData)

                    protected:
                        mutable Data m_dt; /**< TODO */

                    public:
                        /**
                         * @brief
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
                         * @param info The data to fill itself with.
                         */
                        Model ( Data& );
                        /**
                         * @brief
                         * @fn Model
                         * @param
                         */
                        Model ( const Model& );

                        /**
                         * @brief
                         * @fn getLexicalMap
                         */
                        const Data& data();

                        /**
                         * @brief
                         *
                         * @fn setData
                         * @param
                         */
                        void setData(const Data& = Data::Null );
                };

                /**
                 * @brief
                 *
                 * @class SaveModel models.hpp "include/wntr/data/models.hpp"
                 */
                class SaveModel : public Model {
                    Q_OBJECT
                    protected:
                        /**
                         * @brief
                         *
                         * @fn SaveModel
                         */
                        SaveModel();

                        /**
                         * @brief
                         *
                         * @fn SaveModel
                         * @param p_lxin
                         */
                        SaveModel ( Data& );

                        /**
                         * @brief
                         *
                         * @fn SaveModel
                         * @param p_mod
                         */
                        SaveModel ( const Model& );

                        /**
                         * @brief
                         *
                         * @fn SaveModel
                         * @param p_smod
                         */
                        SaveModel ( const SaveModel& );
                        /**
                         * @brief
                         *
                         * @fn ~SaveModel
                         */
                        ~SaveModel();

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
                        virtual void saveFrom(const Data& ) = 0;

                    signals:
                        /**
                         * @brief
                         *
                         * @fn saved
                         */
                        void saved();
                };

                /**
                 * @brief
                 *
                 * @class LoadModel models.hpp "include/wntr/data/models.hpp"
                 */
                class LoadModel : public Model {
                    Q_OBJECT

                    protected:
                        /**
                         * @brief
                         * @fn LoadModel
                         */
                        LoadModel();

                        /**
                         * @brief
                         * @fn LoadModel
                         * @param loadModel
                         */
                        LoadModel ( const LoadModel& );

                        /**
                         * @brief
                         * @fn LoadModel
                         * @param model
                         */
                        LoadModel ( const Model& );
                        /**
                         * @brief
                         * @fn ~LoadModel
                         */
                        ~LoadModel();

                    public:

                        /**
                         * @brief
                         *
                         * @fn load
                         */
                        virtual const Data* load( ) const = 0;
                        /**
                         * @brief
                         *
                         * @fn loadTo
                         * @param
                         */
                        virtual bool loadTo(Data& ) const = 0;

                    signals:
                        /**
                         * @brief
                         *
                         * @fn loaded
                         */
                        void loaded() const;
                };

                /**
                 * @brief
                 * @todo Attempt to drop the Boost dependency here and find another means of implementing inherited interfaces to this class.
                 * @class Storage models.hpp "include/wntr/data/models.hpp"
                 */
                class Storage {
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
                    friend class Wintermute::Data::Linguistics::Configuration;
                    typedef QList<Storage*> StorageList;

                    private:
                        static StorageList s_stores; /**< TODO */

                        /**
                         * @brief
                         *
                         * @fn addStorage
                         * @param
                         */
                        static void addStorage(Storage*);

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
                struct Cache;

                /**
                 * @brief Represents a key-value list of strings.
                 * @typedef StringMap
                 * @bug Qt's container classes force us to use pointers instead of values,
                 *      unlike the typical STL library. I want to use Qt for memory management,
                 *      but that feature messes things up. This'll use the std::map until we
                 *      find a work-around. (Apply the bug number here when reported).
                 */
                typedef map<const QString, QString> StringMap;

                /**
                 * @brief Represents a list of Bonds.
                 * @typedef BondVector
                 */
                typedef QList<Bond*> BondList;

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
                        bool operator==(const Bond&) const;
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

                /**
                 * @brief
                 * @class Storage models.hpp "include/wntr/data/models.hpp"
                 */
                class Storage {
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
                         * @param
                         */
                        Storage(const QString&, const QString&);
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
                    friend class Wintermute::Data::Linguistics::Configuration;

                    private:
                        static StorageList s_stores; /**< Holds the storage. */
                        Cache();
                        virtual ~Cache();
                        /**
                         * @brief
                         *
                         * @fn addStorage
                         * @param
                         */
                        static void addStorage(Storage* );

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



#endif /* MODELS_HPP */
// kate: indent-mode cstyle; space-indent on; indent-width 4;

/**
 * @file rules.hpp
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
 
#ifndef RULES_HPP
#define RULES_HPP

#include <QMap>
#include <QList>
#include <QObject>
#include <QDebug>
#include <QtDBus/QDBusMetaType>
#include <QtXml/QDomElement>
#include <QMetaType>
#include "linguistics.hpp"

namespace Wintermute {
    namespace Data {
        struct RuleAdaptor;
        namespace Linguistics {
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
                 * @class Bond models.hpp "src/models.hpp"
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
                         * @brief Equality operator.
                         * @fn operator ==
                         * @param Bond The source Bond to be compared.
                         */
                        const bool operator==(const Bond&) const;
                        /**
                         * @brief Destructor.
                         * @fn ~Bond
                         */
                        virtual ~Bond();
                        /**
                         * @brief 
                         * @fn with
                         */
                        const QString with() const;
                        /**
                         * @brief
                         * @fn attribute
                         * @param
                         */
                        const QString attribute(const QString& ) const;
                        /**
                         * @brief
                         * @fn attributes
                         */
                        const StringMap attributes() const;
                        /**
                         * @brief
                         * @fn hasAttribute
                         * @param
                         */
                        const bool hasAttribute(const QString& ) const;
                        /**
                         * @brief
                         * @fn setWith
                         * @param
                         */
                        void setWith( QString& );
                        /**
                         * @brief
                         * @fn setAttribute
                         * @param
                         * @param
                         */
                        void setAttribute(const QString& , QString& );
                        /**
                         * @brief
                         * @fn setAttributes
                         * @param
                         */
                        void setAttributes(const StringMap& );
                        /**
                         * @brief
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
                 * @class Chain models.hpp "src/models.hpp"
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
                         * @brief Null constructor.
                         * @fn Chain
                         */
                        Chain();
                        
                        /**
                         * @brief Copy constructor.
                         * @fn Chain
                         * @param p_chn The Chain to be copied.
                         */
                        Chain(const Chain&);
                        
                        /**
                         * @brief Default construcotr.
                         * @fn Chain
                         * @param
                         * @param
                         */
                        explicit Chain(const QString&, const QString& = QString::null, const BondList& = BondList());
                        
                        /**
                         * @brief Assignment operator.
                         * @fn operator =
                         * @param p_chn The Chain to be copied.
                         */
                        void operator=(const Chain&);
                        
                        /**
                         * @brief Deconstructor.
                         * @fn ~Chain
                         */
                        virtual ~Chain();
                        
                        /**
                         * @brief
                         * @fn setBonds
                         * @param
                         */
                        void setBonds(const BondList&);
                        
                        /**
                         * @brief
                         * @fn setType
                         */
                        void setType(const QString&);
                        
                        /**
                         * @brief
                         * @fn bonds
                         */
                        const BondList bonds() const;
                        
                        /**
                         * @brief
                         * @fn locale
                         */
                        const QString locale() const;
                        
                        /**
                         * @brief
                         * @fn type
                         */
                        const QString type() const;
                        
                        /**
                         * @brief
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
                 * @class Model models.hpp "src/models.hpp"
                 */
                class Model : public QObject {
                    Q_OBJECT
                    Q_PROPERTY(Chain Chain READ chain WRITE setChain)

                    protected:
                        Chain m_chn; /**< Represents the information used to form a binding. */
                        /**
                         * @brief Constructor.
                         * @fn Model
                         * @param
                         */
                        explicit Model(const Bond& );

                    public:
                        /**
                         * @brief Null constructor.
                         * @fn Model
                         */
                        Model();
                        
                        /**
                         * @brief Copy constructor.
                         * @fn Model
                         * @param p_mdl The Model to be copied.
                         */
                        Model(const Model& );
                        
                        /**
                         * @brief Deconstructor.
                         * @fn ~Model
                         */
                        virtual ~Model();
                        
                        /**
                         * @brief Obtains the Chain representing this Model.
                         * @fn chain The stored Chain.
                         */
                        const Chain chain() const;
                        
                        /**
                         * @brief Changes the internal Chain to a different Chain.
                         * @fn setChain
                         * @param p_chn The Chain to be saved.
                         */
                        void setChain(const Chain& );
                };

                /**
                 * @brief
                 * @class SaveModel models.hpp "src/models.hpp"
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

                        /**
                         * @brief
                         *
                         * @fn obtainType
                         */
                        virtual void setType(const QString&) = 0;
                        
                        /**
                         * @brief
                         *
                         * @fn obtainBonds
                         */
                        virtual void setBonds(const BondList& ) = 0;
                };

                /**
                 * @brief
                 * @class LoadModel models.hpp "src/models.hpp"
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
                        /**
                         * @brief
                         *
                         * @fn obtainType
                         * @param QDomElement
                         */
                        virtual const QString obtainType(QDomElement ) const = 0;
                        /**
                         * @brief
                         *
                         * @fn obtainBonds
                         * @param
                         * @param QDomElement
                         */
                        virtual void obtainBonds(BondList&, QDomElement) const = 0;
                };

                class Backend { };

                /**
                 * @brief
                 * @class Storage models.hpp "src/models.hpp"
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
                 * @class DomLoadModel models.hpp "src/models.hpp"
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
                        virtual const QString obtainType(QDomElement ) const;
                        /**
                         * @brief
                         *
                         * @fn obtainBonds
                         * @param
                         * @param QDomElement
                         */
                        virtual void obtainBonds(BondList&, QDomElement) const;
                };

                /**
                 * @brief
                 *
                 * @class DomSaveModel models.hpp "src/models.hpp"
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
                    private:
                        /**
                         * @brief
                         *
                         * @fn obtainType
                         * @param QDomElement
                         */
                        virtual void setType(const QString& );
                        /**
                         * @brief
                         *
                         * @fn obtainBonds
                         * @param
                         * @param QDomElement
                         */
                        virtual void setBonds(const BondList&);

                };

                /**
                 * @brief
                 *
                 * @class DomStorage models.hpp "src/models.hpp"
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
                 * @class Cache models.hpp "src/models.hpp"
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

Q_DECLARE_METATYPE(Wintermute::Data::Linguistics::Rules::Bond)
Q_DECLARE_METATYPE(Wintermute::Data::Linguistics::Rules::Chain)

#endif

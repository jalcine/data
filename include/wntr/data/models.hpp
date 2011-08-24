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
 */

#ifndef MODELS_HPP
#define MODELS_HPP

#include <QtDebug>
#include <QMultiMap>
#include <QtXml/QDomDocument>
#include "linguistics.hpp"

using namespace std;
using std::multimap;

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
                typedef QMultiMap<const QString*, const QString*> DataFlagMap;

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
                        QDomElement* m_doc;
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
                };
            }

            /// @todo Define these classes for the Binding, Rule, and RuleSet classes. This'll add some dynamic means of implementing a back-end.
            namespace Rules {
                struct BondData;
                struct Model;
                struct LoadModel;
                struct SaveModel;
                struct Storage;

                /**
                 * @brief
                 *
                 * @typedef SyntaxList
                 */
                typedef QList<BondData> SyntaxList;

                /**
                 * @brief
                 *
                 * @class Syntax models.hpp "include/wntr/data/models.hpp"
                 */
                class BondData : public QObject {
                    Q_OBJECT
                    Q_PROPERTY(string locale READ locale WRITE setLocale)
                    Q_PROPERTY(string ruleText READ ruleText WRITE setRuleText)
                    Q_PROPERTY(string linkText READ linkText WRITE setLinkText)

                    private:
                        string m_lcl;
                        string m_rlTxt;
                        string m_lnkTxt;

                    public:
                        /**
                         * @brief
                         * @fn Syntax
                         */
                        BondData();
                        /**
                         * @brief
                         * @fn Syntax
                         * @param p_syntx
                         */
                        BondData(const BondData&);
                        /**
                         * @brief
                         * @fn ~Syntax
                         */
                        ~BondData();
                        /**
                         * @brief
                         *
                         * @fn operator =
                         * @param
                         */
                        BondData& operator=(const BondData& );
                        /**
                         * @brief
                         * @fn setLocale
                         * @param string
                         */
                        void setLocale(const string);
                        /**
                         * @brief
                         * @fn setRuleText
                         * @param string
                         */
                        void setRuleText(const string);
                        /**
                         * @brief
                         * @fn setLinkText
                         * @param string
                         */
                        void setLinkText(const string);
                        /**
                         * @brief
                         * @fn locale
                         */
                        const string locale() const;
                        /**
                         * @brief
                         * @fn ruleText
                         */
                        const string ruleText() const;
                        /**
                         * @brief
                         * @fn linkText
                         */
                        const string linkText() const;
                };

                /**
                 * @brief
                 * @class Model models.hpp "include/wntr/data/models.hpp"
                 * @todo This should provide the practical methods of obtaining Bindings.
                 */
                class Model : public QObject {
                    Q_OBJECT
                    Q_PROPERTY(BondData syntax READ syntax WRITE setSyntax)

                    protected:
                        BondData m_syntx; /**< Represents the information used to form a binding. */
                        /**
                         * @brief
                         *
                         * @fn Model
                         * @param
                         */
                        Model(const BondData& );

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
                         * @fn syntax
                         */
                        const BondData syntax() const;
                        /**
                         * @brief
                         *
                         * @fn setSyntax
                         * @param
                         */
                        void setSyntax(BondData& );
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
                         * @fn SaveModel
                         * @param p_saveModel
                         */
                        SaveModel(const SaveModel& );
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
                        virtual void load() const = 0;

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
                         * @fn LoadModel
                         * @param p_model
                         */
                        LoadModel(const LoadModel& );
                        /**
                         * @brief
                         *
                         * @fn ~LoadModel
                         */
                        virtual ~LoadModel() = 0;
                };

                class Storage : virtual public SaveModel, virtual public LoadModel {
                    public:
                        Storage();
                        virtual ~Storage();
                    protected:
                    private:
                };
            }
        }
    }
}

#endif /* MODELS_HPP */
// kate: indent-mode cstyle; space-indent on; indent-width 4;

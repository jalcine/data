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

#include <boost/signals2/signal.hpp>
#include <boost/signals2/slot.hpp>
#include <QtDebug>
#include <QMultiMap>
#include <QtXml/QDomElement>

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
                    Q_PROPERTY(QString locale READ locale)
                    Q_PROPERTY(QString symbol READ symbol)
                    Q_PROPERTY(QString id READ id)
                    Q_PROPERTY(DataFlagMap flags READ flags)

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
                        bool operator==(const Data&);
                        /**
                         * @brief
                         *
                         * @fn operator =
                         * @param
                         */
                        Data& operator=(const Data&);
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
                        static const Data createData(const QString, const QString, const QString = "", const DataFlagMap = DataFlagMap());
                        /**
                         * @brief
                         *
                         * @fn idFromString
                         * @param string
                         */
                        static const string idFromString(const QString);
                        static const Data Null; /**< Represents an empty set of data. */
                };

                /**
                 * @brief
                 *
                 * @class Model models.hpp "include/wntr/data/models.hpp"
                 */
                class Model : public QObject {
                    Q_OBJECT
                    Q_PROPERTY(Data* data READ data)

                    protected:
                        Data m_dt;

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
                        Model ( Data* );
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
                        Data* data();
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
                        SaveModel ( Data* );

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
                        virtual void save(QDomElement* = NULL) = 0;
                        /**
                         * @brief
                         *
                         * @fn saveFrom
                         * @param
                         */
                        virtual void saveFrom(const Data& ) = 0;
                };

                /**
                 * @brief
                 *
                 * @class LoadModel models.hpp "include/wntr/data/models.hpp"
                 */
                class LoadModel : public Model{
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
                         * @param map
                         */
                        LoadModel ( Data* p_in );

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
                        virtual Data* load(QDomElement* = NULL) = 0;
                        /**
                         * @brief
                         *
                         * @fn loadTo
                         * @param
                         */
                        virtual bool loadTo(Data& ) = 0;
                };

                /**
                 * @brief
                 * @todo Attempt to drop the Boost dependency here and find another means of implementing inherited interfaces to this class.
                 * @class Storage models.hpp "include/wntr/data/models.hpp"
                 */
                class Storage : virtual public SaveModel, virtual public LoadModel {
                    public:
                        /**
                        * @brief Generates a Storage object.
                        * @fn obtain
                        * @param id The ID of the Storage object.
                        * @param locale The locale of the Storage object.
                        * @return Return a pointer to a formed Storage object or NULL if it cannot be found.
                        */
                        static Storage* obtain ( const Data* );

                        /**
                         * @brief Determines existence of a Storage object.
                         * @fn exists
                         * @param id The ID of the Storage object.
                         * @param locale The locale of the Storage object.
                         */
                        static const bool exists ( const Data* );

                    protected:
                        /**
                         * @brief
                         *
                         * @fn GenericStorage
                         */
                        Storage();

                        /**
                         * @brief
                         *
                         * @fn GenericStorage
                         * @param
                         */
                        Storage ( Data* );

                        /**
                         * @brief
                         *
                         * @fn GenericStorage
                         * @param
                         */
                        Storage ( const Model& );
                        /**
                         * @brief
                         *
                         * @fn Storage
                         * @param
                         */
                        Storage ( const Storage& );
                        /**
                         * @brief
                         *
                         * @fn ~GenericStorage
                         */
                        virtual ~Storage();
                };

                /**
                 * @brief
                 *
                 * @class Cache models.hpp "include/wntr/data/models.hpp"
                 */
                class Cache {

                };

                /**
                 * @brief
                 * @class XmlBackend models.hpp "include/wntr/data/models.hpp"
                 */
                class XmlBackend {
                    public:
                        /**
                         * @brief
                         *
                         * @fn XmlBackend
                         */
                        XmlBackend();
                        /**
                         * @brief
                         *
                         * @fn XmlBackend
                         * @param
                         */
                        XmlBackend(QDomElement* );
                        /**
                         * @brief
                         *
                         * @fn XmlBackend
                         * @param
                         */
                        XmlBackend(const XmlBackend&);
                        /**
                         * @brief
                         *
                         * @fn ~XmlBackend
                         */
                        virtual ~XmlBackend();

                    private:
                        QDomElement* m_ele; /**< Represents the XML data of the object. */
                };

                /**
                 * @brief
                 *
                 * @class XmlLoadModel models.hpp "include/wntr/data/models.hpp"
                 */
                class XmlLoadModel : public LoadModel, public XmlBackend {
                    public:
                        /**
                         * @brief
                         *
                         * @fn XmlLoadModel
                         */
                        XmlLoadModel();
                        /**
                         * @brief
                         *
                         * @fn XmlLoadModel
                         * @param
                         */
                        XmlLoadModel(QDomElement* );
                        /**
                         * @brief
                         *
                         * @fn XmlLoadModel
                         * @param
                         */
                        XmlLoadModel(const XmlLoadModel&);
                        /**
                         * @brief
                         *
                         * @fn ~XmlLoadModel
                         */
                        virtual ~XmlLoadModel();
                        /**
                         * @brief
                         *
                         * @fn load
                         * @param
                         */
                        virtual Data* load(QDomElement* = NULL);
                        /**
                         * @brief
                         *
                         * @fn loadTo
                         * @param
                         */
                        virtual bool loadTo(Data&);

                };

                /**
                 * @brief
                 *
                 * @class XmlSaveModel models.hpp "include/wntr/data/models.hpp"
                 */
                class XmlSaveModel : public SaveModel, public XmlBackend {
                    public:
                        /**
                         * @brief
                         *
                         * @fn XmlSaveModel
                         */
                        XmlSaveModel();
                        /**
                         * @brief
                         *
                         * @fn XmlSaveModel
                         * @param
                         */
                        XmlSaveModel(QDomElement* );
                        /**
                         * @brief
                         *
                         * @fn XmlSaveModel
                         * @param
                         */
                        XmlSaveModel(const XmlSaveModel&);
                        /**
                         * @brief
                         *
                         * @fn ~XmlSaveModel
                         */
                        virtual ~XmlSaveModel();
                        /**
                         * @brief
                         *
                         * @fn save
                         * @param
                         */
                        virtual void save(QDomElement* = NULL);
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
                 * @class XmlStorage models.hpp "include/wntr/data/models.hpp"
                 */
                class XmlStorage : public virtual XmlLoadModel, public virtual XmlSaveModel {
                    public:
                        /**
                         * @brief
                         *
                         * @fn XmlStorage
                         */
                        XmlStorage();
                        /**
                         * @brief
                         *
                         * @fn XmlStorage
                         * @param
                         */
                        XmlStorage(QDomElement* );
                        /**
                         * @brief
                         *
                         * @fn XmlStorage
                         * @param
                         */
                        XmlStorage(Data*);
                        /**
                         * @brief
                         *
                         * @fn XmlStorage
                         * @param
                         */
                        XmlStorage(const XmlStorage& );
                        /**
                         * @brief
                         *
                         * @fn XmlStorage
                         * @param
                         */
                        XmlStorage(const Model& );
                        /**
                         * @brief
                         *
                         * @fn ~XmlStorage
                         */
                        virtual ~XmlStorage();
                };
            }

            /// @todo Define these classes for the Binding, Rule, and RuleSet classes. This'll add some dynamic means of implementing a back-end.
            namespace Rules {
                struct Syntax;
                struct Model;
                struct LoadModel;
                struct SaveModel;
                struct Storage;

                /**
                 * @brief
                 *
                 * @typedef SyntaxList
                 */
                typedef QList<Syntax> SyntaxList;

                /**
                 * @brief
                 *
                 * @class Syntax models.hpp "include/wntr/data/models.hpp"
                 */
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
                        /**
                         * @brief
                         * @fn Syntax
                         */
                        Syntax();
                        /**
                         * @brief
                         * @fn Syntax
                         * @param p_syntx
                         */
                        Syntax(const Syntax&);
                        /**
                         * @brief
                         * @fn ~Syntax
                         */
                        ~Syntax();
                        /**
                         * @brief
                         *
                         * @fn operator =
                         * @param
                         */
                        Syntax& operator=(const Syntax& );
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
                    Q_PROPERTY(Syntax syntax READ syntax WRITE setSyntax)

                    protected:
                        Syntax m_syntx; /**< Represents the information used to form a binding. */
                        /**
                         * @brief
                         *
                         * @fn Model
                         * @param
                         */
                        Model(const Syntax& );

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
                        const Syntax syntax() const;
                        /**
                         * @brief
                         *
                         * @fn setSyntax
                         * @param
                         */
                        void setSyntax(Syntax& );
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

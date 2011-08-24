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

#include "md5.hpp"
#include "models.hpp"
#include <QDir>
#include <QFile>
#include <QtDebug>
#include <QMetaType>
#include <boost/progress.hpp>

namespace Wintermute {
    namespace Data {
        namespace Linguistics {
            namespace Lexical {
                Cache::StorageList Cache::s_stores;
                const Data Data::Null = Data();

                Data::Data() : m_id(), m_lcl(), m_sym(), m_flg() { }

                Data::Data(const Data &p_dt) : m_id(p_dt.m_id),
                    m_lcl(p_dt.m_lcl), m_sym(p_dt.m_sym), m_flg(p_dt.m_flg) { }

                Data::Data(QString p_id, QString p_lcl, QString p_sym, DataFlagMap p_flg) : m_id(p_id),
                    m_lcl(p_lcl), m_sym(p_sym), m_flg(p_flg) { }

                const DataFlagMap Data::flags () const { return m_flg; }

                const QString Data::symbol () const { return m_sym; }

                const QString Data::id () const { return m_id; }

                const QString Data::locale () const { return m_lcl; }

                void Data::setSymbol(const QString& l_sym) { m_sym = l_sym; m_id = Data::idFromString (m_sym); }

                void Data::setFlags(const DataFlagMap& l_flg) {
                    m_flg = l_flg;
                    Q_ASSERT(m_flg == l_flg);
                }

                bool Data::operator== (const Data& p_otherDt) const {
                    return ((m_flg == p_otherDt.m_flg) && (m_sym == p_otherDt.m_sym) &&
                           (m_lcl == p_otherDt.m_lcl) && (m_id == p_otherDt.m_id));
                }

                void Data::operator= (const Data& p_otherDt) {
                    m_flg = p_otherDt.m_flg;
                    m_sym = p_otherDt.m_sym;
                    m_lcl = p_otherDt.m_lcl;
                    m_id = p_otherDt.m_id;
                }

                const bool Data::isEmpty () const { return Data::Null == *this; }

                Data Data::createData (const QString p_id, const QString p_lcl, const QString p_sym, const DataFlagMap p_flg) {
                    return Data(p_id,p_lcl,p_sym,p_flg);
                }

                const QString Data::idFromString (const QString p_sym) { return QString::fromStdString (md5(p_sym.toLower ().toStdString ())); }

                Data::~Data () { }

                Model::Model() { }

                Model::Model( Data& p_dt ) : m_dt(p_dt) { }

                Model::Model( const Model& p_mdl ) : m_dt(p_mdl.m_dt) { }

                const Data& Model::data() { return m_dt; }

                void Model::setData( const Data& p_dt ) { m_dt = p_dt; }

                Model::~Model() { }

                LoadModel::LoadModel() { }

                LoadModel::LoadModel( const LoadModel& p_mdl ) : Model(p_mdl) { }

                LoadModel::LoadModel( const Model& p_mdl ) : Model(p_mdl) { }

                LoadModel::~LoadModel() { }

                SaveModel::SaveModel() { }

                SaveModel::SaveModel( Data& p_dt ) : Model(p_dt) { }

                SaveModel::SaveModel( const SaveModel& p_mdl ) : Model(p_mdl) { }

                SaveModel::SaveModel( const Model& p_mdl ) : Model(p_mdl) { }

                SaveModel::~SaveModel() { }

                Storage::Storage() { }

                bool Storage::operator==(const Storage& p_store) const { return type() == p_store.type(); }

                DomBackend::DomBackend(QDomElement* p_ele) : m_ele(p_ele) { }

                DomStorage::DomStorage() { }

                const QString DomStorage::getPath(const Data& p_dt){
                    return QString::fromStdString (Configuration::directory ()) + QString("/")
                           + p_dt.locale() + QString("/node/") + p_dt.id () + QString(".node");
                }

                const bool DomStorage::exists (const Data &p_dt) const {
                    const QString l_pth = DomStorage::getPath (p_dt);
                    return QFile::exists (l_pth);
                }

                void DomStorage::loadTo (Data &p_dt) const {
                    if (!DomStorage::exists (p_dt))
                        return;

                    QDomDocument l_dom("Data");
                    QFile* l_file = new QFile(getPath (p_dt));
                    l_dom.setContent (l_file);

                    QDomElement l_elem = l_dom.documentElement ();
                    DomLoadModel l_model(&l_elem);
                    l_model.loadTo (p_dt);
                }

                /// @todo Should it create the locale file if it's not defined?
                void DomStorage::saveFrom (const Data &p_dt){
                    if (!DomStorage::exists (p_dt))
                        return;

                    QDomDocument l_dom("Data");
                    QFile* l_file = new QFile(getPath (p_dt));

                    if (!l_file->exists ()){
                        /// @todo Create the file and just leave an empty base node.
                    }

                    l_dom.setContent (l_file);
                    QDomElement l_elem = l_dom.documentElement ();
                    DomSaveModel l_domSvMdl(&l_elem);
                    l_domSvMdl.saveFrom (p_dt);
                }

                const QString DomStorage::type () const { return "Dom"; }

                void DomStorage::generate() {
                    QDir d(QString::fromStdString (Configuration::directory ()));
                    d.setFilter (QDir::Dirs | QDir::NoDotAndDotDot);
                    QStringList l_lst = d.entryList ();

                    foreach(QString l_str, l_lst){
                        const QString l_pth = d.absolutePath () + "/" + l_str + "/node.xml";
                        QDomDocument l_spawnDom;
                        QFile* l_file;

                        if (!QFile::exists (l_pth)){
                            qWarning() << "(data) [DomStorage] Can't find" << l_pth << ".";
                            continue;
                        }

                        l_file = new QFile(l_pth);
                        if (!l_spawnDom.setContent (l_file)){
                            qWarning() << "(data) [DomStorage] Parse error in" << l_pth << ".";
                            continue;
                        }

                        spawn(l_spawnDom);
                    }
                }

                void DomStorage::spawn(const QDomDocument& p_dom){
                    const QDomElement l_root = p_dom.documentElement ();
                    const QString l_lcl = l_root.attribute ("locale");
                    const QDomNodeList l_lst = l_root.elementsByTagName ("Data");
                    boost::progress_display l_prgs(l_lst.count ());
                    DomStorage l_str;

                    for (int i = 0; i < l_lst.count (); i++){
                        QDomElement l_ele = l_lst.at (i).toElement ();

                        if (l_ele.isNull ()){
                            qWarning() << "null" << i;
                            continue;
                        }

                        Data l_dt;

                        DomLoadModel l_ldM(&l_ele);
                        QDomDocument l_newDom("Data");
                        const Data* l_bsDt = l_ldM.load();
                        l_dt = Data::createData (l_bsDt->id (),l_lcl,l_bsDt->symbol (),l_bsDt->flags ());

                        QDomElement l_root = l_newDom.createElement ("Data");
                        l_root.setAttribute ("locale",l_lcl);
                        DomSaveModel l_svM(&l_root);
                        l_svM.saveFrom (l_dt);
                        l_newDom.appendChild (l_root);

                        if (!l_dt.flags ().isEmpty ()){
                            QFile* l_file = new QFile(getPath(l_dt));
                            if (!l_file->open (QIODevice::ReadWrite | QIODevice::Truncate)){
                                //qWarning() << "(data) [DomStorage] Generation failed for" << l_dt.symbol () << ":" << l_file->errorString ();
                                continue;
                            } else {
                                l_file->write (l_newDom.toByteArray (4));
                                l_file->close();
                            }
                        }
                        ++l_prgs;
                    }
                    cout << endl << endl;
                }

                DomStorage::~DomStorage() { }

                const bool Cache::read (Data &p_dt) {
                    foreach (Storage* l_str, Cache::s_stores){
                        if (l_str->exists (p_dt)){
                            l_str->loadTo (p_dt);
                            return true;
                        } else continue;
                    }

                    return false;
                }

                DomLoadModel::DomLoadModel(QDomElement* p_ele) : DomBackend(p_ele) { }

                DomLoadModel::~DomLoadModel() { }

                const Data* DomLoadModel::load () const {
                    if (loadTo(this->Model::m_dt))
                        return &this->Model::m_dt;
                    else {
                        //qWarning() << "(data) [DomLoadModel] Failed to load" << m_dt.id () << m_dt.symbol ();
                        return NULL;
                    }
                }

                bool DomLoadModel::loadTo (Data &p_dt) const {
                    if (this->DomBackend::m_ele == NULL || this->DomBackend::m_ele->isNull ())
                        return false;

                    DataFlagMap l_mp;
                    QDomNodeList l_ndlst = this->DomBackend::m_ele->elementsByTagName ("Flag");

                    for (int i = 0; i < l_ndlst.count (); i++){
                        QDomElement l_elem = l_ndlst.at (i).toElement ();
                        l_mp.insert ((new QString (l_elem.attribute ("guid"))), (new QString (l_elem.attribute ("link"))));
                    }

                    if (l_mp.isEmpty () || l_ndlst.isEmpty ()){
                        //qWarning() << "(data) [DomLoadModel] No flags loaded for" << p_dt.symbol ();
                    }

                    p_dt.setSymbol(this->DomBackend::m_ele->attribute ("symbol").toLower());
                    p_dt.setFlags(l_mp);
                    return true;
                }

                DomSaveModel::DomSaveModel(QDomElement* p_ele) : DomBackend(p_ele) { }

                DomSaveModel::~DomSaveModel() { }

                void DomSaveModel::save()  {
                    if (this->DomBackend::m_ele->isNull()) return;

                    this->DomBackend::m_ele->setAttribute("symbol" , this->Model::m_dt.symbol().toLower ());

                    while (this->DomBackend::m_ele->hasChildNodes ())
                        this->DomBackend::m_ele->removeChild (this->DomBackend::m_ele->firstChild ());

                    QDomDocument l_dom = this->DomBackend::m_ele->ownerDocument ();

                    for(DataFlagMap::ConstIterator itr = this->Model::m_dt.flags ().begin ();
                        itr != this->Model::m_dt.flags ().end (); itr++) {
                        QDomElement l_ele = l_dom.createElement ("Flag");
                        l_ele.setAttribute ("guid",*itr.key ());
                        l_ele.setAttribute ("link",*itr.value ());
                        this->DomBackend::m_ele->appendChild (l_ele);
                    }

                    //qDebug() << "(data) [DomSaveModel] Saved" << this->Model::m_dt.id ();
                }

                void DomSaveModel::saveFrom(const Data& p_dt) {
                    if (!(p_dt == Data::Null))
                        this->Model::m_dt = p_dt;

                    save();
                }

                void Cache::write (const Data &p_dt){
                    Storage* l_fdStr;
                    foreach (Storage* l_str, Cache::s_stores){
                        if (l_str->exists (p_dt))
                            l_fdStr = l_str;
                        else continue;
                    }

                    if (l_fdStr)
                        l_fdStr->saveFrom (p_dt);
                    else {
                        // save this locally. We consider the DOM storage to be local.
                        l_fdStr = new DomStorage;
                        l_fdStr->saveFrom (p_dt);
                    }
                }

                const bool Cache::exists(const Data& p_dt) {
                    foreach (Storage* l_str, Cache::s_stores){
                        if (l_str->exists (p_dt))
                            return true;
                    }

                    return false;
                }

                void Cache::pseudo (Data &p_psDt){
                    DataFlagMap l_mp;
                    l_mp.insert ((new QString("-1")),(new QString("`")));
                    p_psDt.setFlags (l_mp);
                }

                const bool Cache::isPseudo(const Data& p_dt){
                    DataFlagMap::ConstIterator itr = p_dt.flags ().begin ();
                    return (p_dt.flags ().count () == 1 && (itr.key ()->toStdString() == "-1" && itr.value ()->toStdString() == "`"));
                }

                void Cache::addStorage(Storage* l_str){
                    s_stores.push_back(l_str);
                }

                void Cache::generate() {
                    qDebug() << "(data) [Cache] Dumping..";

                    foreach (Storage* l_str, Cache::s_stores){
                        qDebug() << "(data) [Cache] Dumping" << l_str->type ();
                        l_str->generate();
                    }

                    qDebug() << "(data) [Cache] Dumped.";
                }

            } /** end namespace Lexical */

            namespace Rules {
                BondData::BondData() { }

                BondData::BondData(const BondData &p_synx) : m_lcl(p_synx.m_lcl),
                    m_lnkTxt(p_synx.m_lnkTxt), m_rlTxt(p_synx.m_rlTxt) { }

                void BondData::setLinkText (const string p_lnkTxt) { m_lnkTxt = p_lnkTxt; }

                void BondData::setLocale (const string p_lcl) { m_lcl = p_lcl; }

                void BondData::setRuleText (const string p_rlTxt) { m_rlTxt = p_rlTxt; }

                const string BondData::locale () const { return m_lcl; }

                const string BondData::ruleText () const { return m_rlTxt; }

                const string BondData::linkText () const { return m_lnkTxt; }

                BondData& BondData::operator = (const BondData& p_synx) {
                    return const_cast<BondData&>(p_synx);
                }

                BondData::~BondData () { }

                Model::Model() { }

                Model::Model(const Model& p_mdl) : m_syntx(p_mdl.m_syntx) { }

                void Model::setSyntax (BondData &p_synx) { m_syntx = p_synx; }

                const BondData Model::syntax () const { return m_syntx; }

                Model::~Model() { }
            }
        } /** end namespace Rules */
    }
}

Q_DECLARE_METATYPE(Wintermute::Data::Linguistics::Lexical::Model)
Q_DECLARE_METATYPE(Wintermute::Data::Linguistics::Lexical::Data)
Q_DECLARE_METATYPE(Wintermute::Data::Linguistics::Rules::Model)
Q_DECLARE_METATYPE(Wintermute::Data::Linguistics::Rules::BondData)
// kate: indent-mode cstyle; space-indent on; indent-width 4;

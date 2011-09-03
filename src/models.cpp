/**
 * @file models.hpp
Wintermute Developers <wintermute-devel@lists.launchpad.net>
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
#include <algorithm>
#include <boost/progress.hpp>

using std::unique;

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

                void DomStorage::saveFrom (const Data &p_dt){
                    QDomDocument l_dom("Data");
                    l_dom.appendChild (l_dom.createElement ("Data"));
                    QFile* l_file = new QFile(getPath (p_dt));

                    if (!l_file->exists ()){
                        l_file->open (QIODevice::WriteOnly | QIODevice::Truncate);
                        l_file->write ("<!-- Generated -->");
                        l_file->close ();
                    }

                    QDomElement l_elem = l_dom.documentElement ();
                    DomSaveModel l_domSvMdl(&l_elem);
                    l_domSvMdl.saveFrom (p_dt);
                    l_file->open (QIODevice::WriteOnly | QIODevice::Truncate);
                    l_file->write (l_dom.toByteArray (4));
                    l_file->close ();
                }

                const QString DomStorage::type () const { return "Dom"; }

                void DomStorage::generate() {
                    QDir d(QString::fromStdString (Configuration::directory ()));
                    d.setFilter (QDir::Dirs | QDir::NoDotAndDotDot);
                    QStringList l_lst = d.entryList ();

                    foreach(QString l_str, l_lst){
                        const QString l_pth = d.absolutePath () + "/" + l_str + "/node.xml";
                        QDomDocument l_spawnDom("Store");
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

                QDomDocument* DomStorage::getSpawnDoc(const Data& p_dt) const {
                    QDir d(QString::fromStdString (Configuration::directory ()));
                    const QString l_pth = d.absolutePath () + "/" + p_dt.locale () + "/node.xml";
                    QDomDocument* l_spawnDom = new QDomDocument("Storage");
                    QFile* l_file = new QFile(l_pth);

                    if (!l_file->exists ()){
                        qWarning() << "(data) [DomStorage] Can't find node cache for" << p_dt.locale ();
                        return NULL;
                    }

                    if (!l_spawnDom->setContent (l_file)){
                        qWarning() << "(data) [DomStorage] Parse error for" << p_dt.locale ();
                        return NULL;
                    }

                    return l_spawnDom;
                }

                const bool DomStorage::hasPseudo(const Data& p_dt) const {
                    QDomDocument* l_dom = getSpawnDoc(p_dt);
                    QDomElement l_root = l_dom->documentElement ();
                    QDomNodeList l_lst = l_root.elementsByTagName ("Pseudo");
                    return !l_lst.isEmpty ();
                }

                void DomStorage::loadPseudo(Data& p_dt) const {
                    if (!hasPseudo(p_dt))
                        return;

                    QDomDocument* l_dom = getSpawnDoc(p_dt);
                    QDomElement l_root = l_dom->documentElement (), l_psElem;
                    QDomNodeList l_lst = l_root.elementsByTagName ("Pseudo");
                    l_psElem = l_lst.at (0).toElement ();

                    DomLoadModel l_ldMdl(&l_psElem);
                    const QString l_sym = p_dt.symbol ();
                    l_ldMdl.loadTo (p_dt);
                    p_dt.setSymbol (l_sym); // The loading process probably overwrote the symbol.
                }

                DomStorage::~DomStorage() { }

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
                    if (this->DomBackend::m_ele == NULL || this->DomBackend::m_ele->isNull ()){
                        qWarning() << "(data) [DomLoadModel] Backend's empty.";
                        return false;
                    }

                    DataFlagMap l_mp;
                    QDomNodeList l_ndlst = this->DomBackend::m_ele->elementsByTagName ("Flag");

                    for (int i = 0; i < l_ndlst.count (); i++){
                        QDomElement l_elem = l_ndlst.at (i).toElement ();
                        l_mp.insert (l_elem.attribute ("guid"),l_elem.attribute ("link"));
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

                    this->DomBackend::m_ele->setAttribute ("symbol" , this->Model::m_dt.symbol().toLower ());
                    this->DomBackend::m_ele->setAttribute ("locale" , this->Model::m_dt.locale ());

                    while (this->DomBackend::m_ele->hasChildNodes ())
                        this->DomBackend::m_ele->removeChild (this->DomBackend::m_ele->firstChild ());

                    QDomDocument l_dom = this->DomBackend::m_ele->ownerDocument ();

                    for(DataFlagMap::ConstIterator itr = this->Model::m_dt.flags ().begin ();
                        itr != this->Model::m_dt.flags ().end (); itr++) {
                        QDomElement l_ele = l_dom.createElement ("Flag");
                        l_ele.setAttribute ("guid",itr.key ());
                        l_ele.setAttribute ("link",itr.value ());
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
                    Storage* l_fdStr = NULL;
                    foreach (Storage* l_str, Cache::s_stores){
                        if (l_str->exists (p_dt))
                            l_fdStr = l_str;
                        else continue;
                    }

                    if (l_fdStr)
                        l_fdStr->saveFrom (p_dt);
                    else {
                        // save this locally. We consider the DOM storage to be local.
                        DomStorage *l_domStr = new DomStorage;
                        l_domStr->saveFrom (p_dt);
                    }
                }

                const bool Cache::exists(const Data& p_dt) {
                    foreach (Storage* l_str, Cache::s_stores){
                        if (l_str->exists (p_dt))
                            return true;
                    }

                    return false;
                }

                const bool Cache::read (Data &p_dt) {
                    foreach (Storage* l_str, Cache::s_stores){
                        if (l_str->exists (p_dt)){
                            l_str->loadTo (p_dt);
                            return true;
                        } else continue;
                    }

                    return false;
                }

                void Cache::pseudo (Data &p_psDt){
                    foreach (Storage* l_str, Cache::s_stores){
                        if (l_str->hasPseudo (p_psDt)){
                            l_str->loadPseudo(p_psDt);
                            return;
                        } else continue;
                    }
                }

                const bool Cache::isPseudo(const Data& p_dt){
                    foreach (Storage* l_str, Cache::s_stores){
                        if (l_str->hasPseudo (p_dt)){
                            Data l_dt;
                            l_str->loadPseudo(l_dt);
                            return (l_dt.id () == p_dt.id () && l_dt.flags () == p_dt.flags ());
                        } else continue;
                    }

                    return false;
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
                Cache::StorageList Cache::s_stores;

                Bond::Bond() { }

                Bond::Bond(const Bond &p_bnd) : m_props(p_bnd.m_props) { }

				void Bond::setWith(QString& p_value) {
			const QString l_with = "with";
					setAttribute(l_with,p_value);
				}

                void Bond::setAttribute(const QString& p_attr, QString& p_val) {
                    if (m_props.find (p_attr) != m_props.end ())
                        m_props.at (p_attr) = p_val;
                    else
                        m_props.insert (StringMap::value_type(p_attr,p_val));
                }

                const QString Bond::attribute(const QString& p_attr) const {
                    if (hasAttribute(p_attr))
                        return m_props.at (p_attr);
                    else
                        return QString::null;
                }

                void Bond::setAttributes(const StringMap& p_props) {
                    m_props = p_props;
                }

                const bool Bond::hasAttribute(const QString& p_attr) const {
                    return !(m_props.find (p_attr) == m_props.end ());
                }

                const QString Bond::with() const {
                    return m_props.at ("with");
                }

                const StringMap Bond::attributes() const {
                    return m_props;
                }

                /// @todo This might be the crowning jewel of the linking system.
                const double Bond::matches(const QString& p_query, const QString& p_regex){
                    const QStringList l_regexList = p_regex.split (",");
                    QList<double> l_rslts;

                    foreach(const QString l_regex, l_regexList){
                        const double l_max = (double) l_regex.length ();
                        double l_cnt = 0.0;

                        if (p_query.at (0) == l_regex.at (0)){
                            l_cnt += 1.0;
                            for (int i = 1; i < p_query.length (); i++){
                                QChar l_chr = p_query.at (i);
                                if (p_regex.contains (l_chr,Qt::CaseSensitive))
                                    l_cnt += 1.0;
                            }
                        }
\
                        l_rslts.push_back((l_cnt / l_max));
                    }

                    qSort(l_rslts.begin (),l_rslts.end ());

                    if (!l_rslts.empty ())
                        return l_rslts.back ();
                    else
                        return 0.0;
                }

                void Bond::operator=(const Bond& p_bnd) { m_props = p_bnd.m_props; }

                Bond::~Bond () { }

                Chain::Chain() { }

                Chain::Chain(const QString& p_lcl, const QString& p_typ, const BondList &p_vtr) : m_bndVtr(p_vtr),
                    m_typ(p_typ), m_lcl(p_lcl) { }

                Chain::Chain(const Chain &p_chn) : m_bndVtr(p_chn.m_bndVtr), m_lcl(p_chn.m_lcl), m_typ(p_chn.m_typ) { }

                void Chain::operator= (const Chain& p_chn) {
                    m_bndVtr = p_chn.m_bndVtr;
                    m_typ = p_chn.m_typ;
                    m_lcl = p_chn.m_lcl;
                }

                const BondList Chain::bonds () const { return m_bndVtr; }

                const QString Chain::type () const { return m_typ; }

                const QString Chain::locale() const { return m_lcl; }

                void Chain::setType (const QString &p_typ) {
                    m_typ = p_typ;
                }

                void Chain::setBonds (const BondList &p_bnd) { m_bndVtr = p_bnd; }

                Chain::~Chain () { }

                Model::Model() { }

                Model::Model(const Model &p_mod) : m_chn(p_mod.m_chn) { }

                LoadModel::LoadModel() { }

                LoadModel::LoadModel(const Model &p_mdl) : Model(p_mdl) { }

                LoadModel::~LoadModel() { }

                SaveModel::SaveModel() { }

                SaveModel::SaveModel(const Model &p_mdl) : Model(p_mdl) { }

                SaveModel::~SaveModel() { }

                Storage::Storage() { }

                Storage::Storage(const QString& p_lcl, const QString& p_flg) : m_flg(p_flg), m_lcl(p_lcl) { }

                Storage::Storage(const Storage &p_str) : m_flg(p_str.m_flg), m_lcl(p_str.m_lcl) { }

                bool Storage::operator == (const Storage& p_str) { return type() == this->type (); }

                const QString Storage::flag () const { return m_flg; }

                const QString Storage::locale() const { return m_lcl; }

                Storage::~Storage () { }

                DomBackend::DomBackend() { }

                DomBackend::DomBackend(const DomBackend &p_bnd) : m_elem(p_bnd.m_elem) { }

                DomBackend::DomBackend(QDomElement *p_ele) : m_elem(p_ele) { }

                DomBackend::~DomBackend () { }

                DomLoadModel::DomLoadModel() { }

                DomLoadModel::DomLoadModel(const DomLoadModel &p_mdl) : LoadModel(p_mdl), DomBackend(p_mdl.m_elem) { }

                DomLoadModel::DomLoadModel(QDomElement *p_ele) : DomBackend(p_ele) { }

                const Chain* DomLoadModel::load () const {
                    Chain* l_chn = NULL;
                    return l_chn;
                }

                void DomLoadModel::obtainBonds(BondList& p_bndVtr, QDomElement p_elem) const {
                    if (p_elem.nodeName () != "Rule")
                        return;

                    QDomNodeList l_lst = p_elem.elementsByTagName ("Bind");

                    for (int i = 0; i < l_lst.count (); i++){
                        QDomElement l_elem = l_lst.at (i).toElement ();
                        Bond* l_bnd = new Bond;
                        QDomNamedNodeMap l_attrs = l_elem.attributes ();

                        for (int i = 0; i < l_attrs.length (); i++){
                            QDomAttr l_attr = l_attrs.item (i).toAttr ();
                            const QString l_nm = l_attr.name ();
                            QString l_vl = l_attr.value ();
                            l_bnd->setAttribute (l_nm,l_vl);
                        }

                        p_bndVtr.push_back (l_bnd);
                    }

                    if (!p_elem.parentNode ().isNull ())
                        obtainBonds(p_bndVtr,p_elem.parentNode ().toElement ());
                }

                const QString DomLoadModel::obtainType(QDomElement p_elem) const {
                    QString l_type;
                    while (!p_elem.isNull()){
                        l_type.prepend(p_elem.attribute("type"));
                        p_elem = p_elem.parentNode().toElement ();
                    }

                    return l_type;
                }

                /// @todo This needs to make a way to inherit rules.
                void DomLoadModel::loadTo (Chain &p_chn) const {
                    BondList l_bndVtr;
                    obtainBonds(l_bndVtr,*m_elem);

                    unique(l_bndVtr.begin (),l_bndVtr.end ());

                    p_chn.setType (obtainType(*m_elem));
                    p_chn.setBonds (l_bndVtr);
                }

                DomLoadModel::~DomLoadModel () { }

                DomSaveModel::DomSaveModel() { }

                DomSaveModel::DomSaveModel(QDomElement *p_ele) : DomBackend(p_ele) { }

                DomSaveModel::DomSaveModel(const DomSaveModel &p_mdl) : SaveModel(p_mdl), DomBackend(p_mdl.m_elem) { }

                void DomSaveModel::save () { }

                void DomSaveModel::saveFrom (const Chain &p_chn) {

                }

                DomSaveModel::~DomSaveModel () { }

                DomStorage::DomStorage() : m_min(DOMSTORAGE_MAXSTR) { }

                DomStorage::DomStorage(const Storage &p_str) : Storage(p_str) { }

                const QString DomStorage::getPath(const Chain& p_chn){
                    return QString::fromStdString (Configuration::directory ()) + "/" + p_chn.locale () + "/grammar.xml";
                }

                QDomDocument* DomStorage::loadDom(const Chain& p_chn) {
                    DomStorage l_str;
                    if (!l_str.exists(p_chn.locale (),p_chn.type ())){
                        qWarning() << "(data) [DomStorage] Can't find grammar for" << p_chn.locale ();
                        return NULL;
                    }

                    const QString l_pth = getPath(p_chn);
                    QDomDocument *l_dom = new QDomDocument;
                    {
                        QString l_errorString;
                        int l_errorLine, l_errorColumn;
                        if (!l_dom->setContent ((new QFile(l_pth)),&l_errorString,&l_errorLine,&l_errorColumn)){
                            qWarning() << "(data) [DataStorage] Error loading grammar:" << l_errorLine << "at l." << l_errorLine << ", col." << l_errorColumn;
                            return NULL;
                        }
                    }

                    return l_dom;
                }

                const bool DomStorage::exists (const QString p_lcl, const QString p_flg) const {
                    Chain l_chn(p_lcl,p_flg);
                    const QString l_pth = getPath (l_chn);
                    return QFile::exists (l_pth);
                }

                /// @todo We need to figure out a more approriate minimum value.
                QDomElement DomStorage::findElement (const Chain& p_chn, QDomElement p_elem) const{
                    QDomElement l_elem = findElement(p_chn,p_elem,"");
                    const double l_minimum = (100 / (double) p_chn.type ().length ()) / 100;

                    while (l_elem.isNull ()){
                        m_min -= 0.01; // Decrease it by 1%.
                        l_elem = findElement(p_chn,p_elem,"");

                        if (!l_elem.isNull ())
                            break;

                        if (m_min == l_minimum)
                            break;
                    }

                    qDebug() << "(data) [DomStorage] Minimum matching:" << l_minimum * 100 << "%";
                    m_min = DOMSTORAGE_MAXSTR;
                    return l_elem;
                }

                /// @todo Have this method round the results of matching up to the nearest whole number. (0.9 == 1, 1.1 == 2, etc). I can't remember if it's ceil or something else.
                /// @todo Have this method use a cache to save at least 30 previous rules, to speed the searching process.`
                QDomElement DomStorage::findElement(const Chain& p_chn, QDomElement p_elem, QString p_prefix) const {
                    if (p_elem.hasAttribute ("type") && p_elem != p_elem.ownerDocument ().documentElement ()){
                        const QString l_data = p_elem.attribute ("type");
                        const QStringList l_lst = l_data.split (",");

                        foreach (const QString l_part, l_lst){
                            p_prefix.append (l_part);
                            const int l_match = (int) (Bond::matches (p_chn.type (),p_prefix) * 100);
                            if (l_match >= (int) (m_min * 100))
                                return p_elem;
                        }
                    }

                    QDomNodeList l_lst = p_elem.elementsByTagName ("Rule");

                    for (int i = 0; i < l_lst.length (); i++){
                        QDomElement l_elem = l_lst.at (i).toElement (), l_rtn;
                        l_rtn = findElement(p_chn,l_elem,p_prefix);
                        if (!l_rtn.isNull ())
                            return l_rtn;
                        else continue;
                    }

                    return *(new QDomElement);
                }

                void DomStorage::loadTo (Chain &p_chn) const {
                    QDomDocument* l_dom = loadDom (p_chn);
                    QDomElement l_elem = findElement(p_chn,l_dom->documentElement());
                    if (l_elem.isNull()){
                        qWarning() << "(data) [DataStorage] No rule can satisfy.";
                        return;
                    }
                    DomLoadModel l_ldMdl(&l_elem);
                    l_ldMdl.loadTo(p_chn);
                }

                void DomStorage::saveFrom(const Chain& p_chn) {
                    QDomDocument* l_dom = loadDom (p_chn);
                    QDomElement l_elem = findElement(p_chn,l_dom->documentElement());
                    if (l_elem.isNull()){
                        qWarning() << "(data) [DataStorage] No rule can satisfy.";
                        return;
                    }
                    DomSaveModel l_svMdl(&l_elem);
                    l_svMdl.saveFrom (p_chn);
                }

                const QString DomStorage::type () const { return "Dom"; }

                DomStorage::~DomStorage() { }

                void Model::setChain (const Chain &p_chn) { m_chn = p_chn; }

                const Chain Model::chain () const { return m_chn; }

                Model::~Model () { }

                const bool Cache::read (Chain &p_chn) {
                    foreach (Storage* l_str, Cache::s_stores){
                        if (l_str->exists (p_chn.locale (),p_chn.type ())){
                            l_str->loadTo (p_chn);
                            return true;
                        } else continue;
                    }

                    return false;
                }

                void Cache::write (const Chain& p_chn) {
                    Storage* l_fdStr;
                    foreach (Storage* l_str, Cache::s_stores){
                        if (l_str->exists (p_chn.locale (),p_chn.type ()))
                            l_fdStr = l_str;
                        else continue;
                    }

                    if (l_fdStr)
                        l_fdStr->saveFrom (p_chn);
                    else {
                        // save this locally. We consider the DOM storage to be local.
                        l_fdStr = new DomStorage;
                        l_fdStr->saveFrom (p_chn);
                    }

                }

                const bool Cache::exists (const QString& p_lcl, const QString& p_flg) {
                    foreach (Storage* l_str, Cache::s_stores){
                        if (l_str->exists (p_lcl,p_flg))
                            return true;
                    }

                    return false;
                }

                void Cache::addStorage (Storage *p_str){
                    s_stores.push_back (p_str);
                }
            }
        } /** end namespace Rules */
    }
}

Q_DECLARE_METATYPE(Wintermute::Data::Linguistics::Lexical::Model)
Q_DECLARE_METATYPE(Wintermute::Data::Linguistics::Lexical::Data)
Q_DECLARE_METATYPE(Wintermute::Data::Linguistics::Rules::Model)
Q_DECLARE_METATYPE(Wintermute::Data::Linguistics::Rules::Bond)
// kate: indent-mode cstyle; space-indent on; indent-width 4;

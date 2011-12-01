/**
 * @file lexical.cpp
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

#include <QDir>
#include <QFile>
#include <qjson/parser.h>
#include <qjson/serializer.h>
#include <qjson/qobjecthelper.h>

#include "md5.hpp"
#include "lexical.hpp"

namespace Wintermute {
    namespace Data {
        namespace Linguistics {
            namespace Lexical {
                Cache::StorageList Cache::s_stores;
                const Data Data::Null = Data();

                Data::Data() : m_id(), m_lcl(), m_sym(), m_flg() { }

                Data::Data(const Data &p_dt) : m_id(p_dt.m_id),
                    m_lcl(p_dt.m_lcl), m_sym(p_dt.m_sym), m_flg(p_dt.m_flg) { }

                Data::Data(QString p_id, QString p_lcl, QString p_sym, QVariantMap p_flg) : m_id(p_id),
                    m_lcl(p_lcl), m_sym(p_sym), m_flg(p_flg) { }

                const QVariantMap Data::flags () const { return m_flg; }

                const QString Data::symbol () const { return m_sym; }

                const QString Data::id () const { return m_id; }

                const QString Data::locale () const { return m_lcl; }

                void Data::setSymbol(const QString& l_sym) { m_sym = l_sym; m_id = Data::idFromString (m_sym); }

                void Data::setLocale(const QString& l_lcl) { m_lcl = l_lcl; }

                void Data::setID (const QString &l_id) { m_id = l_id; }

                void Data::setFlags(const QVariantMap& l_flg) {
                    m_flg = l_flg;
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

                const bool Data::isNull () const { return Data::Null == *this; }

                /// @todo Use MD-5 hashing from another library (QCA has it) so we can eliminate md5.*pp.
                const QString Data::idFromString (const QString p_sym) { return QString::fromStdString (md5(p_sym.toLower ().toStdString ())); }

                QString Data::toString() const {
                    QJson::Serializer* l_serializer = new QJson::Serializer;
                    QVariantMap l_map = QJson::QObjectHelper::qobject2qvariant(this);
                    return QString(l_serializer->serialize(l_map));
                }

                Data Data::fromString(const QString &p_str) {
                    Data l_dt;
                    QJson::Parser* l_parser = new QJson::Parser;
                    QVariantMap l_map = l_parser->parse(p_str.toAscii()).toMap();
                    QJson::QObjectHelper::qvariant2qobject(l_map,&l_dt);
                    return l_dt;
                }

                Data::operator QVariant() const {
                    return QVariant::fromValue(toString());
                }

                Data::~Data () { }

                QDebug operator<<(QDebug dbg, const Data& p_nd) {
                    dbg << "(Lexical::Data) ID:" << p_nd.m_id << ", Locale:" << p_nd.m_lcl
                        << ", Symbol:"<< p_nd.m_sym << ", Flags:" << p_nd.m_flg;
                    return dbg.nospace();
                }

                const QDBusArgument& operator>> (const QDBusArgument &p_arg, Data& p_dt) {
                    p_arg.beginStructure();
                    p_arg >> p_dt.m_lcl >> p_dt.m_id >> p_dt.m_sym;

                    p_arg.beginMap();

                    while (!p_arg.atEnd()){
                        QVariantMap::key_type l_key;
                        QVariantMap::mapped_type l_value;
                        p_arg.beginMapEntry();
                        p_arg >> l_key >> l_value;
                        p_arg.endMapEntry();
                        p_dt.m_flg.insert(l_key,l_value);
                    }

                    p_arg.endMap();

                    p_arg.endStructure();
                    return p_arg;
                }

                QDBusArgument& operator<< (QDBusArgument &p_arg, const Data& p_dt) {
                    p_arg.beginStructure();
                    p_arg << p_dt.m_lcl << p_dt.m_id << p_dt.m_sym;

                    p_arg.beginMap(QVariant::String, QVariant::String);

                    QVariantMap::ConstIterator l_itr = p_dt.m_flg.constBegin(), l_end = p_dt.m_flg.constEnd();
                    for (; l_itr != l_end; ++l_itr){
                        p_arg.beginMapEntry();
                        p_arg << l_itr.key()
                              << l_itr.value().toString();
                        p_arg.endMapEntry();
                    }

                    p_arg.endMap();

                    p_arg.endStructure();
                    return p_arg;
                }

                QDataStream& operator<<(QDataStream& p_strm, const Data& p_dt){
                    qDebug() << p_dt.m_id << p_dt.m_lcl << p_dt.m_sym << p_dt.m_flg;
                    p_strm << p_dt.m_id << p_dt.m_lcl << p_dt.m_sym << p_dt.m_flg;
                    return p_strm;
                }

                QDataStream& operator>>(QDataStream& p_strm, Data& p_dt){
                    qDebug() << p_dt.m_id << p_dt.m_lcl << p_dt.m_sym << p_dt.m_flg;
                    p_strm >> p_dt.m_id >> p_dt.m_lcl >> p_dt.m_sym >> p_dt.m_flg;
                    return p_strm;
                }

                Model::Model() { }

                Model::Model( Data& p_dt ) : m_dt(p_dt) { }

                Model::Model( const Model& p_mdl ) : m_dt(p_mdl.m_dt) { }

                const Data& Model::data() { return m_dt; }

                void Model::setData( const Data& p_dt ) { m_dt = p_dt; }

                Model::~Model() { }

                LoadModel::LoadModel() : Model() { }

                LoadModel::LoadModel( const LoadModel& p_mdl ) : Model(p_mdl) { }

                LoadModel::LoadModel( const Model& p_mdl ) : Model(p_mdl) { }

                LoadModel::~LoadModel() { }

                SaveModel::SaveModel() : Model() { }

                SaveModel::SaveModel( Data& p_dt ) : Model(p_dt) { }

                SaveModel::SaveModel( const SaveModel& p_mdl ) : Model(p_mdl) { }

                SaveModel::SaveModel( const Model& p_mdl ) : Model(p_mdl) { }

                SaveModel::~SaveModel() { }

                Storage::Storage() { }

                bool Storage::operator==(const Storage& p_store) const { return type() == p_store.type(); }

                DomBackend::DomBackend(QDomElement* p_ele) : m_ele(p_ele) { }

                DomStorage::DomStorage() : Storage() { }

                const QString DomStorage::getPath(const Data& p_dt){
                    const QString l_bsPth = System::directory () + QString("/") + p_dt.locale() + QString("/node/");
                    QDir* l_bsDir = new QDir(l_bsPth);
                    if (!l_bsDir->exists())
                        l_bsDir->mkpath(l_bsPth);

                    return l_bsPth + p_dt.id() + QString(".node");
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

                /// @todo Add a timestamp and system-user information to the generated file.
                /// @todo When semantics become powerful, add the generating Wintermute's ID (and place of origin, if possible).
                void DomStorage::saveFrom (const Data &p_dt){
                    QDomDocument l_dom("Data");
                    QDomElement l_elem = l_dom.createElement ("Data");
                    l_dom.appendChild (l_elem);

                    DomSaveModel l_domSvMdl(&l_elem);
                    l_domSvMdl.saveFrom (p_dt);
                    const QString l_str = l_dom.toByteArray(4);

                    QFile* l_file = new QFile(DomStorage::getPath(p_dt));
                    if (!l_file->exists ()){
                        l_file->open (QIODevice::WriteOnly | QIODevice::Truncate);
                        l_file->write ("<!--Generated-->\n");
                        l_file->close ();
                    }

                    l_file->open (QIODevice::WriteOnly | QIODevice::Append);
                    l_file->write(l_str.toUtf8());
                    l_file->close ();

                }

                const QString DomStorage::type () const { return "Dom"; }

                void DomStorage::generate() {
                    QDir d(System::directory ());
                    d.setFilter (QDir::Dirs | QDir::NoDotAndDotDot);
                    QStringList l_lclLst = d.entryList ();

                    foreach(const QString l_lcl, l_lclLst){
                        qDebug() << "(data) [DomStorage] Parsing locale" << l_lcl << "...";
                        const QString l_pth = d.absolutePath () + "/" + l_lcl + "/node.xml";
                        QDomDocument l_spawnDom("Store");
                        QFile* l_file = new QFile(l_pth);

                        if (!l_file->exists ()){
                            qWarning() << "(data) [DomStorage] Can't access" << l_pth << ".";
                            l_file->deleteLater ();
                            continue;
                        }

                        if (!l_spawnDom.setContent (l_file)){
                            qWarning() << "(data) [DomStorage] Parse error in" << l_pth << ".";
                            continue;
                        }

                        spawn(l_spawnDom);
                    }
                }

                const QString DomStorage::obtainFullSuffix(const QString& p_lcl, const QString& p_sfx) const {
                    const Data l_dt(QString::null,p_lcl);
                    const QDomDocument* l_spawnDoc = getSpawnDoc(l_dt);
                    if (!l_spawnDoc){
                        qWarning() << "(ling) [DomStorage] Data not found for locale" << p_lcl;
                        return QString::null;
                    }

                    const QDomElement l_dom = l_spawnDoc->documentElement ();
                    const QDomNodeList l_domList = l_dom.elementsByTagName ("Mapping").at (0).toElement ().elementsByTagName ("Suffix");

                    for (int i = 0; i < l_domList.count (); i++){
                        QDomElement l_ele = l_domList.at (i).toElement ();

                        if (l_ele.attribute ("from") == p_sfx)
                            return l_ele.attribute ("to");
                        else
                            continue;
                    }

                    return "";
                }

                void DomStorage::spawn(const QDomDocument& p_dom){
                    const QDomElement l_root = p_dom.documentElement ();
                    const QString l_lcl = l_root.attribute ("locale");
                    const QDomNodeList l_lst = l_root.elementsByTagName ("Data");
                    qDebug () << "(data) [DomStorage] Spawning locale" << l_lcl << "...";

                    for (int i = 0; i < l_lst.count (); i++){
                        QDomElement l_ele = l_lst.at (i).toElement ();
                        if (l_ele.isNull ()) continue;

                        DomLoadModel l_ldM(&l_ele);
                        QDomDocument l_newDom("Data");
                        const Data* l_bsDt = l_ldM.load();
                        Data l_dt(l_bsDt->id (),l_lcl,l_bsDt->symbol (),l_bsDt->flags ());
                        QVariant l_vr = QVariant::fromValue(l_dt);

                        QDomElement l_root = l_newDom.createElement ("Data");
                        l_root.setAttribute ("locale",l_lcl);
                        DomSaveModel l_svM(&l_root);
                        l_svM.saveFrom (l_dt);
                        l_newDom.appendChild (l_root);

                        const QString l_str = l_newDom.toByteArray(4);

                        QFile* l_file = new QFile(DomStorage::getPath(l_dt));
                        if (!l_file->exists ()){
                            l_file->open (QIODevice::WriteOnly | QIODevice::Truncate);
                            l_file->write ("<!-- Generated -->");
                            l_file->close ();
                        }

                        l_file->open (QIODevice::WriteOnly | QIODevice::Append);
                        l_file->write(l_str.toUtf8());
                        l_file->close ();
                    }

                    qDebug () << "(data) [DomStorage] Locale" << l_lcl << "spawned.";
                }

                QDomDocument* DomStorage::getSpawnDoc(const Data& p_dt) {
                    QDir l_dir(System::directory ());
                    const QString l_pth = l_dir.absolutePath () + "/" + p_dt.locale () + "/node.xml";
                    QDomDocument* l_spawnDom = new QDomDocument("Storage");
                    QFile* l_file = new QFile(l_pth);

                    if (!l_file->exists () || !l_dir.exists ()){
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
                    p_dt.setSymbol (l_sym); // The loading process probably overwrote the symbol; depends on the definition.
                }

                /// @todo Allow a means of slowing this process down (very system intenstive, depends heavily on backend's speed).
                const int DomStorage::countFlags() {
                    const QStringList l_locales = System::locales();
                    int l_cnt = 0;

                    foreach (const QString l_locale, l_locales){
                        qDebug() << l_locale;
                        Data l_dt(QString::null,l_locale);
                        const QStringList l_nodeIDs = Cache::allNodes(l_locale);
                        foreach (const QString l_nodeID, l_nodeIDs){
                            l_dt.setID(l_nodeID);
                            Cache::read (l_dt);
                            l_cnt += l_dt.flags ().count ();
                        }
                    }

                    return l_cnt;
                }

                const int DomStorage::countSymbols() {
                    const QStringList l_locales = System::locales();
                    int l_cnt = 0;

                    foreach (const QString l_locale, l_locales){
                        qDebug() << l_locale;
                        const QStringList l_nodeIDs = Cache::allNodes(l_locale);
                        l_cnt += l_nodeIDs.count ();
                    }

                    return l_cnt;
                }

                DomStorage::~DomStorage() { }

                DomLoadModel::DomLoadModel(QDomElement* p_ele) : DomBackend(p_ele) { }

                DomLoadModel::~DomLoadModel() { }

                const Data* DomLoadModel::load () const {
                    if (loadTo(this->Model::m_dt))
                        return &this->Model::m_dt;
                    else {
                        qWarning() << "(data) [DomLoadModel] Failed to load" << m_dt.id () << m_dt.symbol ();
                        return NULL;
                    }
                }

                bool DomLoadModel::loadTo (Data &p_dt) const {
                    if (this->DomBackend::m_ele == NULL || this->DomBackend::m_ele->isNull ()){
                        qCritical () << "(data) [DomLoadModel] Backend's empty.";
                        return false;
                    }

                    QVariantMap l_mp;
                    QDomNodeList l_ndlst = this->DomBackend::m_ele->childNodes();

                    if (!l_ndlst.isEmpty ()) {
                        for (int i = 0; i < l_ndlst.count (); i++){
                            QDomElement l_elem = l_ndlst.at (i).toElement ();
                            l_mp.insert (l_elem.attribute ("guid"),l_elem.attribute ("link"));
                        }
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

                    /*while (this->DomBackend::m_ele->hasChildNodes ())
                        this->DomBackend::m_ele->removeChild (this->DomBackend::m_ele->firstChild ());*/

                    QDomDocument l_dom = this->DomBackend::m_ele->ownerDocument ();

                    if (!this->Model::m_dt.flags().empty()){
                        for(QVariantMap::ConstIterator itr = this->Model::m_dt.flags ().begin ();
                            itr != this->Model::m_dt.flags ().end (); itr++) {
                            QDomElement l_ele = l_dom.createElement ("Flag");
                            l_ele.setAttribute ("guid",itr.key ());
                            l_ele.setAttribute ("link",itr.value ().toString());
                            this->DomBackend::m_ele->appendChild (l_ele);
                        }
                    } else {
                        //qDebug() << "No flags for" << this->Model::m_dt.id();
                    }

                    //qDebug() << "(data) [DomSaveModel] Saved" << this->Model::m_dt.id ();
                }

                void DomSaveModel::saveFrom(const Data& p_dt) {
                    if (!(p_dt == Data::Null))
                        this->Model::m_dt = p_dt;

                    save();
                }

                const QString Cache::obtainFullSuffix(const QString& p_lcl, const QString& p_sfx){
                    foreach (Storage* l_str, Cache::s_stores){
                        const QString l_fl = l_str->obtainFullSuffix(p_lcl,p_sfx);
                        if (!l_fl.isEmpty ())
                            return l_fl;
                        else continue;
                    }

                    return "";
                }

                void Cache::write (const Data &p_dt){
                    if (!Cache::s_stores.empty()){
                        foreach (Storage* l_str, Cache::s_stores)
                            l_str->saveFrom (p_dt);
                    } else {
                        DomStorage *l_domStr = new DomStorage;
                        l_domStr->saveFrom (p_dt);
                        delete l_domStr;
                    }
                }

                /// @note Wintermute considers its local data to be stored to disk. Currently, DomStorage represents the local nodes.
                const int Cache::countFlags(){
                    return DomStorage::countFlags();
                }

                /// @note Wintermute considers its local data to be stored to disk. Currently, DomStorage represents the local nodes.
                const int Cache::countSymbols(){
                    return DomStorage::countSymbols();
                }

                /// @todo Find a way to call all of the storages in parallel and then kill all of the other ones when none (or one has) found information.
                const bool Cache::exists(const Data& p_dt) {
                    foreach (Storage* l_str, Cache::s_stores){
                        if (l_str->exists (p_dt))
                            return true;
                    }

                    return false;
                }

                /// @todo Find a way to call all of the storages in parallel and then kill all of the other ones when none (or one has) found information.
                const bool Cache::read (Data &p_dt) {
                    foreach (Storage* l_str, Cache::s_stores){
                        if (l_str->exists (p_dt)){
                            l_str->loadTo (p_dt);
                            return true;
                        } else continue;
                    }

                    return false;
                }

                /// @todo Find a way to call all of the storages in parallel and then kill all of the other ones when none (or one has) found information.
                void Cache::pseudo (Data &p_psDt){
                    foreach (Storage* l_str, Cache::s_stores){
                        if (l_str->hasPseudo (p_psDt)){
                            l_str->loadPseudo(p_psDt);
                            return;
                        } else continue;
                    }
                }

                /// @todo Find a way to call all of the storages in parallel and then kill all of the other ones when none (or one has) found information.
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

                /// @todo Allow this to be configurable (adding to plug-in settings). Default would be 'DomStorage'.
                Storage* Cache::addStorage(Storage* p_str){
                    if (!hasStorage(p_str->type ())){
                        s_stores << p_str;
                        qDebug() << "(data) [Cache] Added lexical cache backend" << p_str->type() << ".";
                    }

                    return p_str;
                }

                /// @todo Find a way to call all of the storages in parallel and then kill all of the other ones when none (or one has) found information.
                void Cache::clearStorage(){
                    foreach(Storage* l_str, s_stores)
                        delete l_str;

                    s_stores.clear ();
                }

                /// @todo Find a way to call all of the storages in parallel and then kill all of the other ones when none (or one has) found information.
                const bool Cache::hasStorage(const QString& p_strName){
                    foreach (Storage* l_str, s_stores){
                        if (l_str->type () == p_strName)
                            return true;
                    }

                    return false;
                }

                /// @todo Find a way to call all of the storages in parallel and then kill all of the other ones when none (or one has) found information.
                Storage* Cache::storage(const QString& p_strName){
                    foreach (Storage* l_str, s_stores){
                        if (l_str->type () == p_strName)
                            return l_str;
                    }

                    return NULL;
                }

                const QStringList Cache::allNodes(const QString& p_lcl){
                    QDir l_dir(Wintermute::Data::Linguistics::System::directory () + "/" + p_lcl + "/node");
                    l_dir.setFilter (QDir::Files);
                    l_dir.setNameFilters (QString("*.node").split(","));
                    return l_dir.entryList ().replaceInStrings (".node","");
                }

                /// @todo Find a way to call all of the storages in parallel and then kill all of the other ones when none (or one has) found information.
                void Cache::generate() {
                    qDebug() << "(data) [Cache] Dumping all data storages...";

                    foreach (Storage* l_str, Cache::s_stores){
                        qDebug() << "(data) [Cache] Dumping" << l_str->type ();
                        l_str->generate();
                    }

                    qDebug() << "(data) [Cache] Dumped data.";
                }

            } /** end namespace Lexical */
        }
    }
}

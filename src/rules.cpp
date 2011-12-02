/**
 * @file rules.cpp
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

#include "rules.hpp"
#include "config.hpp"
#include <QFile>
#include <QDir>
#include <algorithm>
#include <qjson/parser.h>
#include <qjson/serializer.h>
#include <qjson/qobjecthelper.h>

using std::unique;

namespace Wintermute {
    namespace Data {
        namespace Linguistics {
            namespace Rules {
                Cache::StorageList Cache::s_stores;

                Bond::Bond() { }

                Bond::Bond(const Bond &p_bnd) : m_props(p_bnd.m_props) { }

		void Bond::setWith(QString& p_value) {
		    setAttribute("with",p_value);
		}

                void Bond::setAttribute(const QString& p_attr, QString& p_val) {
                    m_props.insert (p_attr,p_val);
                }

                const QString Bond::attribute(const QString& p_attr) const {
                    return m_props.value(p_attr);
                }

                void Bond::setAttributes(const StringMap& p_props) {
                    m_props = p_props;
                }

                const bool Bond::hasAttribute(const QString& p_attr) const {
                    return m_props.contains(p_attr);
                }

                const QString Bond::with() const {
                    return attribute("with");
                }

                const StringMap Bond::attributes() const {
                    return m_props;
                }

                /// @note This might be the crowning jewel of the linking system.
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

                        l_rslts.push_back((l_cnt / l_max));
                    }

                    qSort(l_rslts.begin (),l_rslts.end ());

                    if (!l_rslts.isEmpty () && !p_regex.isEmpty ())
                        return l_rslts.last ();
                    else
                        return 0.0;
                }

                void Bond::operator=(const Bond& p_bnd) { m_props = p_bnd.m_props; }

                const bool Bond::operator == (const Bond& p_bnd) const { return m_props == p_bnd.m_props; }

                Bond Bond::fromString(const QString &p_str) {
                    QJson::Parser* l_parser = new QJson::Parser;
                    QVariantMap l_map = l_parser->parse(p_str.toAscii()).toMap();
                    Bond l_bnd;
                    QVariantMap::ConstIterator l_itr = l_map.constBegin(), l_end = l_map.constEnd();
                    for (; l_itr != l_end; ++l_itr)
                        l_bnd.m_props.insert(l_itr.key(),l_itr.value().toString());
                    return l_bnd;
                }

                QString Bond::toString() const {
                    QJson::Serializer* l_serializer = new QJson::Serializer;
                    QVariantMap l_map;
                    StringMap::ConstIterator l_itr = m_props.constBegin(), l_end = m_props.constEnd();
                    for (; l_itr != l_end; ++l_itr)
                        l_map.insert(l_itr.key(),l_itr.value());
                    return QString(l_serializer->serialize(l_map));
                }

                QDebug operator<<(QDebug p_dbg, const Bond& p_bnd){
                    p_dbg << p_bnd.m_props;
                    return p_dbg;
                }

                Bond::~Bond () { }

                Chain::Chain() { }

                Chain::Chain(const QString& p_lcl, const QString& p_typ, const QList<Bond> &p_vtr) : m_bndVtr(p_vtr),
                    m_typ(p_typ), m_lcl(p_lcl) { }

                Chain::Chain(const Chain &p_chn) : m_bndVtr(p_chn.m_bndVtr), m_lcl(p_chn.m_lcl), m_typ(p_chn.m_typ) { }

                void Chain::operator= (const Chain& p_chn) {
                    m_bndVtr = p_chn.m_bndVtr;
                    m_typ = p_chn.m_typ;
                    m_lcl = p_chn.m_lcl;
                }

                const QList<Bond> Chain::bonds () const { return m_bndVtr; }

                const QString Chain::type () const { return m_typ; }

                const QString Chain::locale() const { return m_lcl; }

                void Chain::setType (const QString &p_typ) {
                    m_typ = p_typ;
                }

                void Chain::setBonds (const QList<Bond> &p_bnd) { m_bndVtr = p_bnd; }

                QString Chain::toString() const {
                    QJson::Serializer* l_serializer = new QJson::Serializer;
                    QVariantMap l_map;
                    QVariantList l_bndLst;

                    foreach (const Bond l_bnd, m_bndVtr)
                        l_bndLst << qVariantFromValue(l_bnd.toString());

                    l_map["Type"] = m_typ;
                    l_map["Bonds"] = l_bndLst;
                    l_map["Locale"] = m_lcl;
                    return QString(l_serializer->serialize(l_map));
                }

                Chain Chain::fromString(const QString &p_str) {
                    QJson::Parser* l_parser = new QJson::Parser;
                    QVariantMap l_map = l_parser->parse(p_str.toAscii()).toMap();
                    Chain l_chn;
                    l_chn.m_lcl = l_map["Locale"].toString();
                    l_chn.m_typ = l_map["Type"].toString();
                    QStringList l_bndLst = l_map["Bonds"].toStringList();
                    l_bndLst.removeAll("{  }");
                    QStringList::ConstIterator l_itr = l_bndLst.constBegin(), l_end = l_bndLst.constEnd();

                    for (; l_itr != l_end; ++l_itr){
                        const QString l_str = *l_itr;
                        l_chn.m_bndVtr << Bond::fromString(l_str);
                    }

                    return l_chn;
                }

                Chain::~Chain () { }

                Model::Model() { }

                Model::Model(const Model &p_mod) : m_chn(p_mod.m_chn) { }

                LoadModel::LoadModel() : Model() { }

                LoadModel::LoadModel(const Model &p_mdl) : Model(p_mdl) { }

                LoadModel::~LoadModel() { }

                SaveModel::SaveModel() : Model() { }

                SaveModel::SaveModel(const Model &p_mdl) : Model(p_mdl) { }

                SaveModel::~SaveModel() { }

                Storage::Storage() { }

                Storage::Storage(const QString& p_lcl, const QString& p_flg) : m_flg(p_flg), m_lcl(p_lcl) { }

                Storage::Storage(const Storage &p_str) : m_flg(p_str.m_flg), m_lcl(p_str.m_lcl) { }

                bool Storage::operator == (const Storage& p_str) { return type() == this->type (); }

                const QString Storage::flag () const { return m_flg; }

                const QString Storage::locale() const { return m_lcl; }

                Storage::~Storage () { }

                DomBackend::DomBackend() : Backend() { }

                DomBackend::DomBackend(const DomBackend &p_bnd) : m_elem(p_bnd.m_elem) { }

                DomBackend::DomBackend(QDomElement *p_ele) : m_elem(p_ele) { }

                DomBackend::~DomBackend () { }

                DomLoadModel::DomLoadModel() { }

                DomLoadModel::DomLoadModel(const DomLoadModel &p_mdl) : LoadModel(p_mdl), DomBackend(p_mdl.m_elem) { }

                DomLoadModel::DomLoadModel(QDomElement *p_ele) : DomBackend(p_ele) { }

                const Chain* DomLoadModel::load () const {
                    Chain* l_chn = new Chain();
                    QList<Bond> l_bndVtr;
                    obtainBonds(l_bndVtr,*m_elem);
                    unique(l_bndVtr.begin (),l_bndVtr.end ());

                    l_chn->setType (obtainType(*m_elem));
                    l_chn->setBonds (l_bndVtr);
                    
                    return l_chn;
                }

                void DomLoadModel::obtainBonds(QList<Bond>& p_bndVtr, QDomElement p_elem) const {
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

                        p_bndVtr << *l_bnd;
                        delete l_bnd;
                    }

                    if (!p_elem.parentNode ().isNull ()){
                        QDomElement l_prntNode = p_elem.parentNode ().toElement ();
                        obtainBonds(p_bndVtr,l_prntNode);
                    }
                }

                const QString DomLoadModel::obtainType(QDomElement p_elem) const {
                    QString l_type;
                    while (!p_elem.isNull()){
                        l_type.prepend(p_elem.attribute("type"));
                        p_elem = p_elem.parentNode().toElement ();
                    }

                    return l_type;
                }

                /// @todo Allow rules to be inherited upon load.
                void DomLoadModel::loadTo (Chain &p_chn) const {
                    QList<Bond> l_bndVtr;
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
                    const QList<Bond> l_bndVtr = p_chn.bonds();
                    const QString l_bndTyp = p_chn.type();
                    
                    setBonds(l_bndVtr);
                    setType(l_bndTyp);
                    save();
                }
                
                void DomSaveModel::setBonds(const QList<Bond>& p_bndVtr) { }
                
                void DomSaveModel::setType(const QString& p_bndTyp) {}

                DomSaveModel::~DomSaveModel () { }

                DomStorage::DomStorage() : m_min(DOMSTORAGE_MAXSTR) { }

                DomStorage::DomStorage(const Storage &p_str) : Storage(p_str) { }

                const QString DomStorage::getPath(const Chain& p_chn){
                    return System::directory () + "/" + p_chn.locale () + "/grammar.xml";
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
                    const double l_minimum = (1.0 / (double) p_chn.type ().length ());

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

                QDomElement DomStorage::findElement(const Chain& p_chn, QDomElement p_elem, QString p_prefix) const {
                    if (p_elem.hasAttribute ("type") && p_elem != p_elem.ownerDocument ().documentElement ()){
                        const QString l_data = p_elem.attribute ("type");
                        const QStringList l_lst = l_data.split (",");

                        foreach (const QString l_part, l_lst){
                            const QString l_prefix = p_prefix + l_part;
                            const double l_match = Bond::matches (p_chn.type (),l_prefix);
                            if (l_match == 1.0){
                                qDebug() << "(data) [DomStorage] findElement: " << p_chn.type () << l_prefix << l_match;
                                return p_elem;
                            }
                        }

                        p_prefix.append (l_data);
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

                Storage* Cache::addStorage (Storage *p_str){
                    if (!hasStorage(p_str->type ())){
                        s_stores << p_str;
                        qDebug() << "(data) [Cache] Added rules cache backend" << p_str->type() << ".";
                    }

                    return p_str;
                }

                void Cache::clearStorage(){
                    foreach (Storage* l_str, s_stores)
                        delete l_str;

                    s_stores.clear ();
                }


                const bool Cache::hasStorage(const QString& p_strName){
                    foreach (Storage* l_str, s_stores){
                        if (l_str->type () == p_strName)
                            return true;
                    }

                    return false;
                }

                Storage* Cache::storage(const QString& p_strName){
                    foreach (Storage* l_str, s_stores){
                        if (l_str->type () == p_strName)
                            return l_str;
                    }

                    return NULL;
                }

                QDBusArgument& operator<< (QDBusArgument &p_arg, const Bond& p_bnd) {
                    p_arg.beginStructure();
                    p_arg << p_bnd.m_props;
                    p_arg.endStructure();
                    return p_arg;
                }

                const QDBusArgument& operator>> (const QDBusArgument &p_arg, Bond& p_bnd) {
                    p_arg.beginStructure();
                    p_arg >> p_bnd.m_props;
                    p_arg.endStructure();
                    return p_arg;
                }

                QDBusArgument& operator<< (QDBusArgument &p_arg, const Chain& p_chn) {
                    p_arg.beginStructure();
                    p_arg << p_chn.m_lcl << p_chn.m_typ << p_chn.m_bndVtr;
                    p_arg.endStructure();
                    return p_arg;
                }

                const QDBusArgument& operator>> (const QDBusArgument &p_arg, Chain& p_chn) {
                    p_arg.beginStructure();
                    p_arg >> p_chn.m_lcl >> p_chn.m_typ >> p_chn.m_bndVtr;
                    p_arg.endStructure();
                    return p_arg;
                }
            } /** end namespace Rules */
        }
    }
}

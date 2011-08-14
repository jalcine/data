/**
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
#include "linguistics.hpp"
#include "models.hpp"
#include <fstream>
#include <iomanip>
#include <sstream>

#include <QFile>
#include <QTextStream>
#include <QDomDocument>
#include <QDomElement>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/progress.hpp>

using namespace boost::archive;
using namespace boost::serialization;

using boost::archive::text_oarchive;
using boost::archive::text_iarchive;
using boost::archive::xml_oarchive;
using boost::archive::xml_iarchive;
using boost::progress_display;

namespace Wintermute {
    namespace Data {
        namespace Linguistics {
            vector<QDomDocument> XMLStorage::s_allDocs;
            Storage::StorageObtainSignal Storage::s_sigObtain;
            Storage::StorageExistsSignal Storage::s_sigExists;

            /// @todo Should these be inline methods? It speeds up the apps but adds weight to code.
            const string Lexidata::locale() const { return m_lcl; }
            const string Lexidata::id() const { return m_id; }
            const string Lexidata::symbol() const { return m_sym; }
            const Leximap* Lexidata::flags() const { return &m_flgs; }

            void Storage::addDataSource(const StorageObtainSignal::slot_type &p_slotObtain, const StorageExistsSignal::slot_type & p_slotExists)
            {
                s_sigObtain.connect(p_slotObtain);
                s_sigExists.connect(p_slotExists);
            }

            Storage* Storage::obtain(const Lexidata& p_lxin){
                return s_sigObtain(p_lxin);
            }

            const bool Storage::exists (const Lexidata& p_lxin){
                return s_sigExists(p_lxin);
            }

            Storage* LocalStorage::create(const Lexidata& p_lxin){
                return new LocalStorage(p_lxin);
            }

            const bool LocalStorage::exists (const Lexidata& p_lxin){
                const string l_pth(LocalStorage::formPath(p_lxin));
                QFile* l_lex = new QFile(l_pth.c_str ());
                //cout << "(data) [LocalStorage] Lexicon: (" << l_lex->size () << ") " << l_pth << endl;
                return l_lex->exists ();
            }

            const string LocalStorage::formPath(const Lexidata& p_lxin){
                return Configuration::directory() + "/locale/" + p_lxin.locale() + "/node/" + p_lxin.id();
            }

            void LocalStorage::serializeToDisk(const Lexidata& lxin){
                if (!exists(lxin)){
                    LocalSaveModel newStorage(LocalStorage::formPath (lxin),lxin);
                    newStorage.save ();
                }
            }

            /// @todo Should this method invoke a 'not implemented' exception?
            void LocalStorage::spawn(){ }

            /// @todo The permissions of the 'wintermute' master executable has to be updated; it can't write to the base directory.
            /// @bug Information recieved here to be saved isn't the same as passed by ::serializeToDisk!
            void LocalSaveModel::save ()
            {
                const char* thePath = this->url().c_str();
                QFile* theFile = new QFile(thePath);
                if (theFile->open (QIODevice::WriteOnly | QIODevice::Truncate)){
                    cout << "(data) [LocalSaveModel] Saving '" <<  m_lxdata.id() << "' ..." << endl;
                    ostringstream outLexidata;
                    outLexidata << m_lxdata.symbol() << endl;

                    const Leximap* flags = m_lxdata.flags();
                    for (Leximap::const_iterator itr = flags->begin (); itr != flags->end (); itr++)
                        outLexidata << itr->first << " " << itr->second << endl;

                    cout << flags->size () << " flags written for (" << m_lxdata.symbol() << ")." << endl;
                    const char* data = outLexidata.str ().c_str ();
                    theFile->write(data);
                    theFile->close ();
                } else {
                    cout << "(data) [LocalSaveModel] Failed to save '" << m_lxdata.id() << "'! Error: " << theFile->errorString ().toStdString () << endl;
                }
            }

            LocalLoadModel::LocalLoadModel(const Lexidata &p_info) : LoadModel(p_info),
                LocalBackend(LocalStorage::formPath (p_info)) {
                this->LocalLoadModel::load();
            }

            LocalLoadModel::LocalLoadModel(const Model &p_model) : LoadModel(p_model),
                LocalBackend() {
                m_url = LocalStorage::formPath (m_lxdata);
                this->LocalLoadModel::load ();
            }

            LocalSaveModel::LocalSaveModel(const Model &p_model) : SaveModel(p_model),
                LocalBackend() {
                m_url = LocalStorage::formPath (m_lxdata);
            }

            LocalSaveModel::LocalSaveModel(const Lexidata &p_info) : SaveModel(p_info),
                LocalBackend(LocalStorage::formPath (p_info)) {
            }

            void LocalLoadModel::load()
            {
                const char* l_pth = this->url().c_str();
                QFile* l_qhndl = new QFile(l_pth);

                if (l_qhndl->open (QIODevice::ReadOnly)){
                    //cout << "(data) [LocalLoadModel] Loading from '" << this->Model::_lxdata.id() << "' (" << theFile->size ()<< " bytes)..." << endl;
                    QTextStream l_inLexData(l_qhndl->readAll ());
                    QString l_sym, l_flg, l_ont;
                    Leximap l_map;

                    l_inLexData >> l_sym;

                    while (!l_inLexData.atEnd ()) {
                        l_inLexData >> l_ont >> l_flg;
                        if (l_ont.isEmpty () || l_flg.isEmpty ())
                            continue;
                        //cout << "(data) [LocalLoadModel] Flagset: " << qPrintable(l_ont) << " " << qPrintable(l_flg) << endl;
                        l_map.insert (Leximap::value_type(l_ont.toStdString (), l_flg.toStdString ()));
                    }

                    if (l_map.size () == 0){
                        //cout << "(data) [LocalLoadModel] WARNING: No flags loaded into the system." << endl << "(" <<  l_qhndl->size () << "): file:///" << l_pth << endl;
                    }
                    else {
                        //cout << "(data) [LocalLoadModel] " << mapping.size () << " flags loaded for "<< _lxdata.getID() << " ( "<< symbol << " )." << endl;
                        this->Model::m_lxdata = Lexidata(m_lxdata.id(),m_lxdata.locale(), qPrintable(l_sym), l_map);
                    }

                    l_qhndl->close ();
                } else {
                    //cout << "(data) [LocalLoadModel] Failed to load '" << this->LoadModel::_lxdata.id() << "'." << endl;
                }
            }

            /// @todo Finish definition of XMLStorage.
            Storage* XMLStorage::create(const Lexidata& p_lxinfo){
                //return new XMLStorage(p_lxinfo);
                return NULL;
            }

            /// @todo Use iteration to determine if this sect of data was already formed.
            const bool XMLStorage::exists (const Lexidata& p_lxinfo){
                return false;
            }

            /// @bug Some of the information stored here gets passed along as malformed information.
            void XMLStorage::spawn (){
                const string l_pthDoc = Configuration::directory () + string("/locale/") + Configuration::locale() + string("/") + string("spawn.xml");
                QFile* l_qhndl = new QFile(QString(l_pthDoc.c_str()));
                QDomDocument l_domDoc;

                if (!l_qhndl->open (QIODevice::ReadOnly | QIODevice::Text)){
                    cout << "(data) [XMLStorage] Failed to open '" << l_pthDoc << "';" << l_qhndl->errorString ().toStdString () << endl;
                    return;
                }

                if (!l_domDoc.setContent(l_qhndl)){
                    cout << "(data) [XMLStorage] Failed to parse '" << l_pthDoc << "';" << l_qhndl->errorString ().toStdString () << endl;
                    return;
                }

                QDomElement l_rootEle = l_domDoc.documentElement ();
                QDomNodeList l_nodLst = l_rootEle.elementsByTagName ("Lexicon");

                {
                    int l_cntSkip = 0;
                    for (int i = 0; i < l_nodLst.length (); i++){
                        QDomElement l_curEle = l_nodLst.at (i).toElement ();
                        const string l_sym = l_curEle.attribute ("symbol").toStdString ();

                        if (!Storage::exists (Lexidata(md5(l_sym),Configuration::locale()))){
                            QDomNodeList l_lnkLst = l_curEle.elementsByTagName ("Link");

                            if (!l_lnkLst.isEmpty ()){
                                Leximap l_theMap;

                                for (unsigned int j = 0; j < l_lnkLst.length (); j++){
                                    QDomElement l_lnkEle = l_lnkLst.item (j).toElement ();
                                    const string l_ont = l_lnkEle.attribute ("ontoid").toStdString ();
                                    const string l_flg = l_lnkEle.attribute ("flags").toStdString ();
                                    l_theMap.insert (Leximap::value_type(l_ont,l_flg));
                                    //cout << "(data) [XMLStorage] Parsed flagset #" << theMap.size ()<< ": " << ontoid << " " << flags << endl;
                                }

                                Lexidata theLex( md5(l_sym) , Configuration::locale() , l_sym , l_theMap);
                                //cout << "(data) [XMLStorage] Size: " << theMap.size ()  << ":" << links.length () << endl;
                                LocalStorage::serializeToDisk (theLex);
                            } else ++l_cntSkip;
                        } else ++ l_cntSkip;
                    }

                    cout << endl << "(data) [XMLStorage] Generated " << ( l_nodLst.size () - l_cntSkip ) << " lexicons, skipped " << l_cntSkip << ", parsed " << l_nodLst.length () << "." << endl;
                }

                cout << "(data) [XMLStorage] Rendered '" << l_pthDoc << "'." << endl;
            }

            void XMLStorage::addDocument(const string& p_url){
                QDomDocument l_domDoc;
                QFile *l_qhndl = new QFile(p_url.c_str());
                if (l_qhndl->exists () && l_domDoc.setContent (l_qhndl))
                    s_allDocs.push_back(l_domDoc);
            }

            /// @todo Replace/swap internal element's XML with the new one.
            void XMLSaveModel::save(){
                string str;
                ostringstream outXml(str);
                {
                    xml_oarchive oxArchive(outXml);
                    //oxArchive << *this;
                }
            }

            /// @todo Pull XML data from document into this node.
            void XMLLoadModel::load()
            {
                string str;
                istringstream inXml(str);
                {
                    xml_iarchive ixArchive(inXml);
                    //ixArchive >> *this;
                }
            }
        }
    } // namespaces
}

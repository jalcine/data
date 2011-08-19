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
#include <iomanip>

#include <QFile>
#include <QtDebug>
#include <QMultiMap>
#include <QTextStream>
#include <QDomDocument>
#include <QDomElement>

namespace Wintermute {
    namespace Data {
        namespace Linguistics {
            namespace Lexical {
                vector<QDomDocument> XMLStorage::s_allDocs;
                Storage::StorageObtainSignal Storage::s_sigObtain;
                Storage::StorageExistsSignal Storage::s_sigExists;

                void Storage::addDataSource ( const StorageObtainSignal::slot_type &p_slotObtain, const StorageExistsSignal::slot_type & p_slotExists ) {
                    s_sigObtain.connect ( p_slotObtain );
                    s_sigExists.connect ( p_slotExists );
                }

                Storage* Storage::obtain ( const Lexidata* p_lxin ) {
                    return s_sigObtain ( const_cast<Lexidata*>(p_lxin) );
                }

                const bool Storage::exists ( const Lexidata* p_lxin ) {
                    return s_sigExists ( const_cast<Lexidata*>(p_lxin) );
                }

                Storage* LocalStorage::create ( const Lexidata* p_lxin ) {
                    return new LocalStorage ( const_cast<Lexidata*>(p_lxin) );
                }

                const bool LocalStorage::exists ( const Lexidata* p_lxin ) {
                    return QFile::exists (QString::fromStdString (LocalStorage::formPath ( p_lxin )));
                }

                const string LocalStorage::formPath ( const Lexidata* p_lxin ) {
                    return Configuration::directory() + "/locale/"
                        + *p_lxin->locale() + "/node/" + *p_lxin->id();
                }

                void LocalStorage::serializeToDisk ( const Lexidata* lxin ) {
                    if ( !exists ( lxin ) ) {
                        LocalSaveModel *newStorage = new LocalSaveModel(LocalStorage::formPath ( const_cast<Lexidata*>(lxin) ),
                                                                        const_cast<Lexidata*>(lxin) );
                        newStorage->save ();
                    }
                }

                /// @todo Should this method invoke a 'not implemented' exception?
                void LocalStorage::spawn(const string& p_lcl) { }

                LocalSaveModel::LocalSaveModel( Lexidata *p_info ) : SaveModel ( p_info ),
                    LocalBackend ( LocalStorage::formPath ( p_info ) ) {

                }

                LocalSaveModel::LocalSaveModel( const Model& p_model ) : SaveModel ( p_model ) {
                    m_file = new QFile(QString::fromStdString (LocalStorage::formPath ( m_lxdt )));
                }

                /// @todo The permissions of the 'wintermute' master executable has to be updated; it can't write to the base directory.
                /// @bug Information recieved here to be saved isn't the same as passed by ::serializeToDisk!
                void LocalSaveModel::save () {
                    if ( m_file->open ( QIODevice::WriteOnly | QIODevice::Truncate ) ) {
                        const Leximap flags = m_lxdt->flags();
                        //qDebug() << "(data) [LocalSaveModel] Saving '" <<  m_lxdt->id()->c_str() << "' ..." << endl;

                        {
                            QTextStream l_strm(m_file);
                            l_strm << m_lxdt->symbol ()->c_str() << endl;

                            for ( Leximap::const_iterator itr = flags.begin (); itr != flags.end (); itr++ ){
                                l_strm << QString::fromStdString (itr->first)   << " "
                                       << QString::fromStdString (itr->second)  << endl;
                            }
                        }


                        //qDebug() << flags.size () << "flags written for (" << m_lxdt->symbol()->c_str() << ")." << endl
                        //         << m_file->readAll ();
                    } else
                        qWarning () << "(data) [LocalSaveModel] Failed to save" << m_lxdt->id()->c_str() << "; Error: " << m_file->errorString ();

                    m_file->close ();
                }

                LocalLoadModel::LocalLoadModel( Lexidata* p_info ): LoadModel ( p_info ),
                    LocalBackend ( LocalStorage::formPath ( p_info ) ) {
                    this->LocalLoadModel::load();
                }

                LocalLoadModel::LocalLoadModel( const Model& p_model ) : LoadModel ( p_model ),
                    LocalBackend(*(new QFile(QString::fromStdString (LocalStorage::formPath ( m_lxdt ))))) {
                    this->LocalLoadModel::load ();
                }

                Lexidata* LocalLoadModel::load() {
                    if (!m_file->open (QIODevice::ReadOnly)){
                        qWarning () << "(data) [LocalLoadModel] Failed to load '" << this->LoadModel::m_lxdt->id ()->c_str() << "'."
                                    << m_file->errorString();
                        return NULL;
                    } else {
                        //qDebug() << "(data) [LocalLoadModel] Loading from '" << this->Model::m_lxdt->id ()->c_str () << "' (" << m_file->size () << " bytes )...";
                        QTextStream l_inLxDt ( m_file->readAll () );
                        QString l_sym, l_flg, l_ont;
                        Leximap l_map;

                        l_inLxDt >> l_sym;

                        while ( !l_inLxDt.atEnd () ) {
                            l_inLxDt >> l_ont >> l_flg;
                            if ( l_ont.isEmpty () || l_flg.isEmpty () )
                                continue;
                            //qDebug() << "(data) [LocalLoadModel] Flagset: " << l_ont << l_flg;
                            l_map.insert ( Leximap::value_type(l_ont.toStdString (), l_flg.toStdString ()) );
                        }

                        m_file->close ();

                        if ( l_map.size () > 0 ) {
                            //qDebug() << "(data) [LocalLoadModel]" << l_map.size () << "flags loaded for" << m_lxdt->id ()->c_str () << "("<< l_sym << ").";
                            const string* l_symStr = new string(l_sym.toStdString ().c_str ());
                            Lexidata* l_lxdt = new Lexidata ( m_lxdt->id() , m_lxdt->locale() , l_symStr , l_map );
                            Q_ASSERT(l_lxdt != NULL);
                            return l_lxdt;
                        } else {
                            qWarning() << "(data) [LocalLoadModel] WARNING: No flags loaded into the system." << endl
                                          << "(" <<  m_file->size () << "): " << url().c_str ();
                            return NULL;
                        }
                    }
                }

                /// @todo Finish definition of XMLStorage.
                Storage* XMLStorage::create ( const Lexidata* p_lxinfo ) {
                    //return new XMLStorage(p_lxinfo);
                    return NULL;
                }

                /// @todo Use iteration to determine if this sect of data was already formed.
                const bool XMLStorage::exists ( const Lexidata* p_lxinfo ) {
                    return false;
                }

                /// @bug Some of the information stored here gets passed along as malformed information.
                /// @bug Two temporary memory locations are used here; need to drop those warnings..
                void XMLStorage::spawn (const string& p_lcl) {
                    const string l_pthDoc = Configuration::directory () + string ( "/locale/" ) + p_lcl + string ( "/spawn.xml" );
                    QFile* l_qhndl = new QFile ( QString::fromStdString (l_pthDoc));
                    QDomDocument l_domDoc;

                    if ( !l_qhndl->open ( QIODevice::ReadOnly | QIODevice::Text ) ) {
                        qWarning () << "(data) [XMLStorage] Failed to open" << l_pthDoc.c_str ()<< ";" << l_qhndl->errorString ();
                        return;
                    }

                    if ( !l_domDoc.setContent ( l_qhndl ) ) {
                        qWarning() << "(data) [XMLStorage] Failed to parse" << l_pthDoc.c_str ()<< ";" << l_qhndl->errorString ();
                        return;
                    }

                    QDomElement l_rootEle = l_domDoc.documentElement ();
                    QDomNodeList l_nodLst = l_rootEle.elementsByTagName ( "Lexicon" );

                    {
                        int l_cntSkip = 0;
                        for ( int i = 0; i < l_nodLst.length (); i++ ) {
                            QDomElement l_curEle = l_nodLst.at ( i ).toElement ();
                            const QString l_sym = l_curEle.attribute ( "symbol" );
                            const Lexidata l_tstLxdt(&md5(l_sym.toStdString ()),&p_lcl);

                            if ( !Storage::exists ( &l_tstLxdt ) ){
                                QDomNodeList l_lnkLst = l_curEle.elementsByTagName ( "Link" );

                                if ( !l_lnkLst.isEmpty () ) {
                                    Leximap l_map;

                                    for ( unsigned int j = 0; j < l_lnkLst.length (); j++ ) {
                                        QDomElement l_lnkEle = l_lnkLst.item ( j ).toElement ();
                                        const QString l_ont = l_lnkEle.attribute ( "ontoid" );
                                        const QString l_flg = l_lnkEle.attribute ( "flags" );

                                        l_map.insert ( Leximap::value_type(l_ont.toStdString(),l_flg.toStdString()) ) ;
                                        qDebug() << "(data) [XMLStorage] Parsed flagset #" << (l_map.size () + 1) << ": " << l_ont << l_flg;
                                    }

                                    Lexidata *theLex = new Lexidata ( const_cast<const string*>(&md5(l_sym.toStdString ())), &p_lcl , const_cast<const string*>(&l_sym.toStdString ()), l_map );
                                    qDebug() << "(data) [XMLStorage] Size:" << l_map.size ()  << ":" << l_lnkLst.size ();
                                    LocalStorage::serializeToDisk ( theLex );
                                } else ++l_cntSkip;
                            } else ++ l_cntSkip;
                        }

                        if ( l_nodLst.size () - l_cntSkip != 0 )
                            qDebug() << "(data) [XMLStorage] Generated" << ( l_nodLst.size () - l_cntSkip ) << "lexicons, skipped" << l_cntSkip << ", parsed" << l_nodLst.length () << ".";
                    }

                    qDebug() << "(data) [XMLStorage] Rendered" << l_pthDoc.c_str ();
                }

                void XMLStorage::addDocument ( const string& p_url ) {
                    QDomDocument l_domDoc;
                    QFile *l_qhndl = new QFile ( p_url.c_str() );
                    if ( l_qhndl->exists () && l_domDoc.setContent ( l_qhndl ) )
                        s_allDocs.push_back ( l_domDoc );
                }

                /// @todo Replace/swap internal element's XML with the new one.
                void XMLSaveModel::save() { }

                /// @todo Pull XML data from document into this node.
                Lexidata* XMLLoadModel::load() { }

            }

            namespace Rules {

            }
        } // namespaces
    }
}
// kate: indent-mode cstyle; space-indent on; indent-width 4;

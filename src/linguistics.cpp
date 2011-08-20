/**
 * @file linguistics.cpp
 * @author Jacky Alcine <jackyalcine@gmail.com>
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

#include <map>
#include <string>
#include <QFile>
#include <QDir>
#include <QDomDocument>
#include <iostream>
#include "md5.hpp"
#include "config.hpp"
#include "linguistics.hpp"
#include "models.hpp"

using namespace std;
using namespace Wintermute::Data;

using std::string;

namespace Wintermute {
    namespace Data {
        namespace Linguistics {
            string Configuration::_storageDir = string ( "./" ) + string ( WNTRDATA_LING_DIR );
            string Configuration::_locale = "en";

            void Configuration::Initialize ( const string storageDir, const string locale ) {
                qDebug() << "(data) [Ling:Config] Initializing data..";
                Configuration::setDirectory ( storageDir );
                Configuration::setLocale ( locale );

                qDebug() << "(data) [Ling:Config] Setting up linguistics data sources... ";
                Lexical::Storage::addDataSource ( Lexical::LocalStorage::create , Lexical::LocalStorage::exists );
                Lexical::Storage::addDataSource ( Lexical::XMLStorage::create , Lexical::XMLStorage::exists );

                qDebug() << "(data) [Ling:Config] Parsing sources.. ";
                Lexical::LocalStorage::spawn();
                Lexical::XMLStorage::spawn();
            }

            void Configuration::Deinitialize() {

            }

            void Configuration::setLocale ( string const locale ) {
                if ( locale.size() == 0 )
                    return;

                Configuration::_locale = locale;
                cout << "(data) [Ling:Config] ## Global locale changed to " << locale << endl;
            }

            void Configuration::setDirectory ( string const configDir ) {
                if ( configDir.size() == 0 )
                    return;

                QDir* d = new QDir(configDir.c_str ());
                Configuration::_storageDir = d->absolutePath().toStdString ();

                cout << "(data) [Ling:Config] ## Root directory changed to " << configDir << endl;
            }
        } // namespaces
    }
}
// kate: indent-mode cstyle; space-indent on; indent-width 4;

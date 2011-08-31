/**
 * @file linguistics.cpp
 * @author Wintermute Developers <wintermute-devel@lists.launchpad.net>
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
#include <QDir>
#include "config.hpp"
#include "models.hpp"
#include "linguistics.hpp"

using namespace std;
using namespace Wintermute::Data;

using std::string;

namespace Wintermute {
    namespace Data {
        namespace Linguistics {
            string Configuration::_storageDir = string( WNTRDATA_DATA_DIR ) + "/" + string ( WNTRDATA_LING_DIR );
            string Configuration::_locale = WNTRDATA_DEFAULT_LOCALE;

            void Configuration::Initialize ( const string storageDir, const string locale ) {
                Configuration::setDirectory ( storageDir );
                Configuration::setLocale ( locale );

                Lexical::Cache::addStorage ((new Lexical::DomStorage));
                Rules::Cache::addStorage ((new Rules::DomStorage));

                Lexical::Cache::generate();

                qDebug() << "(ling) [Config] ## System configured.";
            }

            void Configuration::Deinitialize() {
                qDebug() << "(ling) [Config] Shutting down..";
            }

            void Configuration::setLocale ( const string p_lcl ) {
                if ( p_lcl.empty() )
                    return;

                Configuration::_locale = p_lcl;
                qDebug() << "(ling) [Config] ## Default locale:" << p_lcl.c_str ();
            }

            void Configuration::setDirectory ( const string p_configDir ) {
                if ( p_configDir.empty() )
                    return;

                QDir* d = new QDir(p_configDir.c_str ());
                if (d->exists ()){
                    Configuration::_storageDir = d->absolutePath().toStdString ();
                    qDebug() << "(ling) [Config] ## Root dir:" << p_configDir.c_str ();
                }
            }
        } // namespaces
    }
}
// kate: indent-mode cstyle; space-indent on; indent-width 4;

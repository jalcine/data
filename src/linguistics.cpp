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
            QString System::m_storageDir = QString(WNTRDATA_DATA_DIR) + "/" + QString(WNTRDATA_LING_DIR);
            QString System::m_lcl = WNTRDATA_DEFAULT_LOCALE;

            void System::load ( const QString storageDir, const QString locale ) {
                qDebug() << "(data) [System] # ling # System loading...";

                System::setDirectory ( storageDir );
                System::setLocale ( locale );

                Lexical::Cache::addStorage ((new Lexical::DomStorage));
                Rules::Cache::addStorage ((new Rules::DomStorage));

                Lexical::Cache::generate();

                qDebug() << "(data) [System] # ling # System loaded.";
            }

            void System::unload() {
                qDebug() << "(ling) [System] System unloading...";
                Lexical::Cache::clearStorage();
                Rules::Cache::clearStorage();
                qDebug() << "(data) [System] # ling # System unloaded.";
            }

            void System::setLocale ( const QString p_lcl ) {
                if ( p_lcl.isEmpty() )
                    return;

                System::m_lcl = p_lcl;
                qDebug() << "(data) [System] # ling # Default locale:" << p_lcl;
            }

            void System::setDirectory ( const QString p_configDir ) {
                if ( p_configDir.isEmpty() )
                    return;

                QDir* d = new QDir(p_configDir);
                if (d->exists ()){
                    System::m_storageDir = d->absolutePath();
                    qDebug() << "(data) [System] # ling # Root dir:" << p_configDir;
                }
            }
        } // namespaces
    }
}
// kate: indent-mode cstyle; space-indent on; indent-width 4;

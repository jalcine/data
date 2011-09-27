/**
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

#include <QtDebug>
#include "wntrdata.hpp"

namespace Wintermute {
    namespace Data {
        System* System::s_config = NULL;

        System::System() : m_dir(WNTRDATA_DATA_DIR) { }

        void System::start ( ) {
            Wintermute::Data::Linguistics::System::load ( System::directory() + QString ( "/" ) + QString ( WNTRDATA_LING_DIR ) );
            Wintermute::Data::Ontology::System::load();
            emit s_config->started();
        }

        void System::stop ( ) {
            Wintermute::Data::Ontology::System::unload();
            Wintermute::Data::Linguistics::System::unload();
            emit s_config->stopped();
        }

        const QString System::directory () { return s_config->m_dir; }

        void System::setDirectory(const QString& p_dir) {
            stop();
            s_config->m_dir = p_dir;
            start();
        }

        System* System::instance () {
            if (!s_config) s_config = new System;
            return s_config;
        }
    }
}
// kate: indent-mode cstyle; space-indent on; indent-width 4;

/**
 * @author Wintermute Developers <wintermute-devel@lists.launchpad.net>
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
 *
 * File providing compile-time data about runtime-configuration.
 * Required for the Wintermute database module.
 */

#ifndef __WNTRDATA_CONFIG_HPP__
#define __WNTRDATA_CONFIG_HPP__

/* #undef DEBUG */

#define WNTRDATA_DATA_DIR "/share/wintermute"
#define WNTRDATA_LING_DIR "ling"
#define WNTRDATA_ONTO_DIR "onto"
#define WNTRDATA_DEFAULT_LOCALE ""

#include <string>
#include <QObject>

using namespace std;

using std::string;

namespace Wintermute {
    namespace Data {
        /**
         * @brief Manages the data location representing WntrData.
         * @class Configuration config.hpp "config.hpp"
         */
        class System : public QObject {
            friend class SystemAdaptor;
            Q_OBJECT
            Q_DISABLE_COPY(System)

            private:
                static System* s_config;
                QString m_dir;
                System();

            public:
                /**
                 * @brief Obtains the directory that of which the data files are stored.
                 * @fn getDirectory
                 * @return const string
                 */
                static const QString directory();

                /**
                 * @brief
                 *
                 * @fn setDirectory
                 * @param
                 */
                static void setDirectory(const QString&);

                /**
                 * @brief
                 *
                 * @fn instance
                 * @return const Configuration *
                 */
                static System* instance();

            signals:
                void started();
                void stopped();

            public slots:
                /**
                 * @brief Initializes the data services.
                 * @fn Initialize
                 */
                static void stop();
                /**
                 * @brief Deinitializes the data services.
                 * @fn Deinitialize
                 */
                static void start();
        };
    }
}

#endif // __WNTRDATA_CONFIG_HPP__

// kate: indent-mode cstyle; space-indent on; indent-width 4;

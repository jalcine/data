/**
 * @file linguistics.hpp
 * @author: Wintermute Developers <wintermute-devel@lists.launchpad.net>
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
 * @date March 29, 2011, 3:08 PM
 */

#ifndef LINGUISTICS_HPP
#define	LINGUISTICS_HPP

#include <string>
#include <QObject>

using std::string;

namespace Wintermute {
    namespace Data {
        namespace Linguistics {
            struct Configuration;

            /**
             * @brief An abstract static class representing the current configuration of the linguistics data.
             * All configuration variables and their access methods specific for linguistic part of WntrData
             * are defined here.
             */
            class Configuration {
                private:
                    static string _storageDir; /**< Holds the location of the linguistic info. */
                    static string _locale; /**< Holds the current locale in use. */

                public:
                    /**
                     * @brief Changes the locale.
                     * Sets the default locale to be used as the standard locale.
                     * @param locale The locale to set.
                     */
                    static void setLocale ( const string );

                    /**
                     * @brief Changes root directory.
                     * Sets the root directory to find data for linguistics from.
                     * @param root The root directory.
                     */
                    static void setDirectory ( const string );

                    /**
                     * @brief Gets current locale used.
                     * Returns the locale considered to be the standard of the linguistics system.
                     * @fn getLocale
                     * @return string String with the current locale.
                     */
                    static const inline string locale() { return _locale; }

                    /**
                     * @brief Gets linguistics directory.
                     * Returns the directory that's considered to be the absolute directory for loading linguistics information.
                     * @fn getDirectory
                     * @return string
                     */
                    static const inline string directory() { return _storageDir; }
                    /**
                     * @brief Initialization.
                     * Initializes the system by running prerequisite code.
                     * @fn Initialize
                     * @param storageDir The directory where storage data are located.
                     * @param locale The locale that should be initially loaded.
                     */
                    static void Initialize ( const string = _storageDir, const string = locale ());

                    /**
                     * @brief Deinitialization.
                     * Deinitializes the system by freeing up resources used by the system.
                     * @fn Deinitialize
                     */
                    static void Deinitialize();
            };
        } // namespaces
    }
}
#endif	/* LINGUISTICS_HPP */
// kate: indent-mode cstyle; space-indent on; indent-width 4;

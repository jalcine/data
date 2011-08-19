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
 * @file ontology.cpp
 * @author Jacky Alcine
 * @date March 29, 2011, 2:25 PM
 * @todo Determine a means of using Soprano to interpret RDF/XML (OWL) files into semantic information.
 * @todo Determine how and where UUIDs will be obtained. (Boost::UUID anyone? :])
 */

#include "config.hpp"
#include "ontology.hpp"
#include <boost/timer.hpp>
#include <boost/progress.hpp>
#include <QUrl>
#include <QtXml>

using namespace Soprano;
using namespace boost;

using boost::progress_display;

namespace Wintermute {
    namespace Data {
        namespace Ontology {
           void Configuration::Initialize() {
                //cout << "(data) [Ontology::Configuration] Loaded." << endl;
            }

            void Configuration::Deinitialize() {
                //cout << "(data) [Ontology::Configuration] Unloaded." << endl;
            }
        }
    }
}
// kate: indent-mode cstyle; space-indent on; indent-width 4;

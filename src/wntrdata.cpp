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

#include <boost/python.hpp>
#include "config.hpp"
#include "wntrdata.hpp"

using namespace boost::python;
using namespace Wintermute::Data::Linguistics;

namespace Wintermute {
    namespace Data {
        void Configuration::Initialize ( void ) {
            Wintermute::Data::Linguistics::Configuration::Initialize ( Configuration::directory() + string ( "/" ) + string ( WNTRDATA_LING_DIR ) );
            Wintermute::Data::Ontology::Configuration::Initialize();
        }

        void Configuration::Deinitialize ( void ) {
            Wintermute::Data::Linguistics::Configuration::Deinitialize();
            Wintermute::Data::Ontology::Configuration::Deinitialize();
        }

        BOOST_PYTHON_MODULE ( wntrdata ) {
            class_<Configuration> ( "Configuration",no_init )
            .def ( "Initialize", Configuration::Initialize )
            .def ( "Deinitialize", Configuration::Deinitialize );

            //class_<Model, boost::noncopyable>("Model",no_init);

            /*class_<SaveModel, boost::noncopyable, bases<Model> >("SaveModel",no_init)
                    .def("save",&SaveModel::save)
            ;

            class_<LoadModel, boost::noncopyable, bases<Model> >("LoadModel",no_init)
                    .def("load",&LoadModel::load)
            ;*/
        }
    }
}
// kate: indent-mode cstyle; space-indent on; indent-width 4;

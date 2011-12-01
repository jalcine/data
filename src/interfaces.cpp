/**
 * @file interfaces.hpp
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
 */

#include "interfaces.hpp"
#include "config.hpp"
#include <wntr/ipc.hpp>
#include <wntr/plugins.hpp>

namespace Wintermute {
    namespace Data {
        NodeInterface::NodeInterface()
            : QDBusAbstractInterface(WNTRDATA_DBUS_SERVICE, "/Nodes", staticInterfaceName(), *IPC::System::bus(), Plugins::Factory::currentPlugin()){
        }

        NodeInterface::~NodeInterface() { }

        RuleInterface::RuleInterface()
            : QDBusAbstractInterface(WNTRDATA_DBUS_SERVICE, "/Rules", staticInterfaceName(), *IPC::System::bus(), Plugins::Factory::currentPlugin()){
        }

        RuleInterface::~RuleInterface() { }

        SystemInterface::SystemInterface()
            : QDBusAbstractInterface(WNTRDATA_DBUS_SERVICE, "/System", staticInterfaceName(), *IPC::System::bus(), Plugins::Factory::currentPlugin()){
        }

        SystemInterface::~SystemInterface()
        {
        }
    }
}

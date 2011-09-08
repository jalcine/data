/**
 * @file    adaptors.cpp
 * @author  Wintermute Developers <wintermute-devel@lists.launchpad.net>
 * @created 9/6/2011
 *
 *
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Wintermute Linguistics is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute Linguistics; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#include "wntr/data/adaptors.hpp"
#include <wntrdata.hpp>
#include <QDBusConnection>

namespace Wintermute {
    namespace Data {
        SystemAdaptor::SystemAdaptor() : Adaptor(System::instance ()) {
            setAutoRelaySignals (true);
            QDBusConnection::sessionBus().connect ("org.thesii.Wintermute","/Master",
                                      "org.thesii.Wintermute.Master","aboutToQuit",
                                      this,SLOT(quit()));
        }

        const QString SystemAdaptor::directory () const { return System::directory (); }

        void SystemAdaptor::setDirectory (const QString &p_dir) const { System::setDirectory(p_dir); }

        void SystemAdaptor::stop(const QDBusMessage& p_msg) {
            System::stop();
            emit stopped ();
        }

        void SystemAdaptor::start(const QDBusMessage& p_msg) {
            System::start();
            emit started ();
        }

        void SystemAdaptor::quit (const QDBusMessage& p_msg) const {
            emit aboutToQuit ();
            System::stop ();
            CoreAdaptor::haltSystem ();
        }

        NodeAdaptor::NodeAdaptor() : Adaptor(System::instance()) { }

        void NodeAdaptor::registerBackend (const Lexical::Backend &p_bcknd, const QDBusMessage& p_msg) {
            //const Lexical::Storage* l_str = Lexical::Cache::addStorage (*p_bcknd);
            //emit nodeBackendRegistered (l_str->type ());
        }

        void NodeAdaptor::write (const Lexical::Data &p_data, const QDBusMessage& p_msg) { Lexical::Cache::write (p_data); }

        void NodeAdaptor::generate (const QDBusMessage& p_msg) { Lexical::Cache::generate (); }

        void NodeAdaptor::read (const QDBusMessage& p_msg, Lexical::Data &p_data) const { Lexical::Cache::read (p_data); }

        void NodeAdaptor::pseudo (const QDBusMessage& p_msg, Lexical::Data &p_data) const { Lexical::Cache::pseudo (p_data); }

        const bool NodeAdaptor::exists(const Lexical::Data &p_data, const QDBusMessage& p_msg) const { return Lexical::Cache::exists (p_data); }

        const bool NodeAdaptor::isPseudo (const Lexical::Data &p_data, const QDBusMessage& p_msg) const { return Lexical::Cache::isPseudo (p_data); }

        void NodeAdaptor::quit(const QDBusMessage& p_msg) const {
            p_msg.createErrorReply (QDBusError::AccessDenied,"Cannot stop the Lexical::Node service.");
        }

        RuleAdaptor::RuleAdaptor() : Adaptor(System::instance()) { }

        void RuleAdaptor::read (const QDBusMessage& p_msg, Rules::Chain &p_chn) const { Rules::Cache::read (p_chn); }

        void RuleAdaptor::write (const Rules::Chain &p_chn, const QDBusMessage& p_msg) { Rules::Cache::write (p_chn); }

        const bool RuleAdaptor::exists (const QString &p_1, const QString &p_2, const QDBusMessage& p_msg) const {
            return Rules::Cache::exists (p_1,p_2);
        }

        void RuleAdaptor::registerBackend (const QDBusMessage& p_msg, Rules::Backend &p_bcknd){

        }

        void RuleAdaptor::quit (const QDBusMessage& p_msg) const {
            p_msg.createErrorReply (QDBusError::AccessDenied,"Cannot stop the Rules::Bond service.");
        }
    }
}

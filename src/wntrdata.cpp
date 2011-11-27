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
#include <QtPlugin>
#include <wntr/core.hpp>
#include <wntr/ipc.hpp>

#include "wntrdata.hpp"
#include "adaptors.hpp"

using namespace Wintermute::Data::Linguistics;

namespace Wintermute {
    namespace Data {
        System* System::s_config = NULL;
        NodeManager* NodeManager::s_inst = NULL;
        RuleManager* RuleManager::s_inst = NULL;

        RuleManager::RuleManager() : QObject(System::instance()) { }

        RuleManager* RuleManager::instance() {
            if (!s_inst) s_inst = new RuleManager;
            return s_inst;
        }

        NodeManager::NodeManager() : QObject(System::instance()) { }

        void NodeManager::generate() {
            Lexical::Cache::generate();
        }

        void NodeManager::pseudo(Lexical::Data &p_dt) const {
            Lexical::Cache::psuedo(p_dt);
        }

        void NodeManager::read(Lexical::Data &p_dt) const {
            Lexical::Cache::read(p_dt);
        }

        void NodeManager::write(const Lexical::Data &p_dt) {
            Lexical::Cache::write(p_dt);
        }

        const bool NodeManager::exists(const Lexical::Data &p_dt) const {
            return Lexical::Cache::exists(p_dt);
        }

        const bool NodeManager::isPseudo(const Lexical::Data &p_dt) const {
            return Lexical::Cache::isPseudo(p_dt);
        }

        NodeManager* NodeManager::instance() {
            if (!s_inst) s_inst = new NodeManager;
            return s_inst;
        }

        System::System() : m_dir(WNTRDATA_DATA_DIR) {
            connect(this,SIGNAL(started()),this,SLOT(registerDataTypes()));
        }

        void System::registerDataTypes() {
            qRegisterMetaType<Lexical::Data>("LexicalData");
            qRegisterMetaType<Rules::Bond>("RulesBond");
            qRegisterMetaType<Rules::Chain>("RulesChain");

            qDBusRegisterMetaType<Lexical::Data>();
            qDBusRegisterMetaType<Rules::Bond>();
            qDBusRegisterMetaType<Rules::Chain>();
        }

        void System::start ( ) {
            Linguistics::System::setLocale ( Core::arguments ()->value ("locale").toString () );
            Linguistics::System::load ( System::directory() + QString ( "/" ) + QString ( WNTRDATA_LING_DIR ) );
            Ontology::System::load();
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

        void Plugin::start () const {
            connect(this,SIGNAL(started()), Wintermute::Data::System::instance (),SLOT(start()));
            connect(this,SIGNAL(stopped()), Wintermute::Data::System::instance (),SLOT(stop()));

            Data::SystemAdaptor* l_adpt = new Data::SystemAdaptor(System::instance());
            Data::NodeAdaptor* l_adpt2 = new Data::NodeAdaptor(NodeManager::instance());
            Data::RuleAdaptor* l_adpt3 = new Data::RuleAdaptor(RuleManager::instance());

            Wintermute::IPC::System::registerObject ("/System" , l_adpt);
            Wintermute::IPC::System::registerObject ("/Nodes"  , l_adpt2);
            Wintermute::IPC::System::registerObject ("/Rules"  , l_adpt3);
        }

        void Plugin::stop () const { }
    }
}

Q_EXPORT_PLUGIN2(WntrData, Wintermute::Data::Plugin)
// kate: indent-mode cstyle; space-indent on; indent-width 4;

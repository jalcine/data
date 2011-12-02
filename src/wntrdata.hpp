/**
 * @file wntrdata.hpp
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
/**
 * @mainpage Wintermute Data (WntrData) Documentation
 * Wintermute relies on the aggregation and pooling of data resources
 * to make sense of the (possibly) ten of hundreds of nodes of information
 * it recieves. There's dedicated systems to obtaining information regarding
 * syntax and linguistics for natural language processing (Wintermute::Data::Linguistics)
 * and ontological resources (Wintermute::Data::Ontology).
 * Currently, the language data system provides an abstract class system so that
 * plug-in developers can implement new and foriegn sources of data for lexical
 * and syntactic binding. Plug-in developers are encouraged to focus on data sources
 * that introduce new sources of words aside from importing new syntactic information
 * for the fact the syntactic system still needs a bit more work before it becomes as
 * decently mature as the lexical system.
 *
 * @section N01 Obtaining Information
 * WntrData holds all of the lexical, syntactical, and ontological data for Wintermute.
 * Thus, the means of information obtaining (at least for lexical and syntactic) information
 * are similar, whereas ontological information resembles a vaguely knit representation of
 * of that shown by typical SPARQL interpretators and parsers.
 * @subsection N011 Lexical Information
 * Lexical information is stored by using the Models system designed by Adrian Borucki <gentoolx@gmail.com>.
 * The syntactical information is also stored accordingly.
 * @see N012
 * @subsubsection N0111 Loading
 * Obtaining lexical information is done by creating a Data object and specifying the ID
 * and locale of the Data object. The symbol value is filled as well as the flags from the
 * loading
 * @endsection
 * @endsection
 */
#ifndef __WNTRDATA_HPP__
#define __WNTRDATA_HPP__

#include "config.hpp"
#include "ontology.hpp"
#include "linguistics.hpp"
#include "models.hpp"
#include "interfaces.hpp"
#include <wntr/plugins.hpp>

using namespace Wintermute::Data::Linguistics;
using Wintermute::Plugins::AbstractPlugin;

namespace Wintermute {
    namespace Data {
        struct Plugin;
        struct System;
        struct NodeManager;
        struct RuleManager;

        class NodeManager : public QObject {
            friend class NodeAdaptor;
            friend class NodeInterface;
            Q_OBJECT
            Q_DISABLE_COPY(NodeManager)

            private:
                static NodeManager* s_inst;
                NodeManager();

            signals:
                void nodeCreated(const QString&);

            public slots:
                void generate();
                Lexical::Data& pseudo(Lexical::Data& ) const;
                Lexical::Data& read(Lexical::Data& ) const;
                const Lexical::Data& write(const Lexical::Data& );
                const bool exists(const Lexical::Data& ) const;
                const bool isPseudo(const Lexical::Data& ) const;
                static NodeManager* instance();
        };

        class RuleManager : public QObject {
            Q_OBJECT
            Q_DISABLE_COPY(RuleManager)

            private:
                static RuleManager* s_inst;
                RuleManager();

            public slots:
                static RuleManager* instance();
                void read(Rules::Chain& );
                void write(Rules::Chain& );
                const bool exists(const QString&, const QString& ) const;
        };

        /**
         * @brief Manages the data location representing WntrData.
         * @class Configuration config.hpp "config.hpp"
         */
        class System : public QObject {
            friend class SystemAdaptor;
            friend class SystemInterface;
            Q_OBJECT
            Q_DISABLE_COPY(System)

            private:
                static System* s_inst;
                QString m_dir;
                System();

            public:
                /**
                 * @brief Obtains the directory that of which the data files are stored.
                 * @fn getDirectory
                 * @return const QString
                 */
                static const QString directory();

                /**
                 * @brief Changes the working directory.
                 * @fn setDirectory
                 * @param const QString
                 */
                static void setDirectory(const QString&);

                /**
                 * @brief Obtains an instance of the data system object.
                 * @fn instance
                 * @return const Configuration
                 */
                static System* instance();

            signals:
                /**
                 * @brief
                 *
                 * @fn started
                 */
                void started();

                /**
                 * @brief
                 *
                 * @fn stopped
                 */
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

                static void registerDataTypes();
        };

       class Plugin : public AbstractPlugin {
            Q_OBJECT
            public:
                Plugin() : AbstractPlugin() { }
                ~Plugin() { }
                Plugin(Plugin const &k) : AbstractPlugin(k) { }

                virtual void start() const;
                virtual void stop() const;
        };
    }
}

#endif /* __WNTRDATA_HPP__ */
// kate: indent-mode cstyle; space-indent on; indent-width 4;

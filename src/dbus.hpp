/**
 * @file    adaptors.hpp
 * @author  Wintermute Developers <wintermute-devel@lists.launchpad.net>
 *
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

#ifdef ADAPTORS_HPP

#include "models.hpp"
#include <wntr/adaptors.hpp>

using namespace Wintermute::Data::Linguistics;

namespace Wintermute {
    namespace Data {
        struct SystemAdaptor;
        struct NodeAdaptor;
        struct RuleAdaptor;

        class System : public Adaptor {
            Q_OBJECT
            Q_CLASSINFO("D-Bus Interface","org.thesii.Wintermute.Data.System")
            Q_PROPERTY(const QString Directory READ directory WRITE setDirectory)

            public:
                explicit System();
                const QString directory() const;
                void setDirectory(const QString&) const;
                void localeChanged(const QString&) const;

            signals:
                void stopped();
                void started();

            public slots:
                virtual void quit() const;
                Q_NOREPLY void stop();
                Q_NOREPLY void start();
                const bool localeExists(const QString& );
        };

        class Node : public Adaptor {
            Q_OBJECT
            Q_CLASSINFO("D-Bus Interface","org.thesii.Wintermute.Data.Nodes")

            public:
                explicit Node();

            signals:
                void nodeCreated(const QString&);

            public slots:
                Q_NOREPLY void generate();
                void pseudo(Lexical::Data&) const;
                void read(Lexical::Data& ) const;
                void write(const Lexical::Data& );
                const bool exists(const Lexical::Data& ) const;
                const bool isPseudo(const Lexical::Data& ) const;
                virtual void quit() const;
        };

        class Rule : public Adaptor {
            Q_OBJECT
            Q_CLASSINFO("D-Bus Interface","org.thesii.Wintermute.Data.Rules")

            public:
                explicit Rule();

            signals:
                void ruleCreated(const QString&);

            public slots:
                void read(Rules::Chain&) const;
                void write(const Rules::Chain& );
                const bool exists(const QString&, const QString& ) const;
                virtual void quit() const;
        };
    }
}

#endif // DBUS_HPP

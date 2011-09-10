/**
 * @file    adaptors.hpp
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

#ifndef WNTRDATA_ADAPTORS_HPP
#define WNTRDATA_ADAPTORS_HPP

#include "models.hpp"
#include <wntr/adaptors.hpp>

using namespace Wintermute::Data::Linguistics;

namespace Wintermute {
    namespace Data {
        struct SystemAdaptor;
        struct NodeAdaptor;
        struct RuleAdaptor;

        class SystemAdaptor : public Adaptor {
            Q_OBJECT
            Q_CLASSINFO("D-Bus Interface","org.thesii.Wintermute.Data.System")
            Q_PROPERTY(const QString Directory READ directory WRITE setDirectory)

            public:
                explicit SystemAdaptor();
                const QString directory() const;
                void setDirectory(const QString&) const;

            signals:
                void stopped();
                void started();

            public slots:
                virtual void quit(const QDBusMessage&) const;
                Q_NOREPLY void stop(const QDBusMessage&);
                Q_NOREPLY void start(const QDBusMessage&);
        };

        class NodeAdaptor : public Adaptor {
            Q_OBJECT
            Q_CLASSINFO("D-Bus Interface","org.thesii.Wintermute.Data.NodeAdaptor")

            public:
                explicit NodeAdaptor();

            signals:
                void backendRegistered(const QString&);
                void nodeCreated(const QString&);

            public slots:
                void registerBackend(const Lexical::Backend&, const QDBusMessage&);
                void pseudo(const QDBusMessage&, Lexical::Data&) const;
                Q_NOREPLY void generate(const QDBusMessage&);
                void read(const QDBusMessage&, Lexical::Data& ) const;
                void write(const Lexical::Data&, const QDBusMessage&);
                const bool exists(const Lexical::Data& ,const QDBusMessage&) const;
                const bool isPseudo(const Lexical::Data& ,const QDBusMessage&) const;
                virtual void quit(const QDBusMessage&) const;
        };

        class RuleAdaptor : public Adaptor {
            Q_OBJECT
            Q_CLASSINFO("D-Bus Interface","org.thesii.Wintermute.Data.RuleAdaptor")

            public:
                explicit RuleAdaptor();

            signals:
                void backendRegistered(const QString&);
                void ruleCreated(const QString&);

            public slots:
                void registerBackend(const QDBusMessage&, Rules::Backend&);
                void read(const QDBusMessage&, Rules::Chain&) const;
                void write(const Rules::Chain&, const QDBusMessage&);
                const bool exists(const QString&, const QString&,const QDBusMessage&) const;
                virtual void quit(const QDBusMessage&) const;
        };
    }
}

#endif // ADAPTORS_HPP

/**
 * @file adaptors.hpp
 * @author Wintermute Developers <wintermute-devel@lists.launchpad.net>
 * @date Sun, 30 Oct 2011 21:54:16
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

#ifndef ADAPTORS_HPP
#define ADAPTORS_HPP

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include "models.hpp"

using namespace Wintermute::Data::Linguistics;

namespace Wintermute {
    namespace Data {
        class NodeAdaptor: public QDBusAbstractAdaptor {
            Q_OBJECT
            Q_CLASSINFO("D-Bus Interface", "org.thesii.Wintermute.Data.Nodes")
            Q_CLASSINFO("D-Bus Introspection", ""
        "  <interface name=\"org.thesii.Wintermute.Data.Nodes\">\n"
        "    <signal name=\"nodeCreated\">\n"
        "      <arg direction=\"out\" type=\"s\"/>\n"
        "    </signal>\n"
        "    <method name=\"generate\">\n"
        "      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
        "    </method>\n"
        "    <method name=\"quit\"/>\n"
        "    <method name=\"read\">\n"
        "      <arg direction=\"out\" type=\"s\"/>\n"
        "      <arg direction=\"in\" type=\"s\"/>\n"
        "    </method>\n"
        "    <method name=\"write\">\n"
        "      <arg direction=\"out\" type=\"s\"/>\n"
        "      <arg direction=\"in\" type=\"s\"/>\n"
        "    </method>\n"
        "    <method name=\"pseudo\">\n"
        "      <arg direction=\"out\" type=\"s\"/>\n"
        "      <arg direction=\"in\" type=\"s\"/>\n"
        "    </method>\n"
        "    <method name=\"exists\">\n"
        "      <arg direction=\"out\" type=\"b\"/>\n"
        "      <arg direction=\"in\" type=\"s\"/>\n"
        "    </method>\n"
        "    <method name=\"isPseudo\">\n"
        "      <arg direction=\"out\" type=\"b\"/>\n"
        "      <arg direction=\"in\" type=\"s\"/>\n"
        "    </method>\n"
        "  </interface>\n"
                "")
        public:
            NodeAdaptor();
            virtual ~NodeAdaptor();

        public: // PROPERTIES
        public Q_SLOTS: // METHODS
            bool exists(QString in0);
            Q_NOREPLY void generate();
            bool isPseudo(QString in0);
            QString pseudo(QString in0);
            void quit();
            QString read(QString in0);
            QString write(QString in0);
        Q_SIGNALS: // SIGNALS
            void nodeCreated(const QString &in0);
        };

        class RuleAdaptor: public QDBusAbstractAdaptor {
            Q_OBJECT
            Q_CLASSINFO("D-Bus Interface", "org.thesii.Wintermute.Data.Rules")
            Q_CLASSINFO("D-Bus Introspection", ""
        "  <interface name=\"org.thesii.Wintermute.Data.Rules\">\n"
        "    <signal name=\"ruleCreated\">\n"
        "      <arg direction=\"out\" type=\"s\"/>\n"
        "    </signal>\n"
        "    <method name=\"write\">\n"
        "      <arg direction=\"out\" type=\"Wintermute::Data::Linguistics::Rules::Chain\"/>\n"
        "      <arg direction=\"in\" type=\"Wintermute::Data::Linguistics::Rules::Chain\"/>\n"
        "      <annotation value=\"Wintermute::Data::Linguistics::Rules::Chain\" name=\"com.trolltech.QtDBus.QtTypeName.In0\"/>\n"
        "      <annotation value=\"Wintermute::Data::Linguistics::Rules::Chain\" name=\"com.trolltech.QtDBus.QtTypeName.Out0\"/>\n"
        "    </method>\n"
        "    <method name=\"read\">\n"
        "      <arg direction=\"out\" type=\"Wintermute::Data::Linguistics::Rules::Chain\"/>\n"
        "      <arg direction=\"in\" type=\"Wintermute::Data::Linguistics::Rules::Chain\"/>\n"
        "      <annotation value=\"Wintermute::Data::Linguistics::Rules::Chain\" name=\"com.trolltech.QtDBus.QtTypeName.In0\"/>\n"
        "      <annotation value=\"Wintermute::Data::Linguistics::Rules::Chain\" name=\"com.trolltech.QtDBus.QtTypeName.Out0\"/>\n"
        "    </method>\n"
        "    <method name=\"exists\">\n"
        "      <arg direction=\"out\" type=\"b\"/>\n"
        "      <arg direction=\"in\" type=\"s\"/>\n"
        "      <arg direction=\"in\" type=\"s\"/>\n"
        "    </method>\n"
        "    <method name=\"quit\"/>\n"
        "  </interface>\n"
                "")
            public:
                RuleAdaptor();
                virtual ~RuleAdaptor();

            public: // PROPERTIES
            public Q_SLOTS: // METHODS
                bool exists(const QString &in0, const QString &in1);
                void quit();
                Rules::Chain read(Rules::Chain in0);
                Rules::Chain write(Rules::Chain in0);
            Q_SIGNALS: // SIGNALS
                void ruleCreated(const QString &in0);
        };

        class SystemAdaptor: public QDBusAbstractAdaptor {
            Q_OBJECT
            Q_CLASSINFO("D-Bus Interface", "org.thesii.Wintermute.Data.System")
            Q_CLASSINFO("D-Bus Introspection", ""
        "  <interface name=\"org.thesii.Wintermute.Data.System\">\n"
        "    <property access=\"readwrite\" type=\"s\" name=\"Directory\"/>\n"
        "    <signal name=\"stopped\"/>\n"
        "    <signal name=\"started\"/>\n"
        "    <method name=\"quit\"/>\n"
        "    <method name=\"stop\">\n"
        "      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
        "    </method>\n"
        "    <method name=\"start\">\n"
        "      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
        "    </method>\n"
        "    <method name=\"localeExists\">\n"
        "      <arg direction=\"out\" type=\"b\"/>\n"
        "      <arg direction=\"in\" type=\"s\"/>\n"
        "    </method>\n"
        "  </interface>\n"
                "")
        public:
            SystemAdaptor();
            virtual ~SystemAdaptor();

        public: // PROPERTIES
            Q_PROPERTY(QString Directory READ directory WRITE setDirectory)
            QString directory() const;
            void setDirectory(const QString &value);

        public Q_SLOTS: // METHODS
            bool localeExists(const QString &in0);
            void quit();
            Q_NOREPLY void start();
            Q_NOREPLY void stop();
        Q_SIGNALS: // SIGNALS
            void started();
            void stopped();
        };
    }
}
#endif

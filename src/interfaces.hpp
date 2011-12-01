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

#ifndef INTERFACES_HPP
#define INTERFACES_HPP

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>
#include "models.hpp"
#include "config.hpp"

using namespace Wintermute::Data::Linguistics;

namespace Wintermute {
    namespace Data {
        struct NodeInterface;
        struct RuleInterface;
        struct SystemInterface;

        class NodeInterface: public QDBusAbstractInterface {
            Q_OBJECT

            public:
                static inline const char *staticInterfaceName()
                { return "org.thesii.Wintermute.Data.Nodes"; }
                NodeInterface();

            ~NodeInterface();

            public slots:
                inline QDBusPendingReply<bool> exists(Lexical::Data in0) {
                    QList<QVariant> argumentList;
                    argumentList << qVariantFromValue(in0);
                    return asyncCallWithArgumentList(QLatin1String("exists"), argumentList);
                }

                inline Q_NOREPLY void generate() {
                    QList<QVariant> argumentList;
                    callWithArgumentList(QDBus::NoBlock, QLatin1String("generate"), argumentList);
                }

                inline QDBusPendingReply<bool> isPseudo(Lexical::Data in0) {
                    QList<QVariant> argumentList;
                    argumentList << qVariantFromValue(in0);
                    return asyncCallWithArgumentList(QLatin1String("isPseudo"), argumentList);
                }

                inline QDBusPendingReply<Lexical::Data> pseudo(Lexical::Data in0) {
                    QList<QVariant> argumentList;
                    argumentList << qVariantFromValue(in0);
                    return asyncCallWithArgumentList(QLatin1String("pseudo"), argumentList);
                }

                inline QDBusPendingReply<> quit() {
                    QList<QVariant> argumentList;
                    return asyncCallWithArgumentList(QLatin1String("quit"), argumentList);
                }

                inline QDBusPendingReply<Lexical::Data> read(Lexical::Data in0) {
                    QList<QVariant> argumentList;
                    argumentList << qVariantFromValue(in0);
                    return asyncCallWithArgumentList(QLatin1String("read"), argumentList);
                }

                inline QDBusPendingReply<Lexical::Data> write(Lexical::Data in0) {
                    QList<QVariant> argumentList;
                    argumentList << qVariantFromValue(in0);
                    return asyncCallWithArgumentList(QLatin1String("write"), argumentList);
                }

            Q_SIGNALS: // SIGNALS
                void nodeCreated(const QString &in0);
         };

        class RuleInterface: public QDBusAbstractInterface {
            Q_OBJECT

            public:
                static inline const char *staticInterfaceName()
                { return "org.thesii.Wintermute.Data.Rules"; }

                RuleInterface();
                ~RuleInterface();

            public slots:
                inline QDBusPendingReply<bool> exists(const QString &in0, const QString &in1) {
                    QList<QVariant> argumentList;
                    argumentList << qVariantFromValue(in0) << qVariantFromValue(in1);
                    return asyncCallWithArgumentList(QLatin1String("exists"), argumentList);
                }

                inline QDBusPendingReply<> quit() {
                    QList<QVariant> argumentList;
                    return asyncCallWithArgumentList(QLatin1String("quit"), argumentList);
                }

                inline QDBusPendingReply<RulesChain> read(RulesChain in0) {
                    QList<QVariant> argumentList;
                    argumentList << qVariantFromValue(in0);
                    return asyncCallWithArgumentList(QLatin1String("read"), argumentList);
                }

                inline QDBusPendingReply<RulesChain> write(RulesChain in0) {
                    QList<QVariant> argumentList;
                    argumentList << qVariantFromValue(in0);
                    return asyncCallWithArgumentList(QLatin1String("write"), argumentList);
                }

            signals:
                void ruleCreated(const QString &in0);
        };

        class SystemInterface: public QDBusAbstractInterface {
            Q_OBJECT
            public:
                static inline const char *staticInterfaceName()
                { return "org.thesii.Wintermute.Data.System"; }

                SystemInterface();
                ~SystemInterface();

            Q_PROPERTY(QString Directory READ directory WRITE setDirectory)

            inline QString directory() const
            { return qvariant_cast< QString >(property("Directory")); }
            inline void setDirectory(const QString &value)
            { setProperty("Directory", qVariantFromValue(value)); }

            public slots:
                inline QDBusPendingReply<bool> localeExists(const QString &in0) {
                    QList<QVariant> argumentList;
                    argumentList << qVariantFromValue(in0);
                    return asyncCallWithArgumentList(QLatin1String("localeExists"), argumentList);
                }

                inline QDBusPendingReply<> quit() {
                    QList<QVariant> argumentList;
                    return asyncCallWithArgumentList(QLatin1String("quit"), argumentList);
                }

                inline Q_NOREPLY void start() {
                    QList<QVariant> argumentList;
                    callWithArgumentList(QDBus::NoBlock, QLatin1String("start"), argumentList);
                }

                inline Q_NOREPLY void stop() {
                    QList<QVariant> argumentList;
                    callWithArgumentList(QDBus::NoBlock, QLatin1String("stop"), argumentList);
                }

            signals:
                void started();
                void stopped();
        };
    }
}

#endif

/**
 * @file adaptors.hpp
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

#include "adaptors.hpp"
#include "wntrdata.hpp"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

namespace Wintermute {
namespace Data {
NodeAdaptor::NodeAdaptor()
        : QDBusAbstractAdaptor(NodeManager::instance()) {
    // constructor
    setAutoRelaySignals(true);
}

NodeAdaptor::~NodeAdaptor() { }

bool NodeAdaptor::exists(QString in0) {
    return NodeManager::instance()->exists(Lexical::Data::fromString(in0));
}

void NodeAdaptor::generate() {
    QMetaObject::invokeMethod(parent(), "generate");
}

bool NodeAdaptor::isPseudo(QString in0) {
    bool out0;
    QMetaObject::invokeMethod(parent(), "isPseudo", Q_RETURN_ARG(bool, out0), Q_ARG(Lexical::Data, Lexical::Data::fromString(in0)));
    return out0;
}

QString NodeAdaptor::pseudo(QString in0) {
    Lexical::Data l_dt = Lexical::Data::fromString(in0);
    return NodeManager::instance()->pseudo(l_dt).toString();
}

void NodeAdaptor::quit() {
    QMetaObject::invokeMethod(parent(), "quit");
}

QString NodeAdaptor::read(QString in0) {
    Lexical::Data out0;
    QMetaObject::invokeMethod(parent(), "read", Q_RETURN_ARG(Lexical::Data, out0), Q_ARG(Lexical::Data, Lexical::Data::fromString(in0)));
    return out0.toString();
}

QString NodeAdaptor::write(QString in0) {
    Lexical::Data out0;
    QMetaObject::invokeMethod(parent(), "write", Q_RETURN_ARG(Lexical::Data, out0), Q_ARG(Lexical::Data, Lexical::Data::fromString(in0)));
    return out0.toString();
}

RuleAdaptor::RuleAdaptor()
        : QDBusAbstractAdaptor(RuleManager::instance()) {
    setAutoRelaySignals(true);
}

RuleAdaptor::~RuleAdaptor() { }

bool RuleAdaptor::exists(const QString &in0, const QString &in1) {
    bool out0;
    QMetaObject::invokeMethod(parent(), "exists", Q_RETURN_ARG(bool, out0), Q_ARG(QString, in0), Q_ARG(QString, in1));
    return out0;
}

void RuleAdaptor::quit() {
    QMetaObject::invokeMethod(parent(), "quit");
}

QString RuleAdaptor::read(QString in0) {
    Rules::Chain l_chn = Rules::Chain::fromString(in0);
    RuleManager::instance()->read(l_chn);
    return l_chn.toString();
}

QString RuleAdaptor::write(QString in0) {
    Rules::Chain l_chn = Rules::Chain::fromString(in0);
    RuleManager::instance()->write(l_chn);
    return l_chn.toString();
}


SystemAdaptor::SystemAdaptor()
        : QDBusAbstractAdaptor(System::instance()) {
    // constructor
    setAutoRelaySignals(true);
}

SystemAdaptor::~SystemAdaptor() { }

QString SystemAdaptor::directory() const {
    return qvariant_cast< QString >(parent()->property("Directory"));
}

void SystemAdaptor::setDirectory(const QString &value) {
    parent()->setProperty("Directory", qVariantFromValue(value));
}

bool SystemAdaptor::localeExists(const QString &in0) {
    bool out0;
    QMetaObject::invokeMethod(parent(), "localeExists", Q_RETURN_ARG(bool, out0), Q_ARG(QString, in0));
    return out0;
}

void SystemAdaptor::quit() {
    QMetaObject::invokeMethod(parent(), "quit");
}

void SystemAdaptor::start() {
    QMetaObject::invokeMethod(parent(), "start");
}

void SystemAdaptor::stop() {
    QMetaObject::invokeMethod(parent(), "stop");
}
}
}

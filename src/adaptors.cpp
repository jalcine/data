#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QDebug>
#include "models.hpp"
#include "adaptors.hpp"
#include "wntrdata.hpp"

namespace Wintermute {
    namespace Data {
        NodeAdaptor::NodeAdaptor(): QDBusAbstractAdaptor(NodeManager::instance()) {
            setAutoRelaySignals(true);
        }

        NodeAdaptor::~NodeAdaptor() { }

        bool NodeAdaptor::exists(LexicalData in0) {
            bool out0;
            QMetaObject::invokeMethod(parent(), "exists", Q_RETURN_ARG(bool, out0), Q_ARG(LexicalData, in0));
            return out0;
        }

        void NodeAdaptor::generate() {
            QMetaObject::invokeMethod(parent(), "generate");
        }

        bool NodeAdaptor::isPseudo(LexicalData in0) {
            bool out0;
            QMetaObject::invokeMethod(parent(), "isPseudo", Q_RETURN_ARG(bool, out0), Q_ARG(LexicalData, in0));
            return out0;
        }

        void NodeAdaptor::quit() {
            QMetaObject::invokeMethod(parent(), "quit");
        }

        LexicalData NodeAdaptor::read(LexicalData in0) {
            /*LexicalData out0;
            QMetaObject::invokeMethod(parent(), "read", Q_RETURN_ARG(LexicalData, out0), Q_ARG(LexicalData, in0));
            return out0;*/
            NodeManager::instance()->read(in0);
            qDebug() << "(data) [NodeAdaptor]" << in0;
            return in0;
        }

        LexicalData NodeAdaptor::write(LexicalData in0) {
            LexicalData out0;
            QMetaObject::invokeMethod(parent(), "write", Q_RETURN_ARG(LexicalData, out0), Q_ARG(LexicalData, in0));
            return out0;
        }

        RuleAdaptor::RuleAdaptor() : QDBusAbstractAdaptor(RuleManager::instance()) {
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

        RulesChain RuleAdaptor::read(RulesChain in0) {
            RulesChain out0;
            QMetaObject::invokeMethod(parent(), "read", Q_RETURN_ARG(RulesChain, out0), Q_ARG(RulesChain, in0));
            return out0;
        }

        RulesChain RuleAdaptor::write(RulesChain in0) {
            RulesChain out0;
            QMetaObject::invokeMethod(parent(), "write", Q_RETURN_ARG(RulesChain, out0), Q_ARG(RulesChain, in0));
            return out0;
        }

        SystemAdaptor::SystemAdaptor() : QDBusAbstractAdaptor(System::instance()) {
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

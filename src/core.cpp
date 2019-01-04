#include "core.hpp"

Core Core::core;

Core::Core(QObject *parent) :
    QObject(parent), m_settings("QAria2", QSettings::NativeFormat, this) {
    QStringList aria2_args;
    aria2_args << "--no-conf"
               << "--enable_rpc=true"
               << "--rpc-listen-all=false";

#define PROP(key, defaultVal, type) \
    m_settings.value(key, defaultVal).to##type()
#define ARG(key, defaultVal, type) \
    QString("--" key "=%1").arg(PROP(key, defaultVal, type))

    m_settings.beginGroup("backend");
    aria2_args << ARG("listen-port", "6881-6999", String)
               << ARG("enable-dht", true, Bool)
               << ARG("dht-file-path", "dht.dat", String)
               << ARG("disable-ipv6", true, Bool)
               << ARG("check-certificate", false, Bool)
               << ARG("user-agent", "Transmission/2.77", String)
               << ARG("max-overall-upload-limit", 20000, Int)
               << ARG("seed-time", 60, Int)
               << ARG("follow-torrent", false, Bool)
               << ARG("follow-metalink", false, Bool)
               << ARG("human-readable", true, Bool);
    m_settings.endGroup();
}

void Core::setup() {

}

Core& Core::instance() noexcept {
    return core;
}

QSettings& Core::settings() noexcept {
    return instance().m_settings;
}

#include "moc_core.cpp"

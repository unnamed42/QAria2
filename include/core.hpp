#ifndef CORE_HPP
#define CORE_HPP

#include <QObject>
#include <QProcess>
#include <QSettings>

class Core : public QObject {
    Q_OBJECT
private:
    static Core core;
private:
    explicit Core(QObject *parent = nullptr);
    ~Core() = default;
public:
    void startDaemon();
signals:
public:
    static void setup();
    static Core&      instance() noexcept;
    static QSettings& settings() noexcept;
private:
    QProcess m_aria2;
    QSettings m_settings;
};

#endif // CORE_HPP

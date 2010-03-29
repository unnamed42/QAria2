// karia2.h --- 
// 
// Created: 2010-03-29 13:13:24 +0800
// Version: $Id$
// 
#ifndef KARIA2_H
#define KARIA2_H

#include <QSystemTrayIcon>
#include <QProcess>

#include "ui_karia2.h"
 
class karia2 : public QMainWindow, private Ui::karia2
{
    Q_OBJECT;
public:
    karia2();
    ~karia2();

private slots:
	void openurl();
	void about();
	void segn();
	void destinazione();
	void update();
	void display();
	void remove();
	void stop();
    bool stopFriendly();
	void resume();
	void setSettingsDirty();
	void settings();

    void logReadStdout();
    void logReadStderr();
    void procFinished(int exitCode, QProcess::ExitStatus exitStatus);

protected:
    virtual void closeEvent(QCloseEvent *event);

private:
    QSystemTrayIcon *trayIcon;
    QProcess aria2;
    /*private slots:
      void setIcon(int index);
      void iconActivated(QSystemTrayIcon::ActivationReason reason);
      void showMessage();
      void messageClicked();
      private:
      QSystemTrayIcon *trayIcon;
      QMenu *trayIconMenu;
    */
};
 
 
 
#endif 

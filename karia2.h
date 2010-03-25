#ifndef KARIA2_H
#define KARIA2_H
#include "ui_karia2.h"
#include <QSystemTrayIcon>
#include <QProcess>
 
class karia2 : public QMainWindow, private Ui::karia2
{
    Q_OBJECT
 
public:
    karia2();
 
 
private slots:
	void openurl();
	void about();
	void segn();
	void destinazione();
	void update();
	void display();
	void remove();
	void stop();
	void resume();
	void setSettingsDirty();
	void settings();
private:
    QProcess aria2;
/*private slots:
      void setIcon(int index);
     void iconActivated(QSystemTrayIcon::ActivationReason reason);
     void showMessage();
     void messageClicked();
private:
QSystemTrayIcon *trayIcon;
QMenu *trayIconMenu;*/
};
 
 
 
#endif 

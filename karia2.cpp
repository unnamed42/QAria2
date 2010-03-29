// karia2.cpp --- 
// 
// Created: 2010-03-29 13:13:05 +0800
// Version: $Id$
// 

#ifdef Q_OS_WIN
#else
#include <signal.h>
#endif
#include <errno.h>
#include <fcntl.h>
#include <iostream>

#include <QtCore>
#include <QtGui> 
#include <QTextCodec>

#include "karia2.h"
#include"settings.h"

// if we include <QtGui> there is no need to include every class used: <QString>, <QFileDialog>,...
 
karia2::karia2()
{
    QPixmap splashpng(":/images/splash.png");
    QSplashScreen *splash = new QSplashScreen(splashpng);
    splash->show();
    splash->showMessage(tr("Loading GUI..."));
    setupUi(this); // this sets up GUI
    
    settings();
    // signals/slots mechanism in action

	connect( actionAbout, SIGNAL( triggered() ), this, SLOT( about() ) );
	connect( actionOpen_URL, SIGNAL( triggered() ), this, SLOT( openurl() ) );
	connect( actionDestination_folder, SIGNAL( triggered() ), this, SLOT( destinazione() ) );
	connect( pushButton_2, SIGNAL( clicked() ), this, SLOT( destinazione() ) );
	connect( pushButton, SIGNAL( clicked() ), this, SLOT( remove() ) );
	connect( pushButton_4, SIGNAL( clicked() ), this, SLOT( stop() ) );
	connect( pushButton_5, SIGNAL( clicked() ), this, SLOT( openurl() ) );
	connect( pushButton_6, SIGNAL( clicked() ), this, SLOT( resume() ) );

    QTimer *timer = new QTimer(this);       //Qui viene inizializzato il timer che 
	connect( timer, SIGNAL(timeout()), this, SLOT( update() ) );  //esegue lo slot update()
	timer->start(2000);  //il tempo di aggiornamento è 2 secondi visto che deve cambiare solo quando si cambiano le opzioni

    QTimer *timer2 = new QTimer(this);       //Qui viene inizializzato il timer che 
	connect( timer2, SIGNAL(timeout()), this, SLOT(display()));  //esegue lo slot display() dello stato scaricamento file
	// timer2->start(1000);// il tempo di aggiornamento è settato ad 1 secondo.

    trayIcon = new QSystemTrayIcon();
    QIcon icon = QIcon(":/images/default.png");
    trayIcon->setIcon(icon);
    trayIcon->show();
    trayIcon->setToolTip(tr("karia2 download manager"));

	connect(spinBox,SIGNAL( valueChanged(int)), this, SLOT ( segn() ) );//quando cambio il valore dello spinbox,viene emesso un segnale che avvia segn()
	connect(spinBox,SIGNAL( valueChanged(int)), this, SLOT ( update() ) );
	connect(dockWidget, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)),this, SLOT(setSettingsDirty()));

    // read output of child process
    QObject::connect(&this->aria2, SIGNAL(readyReadStandardOutput()),
                     this, SLOT(logReadStdout()));
    QObject::connect(&this->aria2, SIGNAL(readyReadStandardError()),
                     this, SLOT(logReadStderr()));
    QObject::connect(&this->aria2, SIGNAL(finished(int, QProcess::ExitStatus)),
                     this, SLOT(procFinished(int, QProcess::ExitStatus)));

    splash->finish(this);
    delete splash;
}

karia2::~karia2()
{
    delete this->trayIcon;
}

void karia2::update()   //aggiorna lo stato delle opzioni
{
	label->clear();
    QDir Home = QDir::home();
    QFile filedestination;
    QFile filedestination2;
    Home.cd(".karia2");
    QDir::setCurrent( Home.path() );
    filedestination.setFileName("dest"); 
    filedestination2.setFileName("seg");
    filedestination.open(QIODevice::ReadOnly); 
    filedestination2.open(QIODevice::ReadOnly); 
	
    QString str1, str2, str3;
    str1 = (filedestination.readAll());
    str1.prepend("Destination: ");
    str2 = (filedestination2.readAll());
    str2.prepend("Segments: ");
    str3 = str1 + "   " + str2;
    label->setText(str3);
}


void karia2::display() //aggiorna lo stato dello scaricamento del file
{
    textBrowser->clear();
    QDir Home = QDir::home();
	QFile filedestination;
	QFile filename;

	filedestination.setFileName( Home.path() + "/.karia2/status");
	filename.setFileName( Home.path() + "/.karia2/url"); 
	filedestination.open(QIODevice::ReadOnly);
	filename.open(QIODevice::ReadOnly);

	QString str4, str5, str6;
	str4 = (filename.readAll());
	str5 = (filename.readAll());
	str6 = str4 + "   " + str5;
	str4.prepend(tr("Downloading..."));
	textBrowser->setPlainText(str6);
}

//////////////////////////////////////////////////////////////////////////////////////////
void karia2::openurl() //aprire un url con l'indirizzo al file da scaricare
{
    QDir Home = QDir::home();
	QFile filedestination;
	QFile filedestination2;

	filedestination.setFileName( Home.path() + "/.karia2/dest"); 
	filedestination2.setFileName( Home.path() + "/.karia2/seg");
	filedestination.open(QIODevice::ReadOnly); 
	filedestination2.open(QIODevice::ReadOnly); 
	QString str1, str2;
	str1 = (filedestination.readAll());
	str2 = (filedestination2.readAll());
	str2.prepend("-s ");
	str2.append(" ");

    str2.prepend(" ");
    str2.prepend("--max-overall-download-limit=3K");

	bool ok;//qinputdialog
    QStringList argList;
	QString text = QInputDialog::getText(this, tr("karia2 Open Url..."),
                                         tr("URL:"), QLineEdit::Normal,
                                         "http://", &ok);
	if (ok && !text.isEmpty()) {
        // aria2.startDetached("aria2c",QStringList() << (str2)<< (text) , str1 );//avvio aria2c nella cartella dettata da str1 con url=text 
        argList<<str2<<text;
        this->aria2.setWorkingDirectory(str1);
        this->aria2.start("aria2c", argList, QIODevice::ReadOnly);
        //TODO:aggiungere stringa relativa alle info nel display text browser.
        listWidget->insertItem( 1, tr("Downloading: " ) + text + tr("seg ") + str2 );

        this->aria2.setProperty("params", argList);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////


void karia2::destinazione()  //opzione per scegliere la cartella di destinazione per i file da scaricare
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("karia2 Set Download Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    QDir Home = QDir::home();  //punto alla cartella home dell'utente
	QFile filedestination;     //dichiaro un file e lo chiamo filedestination
	Home.cd(".karia2");	   //mi sposto alla sottodirectory .karia2 nella home
	QDir::setCurrent( Home.path() ); //dichiaro che la mia cartella attuale (.karia2) è da ora la mia home
	filedestination.setFileName("dest"); //associo alla variabile filedestination di tipo file, il file reale dest con l'informazione sulla destinazione dove scaricare i file

    if (!filedestination.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&filedestination);
    out << (dir);
}

void karia2::segn()
{

    int value = spinBox->value(); //dichiaro la variabile value che contiene il valore dello spinbox
    QString textval;//dichiaro la variabile textval di tipo qstring
    textval.setNum(value); //la stringa textval prende il numero value e lo converte in stringa
    //QMessageBox::about(this, "About AcetoneISO2 Pro",textval);

    QDir Home = QDir::home();  //punto alla cartella home dell'utente
	QFile filedestination2;     //dichiaro un file e lo chiamo filedestination2 che conterrà il numero di segmenti
	Home.cd(".karia2");	   //mi sposto alla sottodirectory .karia2 nella home
	QDir::setCurrent( Home.path() ); //dichiaro che la mia cartella attuale (.karia2) è da ora la mia home
	filedestination2.setFileName("seg"); //associo alla variabile filedestination di tipo file, il file reale dest con l'informazione sulla destinazione dove scaricare i file

    if (!filedestination2.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&filedestination2);
    out << (textval);
}

void karia2::remove()
{
    textBrowser->clear();
    textBrowser->append(tr("Stoping Downloading ..."));
    // system("rm -rf $HOME/.karia2/url; rm -rf $HOME/.karia2/status");
    if (this->aria2.state() == QProcess::Starting || this->aria2.state() == QProcess::Running) {
        this->aria2.kill();
    }
    QFile::remove(QDir::homePath() + "./karia2/url");
    QFile::remove(QDir::homePath() + "./karia2/status");
}

void karia2::stop()
{
    //system("killall aria2c&");
    // QMessageBox::about(this, "About karia2","ciao");
    if (this->aria2.state() == QProcess::Starting || this->aria2.state() == QProcess::Running) {
        QStringList argList = this->aria2.property("params").toStringList();

        #ifdef Q_OS_WIN
        this->aria2.kill();
        #else
        this->stopFriendly();
        #endif

        QFile procParams;
        procParams.setFileName(QDir::homePath() + "/.karia2/resume_params");
        procParams.open(QIODevice::ReadWrite);
        QDataStream out(&procParams);
        out<<argList;
    } else {
        // aria2c not started
    }
}

bool karia2::stopFriendly()
{
    int rv = 0;

#ifdef Q_OS_WIN
    qDebug()<<"Not impled";
#else
    rv = kill(this->aria2.pid(), SIGINT);
    if (rv != 0) {
        qDebug()<<"Can not stop the process: "<<this->aria2.pid()
                <<strerror(errno);
    } else {
        return true;
    }
#endif

    return false;
}

void karia2::resume()
{
    QString resumeInfoFile = QDir::homePath() + "/.karia2/resume_params";
    if (QFile(resumeInfoFile).exists()) {
        QFile procParams(resumeInfoFile);
        procParams.open(QIODevice::ReadOnly);
        QDataStream in(&procParams);
        QStringList argList;
        in>>argList;
        if (argList.count() > 0) {
            argList << "-c";

            this->aria2.start("aria2c", argList, QIODevice::ReadOnly);
            this->textBrowser->append(tr("Resume downloading: " ) + argList.join(" "));
        }
        //
        QFile::remove(resumeInfoFile);
    } else {
        qDebug()<<"No resume task exists";
    }
    // system("/opt/karia2/resume&");
}

void karia2::setSettingsDirty()
{
    QDir Home = QDir::home();  //punto alla cartella home dell'utente
    Home.cd(".karia2");
    QDir::setCurrent( Home.path() ); 
    QFile widgetstat;
    QString strWStat;
    widgetstat.setFileName("widgetstat");
    widgetstat.open(QIODevice::ReadWrite| QIODevice::Text);

    strWStat = (widgetstat.readAll()); //legge il contenuto del file widgetstat.se 0 sinistra, se 1 destra.
    int Wstat;
    Wstat = strWStat.toInt();
    if (Wstat == 0) {
        widgetstat.reset();
        QTextStream out(&widgetstat);
        out << "1";
    } else {
        widgetstat.reset();
        QTextStream out(&widgetstat);
        out << "0";
    }
}

void karia2::about()
{
    QMessageBox::about(this, "About karia2",
                       "karia2 is a Qt4 gui for aria2 download manager\n\n"
                       "karia2 is published under the terms of \n"
                       "the GPL v.3  \n"
                       "Author: Fabrizio Di Marco 2007-2010\n"
                       "       drswinghead 2010\n\n"
                       "For technical support, mailto: fdmarco3@gmail.com\n"
                       "or visit: http://sourceforge.net/projects/karia2\n");
}

void karia2::logReadStdout()
{
    QString logLine;
    while(this->aria2.bytesAvailable() > 0) {
        if (this->aria2.canReadLine()) {
            logLine = this->aria2.readLine();
        } else {
            logLine = this->aria2.readAll();
        }
        this->textBrowser->append(logLine);
    }
}
void karia2::logReadStderr()
{
    
}
void karia2::procFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QString resumeInfoFile = QDir::homePath() + "/.karia2/resume_params";
    
    switch (exitCode) {
    case 0:
        QFile::remove(resumeInfoFile);
        break;
    default:
        if (exitStatus == QProcess::NormalExit) {
            QFile::remove(resumeInfoFile);
        } else {
        }
        break;
    };
}

void karia2::closeEvent(QCloseEvent *event)
{
    this->stop();
    QMainWindow::closeEvent(event);
}




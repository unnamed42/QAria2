// settings.h --- 
// 
// Created: 2010-03-29 13:13:49 +0800
// Version: $Id$
// 

void karia2::settings()
{
	QDir Home = QDir::home();  //punto alla cartella home dell'utente
	QFile filedestination;     //dichiaro un file e lo chiamo filedestination
	QFile filedestination2;
	QFile widgetstat;

	Home.mkdir(Home.path() + "/.karia2");
	QDir kariaHome( Home.path() + "/.karia2" );
	filedestination.setFileName( kariaHome.path() + "/dest");
	filedestination2.setFileName( kariaHome.path() + "/seg");
	widgetstat.setFileName( kariaHome.path() + "/widgetstat");

	if (!filedestination2.exists()) {
		//if file seg does not exists, it will be created 		
        if (!filedestination2.open(QIODevice::WriteOnly | QIODevice::Text)) {
            return;
        }

        QTextStream out(&filedestination2);
		out << "1"; //default 1 single segment
    }

	if (!widgetstat.exists()) {
		if (!widgetstat.open(QIODevice::WriteOnly | QIODevice::Text))
         	return;

        QTextStream out(&widgetstat);
        out << "0";
    }

	if (!filedestination.exists()) {
		if (!filedestination.open(QIODevice::WriteOnly | QIODevice::Text))
         	return;

        QTextStream out(&filedestination);
		Home.cd("..");
		QDir::setCurrent( Home.path() );
		out << (Home.path());
    }

	filedestination.open(QIODevice::ReadOnly); // apro il file dichiarato prima in sola lettura
	filedestination2.open(QIODevice::ReadOnly);
	QString str1, str2, str3, strWStat;
	str1 = (filedestination.readAll());
	str1.prepend("Destination: ");
	str2 = (filedestination2.readAll());
	int segNumber;
	segNumber = str2.toInt();
	spinBox->setValue(segNumber);

	str2.prepend("Segments: ");
	str3 = str1 + "\t " + str2;
	label->setText(str3);

    widgetstat.open(QIODevice::ReadOnly);
    strWStat = (widgetstat.readAll());  //legge il contenuto del file widgetstat.se 0 sinistra, se 1 destra.
    int Wstat;
    Wstat = strWStat.toInt();
    if (Wstat == 1) {
        addDockWidget(Qt::RightDockWidgetArea, dockWidget);// se il file di configurazione widgetstat contiene 1, il widget è a destra.
    }

}
 

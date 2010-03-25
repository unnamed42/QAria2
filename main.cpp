#include <QApplication>
 #include "karia2.h"
 
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    karia2 *dialog = new karia2();
 
    dialog->show();
    return app.exec();
}

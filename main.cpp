#include <QtGui/QApplication>
#include "openCVStudents.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    openCVStudents opencvstudents;
    opencvstudents.show();
    return app.exec();
}

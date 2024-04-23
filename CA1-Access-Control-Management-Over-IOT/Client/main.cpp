#include "cpsapplication.h"

#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication qApplication(argc, argv);

    CPS::Application application;
    application.show();

    return qApplication.exec();
}

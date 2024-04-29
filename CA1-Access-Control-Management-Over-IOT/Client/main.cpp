#include <QApplication>

#include "cpsapplication.h"

int main(int argc, char* argv[]) {
    QApplication qApplication(argc, argv);

    qSetMessagePattern("[%{type}] %{appname}: %{message}");

    CPS::Application application;
    application.show();

    return qApplication.exec();
}

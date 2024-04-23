#include "cpsapplication.h"

#include <QJsonArray>
#include <QJsonObject>

#include "cpshistorywindow.h"
#include "cpsmainwindow.h"
#include "cpswindowsapitools.h"

namespace CPS {

Application::Application(QObject* parent)
    : QObject(parent),
      window_(new MainWindow),
      history_(new HistoryWindow) {
    setWindowsThemeToDark<MainWindow>(*window_);

    QObject::connect(window_, &MainWindow::historyBtnClicked, this, &Application::showHistoryWindow);

    // TODO:
    // QObject::connect(&YourSocketClassInstance, &YourSocketClass::newUser, &window, &MainWindow::showUserDetails);
    // QObject::connect(&window, &MainWindow::connectBtnClicked, &YourSocketClassInstance, &YourSocketClass::connectToServer);
    // QObject::connect(&YourSocketClassInstance, &YourSocketClass::connectionChanged, &window, &MainWindow::changeRightPanelEnabled);
}

Application::~Application() {
    delete this->window_;
    delete this->history_;

    // TODO:
    // delete this->YourSocketClassInstance_;
}

void Application::show() {
    this->window_->show();
}

void Application::showHistoryWindow() {
    setWindowsThemeToDark<HistoryWindow>(*history_);

    // TODO:
    /*
     * fetch data from server and show it in history window.
     * your data must be in QJsonArray format.
     * something like this:
     *
     * [
     *     {
     *          username: string,
     *          date: string,
     *          time: string,
     *     },
     *
     *     {
     *          username: string,
     *          date: string,
     *          time: string,
     *     }
     * ]
     *
     *  below is an example of how to create a QJsonArray from QVariantList: (beginner level)
     *  please erase this horrible example and implement your own logic.
     *  you must fetch a json from server
     *
     * */

    QJsonObject obj1;
    QJsonObject obj2;
    QJsonObject obj3;

    obj1["username"] = "1234567890";
    obj1["date"] = "12/12/2024";
    obj1["time"] = "10:00";

    obj2["username"] = "0987654321";
    obj2["date"] = "03/28/2024";
    obj2["time"] = "12:00";

    obj3["username"] = "5432167890";
    obj3["date"] = "09/08/2024";
    obj3["time"] = "14:00";

    QVariantList list;
    list.append(obj1);
    list.append(obj2);
    list.append(obj3);

    QJsonArray data = QJsonArray::fromVariantList(list);

    history_->show(data);
}

} // namespace CPS

#include "cpsapplication.h"

#include <QJsonArray>
#include <QJsonObject>

#include "websocket/cpswebsocketclient.h"
#include "windows/cpshistorywindow.h"
#include "windows/cpsmainwindow.h"
#include "windows/cpswindowsapitools.h"

namespace CPS {

Application::Application(QObject* parent)
    : QObject(parent),
      window_(new MainWindow),
      history_(new HistoryWindow),
      webSocketClient_(new WebSocketClient) {
    setWindowsThemeToDark<MainWindow>(*window_);
    setWindowsThemeToDark<HistoryWindow>(*history_);

    QObject::connect(window_, &MainWindow::connectBtnClicked, webSocketClient_, &WebSocketClient::connectToServer);
    QObject::connect(window_, &MainWindow::historyBtnClicked, webSocketClient_, &WebSocketClient::sendHistoryRequest);

    QObject::connect(webSocketClient_, &WebSocketClient::newUser, window_, &MainWindow::showUserDetails);
    QObject::connect(webSocketClient_, &WebSocketClient::historyReady, this, &Application::showHistoryWindow);
    QObject::connect(webSocketClient_, &WebSocketClient::connectionChanged, window_, &MainWindow::changeRightPanelEnabled);
}

Application::~Application() {
    delete this->window_;
    delete this->history_;
    delete this->webSocketClient_;
}

void Application::show() {
    this->window_->show();
}

void Application::showHistoryWindow(const QList<RfidUser>& history) {
    QJsonArray data;
    for (const RfidUser& item : history) {
        data << item.toJsonObject();
    }
    history_->show(data);
}

} // namespace CPS

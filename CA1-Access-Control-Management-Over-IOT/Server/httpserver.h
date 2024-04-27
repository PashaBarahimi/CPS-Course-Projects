#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QHttpServer>
#include <QObject>

#include "rfidauthenticator.h"

namespace CPS {

class HttpServer : public QObject {
    Q_OBJECT
public:
    HttpServer(int port, RfidAuthenticator* rfidAuthenticator, QObject* parent = nullptr);
    ~HttpServer();
    void start();
    void stop();
    QHttpServerResponse requestHandler(const QHttpServerRequest& request);

Q_SIGNALS:

public Q_SLOTS:

private:
    int port_;
    QHttpServer* server_;
    RfidAuthenticator* rfidAuthenticator_;
};

} // namespace CPS

#endif // HTTPSERVER_H

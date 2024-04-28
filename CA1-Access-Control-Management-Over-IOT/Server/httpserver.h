#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QHttpServer>

#include "rfidauthenticator.h"

namespace CPS {

class HttpServer : public QObject {
    Q_OBJECT
public:
    HttpServer(int port, RfidAuthenticator* rfidAuthenticator);
    ~HttpServer();
    void start();
    void stop();
    QHttpServerResponse authenticationHandler(const QHttpServerRequest& request);

private:
    int port_;
    QHttpServer* server_;
    RfidAuthenticator* rfidAuthenticator_;
};

} // namespace CPS

#endif // HTTPSERVER_H

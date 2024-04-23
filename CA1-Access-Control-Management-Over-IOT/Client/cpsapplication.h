#ifndef CPSAPPLICATION_H
#define CPSAPPLICATION_H

#include <QObject>

namespace CPS {

class MainWindow;
class HistoryWindow;

class Application : public QObject {
    Q_OBJECT
public:
    explicit Application(QObject* parent = nullptr);
    ~Application();

    void show();

private Q_SLOTS:
    void showHistoryWindow();

private:
    MainWindow* window_;
    HistoryWindow* history_;

    // TODO:
    // YourSocketClass *_YourSocketClassInstance;
};

} // namespace CPS

#endif // CPSAPPLICATION_H

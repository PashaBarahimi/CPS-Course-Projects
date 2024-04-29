#ifndef CPSHISTORYWINDOW_H
#define CPSHISTORYWINDOW_H

#include <QCloseEvent>
#include <QJsonArray>
#include <QListWidget>
#include <QWidget>

namespace CPS {

class HistoryWindow : public QWidget {
    Q_OBJECT
public:
    explicit HistoryWindow(QWidget* parent = nullptr);
    ~HistoryWindow();

    void show(const QJsonArray& array);

private:
    void closeEvent(QCloseEvent* event) override;

private:
    QListWidget* list_;
};

} // namespace CPS

#endif // CPSHISTORYWINDOW_H

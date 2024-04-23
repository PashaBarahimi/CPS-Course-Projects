#ifndef CPSMAINWINDOW_H
#define CPSMAINWINDOW_H

#include <QCloseEvent>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

namespace CPS {

class Button;
class Label;
class TextField;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void setTime(const QString& time);
    void setDate(const QString& date);
    void setUsername(const QString& username);

Q_SIGNALS:
    void historyBtnClicked();
    void connectBtnClicked(const QString& address, const QString& username, const QString& password);

public Q_SLOTS:
    void changeRightPanelEnabled(bool enabled);
    void showUserDetails(const QString& username, const QString& date, const QString& time);

private:
    void setupConnections();
    void setupGlobalStyle();
    void setupWindowLayout();
    void setupHeader();
    void setupRightPanel();
    void setupLeftPanel();
    void closeEvent(QCloseEvent* event) override;

private:
    Button* connectBtn_;
    TextField* addressInput_;
    TextField* usernameInput_;
    TextField* passwordInput_;

    Label* date_;
    Label* time_;
    Label* username_;
    Button* historyBtn_;

    QVBoxLayout* leftPanel_;
    QHBoxLayout* mainLayout_;
    QVBoxLayout* rightPanel_;

    const int windowMargin_ = 30;
};

} // namespace CPS

#endif // CPSMAINWINDOW_H

#include "cpsmainwindow.h"

#include <QColor>
#include <QPalette>

#include "../widgets/cpsbutton.h"
#include "../widgets/cpslabel.h"
#include "../widgets/cpstextfield.h"

namespace CPS {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      connectBtn_(new Button("اتصال به سرور", this)),
      addressInput_(new TextField("آدرس سوکت سرور", this)),
      usernameInput_(new TextField("نام کاربری", this)),
      passwordInput_(new TextField("رمز عبور", this)),

      date_(new Label("تاریخ:  __ / __ / ____", this)),
      time_(new Label("ساعت:  __ : __", this)),
      username_(new Label("شناسه کاربری:  __________", this)),
      historyBtn_(new Button("نمایش تاریخچه", this)),

      mainLayout_(new QHBoxLayout(this)),
      rightPanel_(new QVBoxLayout),
      leftPanel_(new QVBoxLayout) {
    this->setupConnections();
    this->setupGlobalStyle();
    this->setupWindowLayout();
    this->setupHeader();
    this->setupRightPanel();
    this->setupLeftPanel();
}

MainWindow::~MainWindow() {
    delete this->connectBtn_;
    delete this->addressInput_;
    delete this->usernameInput_;
    delete this->passwordInput_;
    delete this->username_;
    delete this->date_;
    delete this->time_;
    delete this->historyBtn_;
    delete this->mainLayout_;
    delete this->rightPanel_;
    delete this->leftPanel_;
}

void MainWindow::setTime(const QString& time) {
    time_->setText(QString("ساعت: %1").arg(time));
}

void MainWindow::setDate(const QString& date) {
    date_->setText(QString("تاریخ: %1").arg(date));
}

void MainWindow::setUsername(const QString& username) {
    username_->setText(QString("شناسه کاربری: %1").arg(username));
}

void MainWindow::changeRightPanelEnabled(bool enabled) {
    // rightPanel_->setEnabled(enabled);
    for (int i = 0; i < rightPanel_->count(); ++i) {
        QWidget* widget = rightPanel_->itemAt(i)->widget();
        if (widget != nullptr) {
            widget->setEnabled(enabled);
        }
    }
    connectBtn_->color(enabled);
}

void MainWindow::showUserDetails(const QString& username, const QString& date, const QString& time, bool denied) {
    this->setDate(date);
    if (denied) {
        this->setTime(time + "        (ناموفق!)");
    }
    else {
        this->setTime(time);
    }
    this->setUsername(username);
}

void MainWindow::setupConnections() {
    auto connectFn = [this]() {
        Q_EMIT connectBtnClicked(addressInput_->text(),
                                 usernameInput_->text(),
                                 passwordInput_->text());
    };

    connect(connectBtn_, &QPushButton::clicked, connectFn);
    connect(historyBtn_, &QPushButton::clicked, this, &MainWindow::historyBtnClicked);
}

void MainWindow::setupGlobalStyle() {
    this->setMinimumSize({720, 480});
    this->setMaximumSize({720, 480});
    this->setBackgroundRole(QPalette::Dark);
    this->setWindowIcon(QIcon("assets/UT.ico"));
    this->setWindowFlag(Qt::CustomizeWindowHint);
    this->setWindowTitle("IoT Monitoring System");
    this->setPalette(QPalette(QColor::fromString("#E0E0E0"), QColor::fromString("#121212")));
}

void MainWindow::setupWindowLayout() {
    mainLayout_->addLayout(rightPanel_);
    mainLayout_->addLayout(leftPanel_);
}

void MainWindow::setupHeader() {
    QFont font;
    font.setPixelSize(25);
    font.setBold(true);

    QLabel* headingLbl = new QLabel(this);
    headingLbl->setFont(font);
    headingLbl->setMargin(20);
    headingLbl->setAlignment(Qt::AlignHCenter);
    headingLbl->setText("سامانه کنترل عبور و مرور");
    headingLbl->setGeometry(0, 0, this->width(), 80);
}

void MainWindow::setupRightPanel() {
    int y = 80;
    int x = this->width() / 2 + windowMargin_ / 2;
    int width = this->width() / 2 - 1.5 * windowMargin_;
    int height = this->height() - y;

    passwordInput_->setEchoMode(QLineEdit::Password);

    rightPanel_->addWidget(addressInput_);
    rightPanel_->addWidget(usernameInput_);
    rightPanel_->addWidget(passwordInput_);
    rightPanel_->addWidget(connectBtn_);

    rightPanel_->setGeometry(QRect(x, y, width, height));
}

void MainWindow::setupLeftPanel() {
    int y = 80;
    int x = windowMargin_;
    int width = this->width() / 2 - 1.5 * windowMargin_;
    int height = this->height() - y;

    leftPanel_->addWidget(date_);
    leftPanel_->addWidget(time_);
    leftPanel_->addWidget(username_);
    leftPanel_->addWidget(historyBtn_);

    leftPanel_->setGeometry(QRect(x, y, width, height));
}

void MainWindow::closeEvent(QCloseEvent* event) {
    event->ignore();
    this->hide();
    event->accept();
    exit(0);
}

} // namespace CPS

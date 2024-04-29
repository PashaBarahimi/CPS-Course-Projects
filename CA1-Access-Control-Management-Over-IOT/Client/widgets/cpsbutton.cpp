#include "cpsbutton.h"

#include <QFont>

namespace CPS {

Button::Button(const QString& text, QWidget* parent)
    : QPushButton(parent) {
    QFont fontInput;
    fontInput.setPixelSize(20);
    fontInput.setBold(false);

    this->setMinimumHeight(70);
    this->setFont(fontInput);
    this->setText(text);
    this->color(true);
}

void Button::color(bool enabled) {
    if (enabled) {
        this->setStyleSheet(
            "QPushButton {"
            "  color: #E0E0E0;"
            "  background-color: #155F0F;"
            "}"
            "QPushButton:hover { background-color: #157F0F; }");
    }
    else {
        this->setStyleSheet(
            "QPushButton {"
            "  color: #E0E0E0;"
            "  background-color: #6D6D6D;"
            "}");
    }
}

} // namespace CPS

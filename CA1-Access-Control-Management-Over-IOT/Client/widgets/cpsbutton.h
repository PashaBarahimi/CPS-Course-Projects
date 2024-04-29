#ifndef CPSBUTTON_H
#define CPSBUTTON_H

#include <QPushButton>
#include <QString>
#include <QWidget>

namespace CPS {

class Button : public QPushButton {
    Q_OBJECT
public:
    explicit Button(const QString& text, QWidget* parent = nullptr);
    void color(bool enabled);
};

} // namespace CPS

#endif // CPSBUTTON_H

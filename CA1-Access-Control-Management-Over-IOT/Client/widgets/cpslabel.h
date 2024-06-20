#ifndef CPSLABEL_H
#define CPSLABEL_H

#include <QLabel>
#include <QString>
#include <QWidget>

namespace CPS {

class Label : public QLabel {
    Q_OBJECT
public:
    explicit Label(const QString& text, QWidget* parent = nullptr);
};

} // namespace CPS

#endif // CPSLABEL_H

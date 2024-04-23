#ifndef CPSTEXTFIELD_H
#define CPSTEXTFIELD_H

#include <QLineEdit>
#include <QString>
#include <QWidget>

namespace CPS {

class TextField : public QLineEdit {
    Q_OBJECT
public:
    explicit TextField(const QString& placeholderText, QWidget* parent = nullptr);
};

} // namespace CPS

#endif // CPSTEXTFIELD_H

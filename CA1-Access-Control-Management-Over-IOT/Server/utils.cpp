#include "utils.h"

#include <QFile>
#include <Qdebug>

namespace CPS {

QByteArray Utils::readFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open file" << filePath;
        return QByteArray();
    }
    return file.readAll();
}

void Utils::writeFile(const QString& filePath, const QByteArray& data) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Could not open file" << filePath;
        return;
    }
    file.write(data);
}

} // namespace CPS

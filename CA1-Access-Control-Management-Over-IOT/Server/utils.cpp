#include "utils.h"

#include <Qdebug>
#include <QFile>

namespace CPS {

QByteArray Utils::readFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open file" << filePath;
        return QByteArray();
    }
    return file.readAll();
}

} // namespace CPS

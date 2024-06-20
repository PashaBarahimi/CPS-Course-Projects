#ifndef UTILS_H
#define UTILS_H

#include <QByteArray>

namespace CPS {

class Utils {
public:
    static QByteArray readFile(const QString& filePath);
    static void writeFile(const QString& filePath, const QByteArray& data);
};

} // namespace CPS

#endif // UTILS_H

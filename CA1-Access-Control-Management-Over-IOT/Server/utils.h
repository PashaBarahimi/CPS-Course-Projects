#ifndef UTILS_H
#define UTILS_H

#include <QByteArray>

namespace CPS {

class Utils
{
public:
    static QByteArray readFile(const QString& filePath);
};

} // namespace CPS

#endif // UTILS_H

#ifndef ANGLE_H
#define ANGLE_H

#include <QObject>
#include <QQmlEngine>
#include <QString>

class Angle : public QObject
{
    Q_OBJECT
    QML_UNCREATABLE("Enum values only")
    QML_ELEMENT

public:
    enum Type {
        Degree0,
        Degree90,
        Degree180,
        Degree270,
    };
    Q_ENUM(Type)

    static QString toString(Type angle)
    {
        switch (angle) {
        case Degree0: return "0";
        case Degree90: return "90";
        case Degree180: return "180";
        case Degree270: return "-90";
        }
    }

    friend Angle::Type operator+(Angle::Type lhs, Angle::Type rhs)
    {
        int result = static_cast<int>(lhs) + static_cast<int>(rhs);
        result %= 4; // Ensure the result wraps around within the range 0-3
        return static_cast<Angle::Type>(result);
    }
};

#endif // ANGLE_H

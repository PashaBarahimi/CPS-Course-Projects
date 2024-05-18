#ifndef DIRECTION_H
#define DIRECTION_H

#include <QObject>
#include <QQmlEngine>
#include <QString>

class Direction : public QObject
{
    Q_OBJECT
    QML_UNCREATABLE("Enum values only")
    QML_ELEMENT

public:
    enum Type {
        Top,
        Bottom,
        Right,
        Left,
    };
    Q_ENUM(Type)

    static QString toString(Type direction)
    {
        switch (direction) {
        case Top: return "top";
        case Bottom: return "bottom";
        case Right: return "right";
        case Left: return "left";
        }
    }
};

#endif // DIRECTION_H

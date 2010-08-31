#ifndef COMMONUTILS_H
#define COMMONUTILS_H
#include <QtCore>
#include <stddef.h>
#include <stdint.h>
#include <QGraphicsItem>
#include "qtop.h"
#include "qarc.h"
#include "qcomment.h"
QString getCRC(QByteArray in);
QString itemTypeToString(QGraphicsItem *item);
bool topUpperThan(const QTop* top1, const QTop* top2);
bool topLeftThan(const QTop* top1, const QTop* top2);
#endif // COMMONUTILS_H

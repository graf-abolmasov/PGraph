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
#endif // COMMONUTILS_H

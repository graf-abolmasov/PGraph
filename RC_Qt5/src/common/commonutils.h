#ifndef COMMONUTILS_H
#define COMMONUTILS_H
#include <QtCore>
#include <stddef.h>
#include <stdint.h>
#include "../../src/editor/qtop.h"

#define UP	0x80
#define LEFT	0x20
#define DOWN	0x08
#define RIGHT	0x02

QString getCRC(QByteArray in);
QString arcTypeToString(const  QArc *arc);
QString actorTypeToString(const Actor *actor);
QString predicateTypeToString(const Predicate *predicate);
bool topUpperThan(const QTop* top1, const QTop* top2);
bool topLeftThan(const QTop* top1, const QTop* top2);
bool orderArcByPriorityAsc(const GraphArc &arc1, const GraphArc &arc2);
int dvec2log(float dx, float dy);
#endif // COMMONUTILS_H

#ifndef ERRORS_H
#define ERRORS_H

#define ERR_TITLE "Ошибка"

#define ERR_TOP_ZERO_SIZE "Вершина %1: Нулевой размер. (Ширина = %2 Высота = %3)"
#define ERR_TOP_NULL_ACTOR "Вершина %1: Нет актора."
#define ERR_TOP_WRONG_NUMBER "Вершина %1: Недопустимый номер."
#define ERR_TOP_WRONG_PROCNUM "Вершина %1: Недопустимое количество параллельных процессов. (%2)"

#define ERR_ARC_UNDEF_STARTOP "Дуга %1->%2: Нет начальной вершины."
#define ERR_ARC_UNDEF_ENDTOP "Дуга %1->%2: Нет конечной вершины."
#define ERR_ARC_UNDEF_PREDICATE "Дуга %1->%2: Нет предиката."
#define ERR_ARC_EMPTY_LINES "Дуга %1->%2: Нулевая длина дуги."

#define ERR_ACTOR_NULL_BASEMODULE "Актор %1: Нет базового модуля."
#define ERR_ACTOR_EMPTY_NAME "Актор %1: Нет внутреннего имени."
#define ERR_ACTOR_EMPTY_EXTNAME "Актор %1: Нет названия."
#define ERR_ACTOR_EMPTY_INLINE "Актор %1: Пустой текст inline-актора."
#define ERR_ACTOR_UNDEF_VAR "Актор %1: Определены не все переменные."
#define ERR_ACTOR_UNDEF_VAR_ACC "Актор %1: Определены не все модификаторы доступа."
#define ERR_ACTOR_USING_IN "Актор используется в слудеющих агрегатах:\r\n%1\r\nУдаление невозможно."

#define ERR_PREDICATE_NULL_BASEMODULE "Предикат %1: Нет базового модуля."
#define ERR_PREDICATE_EMPTY_NAME "Предикат %1: Нет внутреннего имени."
#define ERR_PREDICATE_EMPTY_EXTNAME "Предикат %1: Нет названия."
#define ERR_PREDICATE_EMPTY_INLINE "Предикат %1: Пустой текст inline-актора."
#define ERR_PREDICATE_UNDEF_VAR "Предикат %1: Определены не все переменные."
#define ERR_PREDICATE_USING_IN "Предикат используется в слудеющих агрегатах:\r\n%1\r\nУдаление невозможно."

#define ERR_DATATYPE_EMPTY_NAME "Тип переменной %1: Нет названия."
#define ERR_DATATYPE_EMPTY_TYPEDEF "Тип переменной %1: Нет описания."

#define ERR_GRAPH_UNDEF_ROOT "%1: Нет корневой вершины."

#define ERR_GRAPHCOMPI_EMPTY_TEMPL "Graph compiler: Пустой шаблон (%1)"

#define ERR_DATACOMPI_EMPTY_TEMPL "Data compiler: Пустой шаблон (%1)"

#endif // ERRORS_H

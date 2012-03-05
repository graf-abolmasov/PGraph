#ifndef ERRORS_H
#define ERRORS_H

#define ERR_TITLE "Ошибка"

#define ERR_TOP_ZERO_SIZE "Top %1: Нулевой размер. (Ширина = %2 Высота = %3)"
#define ERR_TOP_NULL_ACTOR "Top %1: Нет актора."
#define ERR_TOP_WRONG_NUMBER "Top %1: Недопустимый номер."
#define ERR_TOP_WRONG_PROCNUM "Top %1: Недопустимое количество параллельных процессов. (%2)"

#define ERR_ARC_UNDEF_STARTOP "Arc %1->%2: Нет начальной вершины."
#define ERR_ARC_UNDEF_ENDTOP "Arc %1->%2: Нет конечной вершины."
#define ERR_ARC_UNDEF_PREDICATE "Arc %1->%2: Нет предиката."
#define ERR_ARC_EMPTY_LINES "Arc %1->%2: Нулевая длина дуги."

#define ERR_ACTOR_NULL_BASEMODULE "Actor %1: Нет базового модуля."
#define ERR_ACTOR_EMPTY_NAME "Actor %1: Нет внутреннего имени."
#define ERR_ACTOR_EMPTY_EXTNAME "Actor %1: Нет названия."
#define ERR_ACTOR_EMPTY_INLINE "Actor %1: Пустой текст inline-актора."
#define ERR_ACTOR_UNDEF_VAR "Actor %1: Определены не все переменные."
#define ERR_ACTOR_UNDEF_VAR_ACC "Actor %1: Определены не все модификаторы доступа."
#define ERR_ACTOR_USING_IN "Актор используется в слудеющих агрегатах:\r\n%1\r\nУдаление невозможно."

#define ERR_PREDICATE_NULL_BASEMODULE "Predicate %1: Нет базового модуля."
#define ERR_PREDICATE_EMPTY_NAME "Predicate %1: Нет внутреннего имени."
#define ERR_PREDICATE_EMPTY_EXTNAME "Predicate %1: Нет названия."
#define ERR_PREDICATE_EMPTY_INLINE "Predicate %1: Пустой текст inline-актора."
#define ERR_PREDICATE_UNDEF_VAR "Predicate %1: Определены не все переменные."
#define ERR_PREDICATE_USING_IN "Предикат используется в слудеющих агрегатах:\r\n%1\r\nУдаление невозможно."

#define ERR_DATATYPE_EMPTY_NAME "DataType %1: Нет названия."
#define ERR_DATATYPE_EMPTY_TYPEDEF "DataType %1: Нет описания."

#define ERR_GRAPH_UNDEF_ROOT "Graph %1: Нет корневой вершины."

#define ERR_GRAPHCOMPI_EMPTY_TEMPL "Graph compiler: Пустой шаблон (%1)"

#define ERR_DATACOMPI_EMPTY_TEMPL "Data compiler: Пустой шаблон (%1)"

#endif // ERRORS_H

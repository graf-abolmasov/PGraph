#define PROJECT_NAMESPACE vcompi

#include <stdio.h>
#include <stdlib.h>

#ifndef PROJECT_NAMESPACE
	# define PROJECT_BEGIN_NAMESPACE
	# define PROJECT_END_NAMESPACE
#else 
	# define PROJECT_BEGIN_NAMESPACE namespace PROJECT_NAMESPACE {
	# define PROJECT_END_NAMESPACE }
#endif


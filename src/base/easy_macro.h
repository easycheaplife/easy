/********************************************************************
	created:	2011/12/18
	created:	18:12:2011   9:42
	filename: 	F:\lee-private-project\trunk\easy\src\base\easy_macro.h
	file path:	F:\lee-private-project\trunk\easy\src\base
	file base:	easy_macro
	file ext:	h
	author:		Lee

	purpose:
*********************************************************************/
#ifndef easy_macro_h__
#define easy_macro_h__

// Some compilers lack the features that are necessary for concept checks.
// On those compilers we define the concept check macros to do nothing.
#ifndef __EASY_USE_CONCEPT_CHECKS
#define __EASY_REQUIRES(__type_var, __concept) do {} while(0)
#else
#define __EASY_REQUIRES(__type_var, __concept) \
	do { \
	void (*__x)( __type_var ) = __concept##_concept_specification< __type_var >\
	::__concept##_requirement_violation; __x = __x; } while (0)
#endif

#endif // easy_macro_h__
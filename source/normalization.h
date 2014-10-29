#ifndef _UTF8NORMALIZATION_H_
#define _UTF8NORMALIZATION_H_

#include "utf8rewind.h"

typedef struct {
	unicode_t codepoint;
	ptrdiff_t offsetC;
	ptrdiff_t offsetD;
	ptrdiff_t offsetKC;
	ptrdiff_t offsetKD;
} CompositionEntry;

CompositionEntry* findcomposition(unicode_t codepoint);

#endif
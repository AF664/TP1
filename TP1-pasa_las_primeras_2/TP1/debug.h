#ifndef _DEBUG_H_
#define _DEBUG_H_

#ifdef DEBUG
#include <cstdlib>
#include <iostream>

#define ASSERT(expr)                                  \
	do {                                          \
		if (!(expr)) {                        \
			std::cerr << "assertion "     \
			          << (#expr)          \
			          << " failed :-\\ {" \
			          << __FUNCTION__     \
			          << "() "            \
			          << __FILE__         \
			          << ":"              \
			          << __LINE__         \
			          << "}"              \
			          << std::endl;       \
			std::abort();                 \
		}                                     \
	} while (0)
#else
#define ASSERT(expr) do {} while (0)
#endif

#endif
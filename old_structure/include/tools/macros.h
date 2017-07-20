#ifndef MACROS_H
#define MACROS_H

#ifndef DISABLE_RESTRICT
#  define RESTRICT __restrict__
#else
#  define RESTRICT
#endif // DISABLE_RESTRICT

#endif // MACROS_H

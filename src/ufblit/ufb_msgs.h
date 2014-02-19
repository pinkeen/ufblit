#ifndef UFB_MSGS_H
#define UFB_MSGS_H

#define xassert(expr, ...) if (!(expr)) __assert(__FILE__, __LINE__, __FUNCTION__, #expr, __VA_ARGS__)
#define xwarn(...) __warn(__VA_ARGS__)
#define xinfo(...) __info(__VA_ARGS__)
#define xfatal(...) __fatal(__VA_ARGS__)

void __assert(const char *file, int line, const char *function, const char *expr, const char *fmt, ...);
void __warn(const char *fmt, ...);
void __info(const char *fmt, ...);
void __fatal(const char *fmt, ...);

#endif // UFB_MSGS_H

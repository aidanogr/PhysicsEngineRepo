#include <stdio.h>
#include <stdarg.h>

void log_printf(FILE *file, const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);
    va_end(args);
}

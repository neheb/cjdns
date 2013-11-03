/* vim: set expandtab ts=4 sw=4: */
/*
 * You may redistribute this program and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#define _POSIX_C_SOURCE 200112L

#define string_strrchr

#include "exception/Except.h"

#include <stdarg.h>
#include "util/platform/libc/string.h"
#include <stdio.h>
#include <stdlib.h>

void Except_raise(struct Except* eh, int code, char* format, ...)
{
    va_list args;
    va_start(args, format);

    if (eh) {
        vsnprintf(eh->message, Except_BUFFER_SZ, format, args);
        eh->exception(eh->message, code, eh);
    } else {
        vprintf(format, args);
    }
    abort();
    exit(100);
}

void Except__throw(char* file, int line, struct Except* eh, char* format, ...)
{
    va_list args;
    va_start(args, format);

    char* subFile = strrchr(file, '/');
    if (!subFile) {
        subFile = file;
    }

    if (eh) {
        int remaining = Except_BUFFER_SZ;
        int len = snprintf(eh->message, remaining, "%s:%d ", subFile, line);
        remaining -= len;
        if (remaining > 0) {
            vsnprintf(&eh->message[len], remaining, format, args);
        }
        eh->exception(eh->message, 0, eh);
    } else {
        printf("%s:%d ", subFile, line);
        vprintf(format, args);
        printf("\n");
    }
    abort();
    exit(100);
}

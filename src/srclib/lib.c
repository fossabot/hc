/*!
 * \brief   The file contains common solution library implementation
 * \author  \verbatim
            Created by: Alexander Egorov
            \endverbatim
 * \date    \verbatim
            Creation date: 2010-03-05
            \endverbatim
 * Copyright: (c) Alexander Egorov 2009-2010
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <time.h>
#ifdef WIN32
#include <windows.h>
#endif
#include "lib.h"


#define BIG_FILE_FORMAT "%.2f %s (%llu %s)" // greater or equal 1 Kb
#define SMALL_FILE_FORMAT "%llu %s" // less then 1 Kb
#define SEC_FMT "%.3f sec"
#define MIN_FMT "%d min "
#define HOURS_FMT "%d h "
#define SECONDS_PER_HOUR 3600
#define SECONDS_PER_MINUTE 60
#define INT64_BITS_COUNT 64

static char* sizes[] = {
    "bytes",
    "Kb",
    "Mb",
    "Gb",
    "Tb",
    "Pb",
    "Eb",
    "Zb",
    "Yb",
    "Bb",
    "GPb"
};

static double span = 0.0;

#ifdef WIN32
static LARGE_INTEGER freq = { 0 };
static LARGE_INTEGER time1 = { 0 };
static LARGE_INTEGER time2 = { 0 };

#else
static clock_t c0 = 0;
static clock_t c1 = 0;
#endif

void PrintSize(uint64_t size)
{
    FileSize normalized = NormalizeSize(size);
    CrtPrintf(normalized.unit ? BIG_FILE_FORMAT : SMALL_FILE_FORMAT,
              normalized.value, sizes[normalized.unit], size, sizes[SizeUnitBytes]);
}

uint64_t ilog(uint64_t x)
{
    uint64_t y = 0;
    uint64_t n = INT64_BITS_COUNT;
    int c = INT64_BITS_COUNT / 2;

    do {
        y = x >> c;
        if (y != 0) {
            n -= c;
            x = y;
        }
        c >>= 1;
    } while (c != 0);
    n -= x >> (INT64_BITS_COUNT - 1);
    return (INT64_BITS_COUNT - 1) - (n - x);
}

FileSize NormalizeSize(uint64_t size)
{
    FileSize result = { 0 };
    result.unit = size == 0 ? SizeUnitBytes : ilog(size) / ilog(BINARY_THOUSAND);
    if (result.unit == SizeUnitBytes) {
        result.value.sizeInBytes = size;
    } else {
        result.value.size = size / pow(BINARY_THOUSAND, result.unit);
    }
    return result;
}

int CrtPrintf(const char* format, ...)
{
    va_list params = NULL;
    int result = 0;
    va_start(params, format);
#ifdef __STDC_WANT_SECURE_LIB__
    result = vfprintf_s(stdout, format, params);
#else
    result = vfprintf(stdout, format, params);
#endif
    va_end(params);
    return result;
}

int CrtFprintf(FILE* file, const char* format, ...)
{
    va_list params = NULL;
    int result = 0;
    va_start(params, format);
#ifdef __STDC_WANT_SECURE_LIB__
    result = vfprintf_s(file, format, params);
#else
    result = vfprintf(file, format, params);
#endif
    va_end(params);
    return result;
}

uint32_t htoi(const char* ptr, int size)
{
    uint32_t value = 0;
    char ch = 0;
    int count = 0;

    if (ptr == NULL) {
        return value;
    }

    ch = *ptr;
    while (ch == ' ' || ch == '\t') {
        ch = *(++ptr);
        ++count;
    }

    for (;;) {
        if (count >= size) {
            return value;
        } else if ((ch >= '0') && (ch <= '9')) {
            value = (value << 4) + (ch - '0');
        } else if ((ch >= 'A') && (ch <= 'F')) {
            value = (value << 4) + (ch - 'A' + 10);
        } else if ((ch >= 'a') && (ch <= 'f')) {
            value = (value << 4) + (ch - 'a' + 10);
        } else {
            return value;
        }
        ch = *(++ptr);
        ++count;
    }
}

Time NormalizeTime(double seconds)
{
    Time result = { 0 };
    result.hours = seconds / SECONDS_PER_HOUR;
    result.minutes = ((uint64_t)seconds % SECONDS_PER_HOUR) / SECONDS_PER_MINUTE;
    result.seconds = ((uint64_t)seconds % SECONDS_PER_HOUR) % SECONDS_PER_MINUTE;
    result.seconds +=
        seconds -
        (result.hours * SECONDS_PER_HOUR + result.minutes * SECONDS_PER_MINUTE + result.seconds);
    return result;
}

void PrintTime(Time time)
{
    if (time.hours) {
        CrtPrintf(HOURS_FMT, time.hours);
    }
    if (time.minutes) {
        CrtPrintf(MIN_FMT, time.minutes);
    }
    CrtPrintf(SEC_FMT, time.seconds);
}

void NewLine(void)
{
    CrtPrintf("\n");
}

void StartTimer(void)
{
#ifdef WIN32
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&time1);
#else
    c0 = clock();
#endif
}

void StopTimer(void)
{
#ifdef WIN32
    QueryPerformanceCounter(&time2);
    span = (double)(time2.QuadPart - time1.QuadPart) / (double)freq.QuadPart;
#else
    c1 = clock();
    span = (double)(c1 - c0) / (double)CLOCKS_PER_SEC;
#endif
}

Time ReadElapsedTime(void)
{
    return NormalizeTime(span);
}
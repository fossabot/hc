/*-
 * Copyright (c) 2001-2003 Allan Saddi <allan@saddi.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY ALLAN SADDI AND HIS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL ALLAN SADDI OR HIS CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id: sha384.h 349 2003-02-23 22:12:21Z asaddi $
 */

#ifndef _SHA384_H
#define _SHA384_H

#if HAVE_INTTYPES_H
# include <inttypes.h>
#else
# if HAVE_STDINT_H
#  include <stdint.h>
# endif
#endif

#define SHA384_HASH_SIZE 48

/* Hash size in 64-bit words */
#define SHA384_HASH_WORDS 6

typedef struct _SHA384Context {
    unsigned long long totalLength[2];
    unsigned long long hash[SHA384_HASH_WORDS + 2];
    unsigned long bufferLength;
    union {
        unsigned long long words[16];
        unsigned char  bytes[128];
    } buffer;
#ifdef RUNTIME_ENDIAN
    int littleEndian;
#endif /* RUNTIME_ENDIAN */
} SHA384Context;

#ifdef __cplusplus
extern "C" {
#endif

void SHA384Init(SHA384Context* sc);
void SHA384Update(SHA384Context* sc, const void* data, unsigned long len);
void SHA384Final(unsigned char* hash, SHA384Context* sc);

#ifdef __cplusplus
}
#endif

#endif /* !_SHA384_H */
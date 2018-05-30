/* zutil.c -- target dependent utility functions for the compression library
 * Copyright (C) 1995 Jean-loup Gailly.
 * For conditions of distribution and use, see copyright notice in zlib.h 
 */

/* $Id: zutil.c,v 1.8 1995/05/03 17:27:12 jloup Exp $ */

#include <stdio.h>

#include "stdafx.h"

#ifdef _SECDLL
#undef AFXAPI_DATA
#define AFXAPI_DATA __based(__segname("_DATA"))
#endif //_SECDLL

#include "compeng.h"


#ifndef __GO32__
//AAB extern void exit OF((int));
#endif

TCHAR *zlib_version = _T("SEC_COMPRESSION");

TCHAR *z_errmsg[8] = {
_T("stream end"),          /* Z_STREAM_END    1 */
_T(""),                    /* Z_OK            0 */
_T("file error"),          /* Z_ERRNO        (-1) */
_T("stream error"),        /* Z_STREAM_ERROR (-2) */
_T("data error"),          /* Z_DATA_ERROR   (-3) */
_T("insufficient memory"), /* Z_MEM_ERROR    (-4) */
_T("buffer error"),        /* Z_BUF_ERROR    (-5) */
_T("")
};


void SECComp::z_error ( char *m)
{
	//AAB _ftprintf(stderr, "%s\n", m);
    //AAB exit(1);
    TRACE1("%s\n", m);
    m;
}

#ifndef HAVE_MEMCPY

void zmemcpy(dest, source, len)
    Bytef* dest;
    Bytef* source;
    uInt  len;
{
    if (len == 0) return;
    do {
        *dest++ = *source++; /* ??? to be unrolled */
    } while (--len != 0);
}

void zmemzero(dest, len)
    Bytef* dest;
    uInt  len;
{
    if (len == 0) return;
    do {
        *dest++ = 0;  /* ??? to be unrolled */
    } while (--len != 0);
}
#endif

#if defined( __TURBOC__) && !defined(__SMALL__) && !defined(__MEDIUM__)
/* Small and medium model are for now limited to near allocation with
 * reduced MAX_WBITS and MAX_MEM_LEVEL
 */
#  define MY_ZCALLOC

/* Turbo C malloc() does not allow dynamic allocation of 64K bytes
 * and farmalloc(64K) returns a pointer with an offset of 8, so we
 * must fix the pointer. Warning: the pointer must be put back to its
 * original form in order to free it, use zcfree().
 */

#define MAX_PTR 10
/* 10*64K = 640K */

static int next_ptr = 0;

typedef struct ptr_table_s {
    voidpf org_ptr;
    voidpf new_ptr;
} ptr_table;

static ptr_table table[MAX_PTR];
/* This table is used to remember the original form of pointers
 * to large buffers (64K). Such pointers are normalized with a zero offset.
 * Since MSDOS is not a preemptive multitasking OS, this table is not
 * protected from concurrent access. This hack doesn't work anyway on
 * a protected system like OS/2. Use Microsoft C instead.
 */

voidpf SECComp::zcalloc (voidpf opaque, unsigned items, unsigned size)
{
    voidpf buf = opaque; /* just to make some compilers happy */
    ulg bsize = (ulg)items*size;

    if (bsize < 65536L) {
        buf = farmalloc(bsize);
        if (*(ush*)&buf != 0) return buf;
    } else {
        buf = farmalloc(bsize + 16L);
    }
    if (buf == NULL || next_ptr >= MAX_PTR) return NULL;
    table[next_ptr].org_ptr = buf;

    /* Normalize the pointer to seg:0 */
    *((ush*)&buf+1) += ((ush)((uch*)buf-0) + 15) >> 4;
    *(ush*)&buf = 0;
    table[next_ptr++].new_ptr = buf;
    return buf;
}

void  SECComp::zcfree (voidpf opaque, voidpf ptr)
{
    int n;
    if (*(ush*)&ptr != 0) { /* object < 64K */
        farfree(ptr);
        return;
    }
    /* Find the original pointer */
    for (n = 0; n < next_ptr; n++) {
        if (ptr != table[n].new_ptr) continue;

        farfree(table[n].org_ptr);
        while (++n < next_ptr) {
            table[n-1] = table[n];
        }
        next_ptr--;
        return;
    }
    ptr = opaque; /* just to make some compilers happy */
    z_error("zcfree: ptr not found");
}
#endif /* __TURBOC__ */

#if defined(M_I86SM)||defined(M_I86MM)||defined(M_I86CM)||defined(M_I86LM)
/* Microsoft C */

#  define MY_ZCALLOC

#if (!defined(_MSC_VER) || (_MSC_VER < 600))
#  define _halloc  halloc
#  define _hfree   hfree
#endif

voidpf SECComp::zcalloc (voidpf opaque, WORD items, WORD size)
{
    if (opaque) opaque = 0; /* to make compiler happy */
    return _halloc((long)items, size);
}

void  SECComp::zcfree (voidpf opaque, voidpf ptr)
{
    if (opaque) opaque = 0; /* to make compiler happy */
    _hfree(ptr);
}

#endif /* MSC */


#ifndef MY_ZCALLOC /* Any system without a special alloc function */

#ifndef __GO32__
//AAB extern voidp  calloc OF((uInt items, uInt size));
//AAB extern void   free   OF((voidpf ptr));
#endif

voidpf SECComp::zcalloc ( voidpf opaque, WORD items, WORD size)
{
    return (voidpf)calloc(items, size);
	opaque; //unused
}

void  SECComp::zcfree ( voidpf opaque, voidpf ptr)
{
	int aabcheck;
	aabcheck=2;
	opaque; // not used
    free(ptr);
}

#endif /* MY_ZCALLOC */

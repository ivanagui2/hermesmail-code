/* deflate.c -- compress data using the deflation algorithm
 * Copyright (C) 1995 Jean-loup Gailly.
 * For conditions of distribution and use, see copyright notice in zlib.h 
 */   

/*
 *  ALGORITHM
 *
 *      The "deflation" process depends on being able to identify portions
 *      of the input text which are identical to earlier input (within a
 *      sliding window trailing behind the input currently being processed).
 *
 *      The most straightforward technique turns out to be the fastest for
 *      most input files: try all possible matches and select the longest.
 *      The key feature of this algorithm is that insertions into the string
 *      dictionary are very simple and thus fast, and deletions are avoided
 *      completely. Insertions are performed at each input character, whereas
 *      string matches are performed only when the previous match ends. So it
 *      is preferable to spend more time in matches to allow very fast string
 *      insertions and avoid deletions. The matching algorithm for small
 *      strings is inspired from that of Rabin & Karp. A brute force approach
 *      is used to find longer strings when a small match has been found.
 *      A similar algorithm is used in comic (by Jan-Mark Wams) and freeze
 *      (by Leonid Broukhis).
 *         A previous version of this file used a more sophisticated algorithm
 *      (by Fiala and Greene) which is guaranteed to run in linear amortized
 *      time, but has a larger average cost, uses more memory and is patented.
 *      However the F&G algorithm may be faster for some highly redundant
 *      files if the parameter max_chain_length (described below) is too large.
 *
 *  ACKNOWLEDGEMENTS
 *
 *      The idea of lazy evaluation of matches is due to Jan-Mark Wams, and
 *      I found it in 'freeze' written by Leonid Broukhis.
 *      Thanks to many people for bug reports and testing.
 *
 *  REFERENCES
 *
 *      Deutsch, L.P.,"'Deflate' Compressed Data Format Specification".
 *      Available in ftp.uu.net:/pub/archiving/zip/doc/deflate-1.1.doc
 *
 *      A description of the Rabin and Karp algorithm is given in the book
 *         "Algorithms" by R. Sedgewick, Addison-Wesley, p252.
 *
 *      Fiala,E.R., and Greene,D.H.
 *         Data Compression with Finite Windows, Comm.ACM, 32,4 (1989) 490-595
 *
 */

/* $Id: deflate.c,v 1.8 1995/05/03 17:27:08 jloup Exp $ */
#include "stdafx.h"

#ifdef _SECDLL
#undef AFXAPI_DATA
#define AFXAPI_DATA __based(__segname("_DATA"))
#endif //_SECDLL
#include "compeng.h"

#pragma warning(disable: 4701)

char copyright[] = " deflate Copyright 1995 Jean-loup Gailly ";
/*
  If you use the zlib library in a product, an acknowledgment is welcome
  in the documentation of your product. If for some reason you cannot
  include such an acknowledgment, I would appreciate that you keep this
  copyright string in the executable of your product.
 */

#define NIL 0
/* Tail of hash chains */

#ifndef TOO_FAR
#  define TOO_FAR 4096
#endif
/* Matches of length 3 are discarded if their distance exceeds TOO_FAR */

#define MIN_LOOKAHEAD (MAX_MATCH+MIN_MATCH+1)
/* Minimum amount of lookahead, except at the end of the input file.
 * See deflate.c for comments about the MIN_MATCH+1.
 */

/* Values for max_lazy_match, good_match and max_chain_length, depending on
 * the desired pack level (0..9). The values given below have been tuned to
 * exclude worst case performance for pathological files. Better values may be
 * found for specific files.
 */

typedef struct config_s {
   ush good_length; /* reduce lazy search above this match length */
   ush max_lazy;    /* do not perform lazy search above this match length */
   ush nice_length; /* quit search above this match length */
   ush max_chain;
} config;

static config configuration_table[10] = {
/*      good lazy nice chain */
/* 0 */ {0,    0,  0,    0},  /* store only */
/* 1 */ {4,    4,  8,    4},  /* maximum speed, no lazy matches */
/* 2 */ {4,    5, 16,    8},
/* 3 */ {4,    6, 32,   32},

/* 4 */ {4,    4, 16,   16},  /* lazy matches */
/* 5 */ {8,   16, 32,   32},
/* 6 */ {8,   16, 128, 128},
/* 7 */ {8,   32, 128, 256},
/* 8 */ {32, 128, 258, 1024},
/* 9 */ {32, 258, 258, 4096}}; /* maximum compression */

/* Note: the deflate() code requires max_lazy >= MIN_MATCH and max_chain >= 4
 * For deflate_fast() (levels <= 3) good is ignored and lazy has a different
 * meaning.
 */

#define EQUAL 0
/* result of memcmp for equal strings */

struct static_tree_desc_s {int dummy;}; /* for buggy compilers */

/* ===========================================================================
 *  Prototypes for local functions.
 */


#ifdef DEBUG
static void check_match OF((deflate_state *s, IPos start, IPos match,
                            int length));
#endif


/* ===========================================================================
 * Update a hash value with the given input byte
 * IN  assertion: all calls to to UPDATE_HASH are made with consecutive
 *    input characters, so that a running hash key can be computed from the
 *    previous key instead of complete recalculation each time.
 */
#define UPDATE_HASH(s,h,c) (h = (((h)<<s->hash_shift) ^ (c)) & s->hash_mask)


/* ===========================================================================
 * Insert string str in the dictionary and set match_head to the previous head
 * of the hash chain (the most recent string with same hash key). Return
 * the previous length of the hash chain.
 * IN  assertion: all calls to to INSERT_STRING are made with consecutive
 *    input characters and the first MIN_MATCH bytes of str are valid
 *    (except for the last MIN_MATCH-1 bytes of the input file).
 */ 


#define INSERT_STRING(s, str, match_head) \
   (UPDATE_HASH(s, s->ins_h, s->window[(str) + (MIN_MATCH-1)]), \
    s->prev[(str) & s->w_mask] = (WORD)s->head[s->ins_h],  match_head = (WORD)s->head[s->ins_h], \
    s->head[s->ins_h] = (str))


/* ===========================================================================
 * Initialize the hash table (avoiding 64K overflow for 16 bit systems).
 * prev[] will be initialized on the fly.
 */
#define CLEAR_HASH(s) \
    s->head[s->hash_size-1] = NIL; \
    zmemzero((charf *)s->head, (unsigned)(s->hash_size-1)*sizeof(*s->head));

/* ========================================================================= */
int SECComp::deflateInit (z_stream *strm, int level)
{
    return deflateInit2 (strm, level, DEFLATED, MAX_WBITS, DEF_MEM_LEVEL, 0);
    /* To do: ignore strm->next_in if we use it as window */
}

/* ========================================================================= */
int SECComp::deflateInit2 (z_stream *strm, int level, int method, 
	int windowBits, int memLevel, int strategy)
{
    deflate_state *s;
    int noheader = 0;

    if (strm == Z_NULL) return Z_STREAM_ERROR;

    strm->msg = Z_NULL;
    if (strm->zalloc == Z_NULL) strm->zalloc = zcalloc;
    if (strm->zfree == Z_NULL) strm->zfree = zcfree;

    if (level == Z_DEFAULT_COMPRESSION) level = 6;

    if (windowBits < 0) { /* undocumented feature: suppress zlib header */
        noheader = 1;
        windowBits = -windowBits;
    }
    if (memLevel < 1 || memLevel > MAX_MEM_LEVEL || method != DEFLATED ||
        windowBits < 8 || windowBits > 15 || level < 1 || level > 9) {
        return Z_STREAM_ERROR;
    }
    //AAB s = (deflate_state *) ZALLOC(strm, 1, sizeof(deflate_state));
    s = (deflate_state *) ZALLOC(strm, 1, sizeof(deflate_state));
    if (s == Z_NULL) return Z_MEM_ERROR;
    //AAB strm->state = (struct internal_state *)s;
    strm->state = (struct internal_state *)s;
    s->strm = strm;

    s->noheader = noheader;
    s->w_bits = (WORD)windowBits;
    s->w_size = (WORD)((WORD)1 << s->w_bits);
    s->w_mask = (WORD)(s->w_size - (WORD)1);	   

    s->hash_bits = memLevel + 7;
    s->hash_size = 1 << s->hash_bits;
    s->hash_mask = s->hash_size - 1;
    s->hash_shift =  ((s->hash_bits+MIN_MATCH-1)/MIN_MATCH);

    s->window = (BYTE *) ZALLOC(strm, s->w_size, 2*sizeof(BYTE));
    s->prev   = (Posf *)  ZALLOC(strm, s->w_size, sizeof(Pos));
    s->head   = (Posf *)  ZALLOC(strm, (WORD)s->hash_size, sizeof(Pos));

    s->lit_bufsize = 1 << (memLevel + 6); /* 16K elements by default */

    s->pending_buf = /*(uchf *)*/ (BYTE *) ZALLOC(strm, (WORD)s->lit_bufsize, 2*sizeof(ush));

    if (s->window == Z_NULL || s->prev == Z_NULL || s->head == Z_NULL ||
        s->pending_buf == Z_NULL) {
        strm->msg = z_errmsg[1-Z_MEM_ERROR];
        deflateEnd (strm);
        return Z_MEM_ERROR;
    }
    s->d_buf = (ushf *) &(s->pending_buf[s->lit_bufsize]);
    s->l_buf = (uchf *) &(s->pending_buf[3*s->lit_bufsize]);
    /* We overlay pending_buf and d_buf+l_buf. This works since the average
     * output size for (length,distance) codes is <= 32 bits (worst case
     * is 15+15+13=33).
     */

    s->level = level;
    s->strategy = strategy;
    s->method = (BYTE)method;

    return deflateReset(strm);
}

/* ========================================================================= */
int SECComp::deflateReset (z_stream *strm)
{
    deflate_state *s;
    
    if (strm == Z_NULL || strm->state == Z_NULL ||
        strm->zalloc == Z_NULL || strm->zfree == Z_NULL) return Z_STREAM_ERROR;

    strm->total_in = strm->total_out = 0;
    strm->msg = Z_NULL; /* use zfree if we ever allocate msg dynamically */
    strm->data_type = Z_UNKNOWN;

    s = (deflate_state *)strm->state;
    s->pending = 0;
    s->pending_out = s->pending_buf;

    if (s->noheader < 0) {
        s->noheader = 0; /* was set to -1 by deflate(..., Z_FINISH); */
    }
    s->status = s->noheader ? BUSY_STATE : INIT_STATE;
    s->adler = 1;

    ct_init(s);
    lm_init(s);

    return Z_OK;
}

/* =========================================================================
 * Put a short in the pending buffer. The 16-bit value is put in MSB order.
 * IN assertion: the stream state is correct and there is enough room in
 * pending_buf.
 */
void SECComp::putShortMSB (deflate_state *s, WORD b)
{
    put_byte(s, (BYTE)(b >> 8));
    put_byte(s, (BYTE)(b & 0xff));
}   

/* =========================================================================
 * Flush as much pending output as possible.
 */
void SECComp::flush_pending(z_stream *strm)
{
	deflate_state *pState = (deflate_state *)strm->state;
    unsigned len = pState->pending;

    if (len > strm->avail_out) len = (unsigned)strm->avail_out;
    if (len == 0) return;

    zmemcpy(strm->next_out, pState->pending_out, len);
    strm->next_out = (void FAR *)((BYTE FAR *)(strm->next_out)  + len);
    pState->pending_out  += len;
    strm->total_out += len;
    strm->avail_out  -= len;
    pState->pending -= len;
    if (pState->pending == 0) {
        pState->pending_out = pState->pending_buf;
    }
}

/* ========================================================================= */
int SECComp::deflate (z_stream *strm, int flush)
{
	deflate_state *pState = (deflate_state *)strm->state;
    if (strm == Z_NULL || pState == Z_NULL) return Z_STREAM_ERROR;
    
    if (strm->next_out == Z_NULL ||
        (strm->next_in == Z_NULL && strm->avail_in != 0)) {
        ERR_RETURN(strm, Z_STREAM_ERROR);
    }
    if (strm->avail_out == 0) ERR_RETURN(strm, Z_BUF_ERROR);

    pState->strm = strm; /* just in case */	 

    /* Write the zlib header */
    if (pState->status == INIT_STATE) {

        WORD header = (WORD)(((WORD)DEFLATED + ((pState->w_bits-(WORD)8)<<4)) << 8);
        WORD level_flags = (WORD)((pState->level-(WORD)1) >> 1);

        if (level_flags > 3) level_flags = 3;
        header |= (level_flags << 6);
        header = (WORD)(header + (WORD)31 - (header % (WORD)31));   

        pState->status = BUSY_STATE;
        putShortMSB(pState, header);
    }

    /* Flush as much pending output as possible */
    if (pState->pending != 0) {
        flush_pending(strm);
        if (strm->avail_out == 0) return Z_OK;
    }

    /* User must not provide more input after the first FINISH: */
    if (pState->status == FINISH_STATE && strm->avail_in != 0) {
        ERR_RETURN(strm, Z_BUF_ERROR);
    }

    /* Start a new block or continue the current one.
     */
    if (strm->avail_in != 0 || pState->lookahead != 0 ||
        (flush != Z_NO_FLUSH && pState->status != FINISH_STATE)) {
        int quit;

        if (flush == Z_FINISH) {
            pState->status = FINISH_STATE;
        }
        if (pState->level <= 3) {
            quit = deflate_fast(pState, flush);
        } else {
            quit = deflate_slow(pState, flush);
        }
        if (quit || strm->avail_out == 0) return Z_OK;
        /* If flush != Z_NO_FLUSH && avail_out == 0, the next call
         * of deflate should use the same flush parameter to make sure
         * that the flush is complete. So we don't have to output an
         * empty block here, this will be done at next call. This also
         * ensures that for a very small output buffer, we emit at most
         * one empty block.
         */
        if (flush != Z_OK && flush != Z_FINISH) {
            if (flush == Z_PARTIAL_FLUSH) {
                ct_align(pState);
            } else { /* FULL_FLUSH or SYNC_FLUSH */
                ct_stored_block(pState, (char*)0, 0L, 0);
                /* For a full flush, this empty block will be recognized
                 * as a special marker by inflate_sync().
                 */
                if (flush == Z_FULL_FLUSH) {
                    CLEAR_HASH(pState);             /* forget history */
                }
            }
            flush_pending(strm);
            if (strm->avail_out == 0) return Z_OK;
        }
    }
    Assert(strm->avail_out > 0, "bug2");

    if (flush != Z_FINISH) return Z_OK;
    if (pState->noheader) return Z_STREAM_END;

    /* Write the zlib trailer (adler32) */
    putShortMSB(pState, (WORD)(pState->adler >> 16));
    putShortMSB(pState, (WORD)(pState->adler & 0xffff));
    flush_pending(strm);
    /* If avail_out is zero, the application will call deflate again
     * to flush the rest.
     */
    pState->noheader = -1; /* write the trailer only once! */
    return pState->pending != 0 ? Z_OK : Z_STREAM_END;
}

/* ========================================================================= */
int SECComp::deflateEnd (z_stream *strm)
{
	deflate_state *pState = (deflate_state *)strm->state;
    if (strm == Z_NULL || pState == Z_NULL) return Z_STREAM_ERROR;

    TRY_FREE(strm, pState->window);
    TRY_FREE(strm, pState->prev);
    TRY_FREE(strm, pState->head);
    TRY_FREE(strm, pState->pending_buf);

    ZFREE(strm, pState);
    pState = Z_NULL;

    return Z_OK;
}

/* ========================================================================= */
int SECComp::deflateCopy (z_stream *dest, z_stream *source)
{
    if (source == Z_NULL || dest == Z_NULL || source->state == Z_NULL) {
        return Z_STREAM_ERROR;
    }
    *dest = *source;
    return Z_STREAM_ERROR; /* to be implemented */
#if 0
    dest->state = (struct internal_state FAR *)
        (*dest->zalloc)(1, sizeof(deflate_state));
    if (dest->state == Z_NULL) return Z_MEM_ERROR;

    *(dest->state) = *(source->state);
    return Z_OK;
#endif
}

/* ===========================================================================
 * Read a new buffer from the current input stream, update the adler32
 * and total number of bytes read.
 */
long SECComp::read_buf(z_stream *strm, char *buf, long size)
{
	deflate_state *pState = (deflate_state *)strm->state;
    long len = strm->avail_in;

    if (len > size) len = size;
    if (len == 0) return 0;

    strm->avail_in  -= len;

    if (!pState->noheader) {
        pState->adler = adler32(pState->adler, strm->next_in, (WORD)len);
    }
    zmemcpy(buf, strm->next_in, (UINT)len);
    strm->next_in  += len;
    strm->total_in += len;

    return (long)len;
}

/* ===========================================================================
 * Initialize the "longest match" routines for a new zlib stream
 */
void SECComp::lm_init (deflate_state *s)
{
    s->window_size = (ulg)2L*s->w_size;

    CLEAR_HASH(s);

    /* Set the default configuration parameters:
     */
    s->max_lazy_match   = configuration_table[s->level].max_lazy;
    s->good_match       = configuration_table[s->level].good_length;
    s->nice_match       = configuration_table[s->level].nice_length;
    s->max_chain_length = configuration_table[s->level].max_chain;

    s->strstart = 0;
    s->block_start = 0L;
    s->lookahead = 0;
    s->match_length = MIN_MATCH-1;
    s->match_available = 0;
    s->ins_h = 0;
#ifdef ASMV
    match_init(); /* initialize the asm code */
#endif
}

/* ===========================================================================
 * Set match_start to the longest match starting at the given string and
 * return its length. Matches shorter or equal to prev_length are discarded,
 * in which case the result is equal to prev_length and match_start is
 * garbage.
 * IN assertions: cur_match is the head of the hash chain for the current
 *   string (strstart) and its distance is <= MAX_DIST, and prev_length >= 1
 */
#ifndef ASMV
/* For 80x86 and 680x0, an optimized version will be provided in match.asm or
 * match.S. The code will be functionally equivalent.
 */
int SECComp::longest_match(deflate_state *s, IPos cur_match)
{
    unsigned chain_length = s->max_chain_length;/* max hash chain length */
    register BYTE *scan = s->window + s->strstart; /* current string */
    register BYTE *match;                       /* matched string */
    register int len;                           /* length of current match */
    WORD best_len = s->prev_length;              /* best match length so far */  //AAB used to be int
    IPos limit = (long)s->strstart > (long)/*AAB(IPos)*/MAX_DIST(s) ?
        s->strstart - (IPos)MAX_DIST(s) : NIL;
    /* Stop when cur_match becomes <= limit. To simplify the code,
     * we prevent matches with the string of window index 0.
     */
    Posf *prev = s->prev;
    WORD wmask = s->w_mask;

#ifdef UNALIGNED_OK
    /* Compare two bytes at a time. Note: this is not always beneficial.
     * Try with and without -DUNALIGNED_OK to check.
     */
    register BYTE *strend = s->window + s->strstart + MAX_MATCH - 1;
    register ush scan_start = *(ushf*)scan;
    register ush scan_end   = *(ushf*)(scan+best_len-1);
#else
    register BYTE *strend = s->window + s->strstart + MAX_MATCH;
    register BYTE scan_end1  = scan[best_len-1];
    register BYTE scan_end   = scan[best_len];
#endif

    /* The code is optimized for HASH_BITS >= 8 and MAX_MATCH-2 multiple of 16.
     * It is easy to get rid of this optimization if necessary.
     */
    Assert(s->hash_bits >= 8 && MAX_MATCH == 258, "Code too clever");

    /* Do not waste too much time if we already have a good match: */
    if (s->prev_length >= s->good_match) {
        chain_length >>= 2;
    }
    Assert((ulg)s->strstart <= s->window_size-MIN_LOOKAHEAD, "need lookahead");

    do {
        Assert(cur_match < s->strstart, "no future");
        match = s->window + cur_match;

        /* Skip to next match if the match length cannot increase
         * or if the match length is less than 2:
         */
#if (defined(UNALIGNED_OK) && MAX_MATCH == 258)
        /* This code assumes sizeof(unsigned short) == 2. Do not use
         * UNALIGNED_OK if your compiler uses a different size.
         */
        if (*(ushf*)(match+best_len-1) != scan_end ||
            *(ushf*)match != scan_start) continue;

        /* It is not necessary to compare scan[2] and match[2] since they are
         * always equal when the other bytes match, given that the hash keys
         * are equal and that HASH_BITS >= 8. Compare 2 bytes at a time at
         * strstart+3, +5, ... up to strstart+257. We check for insufficient
         * lookahead only every 4th comparison; the 128th check will be made
         * at strstart+257. If MAX_MATCH-2 is not a multiple of 8, it is
         * necessary to put more guard bytes at the end of the window, or
         * to check more often for insufficient lookahead.
         */
        Assert(scan[2] == match[2], "scan[2]?");
        scan++, match++;
        do {
        } while (*(ushf*)(scan+=2) == *(ushf*)(match+=2) &&
                 *(ushf*)(scan+=2) == *(ushf*)(match+=2) &&
                 *(ushf*)(scan+=2) == *(ushf*)(match+=2) &&
                 *(ushf*)(scan+=2) == *(ushf*)(match+=2) &&
                 scan < strend);
        /* The funny "do {}" generates better code on most compilers */

        /* Here, scan <= window+strstart+257 */
        Assert(scan <= s->window+(unsigned)(s->window_size-1), "wild scan");
        if (*scan == *match) scan++;

        len = (MAX_MATCH - 1) - (int)(strend-scan);
        scan = strend - (MAX_MATCH-1);

#else /* UNALIGNED_OK */

        if (match[best_len]   != scan_end  ||
            match[best_len-1] != scan_end1 ||
            *match            != *scan     ||
            *++match          != scan[1])      continue;

        /* The check at best_len-1 can be removed because it will be made
         * again later. (This heuristic is not always a win.)
         * It is not necessary to compare scan[2] and match[2] since they
         * are always equal when the other bytes match, given that
         * the hash keys are equal and that HASH_BITS >= 8.
         */
        scan += 2, match++;
        Assert(*scan == *match, "match[2]?");

        /* We check for insufficient lookahead only every 8th comparison;
         * the 256th check will be made at strstart+258.
         */
        do {
        } while (*++scan == *++match && *++scan == *++match &&
                 *++scan == *++match && *++scan == *++match &&
                 *++scan == *++match && *++scan == *++match &&
                 *++scan == *++match && *++scan == *++match &&
                 scan < strend);

        Assert(scan <= s->window+(unsigned)(s->window_size-1), "wild scan");

        len = MAX_MATCH - (int)(strend - scan);
        scan = strend - MAX_MATCH;

#endif /* UNALIGNED_OK */

        if ((long)len > (long)best_len) {
            s->match_start = (WORD)cur_match;	  
            best_len = (WORD)len;
            if (len >= s->nice_match) break;
#ifdef UNALIGNED_OK
            scan_end = *(ushf*)(scan+best_len-1);
#else
            scan_end1  = scan[best_len-1];
            scan_end   = scan[best_len];
#endif
        }
    } while ((cur_match = prev[cur_match & wmask]) > limit
             && --chain_length != 0);

    return best_len;
}
#endif /* ASMV */

#ifdef DEBUG
/* ===========================================================================
 * Check that the match at match_start is indeed a match.
 */
void SECComp::check_match(deflate_state *s, IPos start, IPos match, int length)
{
    /* check that the match is indeed a match */
    if (memcmp((charf *)s->window + match,
                (charf *)s->window + start, length) != EQUAL) {
        _ftprintf(stderr,
            " start %u, match %u, length %d\n",
            start, match, length);
        do { _ftprintf(stderr, "%c%c", s->window[match++],
                     s->window[start++]); } while (--length != 0);
        z_error("invalid match");
    }
    if (verbose > 1) {
        _ftprintf(stderr,"\\[%d,%d]", start-match, length);
        do { _puttc(s->window[start++], stderr); } while (--length != 0);
    }
}
#else
#  define check_match(s, start, match, length)
#endif

/* ===========================================================================
 * Fill the window when the lookahead becomes insufficient.
 * Updates strstart and lookahead.
 *
 * IN assertion: lookahead < MIN_LOOKAHEAD
 * OUT assertions: strstart <= window_size-MIN_LOOKAHEAD
 *    At least one byte has been read, or avail_in == 0; reads are
 *    performed for at least two bytes (required for the zip translate_eol
 *    option -- not supported here).
 */
void SECComp::fill_window(deflate_state *s)
{
    //AAB register unsigned n, m;
    register WORD n, m;
    register Posf *p;
    WORD more;    /* Amount of free space at the end of the window. */
    WORD wsize = s->w_size;

    do {
        more = (WORD)(s->window_size -(WORD)s->lookahead -(WORD)s->strstart);

        /* Deal with !@#$% 64K limit: */
        if (more == 0 && s->strstart == 0 && s->lookahead == 0) {
            more = wsize;
        } else if (more == (WORD)(-1)) {
            /* Very unlikely, but possible on 16 bit machine if strstart == 0
             * and lookahead == 1 (input done one byte at time)
             */
            more--;

        /* If the window is almost full and there is insufficient lookahead,
         * move the upper half to the lower one to make room in the upper half.
         */
        } else if (s->strstart >= wsize+MAX_DIST(s)) {

            /* By the IN assertion, the window is not empty so we can't confuse
             * more == 0 with more == 64K on a 16 bit machine.
             */
            zmemcpy((charf *)s->window, (charf *)s->window+wsize,
                   (unsigned)wsize);
            s->match_start = (WORD)(s->match_start - (WORD)wsize);
            s->strstart    = (WORD)(s->strstart - (WORD)wsize); /* we now have strstart >= MAX_DIST */

            s->block_start -= (long) wsize;

            /* Slide the hash table (could be avoided with 32 bit values
               at the expense of memory usage):
             */
            n = (WORD)s->hash_size;	  
            p = &s->head[n];
            do {
                m = *--p;
                *p = (Pos)(m >= wsize ? m-wsize : NIL);
				--n;
            } while (n);

            n = wsize;
            p = &s->prev[n];
            do {
                m = *--p;
                *p = (Pos)(m >= wsize ? m-wsize : NIL);
                /* If n is not on any hash chain, prev[n] is garbage but
                 * its value will never be used.
                 */
				 --n;
            } while (n);

            more = (WORD)(more + (WORD)wsize);
        }
        if (s->strm->avail_in == 0) return;

        /* If there was no sliding:
         *    strstart <= WSIZE+MAX_DIST-1 && lookahead <= MIN_LOOKAHEAD - 1 &&
         *    more == window_size - lookahead - strstart
         * => more >= window_size - (MIN_LOOKAHEAD-1 + WSIZE + MAX_DIST-1)
         * => more >= window_size - 2*WSIZE + 2
         * In the BIG_MEM or MMAP case (not yet supported),
         *   window_size == input_size + MIN_LOOKAHEAD  &&
         *   strstart + s->lookahead <= input_size => more >= MIN_LOOKAHEAD.
         * Otherwise, window_size == 2*WSIZE so more >= 2.
         * If there was sliding, more >= WSIZE. So in all cases, more >= 2.
         */
        Assert(more >= 2, "more < 2");  

        n = (WORD)read_buf(s->strm, /*(charf *)*/(char *)s->window + s->strstart + s->lookahead,
                     more);
        s->lookahead = (WORD)(s->lookahead + n);

        /* Initialize the hash value now that we have some input: */
        if (s->lookahead >= MIN_MATCH) {
            s->ins_h = s->window[s->strstart];
            UPDATE_HASH(s, s->ins_h, s->window[s->strstart+1]);
#if MIN_MATCH != 3
            Call UPDATE_HASH() MIN_MATCH-3 more times
#endif
        }
        /* If the whole input has less than MIN_MATCH bytes, ins_h is garbage,
         * but this is not important since only literal bytes will be emitted.
         */

    } while (s->lookahead < MIN_LOOKAHEAD && s->strm->avail_in != 0);
}

/* ===========================================================================
 * Flush the current block, with given end-of-file flag.
 * IN assertion: strstart is set to the end of the current match.
 */
#define FLUSH_BLOCK_ONLY(s, eof) { \
   ct_flush_block(s, (s->block_start >= 0L ? \
               (char *)&s->window[(unsigned)s->block_start] : \
               (char *)Z_NULL), (long)s->strstart - s->block_start, (eof)); \
   s->block_start = s->strstart; \
   flush_pending(s->strm); \
   ZTracev((stderr,"[FLUSH]")); \
}

/* Same but force premature exit if necessary. */
#define FLUSH_BLOCK(s, eof) { \
   FLUSH_BLOCK_ONLY(s, eof); \
   if (s->strm->avail_out == 0) return 1; \
}

/* ===========================================================================
 * Compress as much as possible from the input stream, return true if
 * processing was terminated prematurely (no more input or output space).
 * This function does not perform lazy evaluationof matches and inserts
 * new strings in the dictionary only for unmatched strings or for short
 * matches. It is used only for the fast compression options.
 */
int SECComp::deflate_fast(deflate_state *s, int flush)
{
    IPos hash_head; /* head of the hash chain */
    int bflush;     /* set if current block must be flushed */

    s->prev_length = MIN_MATCH-1;

    for (;;) {
        /* Make sure that we always have enough lookahead, except
         * at the end of the input file. We need MAX_MATCH bytes
         * for the next match, plus MIN_MATCH bytes to insert the
         * string following the next match.
         */
        if (s->lookahead < MIN_LOOKAHEAD) {
            fill_window(s);
            if (s->lookahead < MIN_LOOKAHEAD && flush == Z_NO_FLUSH) return 1;

            if (s->lookahead == 0) break; /* flush the current block */
        }

        /* Insert the string window[strstart .. strstart+2] in the
         * dictionary, and set hash_head to the head of the hash chain:
         */
        if (s->lookahead >= MIN_MATCH) {
            INSERT_STRING(s, s->strstart, hash_head);
        }

        /* Find the longest match, discarding those <= prev_length.
         * At this point we have always match_length < MIN_MATCH
         */
        if (hash_head != NIL && s->strstart - hash_head <= MAX_DIST(s)) {
            /* To simplify the code, we prevent matches with the string
             * of window index 0 (in particular we have to avoid a match
             * of the string with itself at the start of the input file).
             */
            if (s->strategy != Z_HUFFMAN_ONLY) {
                s->match_length = longest_match (s, hash_head);
            }
            /* longest_match() sets match_start */

            if (s->match_length > s->lookahead) s->match_length = s->lookahead;
        }
        if (s->match_length >= MIN_MATCH) {
            check_match(s, s->strstart, s->match_start, s->match_length);

            bflush = ct_tally(s, s->strstart - s->match_start,
                              s->match_length - MIN_MATCH);

            s->lookahead = (WORD) ( (WORD)s->lookahead - (WORD)s->match_length);

            /* Insert new strings in the hash table only if the match length
             * is not too large. This saves time but degrades compression.
             */
            if (s->match_length <= s->max_insert_length &&
                s->lookahead >= MIN_MATCH) {
                s->match_length--; /* string at strstart already in hash table */
                do {
                    s->strstart++;
                    INSERT_STRING(s, s->strstart, hash_head);
                    /* strstart never exceeds WSIZE-MAX_MATCH, so there are
                     * always MIN_MATCH bytes ahead.
                     */
                } while (--s->match_length != 0);
                s->strstart++; 
            } else {
                s->strstart = (WORD)(s->strstart + (WORD)s->match_length);
                s->match_length = 0;
                s->ins_h = s->window[s->strstart];
                UPDATE_HASH(s, s->ins_h, s->window[s->strstart+1]);
#if MIN_MATCH != 3
                Call UPDATE_HASH() MIN_MATCH-3 more times
#endif
                /* If lookahead < MIN_MATCH, ins_h is garbage, but it does not
                 * matter since it will be recomputed at next deflate call.
                 */
            }
        } else {
            /* No match, output a literal byte */
            ZTracevv((stderr,"%c", s->window[s->strstart]));
            bflush = ct_tally (s, 0, s->window[s->strstart]);
            s->lookahead--;
            s->strstart++; 
        }
        if (bflush) FLUSH_BLOCK(s, 0);
    }
    FLUSH_BLOCK(s, flush == Z_FINISH);
    return 0; /* normal exit */
}

/* ===========================================================================
 * Same as above, but achieves better compression. We use a lazy
 * evaluation for matches: a match is finally adopted only if there is
 * no better match at the next window position.
 */
int SECComp::deflate_slow(deflate_state *s, int flush)
{
    IPos hash_head;          /* head of hash chain */
    int bflush;              /* set if current block must be flushed */
	int nCount = 0;

    /* Process the input block. */
    for (;;) {
        /* Make sure that we always have enough lookahead, except
         * at the end of the input file. We need MAX_MATCH bytes
         * for the next match, plus MIN_MATCH bytes to insert the
         * string following the next match.
         */
        if (s->lookahead < MIN_LOOKAHEAD) {
            fill_window(s); 
			nCount++;
            if (s->lookahead < MIN_LOOKAHEAD && flush == Z_NO_FLUSH) 
				return 1;

            if (s->lookahead == 0) break; /* flush the current block */
        }

        /* Insert the string window[strstart .. strstart+2] in the
         * dictionary, and set hash_head to the head of the hash chain:
         */
        if (s->lookahead >= MIN_MATCH) {
			nCount++;
            INSERT_STRING(s, s->strstart, hash_head);
        }

        /* Find the longest match, discarding those <= prev_length.
         */	    
        s->prev_length = (WORD)s->match_length, s->prev_match = s->match_start;
        s->match_length = MIN_MATCH-1;
										   
        if (hash_head != NIL && s->prev_length < s->max_lazy_match &&
            s->strstart - hash_head <= MAX_DIST(s)) {
            /* To simplify the code, we prevent matches with the string
             * of window index 0 (in particular we have to avoid a match
             * of the string with itself at the start of the input file).
             */
            if (s->strategy != Z_HUFFMAN_ONLY) {
				nCount++;
                s->match_length = longest_match (s, hash_head);
            }
            /* longest_match() sets match_start */
            if (s->match_length > s->lookahead) s->match_length = s->lookahead;

            if (s->match_length <= 5 && (s->strategy == Z_FILTERED ||
                 (s->match_length == MIN_MATCH &&
                  s->strstart - s->match_start > TOO_FAR))) {

                /* If prev_match is also MIN_MATCH, match_start is garbage
                 * but we will ignore the current match anyway.
                 */
				nCount++;
                s->match_length = MIN_MATCH-1;
            }
        }
        /* If there was a match at the previous step and the current
         * match is not better, output the previous match:
         */
        if (s->prev_length >= MIN_MATCH && s->match_length <= s->prev_length) {
            WORD max_insert = (WORD)(s->strstart + s->lookahead - (WORD)MIN_MATCH);
            /* Do not insert strings in hash table beyond this. */

            check_match(s, s->strstart-1, s->prev_match, s->prev_length);

            bflush = ct_tally(s, s->strstart -1 - s->prev_match,
                              s->prev_length - MIN_MATCH);

            /* Insert in hash table all strings up to the end of the match.
             * strstart-1 and strstart are already inserted. If there is not
             * enough lookahead, the last two strings are not inserted in
             * the hash table.
             */
			nCount++;
            s->lookahead = (WORD)((WORD)s->lookahead - (WORD)(s->prev_length-(WORD)1));
            s->prev_length -= 2;
            do {
                if (++s->strstart <= max_insert) {
                    INSERT_STRING(s, s->strstart, hash_head);
                }
            } while (--s->prev_length != 0);
            s->match_available = 0;
            s->match_length = MIN_MATCH-1;
            s->strstart++;

            if (bflush) FLUSH_BLOCK(s, 0);

        } else if (s->match_available) {
            /* If there was no match at the previous position, output a
             * single literal. If there was a match but the current match
             * is longer, truncate the previous match to a single literal.
             */
			nCount++;
            ZTracevv((stderr,"%c", s->window[s->strstart-1]));
            if (ct_tally (s, 0, s->window[s->strstart-1])) {
                FLUSH_BLOCK_ONLY(s, 0);
            }
            s->strstart++;
            s->lookahead--;
            if (s->strm->avail_out == 0) 
				return 1;
        } else {
            /* There is no previous match to compare with, wait for
             * the next step to decide.
             */
			nCount++;
            s->match_available = 1;
            s->strstart++;
            s->lookahead--;
        }
    }
    Assert (flush != Z_NO_FLUSH, "no flush?");
    if (s->match_available) {
		nCount++;
        ZTracevv((stderr,"%c", s->window[s->strstart-1]));
        ct_tally (s, 0, s->window[s->strstart-1]);
        s->match_available = 0;
    }
    FLUSH_BLOCK(s, flush == Z_FINISH);
    return 0;
}


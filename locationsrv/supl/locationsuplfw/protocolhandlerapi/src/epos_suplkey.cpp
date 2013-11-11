/*
 ---------------------------------------------------------------------------
 Copyright (c) 2002, Dr Brian Gladman <                 >, Worcester, UK.
 All rights reserved.

 LICENSE TERMS

 The free distribution and use of this software in both source and binary
 form is allowed (with or without changes) provided that:

   1. distributions of this source code include the above copyright
      notice, this list of conditions and the following disclaimer;

   2. distributions in binary form include the above copyright
      notice, this list of conditions and the following disclaimer
      in the documentation and/or other associated materials;

   3. the copyright holder's name is not used to endorse products
      built using this software without specific written permission.

 ALTERNATIVELY, provided that this notice is retained in full, this product
 may be distributed under the terms of the GNU General Public License (GPL),
 in which case the provisions of the GPL apply INSTEAD OF those given above.

 DISCLAIMER

 This software is provided 'as is' with no explicit or implied warranties
 in respect of its properties, including, but not limited to, correctness
 and/or fitness for purpose.
 ---------------------------------------------------------------------------
*/


//#include "stdafx.h"


#include <libc/string.h>     /* for memcpy() etc.        */
#include <libc/stdlib.h>     /* for _lrotl with VC++     */

#if defined(__GNUC__) || defined(__GNU_LIBRARY__)
#include <byteswap.h>
#include <endian.h>
#endif

#include "sha1.h"
#include "hmac.h"

void derive_key(const unsigned char pwd[],  /* the PASSWORD     */
               unsigned int pwd_len,        /* and its length   */
               const unsigned char salt[],  /* the SALT and its */
               unsigned int salt_len,       /* length           */
               unsigned int iter,   /* the number of iterations */
               unsigned char key[], /* space for the output key */
               unsigned int key_len)/* and its required length  */
{
    unsigned int    i, j, k, n_blk;
    unsigned char uu[OUT_BLOCK_LENGTH], ux[OUT_BLOCK_LENGTH];
    hmac_ctx c1[1], c2[1], c3[1];

    /* set HMAC context (c1) for password               */
    hmac_sha1_begin(c1);
    hmac_sha1_key(pwd, pwd_len, c1);

    /* set HMAC context (c2) for password and salt      */
    memcpy(c2, c1, sizeof(hmac_ctx));
    hmac_sha1_data(salt, salt_len, c2);

    /* find the number of SHA blocks in the key         */
    n_blk = 1 + (key_len - 1) / OUT_BLOCK_LENGTH;

    for(i = 0; i < n_blk; ++i) /* for each block in key */
    {
        /* ux[] holds the running xor value             */
        memset(ux, 0, OUT_BLOCK_LENGTH);

        /* set HMAC context (c3) for password and salt  */
        memcpy(c3, c2, sizeof(hmac_ctx));

        /* enter additional data for 1st block into uu  */
        uu[0] = (unsigned char)((i + 1) >> 24);
        uu[1] = (unsigned char)((i + 1) >> 16);
        uu[2] = (unsigned char)((i + 1) >> 8);
        uu[3] = (unsigned char)(i + 1);

        /* this is the key mixing iteration         */
        for(j = 0, k = 4; j < iter; ++j)
        {
            /* add previous round data to HMAC      */
            hmac_sha1_data(uu, k, c3);

            /* obtain HMAC for uu[]                 */
            hmac_sha1_end(uu, OUT_BLOCK_LENGTH, c3);

            /* xor into the running xor block       */
            for(k = 0; k < OUT_BLOCK_LENGTH; ++k)
                ux[k] ^= uu[k];

            /* set HMAC context (c3) for password   */
            memcpy(c3, c1, sizeof(hmac_ctx));
        }

        /* compile key blocks into the key output   */
        j = 0; k = i * OUT_BLOCK_LENGTH;
        while(j < OUT_BLOCK_LENGTH && k < key_len)
            key[k++] = ux[j++];
    }
}

/*#ifdef TEST*/

#include <stdio.h>

#if 0

struct
{   unsigned int    pwd_len;
    unsigned int    salt_len;
    unsigned int    it_count;
    unsigned char   *pwd;
    unsigned char   salt[32];
    unsigned char   key[32];
} tests[] =
{
    {   8, 4, 5, (unsigned char*)"password",
        {   0x12, 0x34, 0x56, 0x78 },
        {   0x5c, 0x75, 0xce, 0xf0, 0x1a, 0x96, 0x0d, 0xf7,
            0x4c, 0xb6, 0xb4, 0x9b, 0x9e, 0x38, 0xe6, 0xb5 } /* ... */
    },
    {   8, 8, 5, (unsigned char*)"password",
        {   0x12, 0x34, 0x56, 0x78, 0x78, 0x56, 0x34, 0x12 },
        {   0xd1, 0xda, 0xa7, 0x86, 0x15, 0xf2, 0x87, 0xe6,
            0xa1, 0xc8, 0xb1, 0x20, 0xd7, 0x06, 0x2a, 0x49 } /* ... */
    }
};






void get_key(unsigned char key[], 
               unsigned int keylength)
{

	unsigned int    i, j, n=0,bitykeylen = 256;
    unsigned char   bitykey[256];

    for(i = 0; i <1; ++i)
    {
        derive_key(tests[i].pwd, tests[i].pwd_len, tests[i].salt,
                    tests[i].salt_len, tests[i].it_count, bitykey, bitykeylen);
        for(j = 0; j < bitykeylen && j < 64; j += 4)
        {
            if(j % ( keylength / 8 ) == 0 && j!=0 )
                break;
			key[n++] = bitykey[j];
			key[n++]=  bitykey[j+1];
			key[n++]=  bitykey[j+2];
			key[n++] = bitykey[j+3];
        }
    }
}

#endif
/*int _tmain(int argc, _TCHAR* argv[])
{
	unsigned char key[16];
	unsigned int  keylength = 128;
	get_key( key , keylength);
	return 0;
}*/
/*.............hmac...................*/
/* initialise the HMAC context to zero */
void hmac_sha1_begin(hmac_ctx cx[1])
{
    memset(cx, 0, sizeof(hmac_ctx));
}

/* input the HMAC key (can be called multiple times)    */
int hmac_sha1_key(const unsigned char key[], unsigned long key_len, hmac_ctx cx[1])
{
    if(cx->klen == HMAC_IN_DATA)                /* error if further key input   */
        return HMAC_BAD_MODE;                   /* is attempted in data mode    */

    if(cx->klen + key_len > IN_BLOCK_LENGTH)    /* if the key has to be hashed  */
    {
        if(cx->klen <= IN_BLOCK_LENGTH)         /* if the hash has not yet been */
        {                                       /* started, initialise it and   */
            sha1_begin(cx->ctx);                /* hash stored key characters   */
            sha1_hash(cx->key, cx->klen, cx->ctx);
        }

        sha1_hash(key, key_len, cx->ctx);       /* hash long key data into hash */
    }
    else                                        /* otherwise store key data     */
        memcpy(cx->key + cx->klen, key, key_len);

    cx->klen += key_len;                        /* update the key length count  */
    return HMAC_OK;
}

/* input the HMAC data (can be called multiple times) - */
/* note that this call terminates the key input phase   */
void hmac_sha1_data(const unsigned char data[], unsigned long data_len, hmac_ctx cx[1])
{   unsigned int i;

    if(cx->klen != HMAC_IN_DATA)                /* if not yet in data phase */
    {
        if(cx->klen > IN_BLOCK_LENGTH)          /* if key is being hashed   */
        {                                       /* complete the hash and    */
            sha1_end(cx->key, cx->ctx);         /* store the result as the  */
            cx->klen = OUT_BLOCK_LENGTH;        /* key and set new length   */
        }

        /* pad the key if necessary */
        //coverity[overrun-local]
        memset(cx->key + cx->klen, 0, IN_BLOCK_LENGTH - cx->klen);

        /* xor ipad into key value  */
        for(i = 0; i < (IN_BLOCK_LENGTH >> 2); ++i)
            ((unsigned long*)cx->key)[i] ^= 0x36363636;

        /* and start hash operation */
        sha1_begin(cx->ctx);
        sha1_hash(cx->key, IN_BLOCK_LENGTH, cx->ctx);

        /* mark as now in data mode */
        cx->klen = HMAC_IN_DATA;
    }

    /* hash the data (if any)       */
    if(data_len)
        sha1_hash(data, data_len, cx->ctx);
}

/* compute and output the MAC value */
void hmac_sha1_end(unsigned char mac[], unsigned long mac_len, hmac_ctx cx[1])
{   unsigned char dig[OUT_BLOCK_LENGTH];
    unsigned int i;

    /* if no data has been entered perform a null data phase        */
    if(cx->klen != HMAC_IN_DATA)
        hmac_sha1_data((const unsigned char*)0, 0, cx);

    sha1_end(dig, cx->ctx);         /* complete the inner hash      */

    /* set outer key value using opad and removing ipad */
    for(i = 0; i < (IN_BLOCK_LENGTH >> 2); ++i)
        ((unsigned long*)cx->key)[i] ^= 0x36363636 ^ 0x5c5c5c5c;

    /* perform the outer hash operation */
    sha1_begin(cx->ctx);
    sha1_hash(cx->key, IN_BLOCK_LENGTH, cx->ctx);
    sha1_hash(dig, OUT_BLOCK_LENGTH, cx->ctx);
    sha1_end(dig, cx->ctx);

    /* output the hash value            */
    for(i = 0; i < mac_len; ++i)
        mac[i] = dig[i];
}

/* 'do it all in one go' subroutine     */
void hmac_sha1(const unsigned char key[], unsigned int key_len,
          const unsigned char data[], unsigned int data_len,
          unsigned char mac[], unsigned int mac_len)
{   hmac_ctx    cx[1];

    hmac_sha1_begin(cx);
    hmac_sha1_key(key, key_len, cx);
    hmac_sha1_data(data, data_len, cx);
    hmac_sha1_end(mac, mac_len, cx);
}

/****************hmac*******************/

/*****************************sha1******************/
/*
    To obtain the highest speed on processors with 32-bit words, this code
    needs to determine the order in which bytes are packed into such words.
    The following block of code is an attempt to capture the most obvious
    ways in which various environemnts specify their endian definitions.
    It may well fail, in which case the definitions will need to be set by
    editing at the points marked **** EDIT HERE IF NECESSARY **** below.
*/
#define SHA_LITTLE_ENDIAN   1234 /* byte 0 is least significant (i386) */
#define SHA_BIG_ENDIAN      4321 /* byte 0 is most significant (mc68k) */
/*
#if !defined(PLATFORM_BYTE_ORDER)
#if defined(LITTLE_ENDIAN) || defined(BIG_ENDIAN)
#  if defined(LITTLE_ENDIAN) && defined(BIG_ENDIAN)
#    if defined(BYTE_ORDER)
#      if   (BYTE_ORDER == LITTLE_ENDIAN)
#        define PLATFORM_BYTE_ORDER SHA_LITTLE_ENDIAN
#      elif (BYTE_ORDER == BIG_ENDIAN)
#        define PLATFORM_BYTE_ORDER SHA_BIG_ENDIAN
#      endif
#    endif
#  elif defined(LITTLE_ENDIAN) && !defined(BIG_ENDIAN)
#    define PLATFORM_BYTE_ORDER SHA_LITTLE_ENDIAN
#  elif !defined(LITTLE_ENDIAN) && defined(BIG_ENDIAN)
#    define PLATFORM_BYTE_ORDER SHA_BIG_ENDIAN
#  endif
#elif defined(_LITTLE_ENDIAN) || defined(_BIG_ENDIAN)
#  if defined(_LITTLE_ENDIAN) && defined(_BIG_ENDIAN)
#    if defined(_BYTE_ORDER)
#      if   (_BYTE_ORDER == _LITTLE_ENDIAN)
#        define PLATFORM_BYTE_ORDER SHA_LITTLE_ENDIAN
#      elif (_BYTE_ORDER == _BIG_ENDIAN)
#        define PLATFORM_BYTE_ORDER SHA_BIG_ENDIAN
#      endif
#    endif
#  elif defined(_LITTLE_ENDIAN) && !defined(_BIG_ENDIAN)
#    define PLATFORM_BYTE_ORDER SHA_LITTLE_ENDIAN
#  elif !defined(_LITTLE_ENDIAN) && defined(_BIG_ENDIAN)
#    define PLATFORM_BYTE_ORDER SHA_BIG_ENDIAN
#  endif
#elif 0     /#* **** EDIT HERE IF NECESSARY **** */
/*#define PLATFORM_BYTE_ORDER SHA_LITTLE_ENDIAN
/#*#elif 0     /#* **** EDIT HERE IF NECESSARY ****# /
#define PLATFORM_BYTE_ORDER SHA_BIG_ENDIAN
#elif (('1234' >> 24) == '1')
#  define PLATFORM_BYTE_ORDER SHA_LITTLE_ENDIAN
#elif (('4321' >> 24) == '1')
#  define PLATFORM_BYTE_ORDER SHA_BIG_ENDIAN
#endif
#endif
*/
//#define PLATFORM_BYTE_ORDER SHA_BIG_ENDIAN
//#  define PLATFORM_BYTE_ORDER SHA_BIG_ENDIAN
#  define PLATFORM_BYTE_ORDER SHA_LITTLE_ENDIAN
#if !defined(PLATFORM_BYTE_ORDER)
#  error Please set undetermined byte order (lines 87 or 89 of sha1.c).
#endif

#define rotl32(x,n) (((x) << n) | ((x) >> (32 - n)))

#if (PLATFORM_BYTE_ORDER == SHA_BIG_ENDIAN)
#define swap_b32(x) (x)
#elif defined(bswap_32)
#define swap_b32(x) bswap_32(x)
#else
#define swap_b32(x) ((rotl32((x), 8) & 0x00ff00ff) | (rotl32((x), 24) & 0xff00ff00))
#endif

#define SHA1_MASK   (SHA1_BLOCK_SIZE - 1)

/* reverse byte order in 32-bit words       */

#define ch(x,y,z)       (((x) & (y)) ^ (~(x) & (z)))
#define parity(x,y,z)   ((x) ^ (y) ^ (z))
#define maj(x,y,z)      (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

/* A normal version as set out in the FIPS  */

#define rnd(f,k)    \
    t = a; a = rotl32(a,5) + f(b,c,d) + e + k + w[i]; \
    e = d; d = c; c = rotl32(b, 30); b = t

void sha1_compile(sha1_ctx ctx[1])
{   sha1_32t    w[80], i, a, b, c, d, e, t;

    /* note that words are compiled from the buffer into 32-bit */
    /* words in big-endian order so an order reversal is needed */
    /* here on little endian machines                           */
    for(i = 0; i < SHA1_BLOCK_SIZE / 4; ++i)
        w[i] = swap_b32(ctx->wbuf[i]);

    for(i = SHA1_BLOCK_SIZE / 4; i < 80; ++i)
        w[i] = rotl32(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);

    a = ctx->hash[0];
    b = ctx->hash[1];
    c = ctx->hash[2];
    d = ctx->hash[3];
    e = ctx->hash[4];

    for(i = 0; i < 20; ++i)
    {
        rnd(ch, 0x5a827999);
    }

    for(i = 20; i < 40; ++i)
    {
        rnd(parity, 0x6ed9eba1);
    }

    for(i = 40; i < 60; ++i)
    {
        rnd(maj, 0x8f1bbcdc);
    }

    for(i = 60; i < 80; ++i)
    {
        rnd(parity, 0xca62c1d6);
    }

    ctx->hash[0] += a;
    ctx->hash[1] += b;
    ctx->hash[2] += c;
    ctx->hash[3] += d;
    ctx->hash[4] += e;
}

void sha1_begin(sha1_ctx ctx[1])
{
    ctx->count[0] = ctx->count[1] = 0;
    ctx->hash[0] = 0x67452301;
    ctx->hash[1] = 0xefcdab89;
    ctx->hash[2] = 0x98badcfe;
    ctx->hash[3] = 0x10325476;
    ctx->hash[4] = 0xc3d2e1f0;
}

/* SHA1 hash data in an array of bytes into hash buffer and */
/* call the hash_compile function as required.              */

void sha1_hash(const unsigned char data[], unsigned int len, sha1_ctx ctx[1])
{   sha1_32t pos = (sha1_32t)(ctx->count[0] & SHA1_MASK),
             space = SHA1_BLOCK_SIZE - pos;
    const unsigned char *sp = data;

    if((ctx->count[0] += len) < len)
        ++(ctx->count[1]);

    while(len >= space)     /* tranfer whole blocks if possible  */
    {
        memcpy(((unsigned char*)ctx->wbuf) + pos, sp, space);
        sp += space; len -= space; space = SHA1_BLOCK_SIZE; pos = 0;
        sha1_compile(ctx);
    }

    /*lint -e{803} conceivable data overrun */
    /* there are two cases: the above while loop entered or not */
    /* entered. If not entered, 'space = SHA1_BLOCK_SIZE - pos' */
    /* and 'len < space' so that 'len + pos < SHA1_BLOCK_SIZE'. */
    /* If entered, 'pos = 0', 'space = SHA1_BLOCK_SIZE' and     */
    /* 'len < space' so that 'pos + len < SHA1_BLOCK_SIZE'. In  */
    /* both cases, therefore, the memory copy is in the buffer  */

    memcpy(((unsigned char*)ctx->wbuf) + pos, sp, len);
}

/* SHA1 final padding and digest calculation  */

#if (PLATFORM_BYTE_ORDER == SHA_LITTLE_ENDIAN)
static sha1_32t  mask[4] =
    {   0x00000000, 0x000000ff, 0x0000ffff, 0x00ffffff };
static sha1_32t  bits[4] =
    {   0x00000080, 0x00008000, 0x00800000, 0x80000000 };
#else
static sha1_32t  mask[4] =
    {   0x00000000, 0xff000000, 0xffff0000, 0xffffff00 };
static sha1_32t  bits[4] =
    {   0x80000000, 0x00800000, 0x00008000, 0x00000080 };
#endif

void sha1_end(unsigned char hval[], sha1_ctx ctx[1])
{   sha1_32t    i = (sha1_32t)(ctx->count[0] & SHA1_MASK);

 
    /* mask out the rest of any partial 32-bit word and then set    */
    /* the next byte to 0x80. On big-endian machines any bytes in   */
    /* the buffer will be at the top end of 32 bit words, on little */
    /* endian machines they will be at the bottom. Hence the AND    */
    /* and OR masks above are reversed for little endian systems    */
    /* Note that we can always add the first padding byte at this   */
    /* point because the buffer always has at least one empty slot  */
    ctx->wbuf[i >> 2] = (ctx->wbuf[i >> 2] & mask[i & 3]) | bits[i & 3];

    /* we need 9 or more empty positions, one for the padding byte  */
    /* (above) and eight for the length count.  If there is not     */
    /* enough space pad and empty the buffer                        */
    if(i > SHA1_BLOCK_SIZE - 9)
    {
        if(i < 60) ctx->wbuf[15] = 0;
        sha1_compile(ctx);
        i = 0;
    }
    else    /* compute a word index for the empty buffer positions  */
        i = (i >> 2) + 1;

    while(i < 14) /* and zero pad all but last two positions        */
        ctx->wbuf[i++] = 0;

    /* assemble the eight byte counter in in big-endian format      */
    ctx->wbuf[14] = swap_b32((ctx->count[1] << 3) | (ctx->count[0] >> 29));
    ctx->wbuf[15] = swap_b32(ctx->count[0] << 3);

    sha1_compile(ctx);

    /* extract the hash value as bytes in case the hash buffer is   */
    /* misaligned for 32-bit words                                  */
    /*lint -e{504} unusual shift operation (unusually formed right argument) */
    for(i = 0; i < SHA1_DIGEST_SIZE; ++i)
        hval[i] = (unsigned char)(ctx->hash[i >> 2] >> (8 * (~i & 3)));
}

void sha1(unsigned char hval[], const unsigned char data[], unsigned int len)
{   sha1_ctx    cx[1];

    sha1_begin(cx); sha1_hash(data, len, cx); sha1_end(hval, cx);
}

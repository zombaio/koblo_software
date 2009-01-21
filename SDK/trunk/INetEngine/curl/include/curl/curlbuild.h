/* include/curl/curlbuild.h.  Generated from curlbuild.h.in by configure.  */
#ifndef __CURL_CURLBUILD_H
#define __CURL_CURLBUILD_H
/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2008, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at http://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 * $Id: curlbuild.h.in,v 1.6 2008-08-25 13:42:53 yangtse Exp $
 ***************************************************************************/

/* ================================================================ */
/*               NOTES FOR CONFIGURE CAPABLE SYSTEMS                */
/* ================================================================ */

/*
 * NOTE 1:
 * -------
 *
 * Nothing in this file is intended to be modified or adjusted by the
 * curl library user nor by the curl library builder.
 *
 * If you think that something actually needs to be changed, adjusted
 * or fixed in this file, then, report it on the libcurl development
 * mailing list: http://cool.haxx.se/mailman/listinfo/curl-library/
 *
 * This header file shall only export symbols which are 'curl' or 'CURL'
 * prefixed, otherwise public name space would be polluted.
 *
 * NOTE 2:
 * -------
 *
 * Right now you might be staring at file include/curl/curlbuild.h.in or
 * at file include/curl/curlbuild.h, this is due to the following reason:
 *
 * On systems capable of running the configure script, the configure process
 * will overwrite the distributed include/curl/curlbuild.h file with one that
 * is suitable and specific to the library being configured and built, which
 * is generated from the include/curl/curlbuild.h.in template file.
 *
 */

/* ================================================================ */
/*  DEFINITION OF THESE SYMBOLS SHALL NOT TAKE PLACE ANYWHERE ELSE  */
/* ================================================================ */


// (lasse) File modified a little to compile for both OS X and Windows


#ifdef CURL_SIZEOF_LONG
#  error "CURL_SIZEOF_LONG shall not be defined except in curlbuild.h"
   Error Compilation_aborted_CURL_SIZEOF_LONG_already_defined
#endif

#ifdef CURL_TYPEOF_CURL_OFF_T
#  error "CURL_TYPEOF_CURL_OFF_T shall not be defined except in curlbuild.h"
   Error Compilation_aborted_CURL_TYPEOF_CURL_OFF_T_already_defined
#endif

#ifdef CURL_FORMAT_CURL_OFF_T
#  error "CURL_FORMAT_CURL_OFF_T shall not be defined except in curlbuild.h"
   Error Compilation_aborted_CURL_FORMAT_CURL_OFF_T_already_defined
#endif

#ifdef CURL_FORMAT_CURL_OFF_TU
#  error "CURL_FORMAT_CURL_OFF_TU shall not be defined except in curlbuild.h"
   Error Compilation_aborted_CURL_FORMAT_CURL_OFF_TU_already_defined
#endif

#ifdef CURL_FORMAT_OFF_T
#  error "CURL_FORMAT_OFF_T shall not be defined except in curlbuild.h"
   Error Compilation_aborted_CURL_FORMAT_OFF_T_already_defined
#endif

#ifdef CURL_SIZEOF_CURL_OFF_T
#  error "CURL_SIZEOF_CURL_OFF_T shall not be defined except in curlbuild.h"
   Error Compilation_aborted_CURL_SIZEOF_CURL_OFF_T_already_defined
#endif

#ifdef CURL_SUFFIX_CURL_OFF_T
#  error "CURL_SUFFIX_CURL_OFF_T shall not be defined except in curlbuild.h"
   Error Compilation_aborted_CURL_SUFFIX_CURL_OFF_T_already_defined
#endif

#ifdef CURL_SUFFIX_CURL_OFF_TU
#  error "CURL_SUFFIX_CURL_OFF_TU shall not be defined except in curlbuild.h"
   Error Compilation_aborted_CURL_SUFFIX_CURL_OFF_TU_already_defined
#endif

/* ================================================================ */
/*  EXTERNAL INTERFACE SETTINGS FOR CONFIGURE CAPABLE SYSTEMS ONLY  */
/* ================================================================ */

/* Configure process defines this to 1 when it finds out that system   */
/* header file sys/types.h must be included by the external interface. */
#define CURL_PULL_SYS_TYPES_H 1
#ifdef CURL_PULL_SYS_TYPES_H
#  include <sys/types.h>
#endif

// (lasse) This part modified
/* Configure process defines this to 1 when it finds out that system */
/* header file stdint.h must be included by the external interface.  */
#define CURL_PULL_STDINT_H 1
#ifdef CURL_PULL_STDINT_H
#	ifndef _WIN32
#		include <stdint.h>
#	endif // _WIN32
#endif

// (lasse) This part modified
/* Configure process defines this to 1 when it finds out that system  */
/* header file inttypes.h must be included by the external interface. */
#define CURL_PULL_INTTYPES_H 1
#ifdef CURL_PULL_INTTYPES_H
#	ifndef _WIN32
#		include <inttypes.h>
#	endif // _WIN32
#endif

/* The size of `long', as computed by sizeof. */
#define CURL_SIZEOF_LONG 4

// (lasse) This part modified
/* Signed integral data type used for curl_off_t. */
#ifdef _WIN32
#	define CURL_TYPEOF_CURL_OFF_T __int64
#else _WIN32
#	define CURL_TYPEOF_CURL_OFF_T int64_t
#endif // _WIN32

/* Data type definition of curl_off_t. */
typedef CURL_TYPEOF_CURL_OFF_T curl_off_t;

// (lasse) This part modified
/* curl_off_t formatting string directive without "%" conversion specifier. */
#ifdef _WIN32
#	define CURL_FORMAT_CURL_OFF_T "I64d"
#else _WIN32
#	define CURL_FORMAT_CURL_OFF_T "lld"
#endif // #else _WIN32

// (lasse) This part modified
/* unsigned curl_off_t formatting string without "%" conversion specifier. */
#ifdef _WIN32
#	define CURL_FORMAT_CURL_OFF_TU "I64u"
#else _WIN32
#	define CURL_FORMAT_CURL_OFF_TU "llu"
#endif // #else _WIN32

// (lasse) This part modified
/* curl_off_t formatting string directive with "%" conversion specifier. */
#ifdef _WIN32
#	define CURL_FORMAT_OFF_T "%I64d"
#else _WIN32
#	define CURL_FORMAT_OFF_T "%lld"
#endif // #else _WIN32

/* The size of `curl_off_t', as computed by sizeof. */
#define CURL_SIZEOF_CURL_OFF_T 8

/* curl_off_t constant suffix. */
#define CURL_SUFFIX_CURL_OFF_T LL

/* unsigned curl_off_t constant suffix. */
#define CURL_SUFFIX_CURL_OFF_TU ULL

#endif /* __CURL_CURLBUILD_H */
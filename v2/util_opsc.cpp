
/*----------------------------------------------------------------------------
    ChucK Concurrent, On-the-fly Audio Programming Language
      Compiler and Virtual Machine

    Copyright (c) 2004 Ge Wang and Perry R. Cook.  All rights reserved.
      http://chuck.cs.princeton.edu/
      http://soundlab.cs.princeton.edu/

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
    U.S.A.
-----------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// file: util_opsc.cpp
// desc: ...
//
// author: Philip L. Davidson (philipd@alumni.princeton.edu)
//         Ge Wang (gewang@cs.princeton.edu)
//         Perry R. Cook (prc@cs.princeton.edu)
// date: Spring 2005
//-----------------------------------------------------------------------------

#include "util_opsc.h"

// squeeze the whole wad of OSC-Kit code in here. 


// OSC-pattern-match.cpp

/*
Copyright � 1998. The Regents of the University of California (Regents). 
All Rights Reserved.

Written by Matt Wright, The Center for New Music and Audio Technologies,
University of California, Berkeley.

Permission to use, copy, modify, distribute, and distribute modified versions
of this software and its documentation without fee and without a signed
licensing agreement, is hereby granted, provided that the above copyright
notice, this paragraph and the following two paragraphs appear in all copies,
modifications, and distributions.

IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF REGENTS HAS
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED
HEREUNDER IS PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

The OpenSound Control WWW page is 
    http://www.cnmat.berkeley.edu/OpenSoundControl
*/



/*
    OSC-pattern-match.c
    Matt Wright, 3/16/98
    Adapted from oscpattern.c, by Matt Wright and Amar Chaudhury
 */

//#include "OSC-pattern-match.h"

static const char *theWholePattern;	/* Just for warning messages */

static bool MatchBrackets (const char *pattern, const char *test);
static bool MatchList (const char *pattern, const char *test);

bool PatternMatch (const char *  pattern, const char * test) {
   theWholePattern = pattern;
   
   if (pattern == 0 || pattern[0] == 0) {
      return test[0] == 0;
   } 
   
   if (test[0] == 0) {
      if (pattern[0] == '*')
         return PatternMatch (pattern+1,test);
      else
         return FALSE;
   }
   
   switch (pattern[0]) {
   case 0      : return test[0] == 0;
   case '?'    : return PatternMatch (pattern + 1, test + 1);
   case '*'    : 
      if (PatternMatch (pattern+1, test)) {
         return TRUE;
      } else {
         return PatternMatch (pattern, test+1);
      }
   case ']'    :
   case '}'    :
      fprintf(stderr, "Spurious %c in pattern \".../%s/...\"",pattern[0], theWholePattern);
      return FALSE;
   case '['    :
      return MatchBrackets (pattern,test);
   case '{'    :
      return MatchList (pattern,test);
   case '\\'   :  
      if (pattern[1] == 0) {
         return test[0] == 0;
      } else if (pattern[1] == test[0]) {
         return PatternMatch (pattern+2,test+1);
      } else {
         return FALSE;
      }
   default     :
      if (pattern[0] == test[0]) {
         return PatternMatch (pattern+1,test+1);
      } else {
         return FALSE;
      }
   }
}


/* we know that pattern[0] == '[' and test[0] != 0 */

static bool MatchBrackets (const char *pattern, const char *test) {
  bool result;
  bool negated = FALSE;
  const char *p = pattern;

  if (pattern[1] == 0) {
    fprintf(stderr, "Unterminated [ in pattern \".../%s/...\"", theWholePattern);
    return FALSE;
  }

  if (pattern[1] == '!') {
    negated = TRUE;
    p++;
  }

  while (*p != ']') {
    if (*p == 0) {
      fprintf(stderr,"Unterminated [ in pattern \".../%s/...\"", theWholePattern);
      return FALSE;
    }
    if (p[1] == '-' && p[2] != 0) {
      if (test[0] >= p[0] && test[0] <= p[2]) {
	result = !negated;
	goto advance;
      }
    }
    if (p[0] == test[0]) {
      result = !negated;
      goto advance;
    }
    p++;
  }

  result = negated;

advance:

  if (!result)
    return FALSE;

  while (*p != ']') {
    if (*p == 0) {
      fprintf(stderr,"Unterminated [ in pattern \".../%s/...\"", theWholePattern);
      return FALSE;
    }
    p++;
  }

  return PatternMatch (p+1,test+1);
}

static bool MatchList (const char *pattern, const char *test) {

 const char *restOfPattern, *tp = test;


 for(restOfPattern = pattern; *restOfPattern != '}'; restOfPattern++) {
   if (*restOfPattern == 0) {
     fprintf(stderr,"Unterminated { in pattern \".../%s/...\"", theWholePattern);
     return FALSE;
   }
 }

 restOfPattern++; /* skip close curly brace */


 pattern++; /* skip open curly brace */

 while (1) {
   
   if (*pattern == ',') {
     if (PatternMatch (restOfPattern, tp)) {
       return TRUE;
     } else {
       tp = test;
       ++pattern;
     }
   } else if (*pattern == '}') {
     return PatternMatch (restOfPattern, tp);
   } else if (*pattern == *tp) {
     ++pattern;
     ++tp;
   } else {
     tp = test;
     while (*pattern != ',' && *pattern != '}') {
       pattern++;
     }
     if (*pattern == ',') {
       pattern++;
     }
   }
 }
}

// end OSC-pattern-match.h


// OSC-timetag.cpp

/*
Copyright (c) 1998.  The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by Matt Wright, The Center for New Music and Audio Technologies,
University of California, Berkeley.

     IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
     ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
     REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

     REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
     DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
     REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
     ENHANCEMENTS, OR MODIFICATIONS.

The OpenSound Control WWW page is 
    http://www.cnmat.berkeley.edu/OpenSoundControl
*/

/*

 OSC_timeTag.c: library for manipulating OSC time tags
 Matt Wright, 5/29/97

 Version 0.2 (9/11/98): cleaned up so no explicit type names in the .c file.

*/

//#include "OSC-timetag.h"


#ifdef HAS8BYTEINT
#define TWO_TO_THE_32_FLOAT 4294967296.0f

OSCTimeTag OSCTT_Immediately(void) {
    return (OSCTimeTag) 1;
}

OSCTimeTag OSCTT_BiggestPossibleTimeTag(void) {
    return (OSCTimeTag) 0xffffffffffffffff;
}

OSCTimeTag OSCTT_PlusSeconds(OSCTimeTag original, float secondsOffset) {
    int64 offset = (int64) (secondsOffset * TWO_TO_THE_32_FLOAT);

/*    printf("* OSCTT_PlusSeconds %llx plus %f seconds (i.e., %lld offset) is %llx\n", original,
	      secondsOffset, offset, original + offset);  */

    return original + offset;
}

int OSCTT_Compare(OSCTimeTag left, OSCTimeTag right) {
#if 0
    printf("***** OSCTT_Compare(%llx, %llx): %d\n", left, right,
	   (left<right) ? -1 : ((left == right) ? 0 : 1));
#endif
    if (left < right) {
        return -1;
    } else if (left == right) {
        return 0;
    } else {
        return 1;
    }
}

#ifdef __sgi
#include <sys/time.h>

#define SECONDS_FROM_1900_to_1970 2208988800 /* 17 leap years */
#define TWO_TO_THE_32_OVER_ONE_MILLION 4295


OSCTimeTag OSCTT_CurrentTime(void) {
    uint64 result;
    uint32 usecOffset;
    struct timeval tv;
    struct timezone tz;

    BSDgettimeofday(&tv, &tz);

    /* First get the seconds right */
    result = (unsigned) SECONDS_FROM_1900_to_1970 + 
	     (unsigned) tv.tv_sec - 
	     (unsigned) 60 * tz.tz_minuteswest +
             (unsigned) (tz.tz_dsttime ? 3600 : 0);

#if 0
    /* No timezone, no DST version ... */
    result = (unsigned) SECONDS_FROM_1900_to_1970 + 
	     (unsigned) tv.tv_sec;
#endif


    /* make seconds the high-order 32 bits */
    result = result << 32;
	
    /* Now get the fractional part. */
    usecOffset = (unsigned) tv.tv_usec * (unsigned) TWO_TO_THE_32_OVER_ONE_MILLION;
    /* printf("** %ld microsec is offset %x\n", tv.tv_usec, usecOffset); */

    result += usecOffset;

/*    printf("* OSCTT_CurrentTime is %llx\n", result); */
    return result;
}

#else /* __sgi */

/* Instead of asking your operating system what time it is, it might be
   clever to find out the current time at the instant your application 
   starts audio processing, and then keep track of the number of samples
   output to know how much time has passed. */

/* Loser version for systems that have no ability to tell the current time: */
OSCTimeTag OSCTT_CurrentTime(void) {
    return (OSCTimeTag) 1;
}

#endif /* __sgi */


#else /* Not HAS8BYTEINT */

OSCTimeTag OSCTT_CurrentTime(void) {
    OSCTimeTag result;
    result.seconds = 0;
    result.fraction = 1;
    return result;
}

OSCTimeTag OSCTT_BiggestPossibleTimeTag(void) {
    OSCTimeTag result;
    result.seconds = 0xffffffff;
    result.fraction = 0xffffffff;
    return result;
}

OSCTimeTag OSCTT_Immediately(void) {
    OSCTimeTag result;
    result.seconds = 0;
    result.fraction = 1;
    return result;
}

OSCTimeTag OSCTT_PlusSeconds(OSCTimeTag original, float secondsOffset) {
    OSCTimeTag result;
    result.seconds = 0;
    result.fraction = 1;
    return result;
}

int OSCTT_Compare(OSCTimeTag left, OSCTimeTag right) {
    /* Untested! */
    int highResult = left.seconds - right.seconds;

    if (highResult != 0) return highResult;

    return left.fraction - right.fraction;
}


#endif /* HAS8BYTEINT */

// end OSC-timetag.cpp

// OSC-client.cpp

/*
Copyright (c) 1996.  The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by Matt Wright, The Center for New Music and Audio Technologies,
University of California, Berkeley.

     IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
     ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
     REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

     REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
     DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
     REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
     ENHANCEMENTS, OR MODIFICATIONS.
*/


/* 
  Author: Matt Wright
  Version 2.2: Calls htonl in the right places 20000620
  Version 2.3: Gets typed messages right.
 */


/* Here are the possible values of the state field: */

#define EMPTY 0	       /* Nothing written to packet yet */
#define ONE_MSG_ARGS 1 /* Packet has a single message; gathering arguments */
#define NEED_COUNT 2   /* Just opened a bundle; must write message name or
                          open another bundle */
#define GET_ARGS 3     /* Getting arguments to a message.  If we see a message
			  name or a bundle open/close then the current message
			  will end. */
#define DONE 4         /* All open bundles have been closed, so can't write 
		          anything else */


//#include "OSC-client.h"

char *OSC_errorMessage;


//static int strlen(char *s);
static int OSC_padString(char *dest, char *str);
static int OSC_padStringWithAnExtraStupidComma(char *dest, char *str);
static int OSC_WritePadding(char *dest, int i);
static int CheckTypeTag(OSCbuf *buf, char expectedType);

void OSC_initBuffer(OSCbuf *buf, int size, char *byteArray) {
    buf->buffer = byteArray;
    buf->size = size;
    OSC_resetBuffer(buf);
}

void OSC_resetBuffer(OSCbuf *buf) {	
    buf->bufptr = buf->buffer;
    buf->state = EMPTY;
    buf->bundleDepth = 0;
    buf->prevCounts[0] = 0;
    buf->gettingFirstUntypedArg = 0;
    buf->typeStringPtr = 0;
}

int OSC_isBufferEmpty(OSCbuf *buf) {
    return buf->bufptr == buf->buffer;
}

int OSC_freeSpaceInBuffer(OSCbuf *buf) {
    return buf->size - (buf->bufptr - buf->buffer);
}

int OSC_isBufferDone(OSCbuf *buf) {
    return (buf->state == DONE || buf->state == ONE_MSG_ARGS);
}

char *OSC_getPacket(OSCbuf *buf) {
#ifdef ERROR_CHECK_GETPACKET
    if (buf->state == DONE || buf->state == ONE_MSG_ARGS) {
	return buf->buffer;
    } else {
	OSC_errorMessage = "Packet has unterminated bundles";
	return 0;
    }
#else
    return buf->buffer;
#endif
}

int OSC_packetSize(OSCbuf *buf) {
#ifdef ERROR_CHECK_PACKETSIZE
    if (buf->state == DONE || buf->state == ONE_MSG_ARGS) {
	return (buf->bufptr - buf->buffer);
    } else {
        OSC_errorMessage = "Packet has unterminated bundles";
        return 0;
    }
#else
    return (buf->bufptr - buf->buffer);
#endif
}

#define CheckOverflow(buf, bytesNeeded) { \
    if ((bytesNeeded) > OSC_freeSpaceInBuffer(buf)) { \
	OSC_errorMessage = "buffer overflow"; \
	return 1; \
    } \
}

static void PatchMessageSize(OSCbuf *buf) {
    int4byte size;
    size = buf->bufptr - ((char *) buf->thisMsgSize) - 4;
    *(buf->thisMsgSize) = htonl(size);
}

int OSC_openBundle(OSCbuf *buf, OSCTimeTag tt) {
    if (buf->state == ONE_MSG_ARGS) {
	OSC_errorMessage = "Can't open a bundle in a one-message packet";
	return 3;
    }

    if (buf->state == DONE) {
	OSC_errorMessage = "This packet is finished; can't open a new bundle";
	return 4;
    }

    if (++(buf->bundleDepth) >= MAX_BUNDLE_NESTING) {
	OSC_errorMessage = "Bundles nested too deeply; change MAX_BUNDLE_NESTING in OpenSoundControl.h";
	return 2;
    }

    if (CheckTypeTag(buf, '\0')) return 9;

    if (buf->state == GET_ARGS) {
	PatchMessageSize(buf);
    }

    if (buf->state == EMPTY) {
	/* Need 16 bytes for "#bundle" and time tag */
	CheckOverflow(buf, 16);
    } else {
	/* This bundle is inside another bundle, so we need to leave
	   a blank size count for the size of this current bundle. */
	CheckOverflow(buf, 20);
	*((int4byte *)buf->bufptr) = 0xaaaaaaaa;
        buf->prevCounts[buf->bundleDepth] = (int4byte *)buf->bufptr;

	buf->bufptr += 4;
    }

    buf->bufptr += OSC_padString(buf->bufptr, "#bundle");


    *((OSCTimeTag *) buf->bufptr) = tt;

    if (htonl(1) != 1) {
	/* Byte swap the 8-byte integer time tag */
	int4byte *intp = (int4byte *)buf->bufptr;
	intp[0] = htonl(intp[0]);
	intp[1] = htonl(intp[1]);

#ifdef HAS8BYTEINT
	{ /* tt is a 64-bit int so we have to swap the two 32-bit words. 
	    (Otherwise tt is a struct of two 32-bit words, and even though
	     each word was wrong-endian, they were in the right order
	     in the struct.) */
	    int4byte temp = intp[0];
	    intp[0] = intp[1];
	    intp[1] = temp;
	}
#endif
    }

    buf->bufptr += sizeof(OSCTimeTag);

    buf->state = NEED_COUNT;

    buf->gettingFirstUntypedArg = 0;
    buf->typeStringPtr = 0;
    return 0;
}

int OSC_closeBundle(OSCbuf *buf) {
    if (buf->bundleDepth == 0) {
	/* This handles EMPTY, ONE_MSG, ARGS, and DONE */
	OSC_errorMessage = "Can't close bundle; no bundle is open!";
	return 5;
    }

    if (CheckTypeTag(buf, '\0')) return 9;

    if (buf->state == GET_ARGS) {
        PatchMessageSize(buf);
    }

    if (buf->bundleDepth == 1) {
	/* Closing the last bundle: No bundle size to patch */
	buf->state = DONE;
    } else {
	/* Closing a sub-bundle: patch bundle size */
	int size = buf->bufptr - ((char *) buf->prevCounts[buf->bundleDepth]) - 4;
	*(buf->prevCounts[buf->bundleDepth]) = htonl(size);
	buf->state = NEED_COUNT;
    }

    --buf->bundleDepth;
    buf->gettingFirstUntypedArg = 0;
    buf->typeStringPtr = 0;
    return 0;
}


int OSC_closeAllBundles(OSCbuf *buf) {
    if (buf->bundleDepth == 0) {
        /* This handles EMPTY, ONE_MSG, ARGS, and DONE */
        OSC_errorMessage = "Can't close all bundles; no bundle is open!";
        return 6;
    }

    if (CheckTypeTag(buf, '\0')) return 9;

    while (buf->bundleDepth > 0) {
	OSC_closeBundle(buf);
    }
    buf->typeStringPtr = 0;
    return 0;
}

int OSC_writeAddress(OSCbuf *buf, char *name) {
    int4byte paddedLength;

    if (buf->state == ONE_MSG_ARGS) {
	OSC_errorMessage = "This packet is not a bundle, so you can't write another address";
	return 7;
    }

    if (buf->state == DONE) {
        OSC_errorMessage = "This packet is finished; can't write another address";
        return 8;
    }

    if (CheckTypeTag(buf, '\0')) return 9;

    paddedLength = OSC_effectiveStringLength(name);

    if (buf->state == EMPTY) {
	/* This will be a one-message packet, so no sizes to worry about */
	CheckOverflow(buf, paddedLength);
	buf->state = ONE_MSG_ARGS;
    } else {
	/* GET_ARGS or NEED_COUNT */
	CheckOverflow(buf, 4+paddedLength);
	if (buf->state == GET_ARGS) {
	    /* Close the old message */
	    PatchMessageSize(buf);
	}
	buf->thisMsgSize = (int4byte *)buf->bufptr;
	*(buf->thisMsgSize) = 0xbbbbbbbb;
	buf->bufptr += 4;
	buf->state = GET_ARGS;
    }

    /* Now write the name */
    buf->bufptr += OSC_padString(buf->bufptr, name);
    buf->typeStringPtr = 0;
    buf->gettingFirstUntypedArg = 1;

    return 0;
}

int OSC_writeAddressAndTypes(OSCbuf *buf, char *name, char *types) {
    int result;
    int4byte paddedLength;

    if (CheckTypeTag(buf, '\0')) return 9;

    result = OSC_writeAddress(buf, name);

    if (result) return result;

    paddedLength = OSC_effectiveStringLength(types);

    CheckOverflow(buf, paddedLength);    

    buf->typeStringPtr = buf->bufptr + 1; /* skip comma */
    buf->bufptr += OSC_padString(buf->bufptr, types);
    
    buf->gettingFirstUntypedArg = 0;
    return 0;
}

static int CheckTypeTag(OSCbuf *buf, char expectedType) {
    if (buf->typeStringPtr) {
	if (*(buf->typeStringPtr) != expectedType) {
	    if (expectedType == '\0') {
		OSC_errorMessage =
		    "According to the type tag I expected more arguments.";
	    } else if (*(buf->typeStringPtr) == '\0') {
		OSC_errorMessage =
		    "According to the type tag I didn't expect any more arguments.";
	    } else {
		OSC_errorMessage =
		    "According to the type tag I expected an argument of a different type.";
		printf("* Expected %c, string now %s\n", expectedType, buf->typeStringPtr);
	    }
	    return 9; 
	}
	++(buf->typeStringPtr);
    }
    return 0;
}


int OSC_writeFloatArg(OSCbuf *buf, float arg) {
    int4byte *intp;

    CheckOverflow(buf, 4);

    if (CheckTypeTag(buf, 'f')) return 9;

    /* Pretend arg is a long int so we can use htonl() */
    intp = ((int4byte *) &arg);
    *((int4byte *) buf->bufptr) = htonl(*intp);

    buf->bufptr += 4;

    buf->gettingFirstUntypedArg = 0;
    return 0;
}



int OSC_writeFloatArgs(OSCbuf *buf, int numFloats, float *args) {
    int i;
    int4byte *intp;

    CheckOverflow(buf, 4 * numFloats);

    /* Pretend args are long ints so we can use htonl() */
    intp = ((int4byte *) args);

    for (i = 0; i < numFloats; i++) {
	if (CheckTypeTag(buf, 'f')) return 9;
	*((int4byte *) buf->bufptr) = htonl(intp[i]);
	buf->bufptr += 4;
    }

    buf->gettingFirstUntypedArg = 0;
    return 0;
}

int OSC_writeIntArg(OSCbuf *buf, int4byte arg) {
    CheckOverflow(buf, 4);
    if (CheckTypeTag(buf, 'i')) return 9;

    *((int4byte *) buf->bufptr) = htonl(arg);
    buf->bufptr += 4;

    buf->gettingFirstUntypedArg = 0;
    return 0;
}

int OSC_writeStringArg(OSCbuf *buf, char *arg) {
    int len;

    if (CheckTypeTag(buf, 's')) return 9;

    len = OSC_effectiveStringLength(arg);

    if (buf->gettingFirstUntypedArg && arg[0] == ',') {
	/* This un-type-tagged message starts with a string
	   that starts with a comma, so we have to escape it
	   (with a double comma) so it won't look like a type
	   tag string. */

	CheckOverflow(buf, len+4); /* Too conservative */
	buf->bufptr += 
	    OSC_padStringWithAnExtraStupidComma(buf->bufptr, arg);

    } else {
	CheckOverflow(buf, len);
	buf->bufptr += OSC_padString(buf->bufptr, arg);
    }

    buf->gettingFirstUntypedArg = 0;
    return 0;

}

/* String utilities */
/*
static int strlen(char *s) {
    int i;
    for (i=0; s[i] != '\0'; i++);
    return i;
}
*/
#define STRING_ALIGN_PAD 4
int OSC_effectiveStringLength(char *string) {
    int len = strlen(string) + 1;  /* We need space for the null char. */
    
    /* Round up len to next multiple of STRING_ALIGN_PAD to account for alignment padding */
    if ((len % STRING_ALIGN_PAD) != 0) {
        len += STRING_ALIGN_PAD - (len % STRING_ALIGN_PAD);
    }
    return len;
}

static int OSC_padString(char *dest, char *str) {
    int i;
    
    for (i = 0; str[i] != '\0'; i++) {
        dest[i] = str[i];
    }
    
    return OSC_WritePadding(dest, i);
}

static int OSC_padStringWithAnExtraStupidComma(char *dest, char *str) {
    int i;
    
    dest[0] = ',';
    for (i = 0; str[i] != '\0'; i++) {
        dest[i+1] = str[i];
    }

    return OSC_WritePadding(dest, i+1);
}
 
static int OSC_WritePadding(char *dest, int i) {
    dest[i] = '\0';
    i++;

    for (; (i % STRING_ALIGN_PAD) != 0; i++) {
	dest[i] = '\0';
    }

    return i;
}


// end OSC-client.cpp



// ACTUAL CODE


//chuck networking code
#include "util_network.h"
#include "util_thread.h"

// FROM PLATFORM.H -UDP TRANSMITTER / RECEIVER 

#if defined(__PLATFORM_WIN32__)
#include <winsock.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#ifdef __MACOSX_CORE__
#define SOCKET int
#define SOCKADDR_IN struct sockaddr_in
#endif

#include "stdarg.h"

enum udp_stat { UDP_NOINIT, UDP_UNBOUND, UDP_BOUND, UDP_READY, UDP_ERROR, UDP_NUM };

class UDP_Receiver { 

protected:
    ck_socket     _sock;
    udp_stat      _status;

    SOCKADDR_IN   _remote_addr;
    int           _remote_addr_len;
    int           _mesgLen;
public:

    UDP_Receiver();
    
    ~UDP_Receiver() { }
    void init();
    void bind_to_port(int port);
    int recv_next(char *buffer, int size);   
    void close_sock();
    
};

class UDP_Transmitter { 
protected:
    
    ck_socket     _sock;
    SOCKADDR_IN    _host_addr;
    udp_stat       _status;
    
public:
    
    UDP_Transmitter() { 
        _status = UDP_NOINIT; 
    }
    ~UDP_Transmitter() {}
    void  init();
    
    void  set_host(char * hostaddress, int port);
    int   send(char *buffer, int size);
    udp_stat status(void) { return _status; } 
    void  close_sock();
    
};


UDP_Receiver::UDP_Receiver() 
{ 
    _status = UDP_NOINIT; 
}


void
UDP_Receiver::init() { 

   //open port 
   _sock = ck_udp_create();    
   _status = ( _sock != NULL ) ?  UDP_UNBOUND : UDP_ERROR;

}

void
UDP_Receiver::bind_to_port(int port) { 

   // Fill in the interface information
   _status =  ( ck_bind ( _sock, port ) ) ?  UDP_BOUND : UDP_ERROR ;

}

int
UDP_Receiver::recv_next(char * buffer, int bsize) { 

   if ( _status != UDP_BOUND ) 
   { 
      fprintf( stderr, "socket not bound!\n"); return 0; 
   }

   _remote_addr_len = sizeof(_remote_addr);
   int ret = ck_recvfrom(_sock, buffer, bsize, (struct sockaddr *) &_remote_addr, &_remote_addr_len );

   return ( ret <= 0 ) ? 0 : ret;

}

void
UDP_Receiver::close_sock() {
    ck_close ( _sock );
    _status = UDP_NOINIT;
}


void
UDP_Transmitter::init() {    
   //open socket
    _sock = ck_udp_create();
    _status = ( _sock != NULL ) ? UDP_READY : UDP_ERROR; 
}


void
UDP_Transmitter::set_host(char * hostaddress, int port) { 

   // Fill in the interface information

   struct hostent * host;
//   printf("gathering host information...");
   _host_addr.sin_family = AF_INET;
   _host_addr.sin_port = htons(port);
   _host_addr.sin_addr.s_addr = inet_addr(hostaddress);

   if(_host_addr.sin_addr.s_addr == INADDR_NONE) // The address wasn't in numeric
   {
      host = NULL;
      host = gethostbyname(hostaddress); // Get the IP address of the server
      if(host == NULL)
      {
         printf("UDP_Transmitter::error\nUnknown host: %s\n", hostaddress);
         _status = UDP_ERROR;
         return;
      }
      memcpy(&_host_addr.sin_addr, host->h_addr_list[0], host->h_length);
//        printf("   ...set\n");
   } 
//   else printf("   ...set\n");
   _status = UDP_READY;
}


int
UDP_Transmitter::send(char * buffer, int bsize) { 

   if ( _status != UDP_READY ) { fprintf( stderr, "socket not bound!\n"); return 0; }   
   int ret = ck_sendto(_sock, buffer, bsize, (struct sockaddr *) &_host_addr, sizeof(_host_addr) );
   return ( ret <= 0 ) ? 0 : ret ;

}

void
UDP_Transmitter::close_sock() {    
    ck_close ( _sock );
}


// OSC_TRANSMITTER

OSC_Transmitter::OSC_Transmitter()  { 
    _out = new UDP_Transmitter();
    _holdMessage = false;
    init();
}

OSC_Transmitter::OSC_Transmitter( UDP_Transmitter * out ) { 
    _out = out;
    _holdMessage = false;
}

OSC_Transmitter::~OSC_Transmitter() { delete _out; } 

void 
OSC_Transmitter::init() { 
   if ( _out->status() == UDP_NOINIT ) _out->init();
   OSC_initBuffer(&_osc, sizeof(_buffer), _buffer );
}

void 
OSC_Transmitter::setHost( char * hadd, int p ) { 
   _out->set_host(hadd, p);
}

void
OSC_Transmitter::presend( char * buf, int sz ) { 
   _out->send(buf,sz);
}

void
OSC_Transmitter::openBundle( OSCTimeTag t) { 
   OSC_openBundle( &_osc, t);
}


void
OSC_Transmitter::closeBundle() { 
   OSC_closeBundle( &_osc );
   tryMessage();
}


void
OSC_Transmitter::addMessage( char *address, char * args, ...) { 

   if (args == NULL || args[0] == '\0')	 { 					   // If There's No Text
       OSC_writeAddress( &_osc, address );				           //  Nothing
      return;
   }

   va_list		tags;							   // Pointer To List Of Arguments
   va_start(tags, args);							   // Parses The String For Variables
   
   OSC_writeAddressAndTypes( &_osc, address, args );

   int argnum = strlen(args);
   int osc_err = 0;
   for ( int j = 1; !osc_err && j < argnum; j++ ) { 
      switch ( args[j] ) { 
      case 'i':
         osc_err = OSC_writeIntArg    ( &_osc, va_arg(tags, int) );
         break;
      case 'f':
         osc_err = OSC_writeFloatArg  ( &_osc, va_arg(tags, double) );
         break;
      case 's':
         osc_err = OSC_writeStringArg ( &_osc, va_arg(tags, char *) );
         break;  
      }
   }
   if ( osc_err ) { 
       fprintf ( stderr, "osc:  error writing OSC packet: %d %s\n", osc_err, OSC_errorMessage );
       //failure action???
   }
   va_end(tags);     // Results Are Stored In Text
   tryMessage();
}

void
OSC_Transmitter::startMessage( char * address, char* args ) { 
    OSC_writeAddressAndTypes( &_osc, address, args );
    tryMessage();
}

void
OSC_Transmitter::addInt ( int i ) { 
    int osc_err = OSC_writeIntArg ( &_osc, i );
    if ( osc_err ) { 
       fprintf ( stderr, "osc:  error writing OSC packet: %d %s\n", osc_err, OSC_errorMessage );
       //failure action???
    }    
    tryMessage();
}

void
OSC_Transmitter::addFloat ( float f ) { 
    int osc_err = OSC_writeFloatArg ( &_osc, f );
    if ( osc_err ) { 
       fprintf ( stderr, "osc:  error writing OSC packet: %d %s\n", osc_err, OSC_errorMessage );
        //failure action???
    }    
    tryMessage();
}

void
OSC_Transmitter::addString ( char * s ) { 
    int osc_err = OSC_writeStringArg ( &_osc, s );
    if ( osc_err ) { 
       fprintf ( stderr, "osc:  error writing OSC packet: %d %s\n", osc_err, OSC_errorMessage );
        //failure action???
    }  
    tryMessage();
}

bool
OSC_Transmitter::packetReady() { 
    if ( _holdMessage ) return false; //message hold is on
    if ( _osc.typeStringPtr ) {       //if it's been typed, check that type is complete
        if ( CheckTypeTag(&_osc, '\0' ) ) return false;
    } 
    return ( OSC_isBufferDone(&_osc) != 0 );
}

void
OSC_Transmitter::tryMessage() { 
    if ( !packetReady() ) return;

    _out->send( OSC_getPacket(&_osc), OSC_packetSize(&_osc) );
    OSC_resetBuffer(&_osc);
}

void
OSC_Transmitter::holdMessage(bool b) { 
    _holdMessage  = b;
}

void
OSC_Transmitter::kickMessage() { 
    if ( !OSC_isBufferDone(&_osc) ) { 
        fprintf(stderr, "OSC: error - sending incomplete packet!");
    }
    _out->send( OSC_getPacket(&_osc), OSC_packetSize(&_osc) );
    OSC_resetBuffer(&_osc);
}


// OSC_RECEIVER

OSC_Receiver::OSC_Receiver():
    _inbufsize(OSCINBUFSIZE),
    _inbox(NULL),
    _address_space (NULL),
    _address_size ( 2 ),
    _address_num ( 0 ), 
    _inbox_size(2),
    _started(false),
    _in_read(0),
    _in_write(1),
    _listening(false)
{ 

    _inbox = (OSCMesg*) malloc (sizeof(OSCMesg) * _inbox_size);
    for ( int i = 0; i < _inbox_size; i++ ) _inbox[i].payload = NULL;

    _address_space = (OSCSrc **) realloc ( _address_space, _address_size * sizeof ( OSCSrc * ) );

    _io_mutex = new XMutex();
    _io_thread = new XThread();

    _in = new UDP_Receiver();

    init();

}

OSC_Receiver::OSC_Receiver(UDP_Receiver* in) {
    _in   = in;
}

void 
OSC_Receiver::init(){ 
   _in->init();
}

OSC_Receiver::~OSC_Receiver() {
    for ( int i=0; i < _inbox_size; i++ ) { 
        if ( _inbox[i].payload ) { 
            free ( _inbox[i].payload );
            _inbox[i].payload = NULL;
        }
        free ( _inbox );
    }
    delete _in;
}

THREAD_RETURN ( THREAD_TYPE osc_recv_thread ) ( void * data )  { 
    OSC_Receiver * oscar = (OSC_Receiver * ) data;
    do { 
        oscar->recv_mesg();
    } while ( true );
}

bool
OSC_Receiver::listen() { 
    if ( !_listening ) { 
        _listening = true;
        _io_thread->start( osc_recv_thread, (void*)this );
    }
    return _listening;
}

void
OSC_Receiver::bind_to_port(int port ) { 
    _in->bind_to_port(port);
}


void
OSC_Receiver::close_sock() { 
   _in->close_sock();
}

void
OSC_Receiver::recv_mesg() { 
   _mesglen = _in->recv_next( _inbuf, _inbufsize );
   if ( _mesglen > 0 ) { 
       //parse();
       handle_mesg(_inbuf, _mesglen);
   }
}
 

bool 
OSC_Receiver::has_mesg()  { 
    return ( _started && ( _in_read+1 ) % _inbox_size != _in_write ); 
}

bool 
OSC_Receiver::get_mesg(OSCMesg* bucket)   {  

    _io_mutex->acquire();

    if ( has_mesg() ) { 
        //next write _may_ fuck with _in_read 
        //where when resizing the buffer...
        *bucket = *next_read();
        //fprintf(stderr, "r"); //read: %d of %d \n", _in_read, _inbox_size);
        _io_mutex->release();
        return true;
    }

    _io_mutex->release();
    return false;

}

void
OSC_Receiver::parse() { 

    //unpack bundles, dump all messages
   //any local prep we need here?

   unsigned int i = 0;
   //de-buggering
   fprintf(stderr, "OSC_Receiver: parsing message length %d\n", _mesglen);
   for ( i = 0 ; i < _mesglen; i++ ) { 
      fprintf(stderr, "%c :", _inbuf[i] );
   }
   fprintf(stderr, "\n");
   for ( i = 0 ; i < _mesglen; i++ ) { 
      fprintf(stderr, "%0x:", _inbuf[i] );
   }
   fprintf(stderr, "\n");

}

void
OSC_Receiver::next_write() { 

   //called by the UDP client ( blocks ) 

   if ( !_started ) { 
      _started = true; 
   } 

   int next = (_in_write+1) % _inbox_size;
   
   if ( next == _in_read ) { 
      
      //fprintf(stderr, "OSC::need to resize...");
         
      _io_mutex->acquire(); //don't let next read return a bogus pointer

      if ( _inbox_size < OSCINBOXMAX) { 
         

         //LOCK MUTEX (on next_read)
         //prevents 

         //mesgs might move around, but their data doesn't..(this is good)..
         //if we return an OSCmesg* to an out-of-thread function, it can expire.
         //so we pass in pointer to our own mesg from outside (get_mesg) and copy
         //from the current 'read' in the buffer...buffer may then be resized...
         //since data buffers do not move, we remain okay.

         //perhaps a monolithic chunk of memory is a better option...
         //but what if we want to handle multiple messages at once?

         //orrr..if we follow timetags and need semirandom access to 
         //mesg contents for scheduling bundles..hmm.
         
         //make a separate case for scheduled messages...
         //throw them into a heap...

         //fuck...back to the todo...
      
         int new_size = ( _inbox_size * 2 > OSCINBOXMAX ) ? OSCINBOXMAX : _inbox_size * 2 ;

         bool err = false;

         //okay.  read and write may be anywhere.  
         if ( _in_write > _in_read ) {
            //easy case
            _inbox = (OSCMesg* ) realloc ((void*)_inbox, new_size * sizeof(*_inbox) );
            err = (!_inbox);
            //indices are still valid
         }
         else { 

            //write is behind read in the buffer
            //copy into buffer so that contents remain contiguous 
            //note:: copy the buffers between write and read...
            //       they may have valid packet pointers.......

            OSCMesg * new_inbox =(OSCMesg*)malloc ( new_size * sizeof(*new_inbox) );
            err = (!new_inbox);
            
            if ( !err ) { 
               int _read_tail = _inbox_size - _in_read;
               memcpy((void*)new_inbox,               (void*)(_inbox+_in_read),  (_read_tail) * sizeof(*_inbox)   );
               memcpy((void*)(new_inbox+_read_tail),  (void*)_inbox,             (_in_read  ) * sizeof(*_inbox)   );
               
               //update indices
               _in_read = 0;
               _in_write = (_in_write + _read_tail);
               
               free ( _inbox );
               _inbox = new_inbox;
            }

            else err = true;

         }

         
         if ( !err ) { 
            //new inbox elements need their payload pointers init to NULL
            for ( int i = _inbox_size; i < new_size; i++ ) _inbox[i].payload = NULL;           
         }
         else { 
            //fprintf(stderr, "fuck!  fucking buffer reallocation error!\n");
            exit(0);
         }

         next = (_in_write+1)%new_size; //where we're headed..
         _inbox_size = new_size;


         //fprintf(stderr, "(%d)", _inbox_size);
         //UNLOCK MUTEX
         
      
      }

      else { 
         //fprintf(stderr, "max buffer...dropping old mesg %d\n", next);
         _in_read = (next+1)%_inbox_size;
         
      }

      _io_mutex->release();

   }

   _in_write = next;
                                        
}

OSCMesg*
OSC_Receiver::next_read() { 
   int next = ( _in_read + 1 ) % _inbox_size;  
   if ( next == _in_write ) return NULL; //can't intrude on write
 
   _in_read = next; //advance
   OSCMesg* ret= _inbox + _in_read; //return this pointer
   return ret;
}

void
OSC_Receiver::set_mesg(OSCMesg* mrp, char * buf, int len ) { 

   //put pointers in the appropriate places
   //this means that mrp has no idea whether
   //its pointers refer to its own payload
   //or to an external buffer

   mrp->len = len; //total size of message
   mrp->address = buf;
   mrp->types   = mrp->address + strlen(mrp->address) + 4 - strlen(mrp->address)%4;
   if ( mrp->types[0] == ',') { 
      //typed message
      mrp->data= mrp->types + strlen(mrp->types) + 4 - strlen(mrp->types)%4;  
   }
   else { 
      //untyped message.  
      //should there be a warning?
      //classes that handle mesgs
      //should take care of that.
      mrp->data  = mrp->types; 
      mrp->types = NULL; 
   }

}

void
OSC_Receiver::handle_mesg( char* buf, int len ) { 

   if ( buf[0] == '#' ) { handle_bundle( buf, len ); return; }
  
   OSCMesg * mrp = write();

   if ( mrp->payload == NULL ) { 
      mrp->payload = (char *)malloc ( OSCINBUFSIZE * sizeof(char) );
      //fprintf(stderr, "oscrecv:mallocing %d bytes\n", OSCINBUFSIZE);
   }

   memcpy( (void*)mrp->payload, (const void*)buf, len ); //copy data from buffer to message payload
   set_mesg( mrp, mrp->payload, len ); //set pointers for the message to its own payload
   mrp->recvtime = 0.000; //GetTime(); //set message time

   distribute_message( mrp );  //send message to any waiting addresses
   
   next_write();
   
}

void
OSC_Receiver::handle_bundle(char*b, int len) { 
   //no scheduling for now
   //just unpack everything

   //fprintf(stderr, "bundle (length %d)\n", len);
   
   int off = 16; //skip "#bundle\0timetags"

   while ( off < len ) 
   { 
      char * z = b+off;
      unsigned long size = ntohl(*((unsigned long*)z));

      char * m = z+4;

      if ( m[0] == '#' ) handle_bundle(m,size);
      else handle_mesg(m,size);

      off += size+4; //beginning of next message
   }
}

OSCSrc * 
OSC_Receiver::new_event ( char * spec) { 
    OSCSrc * event = new OSCSrc ( spec );
    add_address ( event );
    return event;
}

void
OSC_Receiver::add_address ( OSCSrc * src ) { 
    if ( _address_num == _address_size ) { 
        _address_size *= 2;
        _address_space = (OSCSrc **) realloc ( _address_space, _address_size * sizeof( OSCSrc **));
    }
    _address_space[_address_num++] = src;
    src->setReceiver(this);
}

void
OSC_Receiver::remove_address ( OSCSrc *odd ) { 
    for ( int i = 0 ; i < _address_num ; i++ ) { 
        while ( _address_space[i] == odd ) { 
            _address_space[i] = _address_space[--_address_num];
        }
    }
}

/*
  This function distributes each message to a matching address inside the 
  chuck shred.  There are some issues with this.
  bundle simultaneity might be solved via mutex?
  
*/

void 
OSC_Receiver::distribute_message ( OSCMesg * msg ) { 
    for ( int i = 0 ; i < _address_num ; i++ ) { 
        if ( _address_space[i]->check_mesg ( msg ) ) {
//            fprintf ( stderr, "broadcasting %x from %x\n", (uint)_address_space[i]->SELF, (uint)_address_space[i] );
            ( (Chuck_Event *) _address_space[i]->SELF )->broadcast(); //if the event has any shreds queued, fire them off..
        }
    }
}


//OSC SOURCE ( Parser ) 
//rename to OSC_address ? 

// this is an OSCSrc - we register it to a particular receiver
// the receiver matches an incoming message against registered 
// address and sends the message to all that match. 

// OSCMesg may still have pointers to a mysterious void.
// we should copy values locally for string and blob.

// public for of inherited chuck_event mfunc.  

OSCSrc::OSCSrc() { 
    init();
    setSpec("/undefined/default,i");
}

OSCSrc::OSCSrc( char * spec ) { 
    init();
    setSpec( spec );
}

void
OSCSrc::init() { 
    _receiver = NULL;
    SELF = NULL;
    _qz = 64;
    _cur_mesg = NULL;
    _queue = NULL;
}

OSCSrc::~OSCSrc() { 
    if ( _queue ) free ( _queue ); 
}

void
OSCSrc::setReceiver(OSC_Receiver * recv) { 
    _receiver = recv;
}

void   
OSCSrc::setSpec( char *c ) { 
    strncpy ( _spec, c, 512); 
    _needparse = true; 
    parseSpec(); 
}
 
void
OSCSrc::parseSpec() { 

   if ( !_needparse ) return;

   char * type = strrchr(_spec, ',');
   if ( !type ) return;

   *type = '\0'; //null the address
   type++; //point to type string

   strcpy ( _address, _spec );
   strcpy ( _type , type );

//   fprintf(stderr," parsing spec- address %s :: type %s\n", _address, _type);

   int n = strlen ( type );
   _noArgs = ( n == 0 );
   resize( ( n < 1 ) ? 1 : n );
   _qread = 0;
   _qwrite = 1;

   _needparse = false;
}

void
OSCSrc::resize( int n ) { 
    _nv = n;
    _queue = ( osc_data * ) realloc ( _queue, _qz * _nv * sizeof( osc_data ) );
    memset ( _queue, 0, _qz * _nv * sizeof( osc_data ) );
}

bool
OSCSrc::message_matches( OSCMesg * m ) {

    //this should test for type as well.
   
    bool addr_match;
    if ( strcmp ( m->address, _address ) == 0 ) addr_match = true;
    else addr_match = PatternMatch( m->address, _address);
    if ( !addr_match ) return false;

    //address AND type must match 
    //but there should be an option for the blank 'information about this' pattern

    char * type = m->types+1;
    if ( m->types == NULL || strcmp( _type , type ) != 0 ) { 
//        if ( type )
//            fprintf(stderr, "error, mismatched type string( %s ) vs ( %s ) \n", _type, type ) ;
//        else 
//            fprintf( stderr, "error, missing type string (expecting %s) \n", _type );
        return false;
    }
    return true;
}

bool
OSCSrc::check_mesg( OSCMesg * m ) 
{ 
    if ( !message_matches ( m ) ) return false;
    queue_mesg( m );
    return true;
}



bool
OSCSrc::has_mesg() { 
    return ( ( _qread + 1 ) % _qz != _qwrite );
}

bool
OSCSrc::next_mesg() { 
    if ( has_mesg() ) { 
        _qread = ( _qread + 1 ) % _qz;
        _cur_mesg = _queue + _qread * _nv;
        _cur_value = 0;
        return true;
    }
    return false;
}

bool
OSCSrc::vcheck( osc_datatype tt ) { 

    if ( _cur_value >= _nv ) { 
        fprintf( stderr, "OSCSrc: read beyond message size\n" );
        return false;
    }
    if ( _cur_mesg[_cur_value].t != tt ) { 
        fprintf( stderr, "error: requested type %d != %d\n", _cur_mesg[_cur_value].t, tt  );
        return false;
    }
    return true;
}

int
OSCSrc::next_int() { 
    return ( vcheck(OSC_INT) )    ?  _cur_mesg[_cur_value++].i : 0 ;
}

float
OSCSrc::next_float() { 
    return ( vcheck(OSC_FLOAT) )  ?  _cur_mesg[_cur_value++].f : 0.0 ;
}

char *
OSCSrc::next_string() { 
    return ( vcheck(OSC_STRING) ) ?  _cur_mesg[_cur_value++].s : NULL ;
}

char * 
OSCSrc::next_string_dup() { 
    if ( !vcheck(OSC_STRING) ) return NULL;
    char * alt_c = _cur_mesg[_cur_value++].s;
    char * dup_c = (char * ) malloc ( (strlen ( alt_c )+1 ) * sizeof(char) );
    strcpy ( dup_c, alt_c );
    return dup_c;
}

char *
OSCSrc::next_blob() { 
    return ( vcheck(OSC_BLOB) )   ?  _cur_mesg[_cur_value++].s : NULL ;
}

void
OSCSrc::queue_mesg ( OSCMesg* m ) 
{ 
    int nqw = ( _qwrite + 1 ) % _qz;
    if ( nqw != _qread ) { 
        _vals = _queue + _qwrite * _nv;
    }
    else { 
        fprintf( stderr, "OSCSrc::( %s )  mesg buffer full ( r:%d , w:%d ), dropping message \n", _address, _qread, _qwrite );
        fprintf( stderr, "--- hasMesg ( %d ) nextMesg ( %d )", (int) has_mesg(), (int)next_mesg()  );
        return;
    }

    if ( _noArgs ) { // if address takes no arguments, 
        _vals[0].t = OSC_NOARGS;
    }
    else { 
        char * type = m->types+1;
        char * data = m->data;
        
        unsigned int endy;
        int i=0;
        
        float *fp;
        int   *ip;
        int   clen;
        while ( *type != '\0' ) { 
            switch ( *type ) { 
            case 'f':
                endy = ntohl(*((unsigned long*)data));
                fp = (float*)(&endy);
                _vals[i].t = OSC_FLOAT;
                _vals[i].f = *fp; 
                data += 4;
                break;
            case 'i':
                endy = ntohl(*((unsigned long*)data));
                ip = (int*)(&endy);
                _vals[i].t = OSC_INT;
                _vals[i].i = *ip; 
                data += 4;
                break;
            case 's':
                //string
                clen = strlen(data) + 1; //terminating!
                _vals[i].t = OSC_STRING;
                _vals[i].s = (char *) realloc ( _vals[i].s, clen * sizeof(char) );
                memcpy ( _vals[i].s, data, clen );
                fprintf(stderr, "add string |%s| ( %d ) \n", _vals[i].s, clen  );
                data += clen + 4 - clen % 4;
            break;
            case 'b':
                // blobs
                endy = ntohl(*((unsigned long*)data));
                clen = *((int*)(&endy));
                _vals[i].t = OSC_BLOB;
                _vals[i].s = (char* ) realloc ( _vals[i].s, clen * sizeof(char) );
                memcpy ( _vals[i].s, data, clen );
                data += clen + 4 - clen % 4;
                break;
            }
            i++;
            type++;
        }
    }
    
    _qwrite = nqw;

    //review
    /*
      for ( i=0; i < _nvals; i++ ) {
      fprintf(stderr, "%d %f : ", i, _val[i] );
      }
      fprintf(stderr, "\n");
    */
   
}



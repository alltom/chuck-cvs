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
// file: chuck_table.c
// desc: ...
//
// copyright (c) 1997 Andrew W. Appel.
//
// author: Andrew Appel (appel@cs.princeton.edu)
// modified: Ge Wang (gewang@cs.princeton.edu)
//           Perry R. Cook (prc@cs.princeton.edu)
// date: Autumn 2002
//-----------------------------------------------------------------------------
#include <stdio.h>
#include "chuck_utils.h"
#include "chuck_table.h"

#define TABSIZE 65437


typedef struct binder_ *binder;
struct binder_ {void *key; void *value; binder next; void *prevtop;};
struct TAB_table_ {
  unsigned int size;
  binder * table;
  void * top;
  TAB_eq_func eq_func;
  TAB_hash_func hash_func;
};


static binder Binder(void *key, void *value, binder next, void *prevtop)
{
    binder b = checked_malloc(sizeof(*b));
    b->key = key; b->value=value; b->next=next; b->prevtop = prevtop; 
    return b;
}


TAB_table TAB_empty(void)
{ 
    return TAB_empty2(TABSIZE);
}


TAB_table TAB_empty2( unsigned int s )
{
    TAB_table t = checked_malloc(sizeof(*t));
    int i;
    t->table = checked_malloc(sizeof(binder)*s);
    t->size = s;
    t->top = NULL;
    for( i = 0; i < s; i++ )
        t->table[i] = NULL;
    return t;
}

TAB_table TAB_empty3( TAB_eq_func eq, TAB_hash_func hash, unsigned int s )
{
    TAB_table t = TAB_empty2(s);
    t->eq_func = eq;
    t->hash_func = hash;
    return t;
}




void TAB_delete( TAB_table t )
{
    int i;
    binder p = NULL, n = NULL;
    for(i = 0; i < t->size; i++ )
    {
        p = t->table[i];
        while( p )
        {
            n = p->next;
            free(p);
            p = n;
        }
    }
    
    // free the table
    free( t->table );
    free( t );
}



/* The cast from pointer to integer in the expression
 *   ((unsigned)key) % TABSIZE
 * may lead to a warning message.  However, the code is safe,
 * and will still operate correctly.  This line is just hashing
 * a pointer value into an integer value, and no matter how the
 * conversion is done, as long as it is done consistently, a
 * reasonable and repeatable index into the table will result.
 */

void TAB_enter(TAB_table t, void *key, void *value)
{
    int index;
    unsigned hval = (unsigned)key;
    assert(t && key);
    if( t->hash_func )
        hval = (unsigned)t->hash_func(key);
    index = ((unsigned)hval) % t->size;
    t->table[index] = Binder(key, value,t->table[index], t->top);
    t->top = key;
}

void *TAB_look(TAB_table t, void *key)
{
    int index;
    unsigned hval = (unsigned)key;
    binder b;
    assert(t && key);
    if( t->hash_func )
        hval = (unsigned)t->hash_func(key);
    index=((unsigned)hval) % t->size;
    if( !t->eq_func )
    {
        for(b=t->table[index]; b; b=b->next)
            if (b->key==key) return b->value;
    }
    else
    {
        for(b=t->table[index]; b; b=b->next)
            if (t->eq_func(b->key, key)) return b->value;
    }

    return NULL;
}

void *TAB_pop(TAB_table t)
{
    void *k; binder b; int index;
    assert (t);
    k = t->top;
    assert (k);
    index = ((unsigned)k) % t->size;
    b = t->table[index];
    assert(b);
    t->table[index] = b->next;
    t->top=b->prevtop;
    return b->key;
}

void TAB_dump(TAB_table t, void (*show)(void *key, void *value))
{
    void *k = t->top;
    int index = ((unsigned)k) % t->size;
    binder b = t->table[index];
    if (b==NULL) return;
    t->table[index]=b->next;
    t->top=b->prevtop;
    show(b->key,b->value);
    TAB_dump(t,show);
    assert(t->top == b->prevtop && t->table[index]==b->next);
    t->top=k;
    t->table[index]=b;
}

int str_eq( void * lhs, void * rhs )
{
    return !strcmp( (char *)lhs, (char *)rhs );
}

int str_hash( void * str )
{
    unsigned int h=0; char *s;
    for(s=str; *s; s++)  
        h = h*65599 + *s;
    return h;
}

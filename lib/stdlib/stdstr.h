/**
  ******************************************************************************
  * @file    stdstr.h
  * @author  SUN Wentao
  * @version V0.0.1
  * @date    17-DEC-2018
  * @brief   
  ******************************************************************************
  */ 

#ifndef __STDSTR_H__
#define __STDSTR_H__

#ifndef NULL
#define NULL 0
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif

extern void  *mem_set   ( void *s, char val, size_t n );
extern int    str_cmp   ( const char *dest, const char *source );
extern size_t str_len   ( const char *s );
extern char  *str_chr   ( const char *p, int ch );
extern int    str_spn   ( const char *p, const char *s );
extern char  *str_pbrk  ( const char *s1, const char *s2 );
extern char  *str_tok_r ( char *s, const char *delim, char **save_ptr );


#endif


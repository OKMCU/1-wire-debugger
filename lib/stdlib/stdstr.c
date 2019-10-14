/**
  ******************************************************************************
  * @file    stdstr.c
  * @author  SUN Wentao
  * @version V0.0.1
  * @date    17-DEC-2018
  * @brief   
  ******************************************************************************
  */ 


#include "stdstr.h"

#ifndef STDSTR_EN_mem_set
#define STDSTR_EN_mem_set       1
#endif

#ifndef STDSTR_EN_str_cmp
#define STDSTR_EN_str_cmp       1
#endif

#ifndef STDSTR_EN_str_len
#define STDSTR_EN_str_len       1
#endif

#ifndef STDSTR_EN_str_chr
#define STDSTR_EN_str_chr       1
#endif

#ifndef STDSTR_EN_str_spn
#define STDSTR_EN_str_spn       1
#endif

#ifndef STDSTR_EN_str_pbrk
#define STDSTR_EN_str_pbrk      1
#endif

#ifndef STDSTR_EN_str_tok_r
#define STDSTR_EN_str_tok_r     1
#endif

#if STDSTR_EN_mem_set > 0
extern void *mem_set  (void *s, char val, size_t n)
{
    char *p_mem;
    p_mem = (char *)s;
    while( n-- > 0 )
    {
        *p_mem++ = val;
    }

    return s;
}
#endif

#if STDSTR_EN_str_len > 0
extern size_t str_len ( const char *s )
{
    size_t len = 0;
    while(s[len++]);
    return len;
}
#endif

#if STDSTR_EN_str_cmp > 0
extern int str_cmp(const char *dest, const char *source)
{
    while (*dest && *source && (*dest == *source))
    {
        dest++;
        source++;
    }
    return *dest - *source;
}
#endif

#if STDSTR_EN_str_chr > 0
extern char *str_chr(const char *p, int ch)
{
	char c;

	c = ch;
	for (;; ++p) {
		if (*p == c)
			return ((char *)p);
		if (*p == '\0')
			return (NULL);
	}
	/* NOTREACHED */
}
#endif

#if STDSTR_EN_str_spn > 0
extern int str_spn(const char *p, const char *s)
{
	int i, j;

	for (i = 0; p[i]; i++) {
		for (j = 0; s[j]; j++) {
			if (s[j] == p[i])
				break;
		}
		if (!s[j])
			break;
	}
	return (i);
}
#endif

#if STDSTR_EN_str_pbrk > 0
extern char *str_pbrk(const char *s1, const char *s2)
{
	for (; *s1; s1++) {
		if (str_chr (s2, *s1) != 0)
			return ((char *)s1);
	}
	return (0);
}
#endif

#if STDSTR_EN_str_tok_r > 0
extern char *str_tok_r(char *s, const char *delim, char **save_ptr) 
{     

    char *token;     
     
    if (s == NULL) s = *save_ptr;     
     
    /* Scan leading delimiters.  */     
    s += str_spn(s, delim);     
    if (*s == '\0')      
        return NULL;     
     
    /* Find the end of the token.  */     
    token = s;     
    s = str_pbrk(token, delim);     
    if (s == NULL)     
        /* This token finishes the string.  */     
        *save_ptr = str_chr(token, '\0');     
    else {     
        /* Terminate the token and make *SAVE_PTR point past it.  */     
        *s = '\0';     
        *save_ptr = s + 1;     
    }     
     
    return token;     
}    
#endif


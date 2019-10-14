/******************************************************************************

 @file  

 @brief 

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 
 *****************************************************************************/
#include "stdint.h"
#include "stringx.h"
#include "stdstr.h"

#ifndef STRINGX_EN_hexstr2uint
#define STRINGX_EN_hexstr2uint          1
#endif

#ifndef STRINGX_EN_decstr2uint
#define STRINGX_EN_decstr2uint          1
#endif

#ifndef STRINGX_EN_decstr2sint
#define STRINGX_EN_decstr2sint          1
#endif

#ifndef STRINGX_EN_tostr_sint
#define STRINGX_EN_tostr_sint           1
#endif

#ifndef STRINGX_EN_tostr_uint
#define STRINGX_EN_tostr_uint           1
#endif

#ifndef STRINGX_EN_tostr_hex8
#define STRINGX_EN_tostr_hex8           1
#endif

#ifndef STRINGX_EN_tostr_hex16
#define STRINGX_EN_tostr_hex16          1
#endif

#ifndef STRINGX_EN_tostr_hex32
#define STRINGX_EN_tostr_hex32          1
#endif

#ifndef STRINGX_EN_str_starts_with
#define STRINGX_EN_str_starts_with      1
#endif

#ifndef STRINGX_EN_str_ends_with
#define STRINGX_EN_str_ends_with        1
#endif


#define SWAP(a, b)  a ^= b; b ^= a; a ^= b

#if STRINGX_EN_hexstr2uint > 0
extern uint8_t hexstr2uint(const char *str, uint32_t *num)
{
    char ch;
    uint8_t cnt = 0;
    uint8_t lenPrefix = 0;
    uint32_t val = 0;
    int32_t test_09;
    int32_t test_AF;
    int32_t test_af;
    
    if(str[0] == '0')
    {
        if(str[1] == 'x' || str[1] == 'X')
        {
            cnt = 2;
            lenPrefix = 2;
        }
    }
    //////////////////////
    
    for(;;)
    {
        ch = str[cnt];
        
        if(ch == '\0')
            break;

        cnt++;
        
        if(cnt-lenPrefix > HEX32_STR_LEN_MAX)
        {
            return 0;
        }
        
        test_09 = (ch - '0') * (ch - '9');
        test_AF = (ch - 'A') * (ch - 'F');
        test_af = (ch - 'a') * (ch - 'f');
        if(test_09 * test_AF * test_af <= 0)
        {
            val <<= 4;
            if(test_09 <= 0)
            {
                val += ch - '0';
            }
            else if(test_AF <= 0)
            {
                val += ch - 'A' + 10;
            }
            else if(test_af <= 0)
            {
                val += ch - 'a' + 10;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        } 
    }

    if(cnt > lenPrefix)
    {
        if(num)
        {
            *num = val;
        }
    }
    
    return cnt;
}
#endif

#if STRINGX_EN_decstr2uint > 0
extern uint8_t decstr2uint(const char *str, uint32_t *num)
{
    char ch;
    uint8_t cnt = 0;
    uint32_t val = 0;

    

    for(;;)
    {
        ch = str[cnt];
        
        if(ch == '\0')
            break;
        
        cnt++;
        
        if(cnt > UINT_STR_LEN_MAX)
            return 0;

        
        if(ch >= '0' && ch <= '9')
        {
            val *= 10;
            val += ch - '0';
        }
        else
        {
            return 0;
        }
        
    }

    if(cnt > 0)
    {
        if(num)
        {
            *num = val;
        }
    }
    
    return cnt;
}
#endif

#if STRINGX_EN_decstr2sint > 0
extern uint8_t decstr2sint(const char *str, int32_t *num)
{
    int32_t val = 0;
    uint32_t u32tmp;
    uint8_t cnt;
    
    if(str[0] == '-')
    {
        cnt = decstr2uint(str+1, &u32tmp);
    }
    else
    {
        cnt = decstr2uint(str, &u32tmp);
    }

    if(cnt > 0)
    {
        if(str[0] == '-')
        {
            if(u32tmp > (uint32_t)INT32_MAX + 1)
            {
                return 0;
            }
            val = (int32_t)u32tmp;
            val =-val;
            cnt++;
        }
        else
        {
            if(u32tmp > (uint32_t)INT32_MAX)
            {
                return 0;
            }
            val = (int32_t)u32tmp;
        }
        
        if(num)
        {
            *num = val;
        }
    }
    
    
    return  cnt;
}
#endif

#if STRINGX_EN_tostr_sint > 0
extern uint8_t tostr_sint(int32_t num, char *str)
{
    uint8_t numLen = 0;
    uint8_t i;
    uint8_t j;
    int8_t flag;
    uint32_t absNum;
    
    if(num < 0)
    {
        flag = -1;
        absNum = (uint32_t)(-num);
    }
    else
    {
        flag = 1;
        absNum = (uint32_t)num;
    }
    
    do{
        str[numLen++] = '0' + (absNum%10);
        absNum /= 10;
    }
    while(absNum != 0);

    if(flag == -1)
    {
        str[numLen++] = '-';
    }

    for(i = 0; i < numLen; i++)
    {
        if(numLen >= i + 1)
        {
            j = numLen - 1 - i;
            if(i < j)
            {
                SWAP(str[i], str[j]);
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    
    return numLen;
}
#endif

#if STRINGX_EN_tostr_uint > 0
extern uint8_t tostr_uint(uint32_t num, char *str)
{
    uint8_t numLen = 0;
    uint8_t i;
    uint8_t j;
    
    do{
        str[numLen++] = '0' + (num%10);
        num /= 10;
    }
    while(num != 0);

    for(i = 0; i < numLen; i++)
    {
        if(numLen >= i + 1)
        {
            j = numLen - 1 - i;
            if(i < j)
            {
                SWAP(str[i], str[j]);
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    return numLen;
}
#endif

#if STRINGX_EN_tostr_hex8 > 0
extern uint8_t tostr_hex8(uint8_t num, char *str)
{
    str[0] = num >> 4;
    str[1] = num & 0x0F;
    
    str[0] = (str[0] <= 9) ? (str[0] + '0') : (str[0] + 'A' - 10);
    str[1] = (str[1] <= 9) ? (str[1] + '0') : (str[1] + 'A' - 10);
    
    return HEX8_STR_LEN_MAX;
}
#endif

#if STRINGX_EN_tostr_hex16 > 0
extern uint8_t tostr_hex16(uint16_t num, char *str)
{
    int8_t i;
    
    for(i = HEX16_STR_LEN_MAX-1; i >=0; i--)
    {
        str[i] = num & 0xF;
        str[i] = (str[i] <= 9) ? (str[i] + '0') : (str[i] + 'A' - 10);
        num >>= 4;
    }

    return HEX16_STR_LEN_MAX;
}
#endif

#if STRINGX_EN_tostr_hex32 > 0
extern uint8_t tostr_hex32(uint32_t num, char *str)
{
    int8_t i;
    
    for(i = HEX32_STR_LEN_MAX-1; i >=0; i--)
    {
        str[i] = num & 0xF;
        str[i] = (str[i] <= 9) ? (str[i] + '0') : (str[i] + 'A' - 10);
        num >>= 4;
    }
    
    return HEX32_STR_LEN_MAX;
}
#endif

#if STRINGX_EN_str_starts_with > 0
extern char *str_starts_with(char *str1, const char *str2)
{
    if(str1 != NULL && str2 != NULL)
    {
        while(*str1)
        {
            if(*str1 == *str2)
            {
                str1++;
                str2++;
                if(*str2 == '\0')
                {
                    return str1;
                }
            }
            else
            {
                break;
            }
        }
    }
        
    return NULL;
}
#endif

#if STRINGX_EN_str_ends_with > 0
extern char *str_ends_with(char *str1, const char *str2)
{
    uint16_t len1;
    uint16_t len2;

    if(str1 != NULL && str2 != NULL)
    {
        len1 = str_len(str1);
        len2 = str_len(str2);

        if(len1 >= len2)
        {
            if(str_cmp(str1+len1-len2, str2) == 0)
            {
                return (str1+len1-len2);
            }
        }
    }
    return NULL;
    
}
#endif


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
#include "osal_config.h"
#include "osal_port.h"

#if (OSAL_ASSERT_EN > 0)
extern void    osal_assert_handler( void )
{
    volatile uint32_t i = 0;
    //HalAssertHandler();
    while(1)
    {
        i++;
    }
}
#endif





/*
 *  This macro compares the size of the first parameter to the integer value
 *  of the second parameter.  If they do not match, a compile time error for
 *  negative array size occurs (even gnu chokes on negative array size).
 *
 *  This compare is done by creating a typedef for an array.  No variables are
 *  created and no memory is consumed with this check.  The created type is
 *  used for checking only and is not for use by any other code.  The value
 *  of 10 in this macro is arbitrary, it just needs to be a value larger
 *  than one to result in a positive number for the array size.
 */
#define OSAL_ASSERT_SIZE(x,y) typedef char x ## _assert_size_t[-1+10*(sizeof(x) == (y))]


/* ------------------------------------------------------------------------------------------------
 *                                    Compile Time Assertions
 * ------------------------------------------------------------------------------------------------
 */
/* integrity check of type sizes */
OSAL_ASSERT_SIZE(  int8_t, 1);
OSAL_ASSERT_SIZE( uint8_t, 1);
OSAL_ASSERT_SIZE( int16_t, 2);
OSAL_ASSERT_SIZE(uint16_t, 2);
OSAL_ASSERT_SIZE( int32_t, 4);
OSAL_ASSERT_SIZE(uint32_t, 4);


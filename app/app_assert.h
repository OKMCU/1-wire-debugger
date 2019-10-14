/******************************************************************************

 @file  app_assert.h

 @brief Describe the purpose and contents of the file.

 Group: 
 Target Device: 

 ******************************************************************************

 ******************************************************************************
 Release Name: 
 Release Date: 
 *****************************************************************************/

#ifndef __APP_ASSERT_H__
#define __APP_ASSERT_H__

#include "app_config.h"
#include "stdmacro.h"

/* ------------------------------------------------------------------------------------------------
 *                                           Macros
 * ------------------------------------------------------------------------------------------------
 */

/*
 *  APP_ASSERT( expression ) - The given expression must evaluate as "true" or else the assert
 *  handler is called.  From here, the call stack feature of the debugger can pinpoint where
 *  the problem occurred.
 *
 *  APP_ASSERT_FORCED( ) - If asserts are in use, immediately calls the assert handler.
 *
 *  APP_ASSERT_STATEMENT( statement ) - Inserts the given C statement but only when asserts
 *  are in use.  This macros allows debug code that is not part of an expression.
 *
 *  APP_ASSERT_DECLARATION( declaration ) - Inserts the given C declaration but only when asserts
 *  are in use.  This macros allows debug code that is not part of an expression.
 *
 *  Asserts can be disabled for optimum performance and minimum code size (ideal for
 *  finalized, debugged production code).  To disable, define the preprocessor
 *  symbol HALNODEBUG at the project level.
 */


#if ( APP_ASSERT_EN == 0 )
#define APP_ASSERT(expr)
#define APP_ASSERT_FORCED()
#define APP_ASSERT_STATEMENT(statement)
#define APP_ASSERT_DECLARATION(declaration)
#else
#define APP_ASSERT(expr)                        st( if (!( expr )) app_assert_handler(); )
#define APP_ASSERT_FORCED()                     app_assert_handler()
#define APP_ASSERT_STATEMENT(statement)         st( statement )
#define APP_ASSERT_DECLARATION(declaration)     declaration
#endif

/* ------------------------------------------------------------------------------------------------
 *                                          Prototypes
 * ------------------------------------------------------------------------------------------------
 */
extern void app_assert_handler(void);


/**************************************************************************************************
 */

/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/

#endif


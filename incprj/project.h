

#if !defined(__PROJECT_H__)
#define __PROJECT_H__

#include "debug.h"

/*************************************************************************
 *  A S S E R T I O N S   C H E C K
 *************************************************************************/

#define ASS_CHECKING_ENABLED 0

#define ASS_CRITICAL(_cond, _file, _line, _inf) /*critical handler e.g. disable all outputs */
#define ASS_OUTPUT(_cond, _file, _line, _inf)   DB_PRINTF(# _cond " in " _file " line %d failed", _line);
#define ASS_EXIT(_cond, _file, _line, _inf)     while(1) ;  /* e.g. reset system*/
#define USE_ASS_FILE_PATH 1

#endif /* _PROJECT_H_ */
/*** EOF ***/

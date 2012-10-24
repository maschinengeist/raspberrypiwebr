
#if !defined(__DEBUG_C__)
#define __DEBUG_C__

/**************************************************************************
 * I N C L U D E S
 ***************************************************************************/
#include "mytypes.h"

#include <stdio.h>
#include <stdarg.h>

#include "debug.h"

#if (_DEBUG >= 1)
/*****************************************************************************/
/**
 *  \brief Die Funktion schreibt einen formatierten String auf einen
 *         definierten DebugPort (hier TerminalPort).
 *         Die Ausgabe ist fuer Debugausgaben waehrend der Programmentwicklung
 *         vorgesehen und ist in der Releaseversion (_DEBUG = 0) abgeschaltet.
 *         Die Stringlaenge ist auf 200 Byte begrenzt.
 *         Anwendungsbeispiel:
 *         DB_PRINTF(("ExRAM_Test: Addr=%lx\r\n", u32MemoryAddress));
 *
 *  \param[in]    *format,        format string ptr.
 *  \param[in]    ...             va_list
 *
 *  \param[out]   -
 *
 *  \return       -
 *
 *  \note Global: -
 *
 ******************************************************************************/
void DbgPrintf(CHAR *format,         /* format string ptr.     */
               ...)                  /* va_list                */
{
   CHAR MSG[200];
   va_list argptr;                   /* define the va_list     */

   va_start(argptr, format);         /* initialize the va_list */
   vsprintf(MSG, format, argptr);    /* print                  */
   va_end(argptr);

   Debug(MSG);                       /* DebugPort: see debug.h */

} /* DbgPrintf */

#endif /* (_DEBUG >= 1) */


#endif /* __DEBUG_C__ */

/*** EOF ***/





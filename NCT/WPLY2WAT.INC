'$INCLUDE: 'MFDLL.INC'

CONST TRUE = 1
CONST FALSE = 0
CONST FAIL = 1
CONST SUCCESS = 0
CONST COMMON = 2

CONST LOGFILE = TRUE
CONST CASESENSITIVE = FALSE

'$INCLUDE: 'WPLAYVAR.INC'

DECLARE FUNCTION SleepDelay       LIB "WATTEvnt.DLL"  ( NumSeconds as integer) As Integer
DECLARE SUB PlayKeys LIB "WATTEVNT.DLL" (k$,l%,f%)

DECLARE FUNCTION ChooseResult  LIB "HCTLIB.DLL" (testname$, i% ) AS INTEGER
DECLARE SUB HctTestBegin  LIB "HCTLIB.DLL" (n% , szTestName$)
DECLARE SUB HctTestDone  LIB "HCTLIB.DLL" (n%)
DECLARE FUNCTION HctCopyErrorsFromLogFile LIB "HCTLIB.DLL" (fname$) AS INTEGER

CONST RS_NOT_RUN=0
CONST RS_RUNNING=1
CONST RS_FAILED=2
CONST RS_PASSED=3
CONST RS_CANT_RUN=4

DECLARE FUNCTION WinExec LIB "KERNEL.EXE" (A$, B%) as Integer

DECLARE SUB WPlayKeys (Keys$)
SUB WPlayKeys (Keys$)
PlayKeys Keys$, len(Keys$), 0
END SUB


DECLARE SUB WLOG (lgFile$, Text$)
SUB WLog (lgFile$, Text$)

open NCTDriveLtr$+":"+NCTPath$+"\"+lgFile$ For Append As #1
Print #1, text$
close #1
END SUB


DECLARE SUB Openlog (newlog$)
SUB OpenLog (newLog$)
   ' NEED TO CHECK FOR VALID LOG NAMES!!!
   ' This subRoutine initializes the global LogName$

   'initialize global variable
   if newLog$ = "" then
      logName$ = "default.log"
   else
      logName$ = newLog$
   endif
 
   print "LOGFILE = ["; logName$; "]"
   open logName$ For Output As #1
   close #1
   wLog logname$, " "
   wLog logname$, "-- BEGIN LOG FILE [" + logName$ + "]"
   wLog logname$, dateTime$
 
END SUB

DECLARE SUB ClearLog (LFile$)
SUB ClearLog (LFile$)

open NCTDriveLtr$+":"+NCTPath$+"\"+LFile$ For Output As #1
close #1
END SUB




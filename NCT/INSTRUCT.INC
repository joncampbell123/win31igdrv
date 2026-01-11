'$IFNDEF _INSTRUCTINC
'skip over whole file if we have already been included
'$DEFINE _INSTRUCTINC

'************* Global variables and subroutines
'$include: 'player.inc'


GLOBAL __hwInstruct%

CONST INSTRUCT_DEFAULT  =         0
CONST INSTRUCT_CONTINUE =         4
CONST INSTRUCT_EXITWIN  =         8
CONST INSTRUCT_ABORT    =        16
CONST INSTRUCT_TOPMOST  =        32

DECLARE FUNCTION CreateInstruct  LIB "INSTRUCT.DLL" (int%, lpsz$) AS INTEGER
DECLARE SUB DestroyInstruct  LIB "INSTRUCT.DLL" (int%)
DECLARE FUNCTION hInstruct  LIB "INSTRUCT.DLL" (int%, wType%, lpsz$) AS INTEGER
DECLARE FUNCTION hWaitForNext  LIB "INSTRUCT.DLL" (int%) AS INTEGER

DECLARE FUNCTION Instruct  (wType%, lpsz$) AS INTEGER
DECLARE FUNCTION WaitForNext  () AS INTEGER


FUNCTION Instruct (wType%, lpsz$) AS INTEGER
    IF (__hwInstruct = 0) THEN
        __hwInstruct = CreateInstruct(0,  "no file")
        PRINT "Create Instruct Returned"; __hwInstruct
    ENDIF
    
    Instruct = hInstruct (__hwInstruct%, wType%, lpsz$)
END Function

FUNCTION WaitForNext  () AS INTEGER

WaitForNext = hWaitForNext(__hwInstruct%)
END FUNCTION

DECLARE SUB hInstructHide  LIB "INSTRUCT.DLL" (handle%)
DECLARE FUNCTION InstructHide () AS INTEGER
FUNCTION InstructHide  () AS INTEGER
hInstructHide __hwInstruct%
END FUNCTION


'************* End of Instruct Section
'$ENDIF         'IFNDEF _INSTRUCTINC

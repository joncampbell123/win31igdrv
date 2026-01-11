'$IFNDEF WINPLAYINC_DONE
'skip over whole file if we have already been included
'$DEFINE WINPLAYINC_DONE


CONST SW_SHOW=1                'was 0 for winplay 
CONST SW_SHOWMINIMIZED = 2    'was 2 for winplay 
CONST SW_SHOWMAXIMIZED = 3    'was 1 for winplay 


DECLARE FUNCTION PlayKeysSpeed    LIB "WATTEvnt.DLL"  (Speed as Long) as Integer
DECLARE FUNCTION PlayKeyshWnd     LIB "WATTEvnt.DLL"  (PlayString as STRING,  LENSTRING as integer,  hWnd as integer) As Integer
DECLARE FUNCTION PlayKeys         LIB "WATTEvnt.DLL"  (PlayString as STRING,  LENSTRING as integer,  Flag as integer) As Integer
DECLARE FUNCTION MouseEvent       LIB "WATTEvnt.DLL"  ( MouseE as integer, x as integer, y as integer, flag as integer) As Integer
DECLARE FUNCTION SleepDelay       LIB "WATTEvnt.DLL"  ( NumSeconds as integer) As Integer

Declare Function SetActiveWindow Lib "User" (hWnd%) As Integer
Declare Function SetWindowText Lib "User" (hWnd%,lpsz$) As Integer
Declare Function GetSystemMetrics Lib "User" (index%) As Integer
CONST SM_MOUSEPRESENT      =  19

DECLARE FUNCTION lpszTime  LIB "_WinUtil.DLL" (iMode AS INTEGER, lpszBuf AS POINTER TO STRING * 200) AS LONG



Declare Function RemoveNulls(szString$) As String
Function RemoveNulls(szString$) As String
        For x% = 1 To Len(szString$)   'FIND THE FIRST NULL CHAR
            If Mid$(szString$, x%, 1) = CHR$(0) Then
                Exit For               'JUMP OUT OF LOOP WHEN FOUND
                End If
            Next
        RemoveNulls = Mid$(szString$, 1, x% - 1)
End FUNCTION




DECLARE FUNCTION szTime (iType as INTEGER) AS STRING
FUNCTION szTime (iType as INTEGER) AS STRING
    DIM sz2Time AS STRING  * 200
    x&=lpszTime(iType, VARPTR(sz2Time))
    szTime = RemoveNulls(sz2Time)
END FUNCTION


' Error levels for the MyBox() function

CONST EL_TERMINATE    =    -1
CONST EL_ABORT        =    -1
CONST EL_ARI          =     1     'abort, retry, ignore
CONST EL_WARN         =     2
CONST EL_RETRY        =      4
CONST EL_RUN          =      5
CONST EL_INFO         =      6
CONST EL_YNC          =      7   'yes no cancel
CONST EL_ERROR        =    8


'DECLARE FUNCTION _MyBox  LIB "_WinUtil.DLL" DECLARE FUNCTION _MyBox

DECLARE FUNCTION MyBoxPascalHackForStupidWatt LIB "_WinUtil.DLL" (iType AS INTEGER, szFormat AS STRING) AS INTEGER
DECLARE FUNCTION MyBox (iType AS INTEGER, szFormat AS STRING) AS INTEGER
FUNCTION MyBox (iType AS INTEGER, szFormat AS STRING) AS INTEGER
   MyBox = MyBoxPascalHackForStupidWatt(iType,szFormat)
END FUNCTION


' possible returns from the MyBox function, same as message box
CONST IDOK          =  1
CONST IDCANCEL      =  2
CONST IDABORT       =  3
CONST IDRETRY       =  4
CONST IDIGNORE      =  5
CONST IDYES         =  6
CONST IDNO          =  7


'*** Windows routines  used
Declare Function FindWindow Lib "User" (lpClassName AS ANY,lpWindowName AS ANY) As Integer
Declare Function WinExec Lib "Kernel" (lpCmdLine As String, nCmdShow As Integer) As Integer


DECLARE FUNCTION ActivateApp  (s1$,s2$) AS INTEGER
FUNCTION ActivateApp (s1$,s2$) AS INTEGER
DIM sz1 AS STRING  * 100
DIM sz2 AS STRING  * 100
DIM hwnd AS INTEGER
sz1 = s1$ + chr$(0)
sz2 = s2$ + chr$(0)

if Len(s1$) = 0 then
    hwnd =  FindWindow(0, sz2)
elseif Len(s2$) = 0 then
    hwnd = FindWindow(sz1, 0)
else
    hwnd = FindWindow(sz1, sz2)
endif    
IF (hwnd = 0) THEN
ELSE
    x=SetActiveWindow(hwnd)
ENDIF
        
ActivateApp = hwnd
END FUNCTION

DECLARE FUNCTION _PlayKeys(Keys$) AS INTEGER
FUNCTION _PlayKeys(Keys$) AS INTEGER
        _PlayKeys = PlayKeys(Keys$, Len(Keys$), 0)
End Function

DECLARE FUNCTION _PlayKeyshWnd(hwnd%,Keys$) AS INTEGER
FUNCTION _PlayKeyshWnd(hwnd%,Keys$) AS INTEGER
        _PlayKeyshWnd = PlayKeyshWnd(Keys$, Len(Keys$), hwnd)
End Function

'************* FOR SLEEPDELAYs LONGER THAN 65 SECONDS ******************
Declare Sub TimeDelay(Seconds%)
Sub TimeDelay(Seconds%)
    Print Seconds%/60;" loops"
    For Loops% = 1 To Seconds%/60
        x% = SleepDelay(60)
        Next Loops%
    x% = SleepDelay(Seconds% - (Seconds%/60*60))
End Sub







'$ENDIF         'IFNDEF WINPLAYINC_DONE

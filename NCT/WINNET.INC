CONST TRUE = 1
CONST FALSE = 0
CONST FAIL = 1
CONST SUCCESS = 0
CONST COMMON = 2


CONST LOGFILE = TRUE
CONST CASESENSITIVE = FALSE


DECLARE SUB WriteLog (logEntry$) 
DECLARE SUB OpenLog  (logName$)
DECLARE SUB LOG (Out$)

DECLARE FUNCTION SleepDelay       LIB "WATTEvnt.DLL"  ( NumSeconds as integer) As Integer
DECLARE FUNCTION _lwrite          LIB "KERNEL.EXE" (A%, B$, C%) as Integer
DECLARE FUNCTION _lread           LIB "KERNEL.EXE" (A%, B$, C%) as Integer
DECLARE FUNCTION _lclose          LIB "KERNEL.EXE" (A%) as Integer
DECLARE FUNCTION OpenFile         LIB "KERNEL.EXE" (A$, B$, C%) as Integer
CONST OF_READ= 0

DECLARE FUNCTION ChooseResult  LIB "HCTLIB.DLL" (testname$, i% ) AS INTEGER
DECLARE SUB HctTestBegin  LIB "HCTLIB.DLL" (n% , szTestName$)
DECLARE SUB HctTestDone  LIB "HCTLIB.DLL" (n%)

CONST RS_NOT_RUN=0
CONST RS_RUNNING=1
CONST RS_FAILED=2
CONST RS_PASSED=3
CONST RS_CANT_RUN=4

DECLARE FUNCTION WNetGetCaps          LIB "USER.EXE" (A%) as Integer
DECLARE FUNCTION WNetDeviceMode       LIB "USER.EXE" (A%) as Integer
DECLARE FUNCTION WNetBrowseDialog     LIB "USER.EXE" (A%, B%, C$) as Integer
DECLARE FUNCTION WNetGetUser          LIB "USER.EXE" (A$, B as Pointer to Integer) as Integer
DECLARE FUNCTION WNetAddConnection    LIB "USER.EXE" (A$, B$, C$) as Integer
DECLARE FUNCTION WNetCancelConnection LIB "USER.EXE" (A$, B%) as Integer
DECLARE FUNCTION WNetGetConnection    LIB "USER.EXE" (A$, B$, C as Pointer to Integer) as Integer
DECLARE FUNCTION WNetWatchQueue       LIB "USER.EXE" (A%, B$, C$, D%) as Integer
DECLARE FUNCTION WNetUnwatchQueue     LIB "USER.EXE" (A%) as Integer
DECLARE FUNCTION WNetLockQueueData    LIB "USER.EXE" (A$, B$, C$) as Integer
DECLARE FUNCTION WNetUnlockQueueData  LIB "USER.EXE" (A$) as Integer
DECLARE FUNCTION WNetOpenJob          LIB "USER.EXE" (A$, B$, C%, D as Pointer to Integer) as Integer
DECLARE FUNCTION WNetCloseJob         LIB "USER.EXE" (A%, B as Pointer to Integer, C$) as Integer
DECLARE FUNCTION WNetHoldJob          LIB "USER.EXE" (A$, B%) as Integer
DECLARE FUNCTION WNetReleaseJob       LIB "USER.EXE" (A$, B%) as Integer
DECLARE FUNCTION WNetCancelJob        LIB "USER.EXE" (A$, B%) as Integer
DECLARE FUNCTION WNetSetJobCopies     LIB "USER.EXE" (A$, B%, C%) as Integer
DECLARE FUNCTION WNetAbortJob         LIB "USER.EXE" (A$, B%) as Integer
DECLARE FUNCTION WNetGetError         LIB "USER.EXE" (A$) as Integer
DECLARE FUNCTION WNetGetErrorText     LIB "USER.EXE" (A%, B$, C$) as Integer

DECLARE FUNCTION WNetWriteJob         LIB "USER.EXE" (A%, B$, C$) as Integer
DECLARE FUNCTION WNetResoreConnection LIB "USER.EXE" (A%, B$) as Integer
DECLARE FUNCTION WNetConnectDialog    LIB "USER.EXE" (A%, B%) as Integer
DECLARE FUNCTION WNetDisconnectDialog LIB "USER.EXE" (A%, B%) as Integer
DECLARE FUNCTION WNetConnectionDialog LIB "USER.EXE" (A%, B%) as Integer
DECLARE FUNCTION WNetViewQueueDialog  LIB "USER.EXE" (A%, B$) as Integer
DECLARE FUNCTION WNetGetPropertyText  LIB "USER.EXE" (A%, B%, C$, D$, E%) as Integer
DECLARE FUNCTION WNetPropertDialog    LIB "USER.EXE" (A%, B%, C%, D$, E%) as Integer
DECLARE FUNCTION WNetGetDirectoryType LIB "USER.EXE" (A$, B$) as Integer
DECLARE FUNCTION WNetDirectoryNotify  LIB "USER.EXE" (A%, B$, C%) as Integer


CONST VOLUME_LONGNAMES = 1

DECLARE FUNCTION LFNFindFirst      LIB "LFN.DLL" (A$, B%, C as POINTER to Integer, D as POINTER to Integer, E%, F as POINTER to Integer) as Integer
DECLARE FUNCTION LFNFindNext       LIB "LFN.DLL" (A%, B&, C%, D&) as Integer
DECLARE FUNCTION LFNFindClose      LIB "LFN.DLL" (A%) as Integer
DECLARE FUNCTION LFNGetAttributes  LIB "LFN.DLL" (A$, B as Pointer to Integer) as Integer
DECLARE FUNCTION LFNSetAttributes  LIB "LFN.DLL" (A$, B%) as Integer
DECLARE FUNCTION LFNCopy           LIB "LFN.DLL" (A$, B$, C&) as Integer
DECLARE FUNCTION LFNMove           LIB "LFN.DLL" (A$, B$) as Integer
DECLARE FUNCTION LFNDelete         LIB "LFN.DLL" (A$) as Integer
DECLARE FUNCTION LFNMKDir          LIB "LFN.DLL" (A$) as Integer
DECLARE FUNCTION LFNRMDir          LIB "LFN.DLL" (A$) as Integer
DECLARE FUNCTION LFNGetVolumeLabel LIB "LFN.DLL" (A%, B$) as Integer
DECLARE FUNCTION LFNSetVolumeLabel LIB "LFN.DLL" (A%, B$) as Integer
DECLARE FUNCTION LFNParse          LIB "LFN.DLL" (A$, B$, C$) as Integer
DECLARE FUNCTION LFNVolumeType     LIB "LFN.DLL" (A%, B as Pointer to Integer) as Integer

DECLARE FUNCTION WinExec LIB "KERNEL.EXE" (A$, B%) as Integer
DECLARE FUNCTION CreateDir (A$) as Integer
FUNCTION CreateDir (A$) as Integer
LOG "Creating Directory : "+A$
rem LOG "Calling WinExec(""COMMAND.COM /C MKDIR "+A$+""", 0)"
rem retcode%=WinExec("COMMAND.COM /C MKDIR "+A$, 0)
LOG "Calling Shell ""MKDIR "+A$+""""
shell "MKDIR "+A$
retcode%=SleepDelay(5)
LOG ""
END FUNCTION

DECLARE FUNCTION RemoveDir (A$) as Integer
FUNCTION RemoveDir (A$) as Integer
LOG "Removing Directory : "+A$
LOG "Calling Shell ""rmDIR "+A$+""""
shell "RMDIR "+A$
rem LOG "Calling WinExec(""COMMAND.COM /C RMDIR "+A$+""", 0)"
rem retcode%=WinExec("COMMAND.COM /C RMDIR "+A$, 0)
retcode%=SleepDelay(5)
END FUNCTION

DECLARE FUNCTION DeleteFile (A$) as Integer
FUNCTION DeleteFile (A$) as Integer
LOG "Deleting File : "+A$
LOG "Calling Shell ""del "+A$+""""
shell "DEL "+A$
rem LOG "Calling WinExec(""COMMAND.COM /C DEL "+A$+""", 0)"
rem retcode%=WinExec("COMMAND.COM /C DEL "+A$, 0)
retcode%=SleepDelay(5)
END FUNCTION


DECLARE FUNCTION GetFocus LIB "USER.EXE" () AS INTEGER
DECLARE FUNCTION MessageBox  LIB "USER.EXE" (A%, b$, c$, d%) AS INTEGER

CONST WN_SUCCESS       = 0
CONST WN_NOT_SUPPORTED = 1
CONST WN_NET_ERROR     = 2  
CONST WN_MORE_DATA     = 3
CONST WN_BAD_POINTER   = 4
CONST WN_BAD_VALUE     = 5
CONST WN_BAD_PASSWORD  = 6
CONST WN_ACCESS_DENIED = 7
CONST WN_FUNCTION_BUSY = 8
CONST WN_WINDOWS_ERROR = 9
CONST WN_BAD_USER      = 10
CONST WN_OUT_OF_MEMORY = 11
CONST WN_CANCELLED     = 12
CONST WN_CONTINUE      = 13
CONST WN_NOT_CONNECTED = 48
CONST WN_OPEN_FILES    = 49
CONST WN_BAD_NETNAME   = 50
CONST WN_BAD_LOCALNAME = 51
CONST WN_ALREADY_CONNECTED = 52
CONST WN_CONNECTION_CLOSED = 54
CONST WN_BAD_JOBID     = 64
CONST WN_JOB_NOT_FOUND = 65
CONST WN_JOB_NOT_HELD  = 66
CONST WN_BAD_QUEUE     = 67
CONST WN_BAD_FILE_HANDLE = 68
CONST WN_CANT_SET_COPIES = 69
CONST WN_ALREADY_LOCKED  = 80
CONST WN_DEVICE_ERROR    = 53

DECLARE FUNCTION WERROR (A%) As String
FUNCTION WERROR (A%) As String
SELECT CASE A%
    CASE 0
        WERROR= "WN_SUCCESS"
    CASE 1
        WERROR= "WN_NOT_SUPPORTED"
    CASE 2
        WERROR= "WN_NET_ERROR"
    CASE 3
        WERROR= "WN_MORE_DATA"
    CASE 4
        WERROR= "WN_BAD_POINTER"
    CASE 5
        WERROR= "WN_BAD_VALUE"
    CASE 6
        WERROR= "WN_BAD_PASSWORD"
    CASE 7
        WERROR= "WN_ACCESS_DENIED"
    CASE 8
        WERROR= "WN_FUNCTION_BUSY"
    CASE 9
        WERROR= "WN_WINDOWS_ERROR"
    CASE 10
        WERROR= "WN_BAD_USER"
    CASE 11
        WERROR= "WN_OUT_OF_MEMORY"
    CASE 12
        WERROR= "WN_CANCELLED"
    CASE 13
        WERROR= "WN_CONTINUE"
    CASE 48
        WERROR= "WN_NOT_CONNECTED"
    CASE 49
        WERROR= "WN_OPEN_FILES"
    CASE 50
        WERROR= "WN_BAD_NETNAME"
    CASE 51
        WERROR= "WN_BAD_LOCALNAME"
    CASE 52
        WERROR= "WN_ALREADY_CONNECTED"
    CASE 53
        WERROR= "WN_DEVICE_ERROR"
    CASE 54
        WERROR= "WN_CONNECTION_CLOSED"
    CASE 64
        WERROR= "WN_BAD_JOBID"
    CASE 65
        WERROR= "WN_JOB_NOT_FOUND"
    CASE 66
        WERROR= "WN_JOB_NOT_HELD"
    CASE 67
        WERROR= "WN_BAD_QUEUE"
    CASE 68
        WERROR= "WN_BAD_FILE_HANDLE"
    CASE 69
        WERROR= "WN_CANT_SET_COPIES"
    CASE 80
        WERROR= "WN_ALREADY_LOCKED"
    CASE ELSE
        WERROR="Undefined: "+str$(A%)
END SELECT
END FUNCTION


CONST WNNC_SPEC_VERSION     = 1
CONST WNNC_NET_TYPE         = 2
CONST WNNC_DRIVER_VERSION   = 3
CONST WNNC_USER             = 4
CONST WNNC_CONNECTION       = 6
CONST WNNC_PRINTING         = 7
CONST WNNC_DIALOG           = 8
CONST WNNC_ADMIN            = 9
CONST WNNC_ERROR            = 10

CONST WNNC_NET_NONE         = 0
CONST WNNC_NET_MSNET        = 256
CONST WNNC_NET_LANMAN       = 512
CONST WNNC_NET_NETWARE      = 768
CONST WNNC_NET_VINES        = 1024
CONST WNNC_NET_10NET        = 1280 

CONST WNNC_USR_GetUser          = 1

CONST WNNC_CON_AddConnection    = 1
CONST WNNC_CON_CancelConnection = 2
CONST WNNC_CON_GetConnections   = 4
CONST WNNC_CON_AutoConnect      = 8
CONST WNNC_CON_BrowseDialog     = 16

CONST WNNC_PRT_OpenJob       = 2
CONST WNNC_PRT_CloseJob      = 4
CONST WNNC_PRT_HoldJob       = 16
CONST WNNC_PRT_ReleaseJob    = 32
CONST WNNC_PRT_CancelJob     = 64
CONST WNNC_PRT_SetJobCopies  = 128
CONST WNNC_PRT_WatchQueue    = 256
CONST WNNC_PRT_UnwatchQueue  = 512
CONST WNNC_PRT_LockQueueData = 1024
CONST WNNC_PRT_UnlockQueueData = 2048
CONST WNNC_PRT_ChangeMsg = 4096
CONST WNNC_PRT_AbortJob  = 8192

CONST WNNC_DLG_DeviceMode = 1
CONST WNNC_DLG_BrowseDialog = 2
CONST WNNC_DLG_ConnectDialog = 4
CONST WNNC_DLG_DisconnectDialog = 8
CONST WNNC_DLG_ViewQueueDialog = 16
CONST WNNC_DLG_PropertyDialog = 32
CONST WNNC_DLG_ConnectionDialog = 64


CONST WNNC_ERR_GetError     = 1
CONST WNNC_ERR_GetErrorText = 2

CONST WNNC_ADM_GetDirectoryType = 1
CONST WNNC_ADM_DirectoryNotify = 2
CONST WNNC_ADM_LongNames       =4

DECLARE FUNCTION checkError (Expected%, Actual%, Verbose%) AS INTEGER
DECLARE FUNCTION CheckCommon(Act%) AS INTEGER
DECLARE FUNCTION checkDOSError(Expected%, Actual%, Verbose%) AS INTEGER

FUNCTION checkDOSError(Expected%, Actual%, Verbose%) AS INTEGER
    IF Expected% = Actual% THEN
        IF (Verbose% = TRUE) THEN
            LOG "Variation Succeeded!"
        END IF
        checkDOSError=SUCCESS
    ELSE
        LOG "FAILURE: Expected "+str$(Expected%)+" but got "+str$(Actual%)
        checkDOSError=FAIL
        TestResult%=RS_FAILED
    END IF
END FUNCTION


FUNCTION checkError (Expected%, Actual%, Verbose%) AS INTEGER
    IF Expected% = Actual% THEN
        IF (Verbose% = TRUE) THEN
           LOG "Expected Returned Value: "+WERROR(Expected%)
           LOG "Actual Received Value: "+WERROR(Actual%)
           LOG "Variation Succeeded!"
        END IF
        checkError=SUCCESS
    ELSE
        TestResult%=RS_FAILED
        LOG "FAILURE: Expected "+WERROR(Expected%)+" but got "+WERROR(Actual%)
        IF Expected% = WN_SUCCESS THEN
            com% = CheckCommon(Actual%)
            checkError=com%
        ELSE
            checkError=FAIL
        END IF   
       
        IF Expected% = WN_NOT_SUPPORTED THEN
            LOG "Return Code: "+WERROR(Actual%)+" and WNetGetCaps indicates that"
            LOG "this function is not supported."
        END IF
        
    END IF
END FUNCTION


FUNCTION CheckCommon (Actual%) AS INTEGER
    SELECT CASE Actual%
        CASE WN_NOT_SUPPORTED
            LOG ( "WNetGetCaps indicates support for this function.")
            CheckCommon=COMMON
           
        CASE WN_OUT_OF_MEMORY
            LOG ( "Procedure: Start the script in manual mode (this will be a command")
            LOG ( "line parameter). Check Available memory in the program manager. Select")
            LOG ( "the API from the manual test menu. If there appears to be adequate memory")
            LOG ( "available and the API persists in returning this error code, report")
            LOG ( "as a bug.:")
            CheckCommon=COMMON

        CASE WN_NET_ERROR
            LOG ( "Procedure: Verify that the network is setup properly and that")
            LOG ( "connections can be made from File Manager and DOS. Try to verify")
            LOG ( "manually that the network is functioning in the area that")
            LOG ( "you are testing. Rerun the test several times to verify")
            LOG ( "that it was not a hardware error. If it continues to reproduce,")
            LOG ( "report as a bug.")
            CheckCommon=COMMON
            
        CASE WN_WINDOWS_ERROR
            LOG ( "The reason for this error message is unknown.")
            CheckCommon=COMMON

        CASE WN_BAD_VALUE
            LOG ( "Procedure:  Check the syntax of the call. If the values look valid,")
            LOG ( "report as a WinBug. If they do not, report as a NCT Bug. If it is")
            LOG ( "confirmed to not be an NCT bug, it will be transferred to WinBug.")
            CheckCommon=COMMON

        CASE WN_CANCELLED
            LOG ( "Procedure: Report as a WinBug as a bogus return code since the script")
            LOG ( "doesn't cancel this part of the test.")
            CheckCommon=COMMON
            
        CASE WN_BAD_POINTER
            LOG ( "Procedure: Check the syntax of the call. If the values look valid,")
            LOG ( "report as a WinBug. If they do not, report as a NCT Bug. If it is ")
            LOG ( "confirmed to not be an NCT bug, it will be transferred to WinBug.")
            CheckCommon=COMMON
            
        CASE WN_BAD_NETNAME
            LOG "Make sure the workstation is logged into the network and that"
            LOG "the connection can be made in a DOS VM, outside Windows"
            LOG "or with File Manager. If the connection can be made elsewhere"
            LOG "and the error occurs again, report as a bug."
            
        CASE WN_BAD_LOCALNAME
            LOG "Check that the drive letter is valid in the Winplay.ini. If"
            LOG "it turns out that the driver letter was already in use, report"
            LOG "as a bug because the correct return code is WN_ALREADY_CONNECTED. "

        CASE WN_BAD_PASSWORD
            LOG "Check that the password is valid in the Winplay.ini. Also, check"
            LOG "that the connection can be made in a DOS VM, outside Windows or"
            LOG "with File Manager using the password. If the connection can be"
            LOG "made elsewhere and the error occurs again, report as a bug."

        CASE WN_ACCESS_DENIED
            LOG "Check that the user has correct permissions on the network server"
            LOG "involved. Also, check that the connection can be made in a DOS VM,"
            LOG "outside Windows or with File Manager. If it can, report as a bug."

        CASE WN_ALREADY_CONNECTED
            LOG "Go to file manager and disconnect all network drives and rerun the script"
            LOG "If the error is returned again, it is a bug."
            
        CASE ELSE
            CheckCommon=FAIL
            
    END SELECT
END FUNCTION



DECLARE SUB header (API$) 
SUB header (API$) 
    LOG("")
    LOG ("**** TESTING "+API$)
END SUB



SUB LOG (Out$)
    IF LOGFILE = TRUE THEN
        WriteLog(Out$)
    END IF
    print Out$
END SUB

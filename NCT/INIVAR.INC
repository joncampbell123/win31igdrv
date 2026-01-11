GLOBAL UserName as String
GLOBAL NCTDriveLtr as String
GLOBAL NCTPath as String

GLOBAL NetDrive1 as String
GLOBAL NetPath1 as String
GLOBAL NetPwd1 as String
GLOBAL NetDir1 as String

GLOBAL NetDrive2 as String
GLOBAL NetPath2 as String
GLOBAL NetPwd2 as String
GLOBAL NetDir2 as String

GLOBAL NetDrive3 as String
GLOBAL NetPath3 as String
GLOBAL NetPwd3 as String
GLOBAL NetDir3 as String

GLOBAL NetPrt1 as String
GLOBAL NetPrtPwd1 as String

DECLARE FUNCTION GetPrivateProfileString LIB "KERNEL.EXE" (A$, B$, C$, D$, C%, D$) as INTEGER
DECLARE FUNCTION GetIniVar (KeyName$, RetString$) AS Integer
DECLARE FUNCTION GetWinplayVars (Logtype%) as Integer
DECLARE FUNCTION GetAnyIniVar2(Section$,Keynames$, Retvalue$, Inifile$) as integer



'************************************************************************
' Function GetAnyIniVar2
'************************************************************************

FUNCTION GetAnyIniVar2(Section$, Keyname$, Retstring$, Inifile$) as integer
size% = len(retstring$)
default$ = "NOVALUE"
retlength%=getprivateprofilestring(Section$,keyname$,default$,retstring$,size%,inifile$)

'Print retstring$
'Print default$

IF retstring$ = default$ THEN
   GETanyinivar2=FALSE
ELSE
   getanyinivar2=TRUE
END IF

retstring$=rtrim$(retstring$)
'Print retstring$

END FUNCTION




FUNCTION GetIniVar (KeyName$, RetString$) as Integer
Size%=len(RetString$)
Default$="NOVALUE"
retlength%=GetPrivateProfileString ("ScriptVars", KeyName$, Default$, RetString$, Size%, "WINPLAY.INI") 
IF RetString$ = Default$ THEN
    GetIniVar=FALSE
ELSE
    GetIniVar=TRUE
END IF
RetString$=rtrim$(RetString$)

'IF asc(mid$(RetString$,len(RetString$),1))=0 THEN
'    RetString$=mid$(RetString$,1,len(RetString$)-1)
'END IF
END FUNCTION




FUNCTION GetWinplayVars (Logtype%) as Integer

GetWinplayVars=TRUE

NCTDriveLtr$="                     "
retcode%=GetIniVar("NCTDriveLtr", NCTDriveLtr$)
if retcode% <> TRUE THEN
    print "Error Reading winplay.ini variable: NCTDriveLtr$"
    GetWinplayVars=FALSE
ELSE
    NCTDriveLtr$=UCASE$(NCTDriveLtr$)
    NCTDriveLtr$=mid$(NCTDriveLtr$, 1, 1)+":"
END IF

NCTPath$="                                         "
retcode%=GetIniVar("NCTPath", NCTPath$)
if retcode% <> TRUE THEN
    print "Error Reading winplay.ini variable: NCTPath"
    GetWinplayVars=FALSE
END IF

IF Logtype% = 0 THEN
    OpenLog(NCTDriveLtr$+NCTPath$+"\WINNET.LOG")
ELSE
    OpenLog(NCTDriveLtr$+NCTPath$+"\WNETPRT.LOG")
END IF

UserName$="                     "
retcode%=GetIniVar("UserName", UserName$)
if retcode% <> TRUE THEN
    LOG "Error Reading winplay.ini variable: UserName"
    GetWinplayVars=FALSE
END IF


NetDrive1$="                     "
retcode%=GetIniVar("NetDrive1", NetDrive1$)
if retcode% <> TRUE THEN
    LOG "Error Reading winplay.ini variable: NetDrive1"
    GetWinplayVars=FALSE
ELSE
    NetDrive1$=UCASE$(NetDrive1$)
    NetDrive1$=mid$(NetDrive1$, 1, 1)+":"
END IF
    
NetPath1$="                                     "
retcode%=GetIniVar("NetPath1", NetPath1$)
if retcode% <> TRUE THEN
    LOG "Error Reading winplay.ini variable: NetPath1"
    GetWinplayVars=FALSE
END IF


NetPwd1$="                     "
retcode%=GetIniVar("NetPwd1", NetPwd1$)
if retcode% <> TRUE THEN
    LOG "Error Reading winplay.ini variable: NetPwd1"
    GetWinplayVars=FALSE
END IF

NetDir1$="                     "
retcode%=GetIniVar("NetDir1", NetDir1$)
if retcode% <> TRUE THEN
    LOG "Error Reading winplay.ini variable: NetDir1"
    GetWinplayVars=FALSE
END IF


NetDrive2$="                     "
retcode%=GetIniVar("NetDrive2", NetDrive2$)
if retcode% <> TRUE THEN
    LOG "Error Reading winplay.ini variable: NetDrive2"
    GetWinplayVars=FALSE
ELSE
    NetDrive2$=UCASE$(NetDrive2$)
    NetDrive2$=mid$(NetDrive2$, 1, 1)+":"
END IF

NetPath2$="                                     "
retcode%=GetIniVar("NetPath2", NetPath2$)
if retcode% <> TRUE THEN
    LOG "Error Reading winplay.ini variable: NetPath2"
    GetWinplayVars=FALSE
END IF

NetPwd2$="                     "
retcode%=GetIniVar("NetPwd2", NetPwd2$)
if retcode% <> TRUE THEN
    LOG "Error Reading winplay.ini variable: NetPwd2"
    GetWinplayVars=FALSE
END IF

NetDir2$="                     "
retcode%=GetIniVar("NetDir2", NetDir2$)
if retcode% <> TRUE THEN
    LOG "Error Reading winplay.ini variable: NetDir2"
    GetWinplayVars=FALSE
END IF

NetDrive3$="                     "
retcode%=GetIniVar("NetDrive3", NetDrive3$)
if retcode% <> TRUE THEN
    LOG "Error Reading winplay.ini variable: NetDrive3"
    GetWinplayVars=FALSE
ELSE
    NetDrive3$=UCASE$(NetDrive3$)
    NetDrive3$=mid$(NetDrive3$, 1, 1)+":"
END IF

NetPath3$="                                      "
retcode%=GetIniVar("NetPath3", NetPath3$)
if retcode% <> TRUE THEN
    LOG "Error Reading winplay.ini variable: NetPath3"
    GetWinplayVars=FALSE
END IF
   
NetPwd3$="                     "
retcode%=GetIniVar("NetPwd3", NetPwd3$)
if retcode% <> TRUE THEN
    LOG "Error Reading winplay.ini variable: NetPwd3"
    GetWinplayVars=FALSE
END IF

NetDir3$="                     "
retcode%=GetIniVar("NetDir3", NetDir3$)
if retcode% <> TRUE THEN
    LOG "Error Reading winplay.ini variable: NetDir3"
    GetWinplayVars=FALSE
END IF

NetPrt1$="                     "
retcode%=GetIniVar("NetPrt1", NetPrt1$)
if retcode% <> TRUE THEN
    LOG "Error Reading winplay.ini variable: NetPrt1"
    GetWinplayVars=FALSE
END IF

NetPrtPwd1$="                     "
retcode%=GetIniVar("NetPrtPwd1", NetPrtPwd1$)
if retcode% <> TRUE THEN
    LOG "Error Reading winplay.ini variable: NetPrtPwd1"
    GetWinplayVars=FALSE
END IF


REM print "UserName ="+UserName$

REM print "NetDrive1 ="+NetDrive1$
REM print "NetPath1 ="+NetPath1$
REM print "NetDir1 ="+NetDir1$
REM print "NetPwd1 ="+NetPwd1$

REM print "NetDrive2 ="+NetDrive2$
REM print "NetPath2 ="+NetPath2$
REM print "NetDir2 ="+NetDir2$
REM print "NetPwd2 ="+NetPwd2$

REM print "NetDrive3 ="+NetDrive3$
REM print "NetPath3 ="+NetPath3$
REM print "NetDir3 ="+NetDir3$
REM print "NetPwd3 ="+NetPwd3$

REM print "NetPrt1 ="+NetPrt1$
REM print "NetPrtPwd1="+NetPrtPwd1$

END FUNCTION

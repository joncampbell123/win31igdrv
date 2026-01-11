'************************************************************************
' Global declarations
'************************************************************************

GLOBAL UserName as String
GLOBAL NCTDriveLtr as String
GLOBAL NCTPath as String
GLOBAL NCTFlopLtr as String
GLOBAL AutoDelay as Integer
GLOBAL RemIOLoc as String
GLOBAL ShortPrnDelay as Integer
GLOBAL LongPrnDelay as Integer
GLOBAL MakeBatchFile as String

GLOBAL NetDrive1 as String
GLOBAL NetPath1 as String
GLOBAL NetPwd1 as String
GLOBAL NetDir1 as String
GLOBAL NetPrt1 as String
GLOBAL NetPrtPwd1 as String

GLOBAL NetDrive2 as String
GLOBAL NetPath2 as String
GLOBAL NetPwd2 as String
GLOBAL NetDir2 as String
GLOBAL NetPrt2 as String
GLOBAL NetPrtPwd2 as String

GLOBAL NetDrive3 as String
GLOBAL NetPath3 as String
GLOBAL NetPwd3 as String
GLOBAL NetDir3 as String
GLOBAL NetPrt3 as String
GLOBAL NetPrtPwd3 as String


'************************************************************************
' Function declarations
'************************************************************************

DECLARE FUNCTION GetPrivateProfileString LIB "KERNEL.EXE" (A$, B$, C$, D$, C%, D$) as INTEGER
DECLARE FUNCTION GetIniVar (KeyName$, RetString$) AS Integer
DECLARE SUB GetWinplayVars
DECLARE FUNCTION GetAnyIniVar(Keyname$, Retstring$, Inifile$) as integer
DECLARE FUNCTION GetAnyIniVar2(Section$, Keyname$, Retstring$, Inifile$) as integer


'************************************************************************
' Function GetAnyIniVar
'************************************************************************

FUNCTION GetAnyIniVar(Keyname$, Retstring$, Inifile$) as integer
size% = len(retstring$)
default$ = "NOVALUE"
retlength%=getprivateprofilestring("network", keyname$,default$,retstring$,size%,inifile$)

'Print retstring$
'Print default$

IF retstring$ = default$ THEN
   GETanyinivar=FALSE
ELSE
   getanyinivar=TRUE
END IF

retstring$=rtrim$(retstring$)
'Print retstring$

END FUNCTION

'************************************************************************
' Function GetAnyIniVar2
'************************************************************************

FUNCTION GetAnyIniVar2(Section$, Keyname$, Retstring$, Inifile$) as integer
size% = len(retstring$)
default$ = "NOVALUE"
retlength%=getprivateprofilestring(Section$, keyname$,default$,retstring$,size%,inifile$)

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





'************************************************************************
' Function GetIniVar
'************************************************************************


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



'************************************************************************
' Function GetWinplayVars
'************************************************************************

SUB GetWinplayVars

UserName$=string$ (64," ")
retcode%=GetIniVar("UserName", UserName$)
if retcode% <> TRUE THEN
    pause "Error Reading winplay.ini variable: UserName"
END IF


NCTDriveLtr$=string$ (64," ")
retcode%=GetIniVar("NCTDriveLtr", NCTDriveLtr$)
if retcode% <> TRUE THEN
    pause "Error Reading winplay.ini variable: NCTDriveLtr"
ELSE
   NCTDriveLtr$=mid$(NCTDriveLtr$, 1, 1)
END IF


NCTPath$=string$ (64," ")
retcode%=GetIniVar("NCTPath", NCTPath$)
if retcode% <> TRUE THEN
    pause "Error Reading winplay.ini variable: NCTPath"
END IF

NCTFlopLtr$=string$ (64," ")
retcode%=GetIniVar("NCTFlopLtr", NCTFlopLtr$)
if retcode% <> TRUE THEN
    pause "Error Reading winplay.ini variable: NCTFlopLtr"
ELSE
   NCTFlopLtr$=mid$(NCTFlopLtr$, 1, 1)
END IF

Buff$=string$ (64," ")
retcode%=GetIniVar("AutoDelay", Buff$)
if retcode% <> TRUE THEN
    pause "Error Reading winplay.ini variable: AutoDelay"
ELSE
    AutoDelay%=VAL(Buff$)
END IF

RemIOLoc$=string$ (64," ")
retcode%=GetIniVar("RemIOLoc", RemIOLoc$)
if retcode% <> TRUE THEN
    pause "Error Reading winplay.ini variable: RemIOLoc"
END IF

Buff$=string$ (64," ")
retcode%=GetIniVar("ShortPrnDelay", Buff$)
if retcode% <> TRUE THEN
    pause "Error Reading winplay.ini variable: ShortPrnDelay"
ELSE
    ShortPrnDelay%=VAL(Buff$)
END IF


Buff$=string$ (64," ")
retcode%=GetIniVar("LongPrnDelay", Buff$)
if retcode% <> TRUE THEN
    pause "Error Reading winplay.ini variable: LongPrnDelay"
ELSE
    LongPrnDelay%=VAL(Buff$)
END IF

MakeBatchFile$=string$ (64," ")
retcode%=GetIniVar("MakeBatchFile", MakeBatchFile$)
if retcode% <> TRUE THEN
    pause "Error Reading winplay.ini variable: MakeBatchFile"
ELSE
    MakeBatchFile$=mid$(MakeBatchFile$, 1, 1)
END IF

NetDrive1$=string$ (64," ")
retcode%=GetIniVar("NetDrive1", NetDrive1$)
if retcode% <> TRUE THEN
    pause "Error Reading winplay.ini variable: NetDrive1"
ELSE
   NetDrive1$=mid$(NetDrive1$, 1, 1)
END IF
    
NetPath1$=string$ (64," ")
retcode%=GetIniVar("NetPath1", NetPath1$)
if retcode% <> TRUE THEN
    pause "Error Reading winplay.ini variable: NetPath1"
END IF


NetPwd1$=string$ (64," ")
retcode%=GetIniVar("NetPwd1", NetPwd1$)
if retcode% <> TRUE THEN
    pause "Error Reading winplay.ini variable: NetPwd1"
END IF

NetDir1$=string$ (64," ")
retcode%=GetIniVar("NetDir1", NetDir1$)
if retcode% <> TRUE THEN
    pause "Error Reading winplay.ini variable: NetDir1"
END IF

NetPrt1$=string$ (64," ")
retcode%=GetIniVar("NetPrt1", NetPrt1$)
if retcode% <> TRUE THEN
    pause "Error Reading winplay.ini variable: NetPrt1"
END IF

NetPrtPwd1$=string$ (64," ")
retcode%=GetIniVar("NetPrtPwd1", NetPrtPwd1$)
if retcode% <> TRUE THEN
    pause "Error Reading winplay.ini variable: NetPrtPwd1"
END IF

NetDrive2$=string$ (64," ")
retcode%=GetIniVar("NetDrive2", NetDrive2$)
if retcode% <> TRUE THEN
    pause "Error Reading winplay.ini variable: NetDrive2"
ELSE
    'NetDrive2$=UCASE$(NetDrive2$)
    NetDrive2$=mid$(NetDrive2$, 1, 1)
END IF

NetPath2$=string$ (64," ")
retcode%=GetIniVar("NetPath2", NetPath2$)
if retcode% <> TRUE THEN
    pause "Error Reading winplay.ini variable: NetPath2"
END IF

NetPwd2$=string$ (64," ")
retcode%=GetIniVar("NetPwd2", NetPwd2$)
if retcode% <> TRUE THEN
    pause "Error Reading winplay.ini variable: NetPwd2"
END IF

NetDir2$=string$ (64," ")
retcode%=GetIniVar("NetDir2", NetDir2$)
if retcode% <> TRUE THEN
    pause "Error Reading winplay.ini variable: NetDir2"
END IF

NetPrt2$=string$ (64," ")
retcode%=GetIniVar("NetPrt2", NetPrt2$)
if retcode% <> TRUE THEN
    pause "Error Reading winplay.ini variable: NetPrt2"
END IF

NetPrtPwd2$=string$ (64," ")
retcode%=GetIniVar("NetPrtPwd2", NetPrtPwd2$)
if retcode% <> TRUE THEN
    pause "Error Reading winplay.ini variable: NetPrtPwd1"
END IF

NetDrive3$=string$ (64," ")
retcode%=GetIniVar("NetDrive3", NetDrive3$)
if retcode% <> TRUE THEN
    pause "Error Reading winplay.ini variable: NetDrive3"
ELSE
    'NetDrive3$=UCASE$(NetDrive3$)
    NetDrive3$=mid$(NetDrive3$, 1, 1)
END IF

NetPath3$=string$ (64," ")
retcode%=GetIniVar("NetPath3", NetPath3$)
if retcode% <> TRUE THEN
    pause "Error Reading winplay.ini variable: NetPath3"
END IF
   
NetPwd3$=string$ (64," ")
retcode%=GetIniVar("NetPwd3", NetPwd3$)
if retcode% <> TRUE THEN
    pause "Error Reading winplay.ini variable: NetPwd3"
END IF

NetDir3$=string$ (64," ")
retcode%=GetIniVar("NetDir3", NetDir3$)
if retcode% <> TRUE THEN
    pause "Error Reading winplay.ini variable: NetDir3"
END IF

NetPrt3$=string$ (64," ")
retcode%=GetIniVar("NetPrt3", NetPrt3$)
if retcode% <> TRUE THEN
    pause "Error Reading winplay.ini variable: NetPrt3"
END IF

NetPrtPwd3$=string$ (64," ")
retcode%=GetIniVar("NetPrtPwd3", NetPrtPwd3$)
if retcode% <> TRUE THEN
    pause "Error Reading winplay.ini variable: NetPrtPwd3"
END IF



REM Remove thse REMs if you want to make sure things
REM are being read correctly
REM print "UserName ="+UserName$
REM print "MakeBatchFile ="+MakeBatchfile$                      

REM print "NetDrive1 ="+NetDrive1$
REM print "NetPath1 ="+NetPath1$
REM print "NetDir1 ="+NetDir1$
REM print "NetPwd1 ="+NetPwd1$
REM print "NetPrt1 ="+NetPrt1$
REM print "NetPrtPwd1="+NetPrtPwd1$

REM print "NetDrive2 ="+NetDrive2$
REM print "NetPath2 ="+NetPath2$
REM print "NetDir2 ="+NetDir2$
REM print "NetPwd2 ="+NetPwd2$
REM print "NetPrt2 ="+NetPrt2$
REM print "NetPrtPwd2="+NetPrtPwd2$

REM print "NetDrive3 ="+NetDrive3$
REM print "NetPath3 ="+NetPath3$
REM print "NetDir3 ="+NetDir3$
REM print "NetPwd3 ="+NetPwd3$
REM print "NetPrt3 ="+NetPrt3$
REM print "NetPrtPwd3="+NetPrtPwd3$


END SUB

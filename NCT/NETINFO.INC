CONST WNNC_SPEC_VERSION     = 1
CONST WNNC_NET_TYPE         = 2
CONST WNNC_DRIVER_VERSION   = 3

CONST WNNC_NET_NONE         = 0
CONST WNNC_NET_MSNET        = 256
CONST WNNC_NET_LANMAN       = 512
CONST WNNC_NET_NETWARE      = 768
CONST WNNC_NET_VINES        = 1024
CONST WNNC_NET_10NET        = 1280 

GLOBAL NetSpecVersion$
GLOBAL NetworkType$
GLOBAL NetDriverVersion$

DECLARE FUNCTION WNetGetCaps          LIB "USER.EXE" (A%) as Integer
DECLARE FUNCTION Trim$ (text$)
DECLARE FUNCTION GetDrvOffset% (drive$)


'******************************************************************
'   Function Trim$
'******************************************************************

FUNCTION Trim$ (text$)
  Trim$ = LTrim$ (RTrim$ (text$))
END FUNCTION
 
  
'******************************************************************
'   Function GETDrvOffset
'******************************************************************

FUNCTION GetDrvOffset% (drive$)

    IF ASC(drive$) >= ASC("a") THEN

       GetDrvOffset% = (ASC(drive$) - ASC("a"))

'      print "i am in lowercase letters!"
'      print str$(asc("a"))+" is asc for lowercase a"
'       print str$(asc(drive$))+" is asc for netdrive"  
            
    ELSE

       GetDrvOffset% = (ASC(drive$) - ASC("A"))
'      print "i am in cap letters!"
'      print str$(asc("A"))+" is asc for cap A"
'      print str$(asc(drive$))+" is asc for netdrive"

       
    END IF
            
END FUNCTION
 
  
'******************************************************************
'   SUBROUTINE GETNETINFO - 
'******************************************************************

DECLARE SUB GETNETINFO
SUB GETNETINFO


'*************************************************
'  Calling WNetGetCaps(WNNC_SPEC_VERSION)"
'*************************************************

retcode%=WNetGetCaps(WNNC_SPEC_VERSION)
Major%=retcode% / 256
Minor%=retcode% - (Major% * 256)
Digit1%=Minor% / 16
Digit2%=Minor% - (Digit1% * 16)

NetSpecVersion$ = Trim$(str$(Major%))+"."+Trim$(str$(Digit1%))+Trim$(str$(Digit2%))
'Print NetSpecVersion$

'**************************************************


'***************************************************
'  Calling WNetGetCaps(WNNC_NET_TYPE)"
'***************************************************

retcode%=WNetGetCaps(WNNC_NET_TYPE)
SELECT CASE retcode%
    CASE WNNC_NET_NONE
        networktype$= "NONE"
    CASE WNNC_NET_MSNET
        networktype$="MSNET"
    CASE WNNC_NET_LANMAN
        networktype$="LANMAN"
    CASE WNNC_NET_NETWARE
        networktype$="NETWARE"
    CASE WNNC_NET_VINES
        networktype$="VINES"
    CASE WNNC_NET_10NET
        networktype$="10NET"
    CASE ELSE
        networktype$="UNDEFINED!"
END SELECT
'PRINT networktype$

'**************************************************
'  Calling WNetGetCaps(WNNC_DRIVER_VERSION)"
'**************************************************

retcode%=WNetGetCaps(WNNC_DRIVER_VERSION)
Major%=retcode% / 256
Minor%=retcode% - (Major% * 256)
Digit1%=Minor% / 16
Digit2%=Minor% - (Digit1% * 16)

NetDriverVersion = Trim$(str$(Major%))+"."+Trim$(str$(Digit1%))+Trim$(str$(Digit2%))
'Print NetDriverVersion$

END SUB             'getnetinfo


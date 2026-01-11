'$INCLUDE: 'strssapp.inc'

'*************************************************************************
'script name:  strsslp.inc
'written by:   jl
'
'rev 3.1
'
'date 06/19/91
'
'description:
'
'This script call FM stress and Applet stress scripts:

'Scripts called by this script:
'
'       * StrssApp              : modified applet test
'
'************************************************************************
'Modification log:
'
'Name    Date           Modification:
'
'*************************************************************************
'  
'*************************************************************************
DECLARE SUB STRSSLP
SUB STRSSLP

lf$ = "stress.log"

FOR I=1 TO 12


wlog lf$, " "
wlog lf$, "***********************************************************"
wlog lf$, "Applet Test Section begins here! - Loop #"+str$(I)
wlog lf$, "***********************************************************"
wlog lf$, " "

   ret%=SleepDelay(25)

   strssapp       'call the modified applet test - DEFINED IN STRSSAPP.INC

wlog lf$, " "
wlog lf$, "***********************************************************"
wlog lf$, "End of Applet Loop #"+str$(I)
wlog lf$, "***********************************************************"

NEXT
END SUB                'strsslp

'**************************************************************************
' The End...of looping strsslp.inc
'**************************************************************************

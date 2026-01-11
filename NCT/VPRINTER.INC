DECLARE SUB VPRINTER (lf$)                   'PRINTER VERIFICATION SUBROUTINE

SUB VPRINTER (lf$)

'***********************************************************************
' verify printer connections
'***********************************************************************
wlog lf$, " "
wlog lf$, "***********************************************************"
wlog lf$, "Verify Network Printer Connections"
wlog lf$, "***********************************************************"
wlog lf$, " "

ret%=WinExec("Control",1)
ret%=SleepDelay(3)

SelectMenuItem "settings"
ret%=SleepDelay(1)
SelectMenuItem "printers"
ret%=SleepDelay(1)

ClickButton "connect"
ret%=SleepDelay(2)

WPlayKeys("%p")                 'ports listbox section

FOR i = 1 to 3
  
  SELECT CASE i
    CASE 1
       netprt$ = netprt1
    CASE 2
       netprt$ = netprt2
    CASE 3
       netprt$ = netprt3
  END SELECT
    
  SelectListBoxItem i-1,1               'select i-1th item - starts @ 0
  
  f$ = STRING$(64," ")                  'init string 
  GetListBoxSelection f$, len(f$)       'get text
  f$ = mid$(f$,7)

  temp2$ = mid$(str$(i),2)

  IF (UCASE$(f$) = UCASE$(netprt$)) THEN
    wlog lf$, "Success:  LPT"+temp2+": connected to "+f$
    wlog lf$, " "
  ELSE
    wlog lf$, "Error: LPT"+temp2+": not connected to "+UCASE$(netprt$)
    wlog lf$, "Verify that your network lists it's network printer connections"
    wlog lf$, "in the ControlPanel.Printers.Connect dialog box for LPTx:."
    wlog lf$, " "
  END IF

NEXT

clickbutton "Cancel"
ret%=SleepDelay(2)   
   
ClickButton "Close"             'printers
ret%=SleepDelay(5)

WPlayKeys("%{F4}")              'close control panel

END SUB                         'vprinter

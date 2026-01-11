'*************************************************************************
'script name:  printerd.inc
'written by:   jl
'
'rev 3.1
'
'date 06/19/91
'
'description:
'
'This script disconnects the printers defined in Winplay.ini:

'Variables Used (winplay.ini)

'for (x=1, x<=3, x++)
'       * NetPrtx               : defines network printer (\\LMserver\printer)
'       * NetPrtPwdx            ; defines network printer password
'
'************************************************************************
'Modification log:
'
'Name    Date           Modification:
'
'*************************************************************************
'  subroutine declarations
'*************************************************************************
DECLARE SUB DEFAULTPD                  'DEFAULT PRINTER DISCONNECT SUB
DECLARE SUB NOVELLPD                   'NOVELL DIALOG PRINTER DISCONNECT SUB
DECLARE SUB LM21PD                     'LANMAN 2.1 DIALOG PRINTER DISCONNECT SUB
DECLARE SUB PRINTERD                   'MAIN PRINTER DISCONNECTION SCRIPT

'*************************************************************************
'  SUB DEFAULTPD - DEFAULT PRINTER CONNECTION SUB
'*************************************************************************
SUB DEFAULTPD

  retcode%=WinExec("Control",1)    'exec control panel

  SelectMenuItem "Settings"                'bring up settings_printers
  ret%=SleepDelay(1)
  SelectMenuItem "Printers"                  
  ret%=SleepDelay(1)
  ClickButton "Connect"                    'connect button
  ret%=SleepDelay(1)
  ClickButton "Network"                    'network button
  ret%=SleepDelay(1)

 
  WPlayKeys("%d")                  'disconnect 3 times regardless.....
  ret%=SleepDelay(2)
  WPlayKeys("%d")
  ret%=SleepDelay(2)
  WPlayKeys("%d")
  ret%=SleepDelay(2)
  
  WPlayKeys("%{f4}")               'close network box - get back to connections
  ret%=SleepDelay(2)

  WPlayKeys("%{f4}%{f4}")          'close connections box - close printers dialog
  ret%=SleepDelay(2)
  WPlayKeys("%{F4}")               'close control panel 
  ret%=SleepDelay(5)


END SUB               'END OF SUB DEFAULTPC



'**************************************************************************
' SUB NOVELLPD - SUBROUTINE FOR NOVELL PRINTER CONNECTIONS
'**************************************************************************
SUB NOVELLPD

  retcode%=WinExec("Control",1)            'exec control panel
  SelectMenuItem "Settings"                'bring up settings_printers
  ret%=SleepDelay(1)
  SelectMenuItem "Printers"                  
  ret%=SleepDelay(1)
  ClickButton "Connect"                    'connect button
  ret%=SleepDelay(1)
  
  FOR i = 3 to 1 STEP -1                'note: have to go backwards due to
                                        'bug in netware.drv v2.0
    ClickButton "Network"
    ret%=SleepDelay(1)   

    SelectListBox "Printer Connections"      'goto correct list box
    ret%=SleepDelay(1)
    SelectListBoxItem i-1,1                  'ensure lptx is selected
    ret%=SleepDelay(1)

    ClickButton "Disconnect"                 'disconnect button
    ret%=SleepDelay(1)

    IF WindowExists ("NetWare Warning") THEN    'netware warning dialog
        ClickButton "OK"
    END IF
   
    ClickButton "Close"             'close out of network dialog

  NEXT i

  ClickButton "OK"                  'close out of connect dialog
  ret%=SleepDelay(1)
  ClickButton "Close"               'close out of printers dialog
  ret%=SleepDelay(1)
  
  SelectMenuItem "Settings"
  ret%=SleepDelay(1)
  SelectMenuItem "Exit"             'exit out of control panel

END SUB          'end of sub NovellPC


'**************************************************************************
' SUB LM21PD - SUBROUTINE FOR LAN MANAGER V2.1 PRINTER DISCONNECTIONS
'**************************************************************************
SUB LM21PD

  retcode%=WinExec("Control",1)            'exec control panel
  SelectMenuItem "Settings"                'bring up settings_printers
  ret%=SleepDelay(1)
  SelectMenuItem "Printers"                  
  ret%=SleepDelay(1)
  ClickButton "Connect"                    'connect button
  ret%=SleepDelay(1)
  ClickButton "Network"
  ret%=SleepDelay(1)
  
  FOR i = 1 to 3                           'disconnect existing printers

    SELECT CASE i
        CASE 1
            netprt$ = netprt1
            port$   = "lpt1:"
        CASE 2
            netprt$ = netprt2
            port$   = "lpt2:"
        CASE 3
            netprt$ = netprt3
            port$   = "lpt3:"
    END SELECT            


    WPlayKeys("%D")                          'device name
  
    NetPrtUsed% = ComboBoxItemExists(port$)  'does lptx: in combobox?
'    print str$(NetPrtUsed)                                      'debugging

    IF NetPrtUsed = 0 THEN                   '0 means it's already connected
       WPlayKeys("%c")                       'goto Network Printer Conn.. list box
       ret%=SleepDelay(1)
       SelectListBoxItem 0,1                 'blindly go to top of list
       ret%=SleepDelay(1)
       ClickButton "Disconnect"              'disconnect
       ret%=SleepDelay(1)
    END IF

  NEXT  
 

  ClickButton "OK"                  'close out of network dialog
  ret%=SleepDelay(1)
  ClickButton "OK"                  'close out of connect dialog
  ret%=SleepDelay(1)
  ClickButton "Close"               'close out of printers dialog
  ret%=SleepDelay(1)
  
  SelectMenuItem "Settings"
  ret%=SleepDelay(1)
  SelectMenuItem "Exit"             'exit out of control panel

END SUB          'end of sub LM21PC


'**************************************************************************
' SUB PRINTERD - Main Script
'**************************************************************************
SUB PRINTERD

ViewPort OFF

GetNetInfo


f$ = string$ (64," ")
defaultdialogs=string$(64," ")


retcode% = GetAnyIniVar ("defaultdialogs",defaultdialogs$, "win.ini")

IF retcode% = TRUE THEN
   DefaultPD                                      'sub call   
Else

   IF (NetworkType$ = "NETWARE") THEN
      NovellPD

   ELSEIF ((NetworkType$ = "LANMAN") and (NetDriverVersion$ = "2.010")) THEN
      LM21PD
   ELSE
      DefaultPD
   END IF     

END IF


END SUB                          'PRINTERD

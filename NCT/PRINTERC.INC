'*************************************************************************
'script name:  printerc.inc
'written by:   jl
'
'rev 3.1
'
'date 06/19/91
'
'description:
'
'This script connects the printers defined in Winplay.ini:

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
DECLARE SUB DEFAULTPC                  'DEFAULT PRINTER CONNECT SUB
DECLARE SUB NOVELLPC                   'NOVELL DIALOG PRINTER CONNECT SUB
DECLARE SUB LM21PC                     'LM21 ENHANCED PRINTER CONNECT SUB
DECLARE SUB PRINTERC                   'MAIN PRINTER CONNECTION SCRIPT

'*************************************************************************
'  SUB DEFAULTPC - DEFAULT PRINTER CONNECTION SUB
'*************************************************************************
SUB DEFAULTPC

  retcode%=WinExec("Control",1)         'exec control panel
  WPlayKeys("%SP")                      'bring up printers

  WPlayKeys("%C")                       'bring up connections dialog
  WPlayKeys("%N")                       'bring up networks dialog
  WPlayKeys("%D%D%D")                   'disconnect any existing printer connections

  WPlayKeys("%T")                       'go to Port dialog box
  WPlayKeys("{PGUP}")                   'ensures LPT1
  WPlayKeys("%N{DELETE}"+NetPrt1)       'enter printer share/name for lpt1
  WPlayKeys("%W"+NetPrtPwd1)            'printer password if available
  WPlayKeys("%C")                       'connect to the network printer
  ret%=SleepDelay(5)                    'wait for connection

  WPlayKeys("%T")                       'go to Port dialog box
  WPlayKeys("{PGUP}")                   'ensures LPT2
  WPlayKeys("%N{DELETE}"+NetPrt2)       'enter printer share/name for lpt2
  WPlayKeys("%W"+NetPrtPwd2)            'printer password if available
  WPlayKeys("%C")                       'connect to the network printer
  ret%=SleepDelay(5)                    'wait for connection

  WPlayKeys("%T")                       'go to Port dialog box
  WPlayKeys("{PGUP}")                   'ensures LPT3
  WPlayKeys("%N{DELETE}"+NetPrt3)       'enter printer share/name for lpt3
  WPlayKeys("%W"+NetPrtPwd3)            'printer password if available
  WPlayKeys("%C")                       'connect to the network printer
  ret%=SleepDelay(5)                    'wait for connection

  WPlayKeys("%{f4}")                    'close network box - get back to connections

  WPlayKeys("%{f4}%{f4}")               'close connections box - close printers dialog
  ret%=SleepDelay(2)
  WPlayKeys("%{F4}")                    'close control panel 
  ret%=SleepDelay(5)


END SUB               'END OF SUB DEFAULTPC



'**************************************************************************
' SUB NOVELLPC - SUBROUTINE FOR NOVELL PRINTER CONNECTIONS
'**************************************************************************
SUB NOVELLPC

  retcode%=WinExec("Control",1)            'exec control panel
  SelectMenuItem "Settings"                'bring up settings_printers
  ret%=SleepDelay(1)
  SelectMenuItem "Printers"                  
  ret%=SleepDelay(1)
  ClickButton "Connect"                    'connect button
  ret%=SleepDelay(1)
  ClickButton "Network"


  FOR i = 1 to 3

    SELECT CASE i
        CASE 1
            netprt$ = netprt1
        CASE 2
            netprt$ = netprt2
        CASE 3
            netprt$ = netprt3
    END SELECT            


    SelectListBox "Printer Connections"      'goto correct list box
    ret%=SleepDelay(1)
    SelectListBoxItem i-1,1                  'ensure lptx is selected
    ret%=SleepDelay(1)

    SelectComboBox "Available Queues"

    WPlayKeys(Netprt)                        'enter first network printer

    ClickButton "Connect"
    
    IF WindowExists ("NetWare Warning") THEN    'case if already connected
        ClickButton "OK"
    END IF

  NEXT 

  ClickButton "Close"               'close out of network dialog
  ret%=SleepDelay(1)
  ClickButton "OK"                  'close out of connect dialog
  ret%=SleepDelay(1)
  ClickButton "Close"               'close out of printers dialog
  ret%=SleepDelay(1)
  
  SelectMenuItem "Settings"
  ret%=SleepDelay(1)
  SelectMenuItem "Exit"             'exit out of control panel
         
END SUB          'end of sub NovellPC

'**************************************************************************
' SUB LM21PC - SUBROUTINE FOR LAN MANAGER V2.1 PRINTER CONNECTIONS
'**************************************************************************
SUB LM21PC

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
  
 
  FOR i = 1 to 3                    'time to connect our printers

    SELECT CASE i
        CASE 1
            netprt$ = netprt1
            port$   = "lpt1:"
            netpwd$ = netprtpwd1
        CASE 2
            netprt$ = netprt2
            port$   = "lpt2:"
            netpwd$ = netprtpwd2
        CASE 3
            netprt$ = netprt3
            port$   = "lpt3:"
            netpwd$ = netprtpwd3
    END SELECT            

    WPlayKeys("%d")                        'device name
    ret%=SleepDelay(1)    
    ClickComboBoxItem port$                'pick the correct port
    ret%=SleepDelay(1)
    WPlayKeys("%n")                        'network path
    ret%=SleepDelay(1)
    WPlayKeys(netprt$)                     'print share/queue
    ret%=SleepDelay(1)    
    ClickButton "Connect"                  'connect
    ret%=SleepDelay(5)    

    IF WindowExists ("Enter Network Password") THEN
        WPlayKeys("%p")                             'password field
        WPlayKeys(netpwd)                           'enter password
        ret%=SleepDelay(1)
        ClickButton "OK"
        ret%=SleepDelay(2)
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
' SUB PRINTERC - Main Script
'**************************************************************************
SUB PRINTERC

ViewPort OFF

GetNetInfo

f$ = string$ (64," ")
defaultdialogs=string$(64," ")

retcode% = GetAnyIniVar ("defaultdialogs",defaultdialogs$, "win.ini")

if retcode% = TRUE THEN
   DefaultPC                                      'sub call   

Else

   IF (NetworkType$ = "NETWARE") THEN
      NovellPC

   ELSEIF ((NetworkType$ = "LANMAN") and (NetDriverVersion$ = "2.010")) THEN
      LM21PC
   ELSE
      DefaultPC
   END IF     

END IF


END SUB                          'PRINTERC

'*************************************************************************
'script name:  drvconn.inc
'written by:   jl
'
'rev 3.1
'
'date 06/19/91
'
'description:
'
'This script connects the 3 network drives: 

'Variables Used (winplay.ini)
'
'For (x=1, x <= 3, x++)
'       * NetDrivex             : defines what drive for network connection
'       * NetPathx              : defines network share/volume name
'       * NetDirx               : defines a network Dir for the given share/vol
'
'************************************************************************
'Modification log:
'
'Initials       Date            Modification
'
'########################################################################
' SUBROUTINE DECLARATIONS.....
'########################################################################
DECLARE SUB MAKENETDIR
DECLARE SUB DEFAULTDIALOG
DECLARE SUB NOVELLSPECIFIC
DECLARE SUB LM21SPECIFIC
DECLARE SUB DRVCONN

'((((((((((((((((((((((((((((((((((())))))))))))))))))))))))))))))))))))
'***********************************************************************
' Subroutine MakeNetDir -> create the necessary dirs on net drives
'***********************************************************************
'((((((((((((((((((((((((((((((((((())))))))))))))))))))))))))))))))))))

SUB MAKENETDIR

'********************************************************************
' make the netdir's for each corresponding drive:
'*********************************************************************
'
SelectMenuItem "View"                                    'check view status

IF IsMenuItemChecked ("Tree and Directory") = 0 THEN     'if not checked...
   SelectMenuItem "Tree and Directory"                   'ensure it's checked
END IF

'SelectMenuItem "options"                'pull down options menu
WPlayKeys("%o") 

ret%=SleepDelay(1)

WPlayKeys("c")
'SelectMenuItem "confirmation"           'select confirmation menuitem

ret%=SleepDelay(1) 
SetButtonState "file delete",0          'do not check file delete
SetButtonState "directory delete",0     'do not check directory delete
ClickButton "OK"

For i = 1 to 3
    
    SELECT CASE i
       CASE 1
	  netdrive$ = netdrive1
	  netpath$ =  netdir1
       CASE 2
	  netdrive$ = netdrive2
	  netpath$ =  netdir2
       CASE 3
	  netdrive$ = netdrive3
	  netpath$ =  netdir3
    END SELECT

'    WPlayKeys("^"+NetDrive$)        'focus & read net drive
'    ret%=SleepDelay(3) 
    
'    For ii = 1 to 25                'read the net drive loop
'       SelectMenuItem "disk"       'done when netconnections is not grayed
 
'        IF MenuItemExists ("Network Connections") THEN          'std & novell
'            IF IsMenuItemGrayed ("Network Connections") = 0 THEN    'finished reading drive?
'              EXIT FOR
'            ELSE
'              WPlayKeys("{ESC}{ESC}")        'get out of menu selection
'              ret%=SleepDelay(10) 
'           END IF 
'        ELSE                                                    'lm21
'            IF IsMenuItemGrayed ("Disconnect Network Drive") = 0 THEN    'finished reading drive?
'              EXIT FOR
'           ELSE
'              WPlayKeys("{ESC}{ESC}")        'get out of menu selection
'              ret%=SleepDelay(10) 
'           END IF 
'        END IF
'          
'    NEXT                        'for ii

    SelectMenuItem "File"
    ret%=SleepDelay(1) 
    SelectMenuItem "Delete"
    
    WPlayKeys(netdrive$+":"+netpath$)
    ClickButton "OK"
    ret%=SleepDelay(3) 
    
    IF WindowExists ("error deleting file") <> 0 THEN   'windows exists=no dir...
       ClickButton "OK"
    ELSE
       While WindowExists ("delete") <> 0               'still in delete op
	  IF WindowExists ("confirm file delete") <> 0 THEN
	     ClickButton "yes"
	  END IF
	  ret%=SleepDelay(1)
       WEND
    END IF
	
    SelectMenuItem "file"
    SelectMenuItem "create"
    WPlayKeys(netdrive$+":"+netpath$)       'enter new path
    ret%=SleepDelay(1)
    ClickButton "OK"
    ret%=SleepDelay(1)
    
NEXT                    'for i

'SelectMenuItem "options"                'pull down options menu
WPlayKeys("%o")

ret%=SleepDelay(1)

'SelectMenuItem "confirmation"           'select confirmation menu_item
WPlayKeys("c")

ret%=SleepDelay(1) 
SetButtonState "file delete",1         'check file delete
SetButtonState "directory delete",1    'check directory delete
ClickButton "OK"       

WPlayKeys("^"+NctDrive$)        'focus & read nct drive
ret%=SleepDelay(5) 
SelectMenuItem "file"
ret%=SleepDelay(1)
SelectMenuItem "exit"           'exit FM


END SUB


'((((((((((((((((((((((((((((((((((())))))))))))))))))))))))))))))))))))
'***********************************************************************
' Subroutine DefaultDialog -> for MSNET, default dialogs etc.
'***********************************************************************
'((((((((((((((((((((((((((((((((((())))))))))))))))))))))))))))))))))))

SUB DEFAULTDIALOG

'***********************************************************************
'  Establish Network Drive Connections
'***********************************************************************

retcode%=WinExec("WinFile",1)               'bring up FM
ret%=SleepDelay(2)

For ii = 1 to 25                            'read the net drive loop
    SelectMenuItem "disk"                   'done when netconnections is not grayed
 
    If IsMenuItemGrayed ("Network Connections") = 0 THEN    'finished reading drive?
	EXIT FOR
    ELSE
	WPlayKeys("{ESC}{ESC}")             'get out of menu selection
	ret%=SleepDelay(10) 
    END IF 
 
NEXT                                        'for ii

SelectMenuItem "Disk"
ret%=SleepDelay(1)
SelectMenuItem "Network Connections"
ret%=SleepDelay(1)

FOR i = 1 to 3

    SELECT CASE i
	CASE 1
	   netdrive$ = netdrive1
	CASE 2 
	    netdrive$ = netdrive2
	CASE 3
	    netdrive$ = netdrive3
    END SELECT

    SelectListBox "Current Drive Connections"           'goto list box
    ret%=SleepDelay(1)
    WPlayKeys(netdrive$)                                'enter netdriveltr
    ret%=SleepDelay(1)
    f$ = String$(64," ")                                'init string
    GetListBoxSelection F$, len(f$)                     'get listbox string
'    print "getlistboxselection text = "+f$              'print for debugging
    choice$ = mid$(f$,1,1)                              'get only the drive letter
'    print "choice$ = "+choice$                          'print out for debug

    IF choice$ = ucase$(netdrive) then                  'if it's the drive we want
	clickbutton "disconnect"                        'then disconnect
	ret%=sleepdelay(1)
    END IF

NEXT


WPlayKeys("%N"+NetPath1)        'enter in the network path/share/volume
WPlayKeys("%R"+NetDrive1)       'enter the network Drive letter
WPlayKeys("%W"+NetPwd1)         'enter the password if available
ret%=SleepDelay(2)
WPlayKeys("%C")                 'try to connect
ret%=SleepDelay(2)             'wait for connection to really occur

IF WindowExists ("Connection Root") THEN            'FOR BANYAN 4.1X
  ClickButton "OK"
ENDIF

ret%=SleepDelay(7)
WPlayKeys("%N"+NetPath2)        'enter in the network path/share/volume
WPlayKeys("%R"+NetDrive2)       'enter the network Drive letter
WPlayKeys("%W"+NetPwd2)         'enter the password if available
ret%=SleepDelay(2)
WPlayKeys("%C")                 'try to connect
ret%=SleepDelay(2)             'wait for connection to really occur

IF WindowExists ("Connection Root") THEN
  ClickButton "OK"
ENDIF

ret%=SleepDelay(7)
WPlayKeys("%N"+NetPath3)        'enter in the network path/share/volume
WPlayKeys("%R"+NetDrive3)       'enter the network Drive letter
WPlayKeys("%W"+NetPwd3)         'enter the password if available
ret%=SleepDelay(2)
WPlayKeys("%C")                  'try to connect
ret%=SleepDelay(2)              'wait for connection to really occur

IF WindowExists ("Connection Root") THEN
  ClickButton "OK"
ENDIF


ret%=SleepDelay(7)

WPlayKeys("%l{Esc}")             'close the netconnections dialog box

'********************************************************************
' make the netdir's for each corresponding drive:
'*********************************************************************
'
MakeNetDir                      ' sub call - defined above.....


END SUB           'defaultdialog



'((((((((((((((((((((((((((((((((((())))))))))))))))))))))))))))))))))))
'***********************************************************************
' Subroutine NOVELLSPECIFIC -> for Novell v2.0 drivers for Win 3.1:
'***********************************************************************
'((((((((((((((((((((((((((((((((((())))))))))))))))))))))))))))))))))))


SUB NOVELLSPECIFIC


'***********************************************************************
'  Establish Network Drive Connections
'***********************************************************************

retcode%=WinExec("WinFile",1)            'bring up FM
ret%=SleepDelay(2)

For ii = 1 to 25                'read the net drive loop
    SelectMenuItem "disk"       'done when netconnections is not grayed
 
    If IsMenuItemGrayed ("Network Connections") = 0 THEN    'finished reading drive?
       EXIT FOR
    ELSE
       WPlayKeys("{ESC}{ESC}")        'get out of menu selection
       ret%=SleepDelay(10) 
    END IF 
    
NEXT                        'for ii

SelectMenuItem "Disk"
ret%=SleepDelay(1)
SelectMenuItem "Network Connections"
ret%=SleepDelay(1)

For i = 1 to 3
    
    SELECT CASE i
       CASE 1
	  netdrive$ = netdrive1
	  netpath$   = netpath1
       CASE 2
	  netdrive$ = netdrive2
	  netpath$   = netpath2
       CASE 3
	  netdrive$ = netdrive3
	  netpath$   = netpath3
    END SELECT
     
    SelectListBox "Data drives"

    offset% = getdrvoffset%(netdrive)        'highlight first net drive
    SelectListBoxItem offset%,1
    ret%=SleepDelay(2)      
   

    IF ButtonEnabled ("Map Delete") <> 0 THEN   'if map delete then there is exiting 
      ClickButton "Map Delete"                  'connection - so delete it first!
      ret%=SleepDelay(2)

      IF WindowExists ("netware warning") <> 0 THEN
	  ClickButton "OK"                          'verify deletion
      END IF
      
      ret%=SleepDelay(2)
      SelectListBox "Data drives"               'ensure listbox focus...
    END IF
      
    
    WPlayKeys("{TAB}")
    WPlayKeys(NetPath$)                     'enter in the network path/share/volume    
    ClickButton "Set Root"                  'set entire path as root
    ClickButton "Map"                       'hit the MAP button
    ret%=SleepDelay(2)

NEXT
    
    ClickButton "Close"
	    

'********************************************************************
' make the netdir's for each corresponding drive:
'*********************************************************************
'
MakeNetDir                              'sub call - defined above...


END SUB       'NovellSpecific


'((((((((((((((((((((((((((((((((((())))))))))))))))))))))))))))))))))))
'***********************************************************************
' Subroutine LM21SPECIFIC -> for LANMAN v2.1 drivers for Win 3.1:
'***********************************************************************
'((((((((((((((((((((((((((((((((((())))))))))))))))))))))))))))))))))))


SUB LM21SPECIFIC


'***********************************************************************
'  Establish Network Drive Connections
'***********************************************************************

retcode%=WinExec("WinFile",1)            'bring up FM
ret%=SleepDelay(2)

For ii = 1 to 5                 'read the net drive loop
    SelectMenuItem "disk"       'done when netconnections is not grayed
 
    If IsMenuItemGrayed ("Disconnect Network Drive") = 0 THEN    'finished reading drive?
       EXIT FOR
    ELSE
       WPlayKeys("{ESC}{ESC}")        'get out of menu selection
       ret%=SleepDelay(10) 
    END IF 
    
NEXT                        'for ii

SelectMenuItem "Disk"
ret%=SleepDelay(1)

If IsMenuItemGrayed ("Disconnect Network Drive") = 0 THEN                     'not gray - so disconnect
  SelectMenuItem "Connect Network Drive"              'disconnect existing drives
  ret%=SleepDelay(1)
  WPlayKeys("%d")                               'drive letter combo box
  ret%=SleepDelay(1)
      
  FOR i = 1 to 3
	
    SELECT CASE i
       CASE 1
	 netdrive$ = netdrive1
       CASE 2
	 netdrive$ = netdrive2
       CASE 3
	 netdrive$ = netdrive3
    END SELECT
     

    NetDrvUsed% = ComboBoxItemExists(netdrive+":")
'    print str$(NetDrvUsed)                                      'debugging

    IF NetDrvUsed = 0 THEN                      '0 means it's already connected
       ClickButton "Cancel"
       ret%=SleepDelay(1)
       SelectMenuItem "Disk"
       ret%=SleepDelay(1)
       SelectMenuItem "Disconnect Network Drive"
       ret%=SleepDelay(1)
       WPlayKeys(netdrive)
       ret%=SleepDelay(1)
       ClickButton "OK"
    ELSE
       ClickButton "Cancel"                'since not - cancel out
    END IF

    ret%=SleepDelay(3)    
    SelectMenuItem "Disk"
    ret%=SleepDelay(1)
    
    If ((IsMenuItemGrayed ("Disconnect Network Drive") = 0) AND (i <> 3))  THEN                     'not gray - so disconnect
       SelectMenuItem "Connect Network Drive"              'disconnect existing drives
       ret%=SleepDelay(1)
       WPlayKeys("%d")                               'drive letter combo box
       ret%=SleepDelay(1)
    ELSE    
      WPlayKeys("{ESC}{ESC}")
      EXIT FOR                               'DRIVE LIST EMPTY - NO USE TO GO AROUND AGAIN
    END IF
    
  NEXT

ELSE
  WPlayKeys("{ESC}{ESC}")
END IF
  
FOR i = 1 to 3                   'now connect the shares as defined in .ini

    SELECT CASE i
       CASE 1
	 netdrive$ = netdrive1
	 netpath$  = netpath1
	 netpwd$   = netpwd1
       CASE 2
	 netdrive$ = netdrive2
	 netpath$  = netpath2
	 netpwd$   = netpwd2
       CASE 3
	 netdrive$ = netdrive3
	 netpath$  = netpath3
	 netpwd$   = netpwd3
    END SELECT
     
    SelectMenuItem "Disk"
    ret%=SleepDelay(1)    
    SelectMenuItem "Connect Network Drive"
    ret%=SleepDelay(1)    
    WPlayKeys("%d")                     'drive letter combo box
    WPlayKeys(netdrive)                 'punch in netdrive letter
    ret%=SleepDelay(1)    
    WPlayKeys("%n")                     'network path field
    ret%=SleepDelay(1)    
    WPlayKeys(netpath)                  'put in netpath
    ret%=SleepDelay(1)    
    ClickButton "OK"                    'ok to connect
    ret%=SleepDelay(6)
    
    IF WindowExists ("Lan Manager") THEN     'handle past persistant connect
	ClickButton "Yes"
	ret%=SleepDelay(2)
    END IF

    IF WindowExists ("Enter Network Password") THEN
	WPlayKeys("%p")                             'password field
	WPlayKeys(netpwd)                           'enter password
	ret%=SleepDelay(1)
	ClickButton "OK"
	ret%=SleepDelay(2)
    END IF       

    IF WindowExists ("Lan Manager") THEN
	ClickButton "Yes"
	ret%=SleepDelay(2)
    END IF
    
NEXT
			
'********************************************************************
' make the netdir's for each corresponding drive:
'*********************************************************************
'
MakeNetDir                              'sub call - defined above...


END SUB       'LM21Specific




'##########################################################################
'  SUB DRVCONN - MAIN PROGRAM SCRIPT
'##########################################################################
SUB DRVCONN

VIEWPORT OFF

getnetinfo                                'GET NETWORK INFO

f$ = string$ (64," ")

defaultdialogs=string$(64," ")


retcode% = getanyinivar ("defaultdialogs",defaultdialogs$, "win.ini")

if retcode% = TRUE THEN
   DefaultDialog                                           'sub call   
Else

   IF (NetworkType$ = "NETWARE") THEN
      NovellSpecific

   ELSEIF ((NetworkType$ = "LANMAN") and (NetDriverVersion = "2.010")) THEN
      LM21Specific
   ELSE
      
      DefaultDialog                 'for everything else...including
				    'banyan 4.xx
   END IF     
END IF

END SUB            'DRVCONN

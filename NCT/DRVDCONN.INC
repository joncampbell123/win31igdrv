'*************************************************************************
'script name:  drvdconn.inc
'written by:   jl
'
'rev 3.1
'
'date 06/19/91
'
'description:
'
'This script disconnects the 3 network drives: 

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
DECLARE SUB DELNETDIR
DECLARE SUB DEFAULTDIALOGDC
DECLARE SUB NOVELLSPECIFICDC
DECLARE SUB LM21SPECIFICDC
DECLARE SUB DRVDCONN

'((((((((((((((((((((((((((((((((((())))))))))))))))))))))))))))))))))))
'***********************************************************************
' Subroutine DELNetDir -> create the necessary dirs on net drives
'***********************************************************************
'((((((((((((((((((((((((((((((((((())))))))))))))))))))))))))))))))))))

SUB DELNETDIR

'********************************************************************
' Delete the netdir's for each corresponding drive:
'*********************************************************************
'
retcode%=WinExec("WinFile",1)               'bring up FM
ret%=SleepDelay(5)

For ii = 1 to 25                'read the net drive loop
    SelectMenuItem "disk"       'done when netconnections is not grayed
 
    IF MenuItemExists ("Network Connections") THEN          'std & novell
	IF IsMenuItemGrayed ("Network Connections") = 0 THEN    'finished reading drive?
	   EXIT FOR
	ELSE
	   WPlayKeys("{ESC}{ESC}")        'get out of menu selection
	   ret%=SleepDelay(10) 
	END IF 
    ELSE                                                    'lm21
	IF IsMenuItemGrayed ("Disconnect Network Drive") = 0 THEN    'finished reading drive?
	   EXIT FOR
	ELSE
	   WPlayKeys("{ESC}{ESC}")        'get out of menu selection
	   ret%=SleepDelay(10) 
	END IF 
    END IF
	  
NEXT                        'for ii
      
				      
SelectMenuItem "View"                                    'check view status

IF IsMenuItemChecked ("Tree and Directory") = 0 THEN     'if not checked...
   SelectMenuItem "Tree and Directory"                   'ensure it's checked
END IF


'SelectMenuItem "options"                'pull down options menu
WPlayKeys("%o")
ret%=SleepDelay(1)

'SelectMenuItem "confirmation"           'select confirmation menuitem
WPlayKeys("c")
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
'   
'    For ii = 1 to 25                'read the net drive loop
'       SelectMenuItem "disk"       'done when netconnections is not grayed
' 
'       If IsMenuItemGrayed ("Network Connections") = 0 THEN    'finished reading drive?
'           EXIT FOR
'       ELSE
'           WPlayKeys("{ESC}{ESC}")        'get out of menu selection
'           ret%=SleepDelay(10) 
'       END IF 
    
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


END SUB


'((((((((((((((((((((((((((((((((((())))))))))))))))))))))))))))))))))))
'***********************************************************************
' Subroutine DefaultDialog -> for MSNET, default dialogs etc.
'***********************************************************************
'((((((((((((((((((((((((((((((((((())))))))))))))))))))))))))))))))))))

SUB DEFAULTDIALOGDC

'***********************************************************************
'  Disconnect Network Drive Connections
'***********************************************************************

SelectMenuItem "Disk"
ret%=SleepDelay(1)
SelectMenuItem "Network Connections"
ret%=SleepDelay(1)

FOR i = 1 to 3

    Select case i
	Case 1
	   netdrive$ = netdrive1
	case 2 
	    netdrive$ = netdrive2
	case 3
	    netdrive$ = netdrive3
    end select

    Selectlistbox "Current Drive Connections"           'goto list box
    ret%=SleepDelay(1)
    WPlayKeys(netdrive$)                                'enter netdriveltr
    ret%=SleepDelay(1)

    f$ = string$(64," ")                                'init string

    getlistboxselection F$, len(f$)                     'get listbox string
    choice$ = mid$(f$,1,1)                              'get only the drive letter
  

    IF choice$ = ucase$(netdrive) then                  'if it's the drive we want
	clickbutton "disconnect"                        'then disconnect
	ret%=sleepdelay(1)
    END IF

NEXT

ClickButton "close"

END SUB           'defaultdialog



'((((((((((((((((((((((((((((((((((())))))))))))))))))))))))))))))))))))
'***********************************************************************
' Subroutine NOVELLSPECIFIC -> for Novell v2.0 drivers for Win 3.1:
'***********************************************************************
'((((((((((((((((((((((((((((((((((())))))))))))))))))))))))))))))))))))


SUB NOVELLSPECIFICDC


'***********************************************************************
'  Disconnect Network Drive Connections
'***********************************************************************


SelectMenuItem "Disk"
ret%=SleepDelay(1)
SelectMenuItem "Network Connections"
ret%=SleepDelay(1)

For i = 1 to 3
    
    SELECT CASE i
       CASE 1
	  netdrive$ = netdrive1
       CASE 2
	  netdrive$ = netdrive2
       CASE 3
	  netdrive$ = netdrive3
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
      
NEXT
    
    ClickButton "Close"
	    

END SUB                     'novellspecificdc

'((((((((((((((((((((((((((((((((((())))))))))))))))))))))))))))))))))))
'***********************************************************************
' Subroutine LM21SPECIFICDC -> for LanManager v2.1 drivers for Win 3.1:
'***********************************************************************
'((((((((((((((((((((((((((((((((((())))))))))))))))))))))))))))))))))))


SUB LM21SPECIFICDC


'***********************************************************************
'  Disconnect Network Drive Connections
'***********************************************************************

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

    ret%=SleepDelay(1)    
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




END SUB                         'lm21specificdc




'##########################################################################
'  SUB DRVDCONN -- MAIN PROGRAM SCRIPT
'##########################################################################
SUB DRVDCONN

getnetinfo

DelNetDir

f$ = string$ (64," ")
defaultdialogs=string$(64," ")


retcode% = getanyinivar ("defaultdialogs",defaultdialogs$, "win.ini")

if retcode% = TRUE THEN
   DefaultDialogDC                                           'sub call   
Else

   IF (NetworkType$ = "NETWARE") THEN
      NovellSpecificDC

   ELSEIF ((NetworkType$ = "LANMAN") and (NetDriverVersion = "2.010")) THEN
      LM21SpecificDC
   ELSE
      DefaultDialogDC                   'for everything else
   END IF     

END IF


WPlayKeys("^"+NctDrive$)        'focus & read nct drive
ret%=SleepDelay(5) 
SelectMenuItem "file"
ret%=SleepDelay(1)
SelectMenuItem "exit"           'exit FM



END SUB                           'drvdconn

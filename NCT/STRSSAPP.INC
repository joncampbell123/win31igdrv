'*************************************************************************
'script name:  strssapp.inc
'written by:   jefflo
'
'rev 3.1
'
'initial date 06/18/91
'
'mode(s) of operation:  to be called by strsslp.wtd
'
'description:
'
'This script tests the following functional areas:
'
'       * loads applets
'       * loads file from local drive
'       * saves file to network drive
'       * loads file from network drive

'Applets tested:
'       * Clock                 * NotePad
'       * Calendar              * Cardfile
'       * Write                 

'Variables Used (winplay.ini)
'
'       * NetDrive1             : defines what drive for network connection
'       * NetPath1              : defines network share/volume name
'       * NetDir1               : defines a network Dir for the given share/vol
'
'       * NctDriveLtr           : defines drive letter where NCT files are
'       * NctPath               : defines directory where NCT file are
'
'
'Supplemental Files needed in Nctdir:
'
'These are the supplemental files which are needed to run the tests.  The
'files should be in the NCT directory which is defined by the 2 variables
'NctDriveLtr and NctPath.
'
'       * Nctwrite.wri          :for write
'       * Nctnote.txt           :for notepad
'       * Nctpaint.bmp          :for paint
'       * Nctcal.cal            :for calendar
'       * Nctcard.crd           :for cardfile
'
'************************************************************************
'Modification log:
'
'Name    Date           Modification:
'
'************************************************************************
'
DECLARE SUB STRSSAPP
SUB STRSSAPP
'
'************************************************************************
'                       Applet Test Section
'************************************************************************

'************************************************************************
' Clock
'************************************************************************

Clock                 'bring up minimized instance of clock

'*************************************************************************
' Write 
'*************************************************************************

retcode%=WinExec("Write",1)
ret%=SleepDelay(3)
WPlayKeys("% X")                 'maximize the app

WPlayKeys("%HA")                 'display help_about
ret%=SleepDelay(2)
WPlayKeys("{Enter}")             'close help_about

WPlayKeys("%FO")                 'File.Open from local drive
WPlayKeys("%N"+NctDriveLtr+":"+NctPath+"\nctwrite.wri{Enter}")
ret%=SleepDelay(25)

WPlayKeys("%FA")                 'save the changed file on the net
WPlayKeys( NetDrive1+":"+NetDir1+"\nctwrite.wri{Enter}%Y")
ret%=SleepDelay(20)

WPlayKeys("% R")                 'restore the app to normal window
WPlayKeys("%{F4}")               'close Write 
ret%=SleepDelay(7)                                 

'verify file saved onto the network as nctwrite.wri....
'cannot compare to original file....
retcode%=WinExec("wverify.exe 11 "+NetDrive1+":"+NetDir1+"\nctwrite.wri /l"+NctDriveLtr+":"+NctPath+"\stress.log",3)
ret%=SleepDelay(7)

'*************************************************************************
' Notepad 
'*************************************************************************

retcode%=WinExec("Notepad",1)
ret%=SleepDelay(3)
WPlayKeys("% X")                 'maximize the app

WPlayKeys("%HA")                 'display help_about
ret%=SleepDelay(2)
WPlayKeys("{Enter}")             'close help_about

WPlayKeys("%FO")                 'File.open from local drive
WPlayKeys("%N"+NctDriveLtr+":"+NctPath+"\nctnote.txt{Enter}")
ret%=SleepDelay(20)

WPlayKeys("%FA")                 'save file on the net
WPlayKeys( NetDrive1+":"+NetDir1+"\nctnote.txt{Enter}%Y")
ret%=SleepDelay(15)

WPlayKeys("% R")                 'restore the app to normal window
WPlayKeys("%{F4}")               'close Notepad
ret%=SleepDelay(7)
				 
'verify file saved to network & compare it to original....
retcode%=WinExec("wverify.exe 7 "+NctDriveLtr+":"+NctPath+"\nctnote.txt "+NetDrive1+":"+NetDir1+"\nctnote.txt /l"+NctDriveLtr+":"+NctPath+"\stress.log",3)
ret%=SleepDelay(7)

'*************************************************************************
'  Calendar
'*************************************************************************

retcode%=WinExec("Calendar",1)
ret%=SleepDelay(3)
WPlayKeys("% X")                 'maximize the app

WPlayKeys("%HA")                 'display help_about
ret%=SleepDelay(2)
WPlayKeys("{Enter}")             'close help_about

WPlayKeys("%FO")                 'file.open from local drive
WPlayKeys("%N"+NctDriveLtr+":"+NctPath+"\Nctcal.cal{Enter}")
ret%=SleepDelay(10)
WPlayKeys("%SD")
WPlayKeys("%S")
WPlayKeys("01-06-92{Enter}")     'display calendar for this date
ret%=SleepDelay(5)
'
WPlayKeys("%FA")                 'save file on the net
WPlayKeys( NetDrive1+":"+NetDir1+"\nctcal.cal{Enter}%Y")
ret%=SleepDelay(10)

'
WPlayKeys("% R")                 'restore the app to normal window
WPlayKeys("%{F4}")               'close Calendar 
ret%=SleepDelay(5)
				 
'verify file saved onto the network as nctcal.cal....
'cannot compare to original file....
retcode%=WinExec("wverify.exe 11 "+NetDrive1+":"+NetDir1+"\nctcal.cal /l"+NctDriveLtr+":"+NctPath+"\stress.log",3)
ret%=SleepDelay(5)

'***********************************************************************
' Cardfile 
'***********************************************************************

retcode%=WinExec("Cardfile",1)
ret%=SleepDelay(3)
WPlayKeys("% X")

WPlayKeys("%HA")                 'display help_about
ret%=SleepDelay(2)
WPlayKeys("{Enter}")             'close help_about


WPlayKeys("%FO")                 'file.open from local drive
WPlayKeys("%N"+NctDriveLtr+":"+NctPath+"\nctcard.crd{Enter}")
ret%=SleepDelay(10)
'
WPlayKeys("%FA")                 'save file on the net
WPlayKeys( NetDrive1+":"+NetDir1+"\nctcard.crd{Enter}%Y")
ret%=SleepDelay(10)

WPlayKeys("% R")                 'restore the app to normal window
WPlayKeys("%{F4}")               'close Cardfile 
ret%=SleepDelay(10)
'
'verify file saved to network & compare it to original....
retcode%=WinExec("wverify.exe 7 "+NctDriveLtr+":"+NctPath+"\nctcard.crd "+NetDrive1+":"+NetDir1+"\nctcard.crd /l"+NctDriveLtr+":"+NctPath+"\stress.log",3)
ret%=SleepDelay(5)


'************************************************************************                                 
'open each applet again & load files from the network
'************************************************************************

'*************************************************************************
' Write
'*************************************************************************

retcode%=WinExec("Write",1)
ret%=SleepDelay(3)
WPlayKeys("% X")                 'maximize the app 

WPlayKeys("%FO")                 'file.open from the net
WPlayKeys( NetDrive1+":"+NetDir1+"\nctwrite.wri{Enter}")
ret%=SleepDelay(20)

WPlayKeys("% R")                 'restore the app to normal window
ret%=SleepDelay(3)
WPlayKeys("%{F4}")               'close Write 
ret%=SleepDelay(5)                                 

'*************************************************************************
' Notepad 
'*************************************************************************

retcode%=WinExec("Notepad",1)
ret%=SleepDelay(3)
WPlayKeys("% X")                 'maximize the app 

WPlayKeys("%FO")                 'file.open from the net
WPlayKeys( NetDrive1+":"+NetDir1+"\nctnote.txt{Enter}")
ret%=SleepDelay(10)

WPlayKeys("% R")                 'restore the app to normal window
ret%=SleepDelay(3)
WPlayKeys("%{F4}")               'close notepad 
ret%=SleepDelay(5)                                 

'*************************************************************************
'  Calendar
'*************************************************************************

retcode%=WinExec("Calendar",1)
ret%=SleepDelay(3)
WPlayKeys("% X")                 'maximize the app 

WPlayKeys("%FO")                 'file.open from the net
WPlayKeys( NetDrive1+":"+NetDir1+"\nctcal.cal{Enter}")
ret%=SleepDelay(10)
WPlayKeys("%SD")
WPlayKeys("%S")
WPlayKeys("01-06-92{Enter}")     'display calendar for this date..
ret%=SleepDelay(5)
'
WPlayKeys("% R")                 'restore the app to normal window
ret%=SleepDelay(3)
WPlayKeys("%{F4}")               'close Calendar 
ret%=SleepDelay(5)                                 

'
'***********************************************************************
' Cardfile 
'***********************************************************************

retcode%=WinExec("Cardfile",1)
ret%=SleepDelay(3)
WPlayKeys("% X")                 'maximize the app 

WPlayKeys("%FO")                 'file.open from the net
WPlayKeys( NetDrive1+":"+NetDir1+"\nctcard.crd{Enter}")
ret%=SleepDelay(10)
'

WPlayKeys("% R")                 'restore the app to normal window
ret%=SleepDelay(3)
WPlayKeys("%{F4}")               'close Cardfile 
ret%=SleepDelay(5)                                 

'
'**************************************************************************
'  close clock
'**************************************************************************
'                                         
retcode%=WinExec("Clock",1)
ret%=SleepDelay(5)

WPlayKeys("%{F4}")               'close Clock
ret%=SleepDelay(5)


'**************************************************************************
'  delete network files & network drive connection
'**************************************************************************

retcode%=WinExec("WinFile",1)            'bring up FM
ret%=SleepDelay(5)
WPlayKeys("^"+NctDriveLtr)      'focus & read Nctdrive....

WPlayKeys("%FD%L")               'file.delete
WPlayKeys( NetDrive1+":"+NetDir1+"\nctwrite.wri{Enter}")   'delete write file
ret%=SleepDelay(1)
WPlayKeys("%Y")                  'yes
ret%=SleepDelay(5)
retcode%=WinExec("wverify.exe 9 "+NetDrive1+":"+NetDir1+"\nctwrite.wri /l"+NctDriveLtr+":"+NctPath+"\stress.log",3)
ret%=SleepDelay(5)

WPlayKeys("%FD%L")                 'file.delete
WPlayKeys( NetDrive1+":"+NetDir1+"\nctnote.txt{Enter}")    'delete notepad file
ret%=SleepDelay(1)
WPlayKeys("%Y")                  'yes
ret%=SleepDelay(5)
retcode%=WinExec("wverify.exe 9 "+NetDrive1+":"+NetDir1+"\nctnote.txt /l"+NctDriveLtr+":"+NctPath+"\stress.log",3)
ret%=SleepDelay(5)

WPlayKeys("%FD%L")                 'file.delete
WPlayKeys( NetDrive1+":"+NetDir1+"\nctcal.cal{Enter}")     'delete calendar file
ret%=SleepDelay(1)
WPlayKeys("%Y")                  'yes
ret%=SleepDelay(5)
retcode%=WinExec("wverify.exe 9 "+NetDrive1+":"+NetDir1+"\nctcal.cal /l"+NctDriveLtr+":"+NctPath+"\stress.log",3)
ret%=SleepDelay(5)

WPlayKeys("%FD%L")                 'file.delete
WPlayKeys( NetDrive1+":"+NetDir1+"\nctcard.crd{Enter}")    'delete cardfile file
ret%=SleepDelay(1)
WPlayKeys("%Y")                  'yes
ret%=SleepDelay(5)
retcode%=WinExec("wverify.exe 9 "+NetDrive1+":"+NetDir1+"\nctcard.crd /l"+NctDriveLtr+":"+NctPath+"\stress.log",3)
ret%=SleepDelay(15)


WPlayKeys("%{F4}")               'close FM

retcode%=WinExec("Wverify.exe 3 /l"+NctDriveLtr+":"+NctPath+"\stress.log",3)

ret%=SleepDelay(10)

END SUB                          'strssapp
'**************************************************************************
' The End...
'***************************************************************************

'*************************************************************************
'script name:  clock.inc
'written by:   jl
'
'rev 3.1
'
'initial date written: 08/19/91
'
'description:
'
'This script brings up an instance of clock in minimized
'mode.  When clock is restored, clock always will have
'a title.  The algorithm used should ensure that clock
'will always have a title.  Used WinExec instead of 
'ActivateApp to restore minimized Clock with title.
'
'
'************************************************************************
'Modification log:
'
'Name    Date           Modification:
'
'
'************************************************************************
' START INSTANCE OF CLOCK 
'************************************************************************
DECLARE SUB CLOCK

SUB CLOCK

retcode%=WinExec("Clock",1)             'exec clock
ret%=SleepDelay(4)                   

retcode%=WinExec("Clock",1)             'exec clock
ret%=SleepDelay(4)                   

ret%=SleepDelay(3)
WPlayKeys("% R")                 'Alt_restore just in case clock comes up in
				'   maximized.....restore size.

ret%=SleepDelay(1)

WPlayKeys("%sd")                 'settings_digital - this two sections
				'  should ensure clock has title
WPlayKeys("%sn")                 'settings_no title
WPlayKeys("{esc}")               'esc to toggle mode

WPlayKeys("%sd")                 'settings_digital
WPlayKeys("%sn")                 'settings_no title
WPlayKeys("{esc}")               'esc to toggle mode
ret%=SleepDelay(2)                   '

WPlayKeys("% X")                 'maximize the app
ret%=SleepDelay(3)

WPlayKeys("%SB")                 'display help_about
ret%=SleepDelay(2)
WPlayKeys("{Enter}")             'close help_about

WPlayKeys("% R")                 'restore the app to normal window
ret%=SleepDelay(1)

WPlayKeys("% n")                 'minimize
ret%=SleepDelay(2)

END SUB                       'CLOCK


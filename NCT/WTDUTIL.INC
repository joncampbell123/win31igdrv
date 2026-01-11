'
'NOTES:  * Use chr$(0) for ""
'        * Call Subroutines with multiple params without ()
'             EX: NetConnect "r:", "\\pyrex\user", chr$(0)
'        * ALWAYS do something with function's return value!!! 
'


DECLARE FUNCTION GetActiveWindow LIB "USER" () AS INTEGER

DECLARE FUNCTION NetConnect    (localName$, netName$, passWord$) AS INTEGER
DECLARE FUNCTION NetDisconnect (localName$) AS INTEGER


DECLARE SUB PlayKeys LIB "WATTEVNT.DLL" (k$,l%,f%) 
DECLARE SUB SendKeys (keys$)

DECLARE SUB delay (hundredths&)
DECLARE FUNCTION GetRandom (maxInt%) AS INTEGER

GLOBAL logName$         ' initialized in OpenLog below

'=========================================================================

SUB delay (hundredths&)
   ' sleeps hundredths& of a second
   print "   Snoozing for " + Str$ (hundredths&) + " 1/100 seconds..."
   tStart = Timer
   while Timer - tStart < hundredths&
      ' check it again...
   wend
END SUB

'=========================================================================

FUNCTION GetRandom (maxInt%) AS INTEGER
   ' returns random integer between 0 and maxInt%
   ' NOTE: You should initialize the random number generator before 
   '       using this.  Something like:
   ' Randomize Timer                  ' get seed

   temp% = (maxInt% * ((Rnd * 100000) / 32767)) / 100000   ' (0 .. 1) * maxInt%
   writeLog "random # = " + str$ (temp%)
   getRandom = temp%
END FUNCTION

'=========================================================================

SUB OpenLog (newLog$)
   ' NEED TO CHECK FOR VALID LOG NAMES!!!
   ' This subRoutine initializes the global LogName$

   'initialize global variable
   if newLog$ = "" then
      logName$ = "default.log"
   else
      logName$ = newLog$
   endif
 
   print "LOGFILE = ["; logName$; "]"
   open logName$ For Output As #1
   close #1
   writeLog (" ")
   writeLog ("-- BEGIN LOG FILE [" + logName$ + "]")
   writeLog dateTime$
   rem writeLog ("USER NAME: " + GetUser())
   rem writeLog ("WINDOWS VERSION: " + GetBuild())
END SUB

'=========================================================================

SUB WriteLog (logEntry$) 
   'would be nice to have log file with the build # for the extension

   IF logName$ <> "" THEN          'global var has been initialized
      open logName$ For Append As #1
      print #1, logEntry$    ' this goes to the file
      close #1       'Close the file after writing to save data incase of UAE's       
   ELSE
      print "WRITELOG: ERROR CANNOT OPEN LOGFILE "; logName$
   END IF
END SUB

'=========================================================================

SUB SendKeys( keys$ )              ' I made a simpler version of PlayKeys
  PlayKeys keys$, len(keys$), 0    ' All you give is the keys to send and
END SUB                            ' I fill in the other values

'=========================================================================

FUNCTION NetConnect (localName$, netName$, passWord$) AS INTEGER
   ' returns 0 if successful

   print "   ESTABLISHING NET CONNECTION: " + localName$ + " " + netName$ + " " + passWord$
   print "   CALLING WNETCANCELCONNECTION"
   rc% = WNetCancelConnection (localName$, 0)
   print "   RETURN CODE = " + " " + str$ (rc%)
 
   print "   CALLING WNETADDCONNECTION"
   code% = WNetAddConnection (netName$, passWord$, localName$) 
   print "   RETURN CODE = " + " " + str$ (code%)

   IF (code% <> 0) THEN
     print "   ERROR CONNECTING TO NETWORK SHARE: " + " " + netName$
   END IF
   
   NetConnect = code%
END FUNCTION   

'=========================================================================

FUNCTION NetDisconnect (localName$) AS INTEGER
   ' returns 0 if successful
   print "   DISCONNECTING NET CONNECTION: " + " " + localName$ 
   print "   CALLING WNETCANCELCONNECTION"
   code% = WNetCancelConnection (localName$, 0)
   print "   RETURN CODE = " + " " + str$ (code%)
 
   IF (code% <> 0) THEN
      print "   ERROR DISCONNECTING FROM LOCAL NETWORK SHARE: " + " " + localName$
   END IF
   
   NetDisconnect = code%
END FUNCTION

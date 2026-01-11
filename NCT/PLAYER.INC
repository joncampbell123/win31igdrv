'************* Player.Inc *********************
'** Contains flow control traps and api which will creates a window that
'** has flow control buttons, such as pause and abort

'$IFNDEF PLAYERINC_DONE
'skip over whole file if we have already been included
'$DEFINE PLAYERINC_DONE




DECLARE FUNCTION CreatePlayerControls LIB "INSTRUCT.DLL" (hwParent AS INTEGER, piState AS LONG, szTitle AS STRING) AS INTEGER
DECLARE FUNCTION UseControls(szTitle$) AS INTEGER


FUNCTION UseControls(szTitle$) AS INTEGER
_UseControls = CreatePlayerControls(0, 0, szTitle)
END FUNCTION


TRAP AbortTrap FROM "INSTRUCT.DLL"
 PRINT "Script Aborted"
'Has hct.inc been included yet?
'$IFDEF CTINC_DONE
  HctLog "script aborted"
  HctScriptEnd (RS_NOT_RUN)
'$ENDIF         'IFNDEF CTINC_DONE

  END
END TRAP

TRAP PauseTrap FROM "INSTRUCT.DLL"
   PAUSE "Script Paused by user"
END TRAP


'$ENDIF         'IFNDEF PLAYERINC_DONE

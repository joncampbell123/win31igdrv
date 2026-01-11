GLOBAL ConnectCaps%
GLOBAL PrintingCaps%
GLOBAL DialogCaps%
GLOBAL AdminCaps%
GLOBAL ErrorCaps%
GLOBAL UserCaps%

REM *********************************************
REM *                                           *
REM * Test WNetGetCaps                          *
REM *                                           *
REM *********************************************


DECLARE SUB GetCaps
SUB GetCaps

LOG ""
LOG "1.  Calling WNetGetCaps(WNNC_SPEC_VERSION)"
retcode%=WNetGetCaps(WNNC_SPEC_VERSION)
Major%=retcode% / 256
Minor%=retcode% - (Major% * 256)
Digit1%=Minor% / 16
Digit2%=Minor% - (Digit1% * 16)

LOG ( "   Network Driver Spec Version: "+str$(Major%)+"."+str$(Digit1%)+str$(Digit2%))


LOG ""
LOG "2.  Calling WNetGetCaps(WNNC_NET_TYPE)"
retcode%=WNetGetCaps(WNNC_NET_TYPE)
NetType$="  Network Type: "
SELECT CASE retcode%
    CASE WNNC_NET_NONE
        NetType$=NetType$ + "NONE (?)"
    CASE WNNC_NET_MSNET
        NetType$=NetType$ + "MSNET"
    CASE WNNC_NET_LANMAN
        NetType$=NetType$ + "LANMAN"
    CASE WNNC_NET_NETWARE
        NetType$=NetType$ + "NETWARE"
    CASE WNNC_NET_VINES
        NetType$=NetType$ + "VINES"
    CASE WNNC_NET_10NET
        NetType$=NetType$ + "10NET"
    CASE ELSE
        NetType$=NetType$ + "UNDEFINED!"
END SELECT
LOG NetType$

LOG ""
LOG "3.  Calling WNetGetCaps(WNNC_DRIVER_VERSION)"
retcode%=WNetGetCaps(WNNC_DRIVER_VERSION)
Major%=retcode% / 256
Minor%=retcode% - (Major% * 256)
Digit1%=Minor% / 16
Digit2%=Minor% - (Digit1% * 16)
LOG ( "   Network Driver Version: "+str$(Major%)+"."+str$(Digit1%)+str$(Digit2%))


LOG ""
LOG "4.  Calling WNetGetCaps(""WNNC_USER"")"
UserCaps%=WNetGetCaps(WNNC_USER)
IF (UserCaps% AND WNNC_USR_GetUser) <> 0 THEN
    LOG("   API GetUser IS supported")
ELSE
    LOG("   API GetUser IS NOT supported")
END IF    



REM *********************************************
LOG ""
LOG "5.  Calling WNetGetCaps(WNNC_CONNECTION)"
ConnectCaps%=WNetGetCaps(WNNC_CONNECTION)

IF (ConnectCaps% AND WNNC_CON_AddConnection) <> 0 THEN
    LOG("   API WNetAddConnection IS supported")
ELSE
    LOG("   API WNetAddConnection IS NOT supported")
END IF       

IF (ConnectCaps% AND WNNC_CON_CancelConnection) <> 0 THEN
    LOG("   API WNetCancelConnection IS supported")
ELSE
    LOG("   API WNetCancelConnection IS NOT supported")
END IF    

IF (ConnectCaps% AND WNNC_CON_GetConnections) <> 0 THEN
    LOG("   API WNetGetConnection IS supported")
ELSE
    LOG("   API WNetGetConnection IS NOT supported")
END IF    

IF (ConnectCaps% AND WNNC_CON_AutoConnect) <> 0 THEN
    LOG("   API Automatic MS-DOS Open for Network Connections IS supported")
ELSE
    LOG("   API Automatic MS-DOS Open for Network Connections IS NOT supported")
END IF    

IF (ConnectCaps% AND WNNC_CON_BrowseDialog) <> 0 THEN
    LOG("   API WNetBrowseDialog IS supported")
ELSE
    LOG("   API WNetBrowseDialog IS NOT supported")
END IF    


REM **********************************************
LOG ""
LOG "6.  Calling WNetGetCaps(WNNC_PRINTING)"
PrintingCaps%=WNetGetCaps(WNNC_PRINTING)
IF (PrintingCaps% AND WNNC_PRT_OpenJob) <> 0 THEN
    LOG("   API WNetOpenJob IS supported")
ELSE
    LOG("   API WNetOpenJob IS NOT supported")
END IF    

IF (PrintingCaps% AND WNNC_PRT_CloseJob) <> 0 THEN
    LOG("   API WNetCloseJob IS supported")
ELSE
    LOG("   API WNetCloseJob IS NOT supported")
END IF    

IF (PrintingCaps% AND WNNC_PRT_HoldJob) <> 0 THEN
    LOG("   API WNetHoldJob IS supported")
ELSE
    LOG("   API WNetHoldJob IS NOT supported")
END IF    

IF (PrintingCaps% AND WNNC_PRT_ReleaseJob) <> 0 THEN
    LOG("   API WNetReleaseJob IS supported")
ELSE
    LOG("   API WNetReleaseJob IS NOT supported")
END IF    

IF (PrintingCaps% AND WNNC_PRT_CancelJob) <> 0 THEN
    LOG("   API WNetCancelJob IS supported")
ELSE
    LOG("   API WNetCancelJob IS NOT supported")
END IF    

IF (PrintingCaps% AND WNNC_PRT_SetJobCopies) <> 0 THEN
    LOG("   API WNetSetJobCopies IS supported")
ELSE
    LOG("   API WNetSetJobCopies IS NOT supported")
END IF    

IF (PrintingCaps% AND WNNC_PRT_WatchQueue) <> 0 THEN
    LOG("   API WNetWatchQueue IS supported")
ELSE
    LOG("   API WNetWatchQueue IS NOT supported")
END IF    

IF (PrintingCaps% AND WNNC_PRT_UnwatchQueue) <> 0 THEN
    LOG("   API WNetUnwatchQueue IS supported")
ELSE
    LOG("   API WNetUnwatchQueue IS NOT supported")
END IF 


IF (PrintingCaps% AND WNNC_PRT_LockQueueData) <> 0 THEN
    LOG("   API WNetLockQueueData IS supported")
ELSE
    LOG("   API WNetLockQueueData IS NOT supported")
END IF    

IF (PrintingCaps% AND WNNC_PRT_UnlockQueueData) <> 0 THEN
    LOG("   API WNetUnlockQueueData IS supported")
ELSE
    LOG("   API WNetUnlockQueueData IS NOT supported")
END IF    

IF (PrintingCaps% AND WNNC_PRT_ChangeMsg) <> 0 THEN
    LOG("   Message SP_QUEUEUCHANGED to PrintMan IS supported")
ELSE
    LOG("   Message SP_QUEUEUCHANGED to PrintMan IS NOT supported")
END IF    

IF (PrintingCaps% AND WNNC_PRT_AbortJob) <> 0 THEN
    LOG("   API WNetAbortJob IS supported")
ELSE
    LOG("   API WNetAbortJob IS NOT supported")
END IF    




REM **********************************************
LOG ""
LOG "7.  Calling WNetGetCaps(WNNC_DIALOG)"
DialogCaps%=WNetGetCaps(WNNC_DIALOG)

IF (DialogCaps% AND WNNC_DLG_DeviceMode) <> 0 THEN
    LOG("   API WNetDeviceMode IS supported")
ELSE
    LOG("   API WNetDeviceMode IS NOT supported")
END IF    

IF (DialogCaps% AND WNNC_DLG_BrowseDialog) <> 0 THEN
    LOG("   API WNetBrowseDialog IS supported")
ELSE
    LOG("   API WNetBrowseDialog IS NOT supported")
END IF    

IF (DialogCaps% AND WNNC_DLG_ConnectDialog) <> 0 THEN
    LOG("   API WNetConnectDialog IS supported")
ELSE
    LOG("   API WNetConnectDialog IS NOT supported")
END IF    

IF (DialogCaps% AND WNNC_DLG_DisconnectDialog) <> 0 THEN
    LOG("   API WNetDisconnectDialog IS supported")
ELSE
    LOG("   API WNetDisconnectDialog IS NOT supported")
END IF    

IF (DialogCaps% AND WNNC_DLG_ViewQueueDialog) <> 0 THEN
    LOG("   API WNetViewQueueDialog IS supported")
ELSE
    LOG("   API WNetViewQueueDialog IS NOT supported")
END IF    

IF (DialogCaps% AND WNNC_DLG_PropertyDialog) <> 0 THEN
    LOG("   API WNetPropertyDialog IS supported")
ELSE
    LOG("   API WNetPropertyDialog IS NOT supported")
END IF    

IF (DialogCaps% AND WNNC_DLG_ConnectionDialog) <> 0 THEN
    LOG("   API WNetConnectionDialog IS supported")
ELSE
    LOG("   API WNetConnectionDialog IS NOT supported")
END IF    


REM **********************************************
LOG ""
LOG "8.  Calling WNetGetCaps(WNNC_ADMIN)"
AdminCaps%=WNetGetCaps(WNNC_ADMIN)
IF (AdminCaps% AND WNNC_ADM_GetDirectoryType) <> 0 THEN
    LOG("   API WNetGetDirectoryType IS supported")
ELSE
    LOG("   API WNetGetDirectoryType IS NOT supported")
END IF    

IF (AdminCaps% AND WNNC_ADM_DirectoryNotify)  <> 0 THEN
    LOG("   API WNetDirectoryNotify IS supported")
ELSE
    LOG("   API WNetDirectoryNotify IS NOT supported")
END IF    

IF (AdminCaps% AND WNNC_ADM_LongNames) <> 0 THEN
    LOG("   Long File Names ARE supported")
ELSE
    LOG("   Long File Names ARE NOT supported")
END IF    



REM **********************************************
LOG ""
LOG "9.  Calling WNetGetCaps(WNNC_ERROR)"
ErrorCaps%=WNetGetCaps(WNNC_ERROR)

IF (ErrorCaps% AND WNNC_ERR_GetError) <> 0 THEN
    LOG("   API WNetGetError IS supported")
ELSE
    LOG("   API WNetGetError IS NOT supported")
END IF    

IF (ErrorCaps% AND WNNC_ERR_GetErrorText) <> 0 THEN
    LOG("   API WNetGetErrorText IS supported")
ELSE
    LOG("   API WNetGetErrorText IS NOT supported")
END IF    


END SUB
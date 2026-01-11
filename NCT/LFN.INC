CONST DIRTYPE  = 4
CONST FILETYPE = 1
CONST NONE     = 0

TYPE FINDFILEBUF
    dateCreation   as Integer
    timeCreation   as Integer
    dateLastAccess as Integer
    timeLastAccess as Integer
    dateLastWrite  as Integer
    timeLastWrite  as Integer
    cbFile          as LONG
    cbFileAlloc     as LONG
    attrFile        as integer
    cbList          as LONG
    cchName         as STRING * 1
    achName         as STRING * 260
END TYPE


DECLARE FUNCTION LFNExists (A$) as Integer
FUNCTION LFNExists (A$) as Integer

DIM FindBuf as FINDFILEBUF

    LFNExists=NONE
    LOG "----CHECKING FOR EXISTENCE OF A FILE OR DIRECTORY"
    NumEntries%=1
    hDir%=0
    LOG "    Calling LFNFindFirst("""+A$+""", 0, VARPTR(NumEntries%), VARPTR(hDir%), 311, VARPTR(FindBuf))"
    retcode%=LFNFindFirst(A$ , 1, VARPTR(NumEntries%), VARPTR(hDir%), 311, VARPTR(FindBuf.dateCreation))
    LOG "    dos retcode%   = "+str$(retcode%)

    IF retcode = 0 THEN
        LFNExists=FILETYPE
    END IF
    
    retcode%=LFNFindClose(hDir%)

    LOG "    Calling LFNFindFirst("""+A$+""", 4, VARPTR(NumEntries%), VARPTR(hDir%), 311, VARPTR(FindBuf))"
    retcode%=LFNFindFirst(A$ , 4, VARPTR(NumEntries%), VARPTR(hDir%), 311, VARPTR(FindBuf.dateCreation))
    LOG "    dos retcode%   = "+str$(retcode%)

    IF retcode%=0 THEN
        LFNExists=DIRTYPE
    END IF

    retcode%=LFNFindClose(hDir%)

END FUNCTION

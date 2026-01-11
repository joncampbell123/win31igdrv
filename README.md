Windows 3.1 integration drivers for DOSBox/DOSBox-X.

#Windows 3.1 monochrome display driver

##Compiling
```
CD \DOSBGA31\286
environ
CD display\1plane
nmake clean
nmake
```

##Testing
Edit C:\WINDOWS\SYSTEM.INI

Change display.drv to point to \DOSBGA31\286\DISPLAY\1PLANE\DBIG\_M.DRV


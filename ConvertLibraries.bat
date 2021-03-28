@echo off

title IX-Ray

cd lib

if exist editor.lib (
    del editor.lib
    del editor.exp
)

coff2omf.exe BugTrap.lib BugTrapB.lib -v
coff2omf.exe ETools.lib EToolsB.lib -v
coff2omf.exe LWO.lib LWOB.lib -v
coff2omf.exe DXT.lib DXTB.lib -v

pause

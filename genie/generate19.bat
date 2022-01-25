@echo OFF
CLS
SETLOCAL EnableDelayedExpansion
curl -O https://raw.githubusercontent.com/bkaradzic/bx/master/tools/bin/windows/genie.exe -o genie.exe -s
genie.exe vs2019
pause
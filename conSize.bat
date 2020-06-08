@echo off
REM Based on code from dbenham on Stackoverflow (https://stackoverflow.com/a/13351373/6837151)
:conSize winWidth winHeight bufHeight
mode con: cols=%1 lines=%2
powershell -command "&{$H=get-host;$W=$H.ui.rawui;$B=$W.buffersize;$B.width=%1;$B.height=%3;$W.buffersize=$B;}"

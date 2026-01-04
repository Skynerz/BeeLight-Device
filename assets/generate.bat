@echo off

for %%f in (*.png) do (
    png2lvimgdsc.exe %%f
    move %%~nf.c ../src/ui/res/
)

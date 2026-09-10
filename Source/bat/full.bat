@echo off
echo ===========================================================================
echo HassaAFR Full demonstration
echo ===========================================================================
echo BOUNCING BALL
echo settings: halton, 32x32 tiles, s = 100
echo ===========================================================================
pause
"bin/afrFull" -c config/full_bball.ini -s scenes/bball.aff

echo ===========================================================================
echo PLANET EARTH
echo settings: halton, 32x32 tiles, s = 50
echo ===========================================================================
pause
"bin/afrFull" -c config/full_earth.ini -s scenes/earth.aff

echo ===========================================================================
echo PAWN
echo settings: halton, 32x32 tiles, s = 100
echo ===========================================================================
pause
"bin/afrFull" -c config/full_pawn.ini -s scenes/pawn.aff

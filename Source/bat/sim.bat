@echo off
echo ===========================================================================
echo HassaAFR Simulator demonstration
echo ===========================================================================
echo BOUNCING BALL
echo settings: halton, 32x32 tiles, s = 100, 9000 samples
echo ===========================================================================
pause
"bin/afrSim" -c config/sim_bball.ini -s scenes/bball.aff

echo ===========================================================================
echo PLANET EARTH
echo settings: halton, 32x32 tiles, s = 100, 9000 samples
echo ===========================================================================
pause
"bin/afrSim" -c config/sim_earth.ini -s scenes/earth.aff

echo ===========================================================================
echo PAWN
echo settings: halton, 32x32 tiles, s = 100, 9000 samples
echo ===========================================================================
pause
"bin/afrSim" -c config/sim_pawn.ini -s scenes/pawn.aff

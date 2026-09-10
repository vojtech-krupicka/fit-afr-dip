@echo off
echo ===========================================================================
echo HassaAFR Demo demonstration
echo ===========================================================================
echo BOUNCING BALL - BASIC FRAMELESS
echo settings: random
echo ===========================================================================
pause
"bin/afrDemo" -c config/demo_fr_random.ini -s scenes/bball.aff

echo ===========================================================================
echo BOUNCING BALL - BASIC FRAMELESS
echo settings: halton
echo ===========================================================================
pause
"bin/afrDemo" -c config/demo_fr_halton.ini -s scenes/bball.aff

echo ===========================================================================
echo BOUNCING BALL - GUIDING SAMPLING
echo settings: halton, 08x08 tiles, s = 250
echo ===========================================================================
pause
"bin/afrDemo" -c config/demo_gs_08x08.ini -s scenes/bball.aff

echo ===========================================================================
echo BOUNCING BALL - GUIDING SAMPLING
echo settings: halton, 64x64 tiles, s = 250
echo ===========================================================================
pause
"bin/afrDemo" -c config/demo_gs_64x64.ini -s scenes/bball.aff

echo ===========================================================================
echo BOUNCING BALL - GUIDING SAMPLING
echo settings: halton, 32x32 tiles, s = 250
echo ===========================================================================
pause
"bin/afrDemo" -c config/demo_gs_32x32_0250.ini -s scenes/bball.aff

echo ===========================================================================
echo BOUNCING BALL - GUIDING SAMPLING
echo settings: halton, 32x32 tiles, s = 1000
echo ===========================================================================
pause
"bin/afrDemo" -c config/demo_gs_32x32_1000.ini -s scenes/bball.aff

echo ===========================================================================
echo BOUNCING BALL - RECONSTRUCTION
echo settings: halton, 32x32 tiles, s = 100, 2250 samples
echo ===========================================================================
pause
"bin/afrDemo" -c config/demo_rc_2250.ini -s scenes/bball.aff

echo ===========================================================================
echo BOUNCING BALL - RECONSTRUCTION
echo settings: halton, 32x32 tiles, s = 100, 4500 samples
echo ===========================================================================
pause
"bin/afrDemo" -c config/demo_rc_4500.ini -s scenes/bball.aff

echo ===========================================================================
echo BOUNCING BALL - RECONSTRUCTION
echo settings: halton, 32x32 tiles, s = 100, 9000 samples
echo ===========================================================================
pause
"bin/afrDemo" -c config/demo_rc_9000.ini -s scenes/bball.aff
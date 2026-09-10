#bin/sh
################################################################################
# Skript pro aplikovani nezalomitelnych mezer do obsahu
################################################################################
do_vlna() {
    mv $1".tex" $2".te~"
    vlna -v KkSsVvZzOoUuAaIi -f $1".te~" $1."tex";
}

#slozka s obrázky

#do_vlna "./content/0_template"

do_vlna "xkrupi06"
do_vlna "xkrupi06_print"
do_vlna "./content/1_prolog"
do_vlna "./content/2_fr"
do_vlna "./content/3_afr"
do_vlna "./content/4_app"
do_vlna "./content/5_tests"
do_vlna "./content/6_epilogue"

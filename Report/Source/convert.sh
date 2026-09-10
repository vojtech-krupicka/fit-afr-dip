#bin/sh
################################################################################
# Skript pro pøevod obrázkù do formátu pdf, který je vyu¾íván pøi kompilaci
################################################################################
convert_figs() {
  EXISTS=`ls $FIG/$2 2> /dev/null | wc -c`;

  if [ $EXISTS -eq "0" ]
  then
    echo "converting $1";
    convert $FIG$1 $FIG$2
  else
    echo "file $1 exists"
  fi
}

#slo¾ka s obrázky
EXISTS=0
RET=0
FIG="./fig/"

#convert_figs "abscissa.eps" "abscissa.pdf"

convert_figs "discs_50.eps" "discs_50.pdf"
convert_figs "halton_i_32.eps" "halton_i_32.pdf"    
convert_figs "random_256.eps" "random_256.pdf" 
convert_figs "jittered_256.eps" "jittered_256.pdf" 
convert_figs "tiling_area.eps" "tiling_area.pdf" 
convert_figs "halton_32.eps" "halton_32.pdf"
convert_figs "nrooks_16.eps" "nrooks_16.pdf"     
convert_figs "tiling_prob.eps" "tiling_prob.pdf"

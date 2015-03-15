#!/bin/bash
# PURPOSE: Do OCR to identify the Container Numbers
######
# get infile and outfile
infile="$1"
outfile="$2"
outtxtfile="$3"
fuzz=30
dilate=5

# test that infile provided
[ "$infile" = "" ] && echo "NO INPUT FILE SPECIFIED"
if [ "$infile" = "./shellscript/images/.jpg" ] 
   then
	exit 0
fi

# test that outfile provided
[ "$outfile" = "" ] && echo "NO OUTPUT FILE SPECIFIED"

if [ "$4" = "" ]
    then
   fuzz=5
   else 
   fuzz="$4" 
fi

if [ "$5" = "" ]
    then
   dilate=3 
   else
   dilate="$5"
fi
tmpF1="./shellscript/tmp/Tmp_$$-_1_FillBlack.jpg"
tmpF2="./shellscript/tmp/Tmp_$$-_2_dilate.jpg"
tmpF3="./shellscript/tmp/Tmp_$$-_3_erode.jpg"
tmpF4="./shellscript/tmp/Tmp_$$-_4_composite.jpg"
tmpF5="./shellscript/tmp/Tmp_$$-_5_threshold.jpg"
tmpF6="./shellscript/tmp/Tmp_$$-_6_trim.jpg"
tmpF7="./shellscript/tmp/Tmp_$$-_7_dilate.jpg"
tmpF8="./shellscript/tmp/Tmp_$$-_8_negate.jpg"
tmpF9="./shellscript/tmp/Tmp_$$-_9_trim.jpg"
tmpFa="./shellscript/tmp/Tmp_$$-_a_rotatm10.jpg"
tmpFb="./shellscript/tmp/Tmp_$$-_b_rotatp10.jpg"
tmpTess="./shellscript/tmp/Tmp_$$-_Tess_Out"
tmpTesstxt="./shellscript/tmp/Tmp_$$-_Tess_Out.txt"
tmpTess2="./shellscript/tmp/Tmp_$$-_Tess_Out2.txt"
cropped="./shellscript/tmp/Tmp_$$-_0_cropped.jpg"

#cat $1 > $cropped
convert $1 -crop 1920x1088+1088+1984 $cropped
convert $cropped -fill black -fuzz $fuzz% +opaque "#ffffff" $tmpF1
convert $tmpF1 -morphology Dilate:$dilate Octagon $tmpF2
convert $tmpF2 -morphology erode:20 diamond -clip-mask $tmpF1 $tmpF3
convert $tmpF3 -negate $cropped -compose plus -composite $tmpF4
convert $tmpF4 -threshold 40% $tmpF5
tesseract $tmpF4 $tmpTess
#cat $tmpTesstxt
cat $tmpTesstxt >> $tmpTess2
convert $tmpF5 -morphology Dilate:1 Octagon $tmpF6
convert $tmpF6 -morphology erode:20 diamond -clip-mask $tmpF5 $tmpF7
convert $tmpF7 $tmpF5 -compose plus -composite $tmpF8
convert $tmpF8 -fuzz 80% -trim +repage $tmpF9
convert $tmpF9 -bordercolor white -border 10x10 $2
tesseract $2 $tmpTess
#cat $tmpTesstxt
cat $tmpTesstxt >> $tmpTess2
convert -rotate -10 $2 $tmpFa 
tesseract $tmpFa $tmpTess
#cat $tmpTesstxt
cat $tmpTesstxt >> $tmpTess2
convert -rotate 10 $2 $tmpFb 
tesseract $tmpFb $tmpTess
#cat $tmpTesstxt
cat $tmpTesstxt >> $tmpTess2
cat $tmpTess2 > $outtxtfile
awk 'NF' $tmpTess2 | awk  '{print "DroneIT::" $0}'
#rm $tmpF1 $tmpF2 $tmpF3 $tmpF4 $tmpF5 $tmpF6 $tmpF7 $tmpF8 $tmpF9 $tmpTesstxt $cropped
exit 0

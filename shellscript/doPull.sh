#!/bin/bash

#open ftp://192.168.42.1
#mirror -v --only-newer /internal_000/Bebop_Drone/media/ /home/ubuntu/FTPScript
#output=$(lftp -e "mirror -v --only-newer /internal_000/Bebop_Drone/media/ /home/ubuntu/FTPScript" ftp://192.168.42.1)
#output=$(lftp -e "mirror --only-newer -e" ftp://192.168.42.1)
output=$(lftp -f ./shellscript/doFTP.txt)
while read -r line; do
    #process "$line"
	#echo "$line"
	#echo ${line:19} | awk -F ".jpg" '{print $1}'
	dotjpg=".jpg"
	dottxt=".txt"
	dotojpg="o.jgp"
        onefile="1"
        dotslashimages="./shellscript/images/"
        dotslashoutimages="./shellscript/outimages/"
        imgName=$(echo ${line:19} | awk -F ".jpg" '{print $1}')
	#echo $imgName
	finalImageName=$dotslashimages$imgName$dotjpg
	echo $finalImageName
	./shellscript/doOCR.sh $finalImageName $dotslashoutimages$imgName$dotojpg $dotslashoutimages$imgName$dototxt 30
        ./shellscript/doOCR.sh $finalImageName $dotslashoutimages$imgName$onefile$dotojpg $dotslashoutimages$imgName$onefile$dototxt 50 5
done <<< "$output"	

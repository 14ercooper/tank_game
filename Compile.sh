#!/bin/bash

# Loads all source files in the list below
# Note do not include the cpp extension
sources=(
    "main"
    "Game"
    "Board"
)

# Compile source files
for i in "${sources[@]}"; do
	filename="${i}.cpp"
        mkdir -p "objects/$(dirname "$filename")"
	objname="${i}.o"
        objpos="objects/${objname}"
	lastmod=$(date +%s -r $filename)
	objmod=$(date +%s -r $objpos)
	modtime=$((lastmod - objmod))
	if [ $modtime -gt 0 ]
	then
		echo "Recompiling ${i}"
                objloc="objects/$objname"
		$(g++ -c $filename -o $objloc)
	else
		echo "${i} already latest version"
	fi
done

# Link source files
echo "Linking files into executable"
gccCommand="g++ "
for i in "${sources[@]}"; do
	gccCommand+="objects/${i}.o "
done
gccCommand+="-o out -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network"
$($gccCommand)

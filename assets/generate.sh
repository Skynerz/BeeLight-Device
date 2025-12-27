 #!/bin/bash

TOOL_PATH=`pwd`
TARGET_WIDTH=80

mkdir tmp
cp *.png tmp
cd tmp
#resize width to 160, ratio kept
magick mogrify -resize $TARGET_WIDTH *.png

for f in *.png ; do
    $TOOL_PATH/png2lvimgdsc $f
    mv `basename $f .png`.c ../../src/ui/res/
done

cd ..
rm -rf tmp


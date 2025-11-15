 #!/bin/bash

for f in *.png ; do
    ./png2lvimgdsc $f
    mv `basename $f .png`.c ../src/ui/res/
done


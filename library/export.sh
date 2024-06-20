# export the include directories into `../build/libraries/include`

if ! test -d "../build/library"; then
  echo "Directory not exists."
fi

rm -rf ../build/library/include
mkdir ../build/library/include
mkdir ../build/library/include/Artifex

cp common/include/Artifex/* ../build/library/include/Artifex
cp window/include/Artifex/* ../build/library/include/Artifex
cp renderer/include/Artifex/* ../build/library/include/Artifex
cp mixer/include/Artifex/* ../build/library/include/Artifex
cp camera/include/Artifex/* ../build/library/include/Artifex
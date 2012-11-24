#!/bin/csh

###
## This script creates "Xcode.tar.gz" in the parent directory
###

# remove build products
rm -rf build
rm -rf playmus/build
rm -rf playwave/build

# remove Finder info files
find . -name ".DS_Store" -exec rm "{}" ";"

# remove user project prefs
find . -name "*.perspective*" -exec rm "{}" ";"
find . -name "*.pbxuser*" -exec rm "{}" ";"
find . -name "*.mode*" -exec rm "{}" ";"
find . -name ".svn" -exec rm "{}" ";"

# create the archive
(cd .. && gnutar -zcvf Xcode.tar.gz Xcode)




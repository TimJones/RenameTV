#!/bin/bash

PROJECT=renametv
SOURCE_DIR=`cd ../ && pwd`
VERSION=`cat $SOURCE_DIR/VERSION`
PWD=`pwd`

# Symlink the source dir here for packaging
ln -snf $SOURCE_DIR/ $PROJECT-$VERSION

# Create the orig tarball, excluding the pwd (within the symlink) so we don't
#  get circular references
tar --exclude-vcs --exclude "$PROJECT-$VERSION/${PWD##*/}/*" --exclude "$PROJECT-$VERSION/${PWD##*/}" -czhvf ${PROJECT}_${VERSION}.orig.tar.gz $PROJECT-$VERSION/

# Remove the symlink and extract the tarball here
#   - debuild will follow the symlink and output it's files in the parent of the 
#      source dir, rather than in the pwd as we need
rm $PROJECT-$VERSION
tar -xvf ${PROJECT}_${VERSION}.orig.tar.gz

# Build & sign the source package
cd $PROJECT-$VERSION
debuild -S -sa

# Now clean up
rm -rf $PROJECT-$VERSION/

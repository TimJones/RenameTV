#!/bin/bash

PROJECT=renametv
TARGETS=( lucid maverick natty oneiric )
SOURCE_DIR=`cd ../ && pwd`
VERSION=`cat $SOURCE_DIR/VERSION`
PWD=`pwd`

# Symlink the source dir here for packaging
ln -snf $SOURCE_DIR/ $PROJECT-$VERSION

# Create the orig tarball, excluding the pwd (within the symlink) so we don't
#  get circular references
tar --exclude-vcs --exclude "$PROJECT-$VERSION/debian/*" --exclude "$PROJECT-$VERSION/debian" --exclude "$PROJECT-$VERSION/build/*" --exclude "$PROJECT-$VERSION/build" --exclude "$PROJECT-$VERSION/${PWD##*/}/*" --exclude "$PROJECT-$VERSION/${PWD##*/}" --exclude "$PROJECT-$VERSION/*.pro.user" -czhvf ${PROJECT}_${VERSION}.orig.tar.gz $PROJECT-$VERSION/

# Remove the symlink and extract the tarball here
#   - debuild will follow the symlink and output it's files in the parent of the 
#      source dir, rather than in the pwd as we need
rm $PROJECT-$VERSION
tar -xvf ${PROJECT}_${VERSION}.orig.tar.gz

# Copy debian package folder into the clean source folder
cp -r $SOURCE_DIR/debian $PROJECT-$VERSION/debian

# Create a copy of the debian changelog file for our editing
cp $PROJECT-$VERSION/debian/changelog ./changelog.orig

# Now edit the changelog for each disto and build the source package
for RELEASE in "${TARGETS[@]}"
do
    sed "s|ReLeAsEgOeShErE|$RELEASE|g" changelog.orig > $PROJECT-$VERSION/debian/changelog
    ( cd $PROJECT-$VERSION && debuild -S -sa )
done

# Now clean up
rm changelog.orig
rm -rf $PROJECT-$VERSION

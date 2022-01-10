#!/bin/sh

# ttygif.SlackBuild - Slackware build script for ttygif
# Copyright © 2022 Necib ÇAPAR <necipcapar@gmail.com>

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


PRGNAM=ttygif
VERSION=${VERSION:-1.5.0} # differentiates from the org. SlackBuild script
BUILD=${BUILD:-1}         # differentiates from the org. SlackBuild script
TAG=${TAG:-_nc}           # differentiates from the org. SlackBuild script

if [ -z "$ARCH" ]; then
  case "$( uname -m )" in
    i?86) ARCH=i586 ;;
    arm*) ARCH=arm ;;
       *) ARCH=$( uname -m ) ;;
  esac
fi

CWD=$(pwd)
TMP=${TMP:-/tmp/SBo}
PKG=$TMP/package-$PRGNAM
OUTPUT=${OUTPUT:-/tmp}

if [ "$ARCH" = "i586" ]; then
  SLKCFLAGS="-O2 -march=i586 -mtune=i686"
  LIBDIRSUFFIX=""
elif [ "$ARCH" = "i686" ]; then
  SLKCFLAGS="-O2 -march=i686 -mtune=i686"
  LIBDIRSUFFIX=""
elif [ "$ARCH" = "x86_64" ]; then
  SLKCFLAGS="-O2 -fPIC"
  LIBDIRSUFFIX="64"
else
  SLKCFLAGS="-O2"
  LIBDIRSUFFIX=""
fi

set -e

rm -rf $PKG
mkdir -p $TMP $PKG $OUTPUT
cd $TMP
rm -rf $PRGNAM-$VERSION
tar xvf $CWD/$PRGNAM-$VERSION.tar.gz || tar xvf $CWD/$VERSION.tar.gz 
cd $PRGNAM-$VERSION
chown -R root:root .
find -L . \
 \( -perm 777 -o -perm 775 -o -perm 750 -o -perm 711 -o -perm 555 \
  -o -perm 511 \) -exec chmod 755 {} \; -o \
 \( -perm 666 -o -perm 664 -o -perm 640 -o -perm 600 -o -perm 444 \
  -o -perm 440 -o -perm 400 \) -exec chmod 644 {} \;

make

find $PKG -print0 | xargs -0 file | grep -e "executable" -e "shared object" | grep ELF \
  | cut -f 1 -d : | xargs strip --strip-unneeded 2> /dev/null || true

mkdir -p $PKG/usr/bin
install -D -m0755 $PRGNAM $PKG/usr/bin/$PRGNAM
install -D -m0755 $PRGNAM $PKG/usr/bin/$PRGNAM

mkdir -p $PKG/usr/doc/$PRGNAM-$VERSION
cp -a README.md $PKG/usr/doc/$PRGNAM-$VERSION
cat $CWD/$PRGNAM.SlackBuild > $PKG/usr/doc/$PRGNAM-$VERSION/$PRGNAM.SlackBuild

mkdir -p $PKG/install
cat $CWD/slack-desc > $PKG/install/slack-desc

cd $PKG
/sbin/makepkg -l y -c n $OUTPUT/$PRGNAM-$VERSION-$ARCH-$BUILD$TAG.${PKGTYPE:-tgz}

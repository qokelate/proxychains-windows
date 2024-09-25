#!/bin/bash
# dist.sh
# Copyright (C) 2020 Feng Shun.
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License version 2 as 
#   published by the Free Software Foundation, either version 3 of the
#   License, or (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License version 2 for more details.
#
#   You should have received a copy of the GNU General Public License
#   version 2 along with this program. If not, see
#   <http://www.gnu.org/licenses/>.

set -euo pipefail

if [ -z "$PXCH_VERSION" ]; then
echo "Please execute \`make dist\`." >&2
exit 1
fi

echo Distributing "$PXCH_VERSION"...

if [ "$OSTYPE" = "cygwin" ]; then
CMDOPTION="/c"
DLLPREFIX="cyg"
else
CMDOPTION="//c"
DLLPREFIX="msys-"
fi

if [ "$(uname -m)" = "x86_64" ]; then
ARCH="x64"
else
ARCH="x86"
fi

mkdir -p ../dist/
#rm -rf ../dist/tmp/
mkdir -p ../dist/tmp

# Build for Win32

if [[ "$*" = *--win32* ]]; then

pushd .
cd $(dirname $0)
cmd $CMDOPTION win32_build.bat
popd

zip -FS -j ../dist/proxychains_"$PXCH_VERSION"_win32_x64_debug.zip ../COPYING ../README*.md ../proxychains.conf ../win32_output/proxychains_win32_x64d.exe ../win32_output/proxychains_hook_x64d.dll ../win32_output/proxychains_hook_x86d.dll ../win32_output/proxychains_helper_win32_x64d.exe ../win32_output/proxychains_helper_win32_x86d.exe
zip -FS -j ../dist/proxychains_"$PXCH_VERSION"_win32_x64.zip ../COPYING ../README*.md ../proxychains.conf ../win32_output/proxychains_win32_x64.exe ../win32_output/proxychains_hook_x64.dll ../win32_output/proxychains_hook_x86.dll ../win32_output/proxychains_helper_win32_x64.exe ../win32_output/proxychains_helper_win32_x86.exe
zip -FS -j ../dist/proxychains_"$PXCH_VERSION"_win32_x86_debug.zip ../COPYING ../README*.md ../proxychains.conf ../win32_output/proxychains_win32_x86d.exe ../win32_output/proxychains_hook_x86d.dll ../win32_output/proxychains_helper_win32_x86d.exe
zip -FS -j ../dist/proxychains_"$PXCH_VERSION"_win32_x86.zip ../COPYING ../README*.md ../proxychains.conf ../win32_output/proxychains_win32_x86.exe ../win32_output/proxychains_hook_x86.dll ../win32_output/proxychains_helper_win32_x86.exe

cp ../COPYING ../README*.md ../proxychains.conf ../win32_output/proxychains_win32_x64d.exe ../win32_output/proxychains_hook_x64d.dll ../win32_output/proxychains_hook_x86d.dll ../win32_output/proxychains_helper_win32_x64d.exe ../win32_output/proxychains_helper_win32_x86d.exe ../win32_output/proxychains_win32_x64.exe ../win32_output/proxychains_hook_x64.dll ../win32_output/proxychains_hook_x86.dll ../win32_output/proxychains_helper_win32_x64.exe ../win32_output/proxychains_helper_win32_x86.exe ../win32_output/proxychains_win32_x86d.exe ../win32_output/proxychains_win32_x86.exe ../dist/tmp

if [[ "$*" = *--install* ]]; then
	pushd .
	cd $(dirname $0)
	cmd $CMDOPTION win32_install.bat
	popd
fi

pushd .
cd $(dirname $0)
cmd $CMDOPTION win32_clean.bat
popd

# Some proxychains_helper_x64* is left at this moment
rm -rf ../win32_output/*

fi


if [[ "$*" = *--cygwin* ]]; then

# Build for Cygwin
make release

zip -FS -j ../dist/proxychains_"$PXCH_VERSION"_"$OSTYPE"_"$ARCH".zip ../COPYING ../README*.md ../proxychains.conf proxychains_"$OSTYPE"_"$ARCH".exe "$DLLPREFIX"proxychains_hook_"$ARCH".dll proxychains_helper_"$OSTYPE"_"$ARCH".exe 

cp proxychains_"$OSTYPE"_"$ARCH".exe "$DLLPREFIX"proxychains_hook_"$ARCH".dll proxychains_helper_"$OSTYPE"_"$ARCH".exe ../dist/tmp

if [[ "$*" = *--install* ]]; then
	cp proxychains_"$OSTYPE"_"$ARCH".exe /bin/proxychains.exe
	ln -sf /bin/proxychains.exe /bin/px.exe
	cp "$DLLPREFIX"proxychains_hook_"$ARCH".dll /bin/
	cp proxychains_helper_*.exe /bin/
fi

make debug

zip -FS -j ../dist/proxychains_"$PXCH_VERSION"_"$OSTYPE"_"$ARCH"_debug.zip ../COPYING ../README*.md ../proxychains.conf proxychains_"$OSTYPE"_"$ARCH"d.exe "$DLLPREFIX"proxychains_hook_"$ARCH"d.dll proxychains_helper_"$OSTYPE"_"$ARCH".exe

cp proxychains_"$OSTYPE"_"$ARCH"d.exe "$DLLPREFIX"proxychains_hook_"$ARCH"d.dll proxychains_helper_"$OSTYPE"_"$ARCH"d.exe ../dist/tmp

zip -FS -j ../dist/proxychains_"$PXCH_VERSION"_all.zip ../dist/tmp/*

if [[ "$*" = *--install* ]]; then
	cp proxychains_"$OSTYPE"_"$ARCH"d.exe /bin/proxychainsd.exe
	ln -sf /bin/proxychainsd.exe /bin/pxd.exe
	cp "$DLLPREFIX"proxychains_hook_"$ARCH"d.dll /bin/
	cp proxychains_helper_*.exe /bin/
fi

if [[ "$*" = *--install* ]]; then
	cp proxychains_"$OSTYPE"_"$ARCH"d.exe /bin/proxychainsd.exe
	ln -sf /bin/proxychainsd.exe /bin/pxd.exe
	cp "$DLLPREFIX"proxychains_hook_"$ARCH"d.dll /bin/
	cp proxychains_helper_*.exe /bin/
fi

make clean
rm -rf ../include/remote_func_bin_*.h

fi
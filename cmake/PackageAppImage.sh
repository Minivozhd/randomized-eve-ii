#!/usr/bin/env bash
# Package an already installed /usr tree. Does not upload or alter the application.
set -euo pipefail
appdir=$(realpath "${1:?AppDir path required}")
version=${2:?Version required}
outdir=$(realpath -m "${3:?Output directory required}")
[[ $version =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]
test -x "$appdir/usr/bin/pe2randomizer"
test -f "$appdir/usr/share/pe2randomizer/assets/aya.png"
mkdir -p "$outdir"
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT
cd "$work"
# Fail closed if the upstream continuous build changes. Refresh the checksum
# deliberately when upgrading the packaging tool; GTK is pinned to a commit.
curl -fL --retry 3 https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage -o linuxdeploy.AppImage
echo '8aea8da0f7f7039d2a2cecb14657d752a222a5e1d3825caeef186c82f751cdd1  linuxdeploy.AppImage' | sha256sum -c -
curl -fL --retry 3 https://raw.githubusercontent.com/linuxdeploy/linuxdeploy-plugin-gtk/7a3fbc31a9e5075073ff8790f26effbac5f84453/linuxdeploy-plugin-gtk.sh -o linuxdeploy-plugin-gtk.sh
chmod +x linuxdeploy.AppImage linuxdeploy-plugin-gtk.sh
# Extraction also works on CI hosts without FUSE.
./linuxdeploy.AppImage --appimage-extract >/dev/null
export PATH="$work:$PATH" APPIMAGE_EXTRACT_AND_RUN=1 DEPLOY_GTK_VERSION=3
# Desktop icons must be square; use the application's existing icon artwork.
convert "$appdir/usr/share/pe2randomizer/assets/icon.ico[0]" -resize 256x256 -background none -gravity center -extent 256x256 pe2randomizer.png
# Preserve package copyright notices for the bundled system libraries.
mkdir -p "$appdir/usr/share/doc/pe2randomizer/dependencies"
for copyright in /usr/share/doc/*/copyright; do
    package=$(basename "$(dirname "$copyright")")
    mkdir -p "$appdir/usr/share/doc/pe2randomizer/dependencies/$package"
    cp -L "$copyright" "$appdir/usr/share/doc/pe2randomizer/dependencies/$package/copyright"
done
cp -RL /usr/share/common-licenses "$appdir/usr/share/doc/pe2randomizer/dependencies/"
dpkg-query -W -f='${Package} ${Version}\n' > "$appdir/usr/share/doc/pe2randomizer/dependencies/build-packages.txt"
# linuxdeploy normally leaves fontconfig to the host. Include it so a minimal
# Arch installation can start without installing a second GUI dependency stack.
mkdir -p "$appdir/etc" "$appdir/apprun-hooks"
cp -RL /etc/fonts "$appdir/etc/"
cat > "$appdir/apprun-hooks/fontconfig.sh" <<'HOOK'
export FONTCONFIG_PATH="$APPDIR/etc/fonts"
HOOK
export LDAI_OUTPUT="$outdir/Randomized-Eve-II-v${version}-x86_64.AppImage" LDAI_VERSION="$version"
"$work/squashfs-root/AppRun" --appdir "$appdir" --desktop-file "$appdir/usr/share/applications/pe2randomizer.desktop" --icon-file "$work/pe2randomizer.png" --library /usr/lib/x86_64-linux-gnu/libfontconfig.so.1 --plugin gtk --output appimage
test -s "$LDAI_OUTPUT"

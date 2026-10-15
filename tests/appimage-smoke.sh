#!/usr/bin/env bash
# Run inside Xvfb. The package must work outside the source/install directory.
set -euo pipefail
image=$(realpath "${1:?AppImage required}")
work=$(mktemp -d)
trap 'test -z "${pid:-}" || kill "$pid" 2>/dev/null || true; rm -rf "$work"' EXIT
cd "$work"
chmod +x "$image"
# Exercise the AppImage runtime's FUSE-free execution path, not the raw binary.
APPIMAGE_EXTRACT_AND_RUN=1 "$image" >app.log 2>&1 &
pid=$!
for attempt in {1..20}; do
    kill -0 "$pid" || { cat app.log; exit 1; }
    if xdotool search --onlyvisible --name '^Randomize Eve II$' >/dev/null 2>&1; then
        sleep 2
        kill -0 "$pid"
        if xdotool search --onlyvisible --name '(Error|Warning)' >/dev/null 2>&1; then cat app.log; exit 1; fi
        if grep -Ei 'error while loading|symbol lookup error|Missing randomizer runtime|Failed to load' app.log; then exit 1; fi
        cat app.log
        echo 'AppImage main window opened without missing-resource dialogs.'
        exit 0
    fi
    sleep 1
done
cat app.log
exit 1

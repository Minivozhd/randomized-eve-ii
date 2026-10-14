# Credits and third-party notices

Randomized Eve II is an unofficial fan project by Strog and TheVozhd. It is not
affiliated with or endorsed by the creators or publishers of Parasite Eve II.
The game, characters, artwork, bitmap lettering and game-derived patch resources
belong to their respective rights holders. This repository includes modification
data and artwork; it does not include game disc images, BIOS files or saved games.
Users must supply their own supported original game images.

The repository does not apply a new blanket license to inherited code or assets.
Public availability does not grant rights to the original game's material.

## Libraries

- [wxWidgets](https://www.wxwidgets.org/about/licence/) provides the desktop UI
  under the wxWindows Library Licence.
- [libqrencode](https://github.com/fukuchi/libqrencode) provides QR encoding under
  LGPL-2.1-or-later. Release packages use the shared library.
- Windows and macOS packages include the shared dependencies required by their
  respective builds. Accompanying dependency license notices are included under
  `licenses/`. Linux packages obtain their libraries through the package manager,
  which provides the corresponding copyright and license files.

Dependency sources and build recipes are available from the
[MSYS2 package repository](https://github.com/msys2/MINGW-packages) and
[Homebrew formula repository](https://github.com/Homebrew/homebrew-core).

## Research and documentation

Special thanks to [GabeRealB](https://github.com/GabeRealB) for the
[Parasite Eve II decompilation](https://github.com/GabeRealB/parasite-eve-2-decomp),
an invaluable reference for the game's internals. Its sources and original game
binaries are not bundled here.

Documentation illustrations were composed with an image generation tool and
original project artwork. The main banner uses glyphs from the original game.
Details are recorded in `docs/images/README.md` and `docs/images/features.md`.

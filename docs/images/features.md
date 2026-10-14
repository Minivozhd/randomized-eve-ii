# Feature illustrations

Revised on 14 October 2026 to replace fictional generated objects with actual
project/game resources at the user's request. Both PNGs are 2172 × 724 pixels.
They are documentation compositions, not screenshots or additional game content.

## Current composition

- `feature-randomization.png`: enemies, items, shops, magic, music and seed-based configuration.
- `feature-customization.png`: new armor, alternate player models and optional gameplay modifiers.

ImageMagick composes the sources directly. Pixel assets are enlarged with
nearest-neighbor sampling; their colors, facial features and shapes are not
regenerated. The Stranger model preview is cropped and reduced. Header, footer
and feature labels use the original USA game font and its glyph metrics, as
described in [the banner notes](README.md). The dark plum panels and restrained
grey-green rules follow the approved banner's menu styling.

## Sources

The following paths refer to the development workspace from which this standalone
source edition was prepared. They are provenance records, not build dependencies.
No ROMs, font atlas or extraction tools are required to display these final PNGs.

| Panel | Actual source | Treatment |
| --- | --- | --- |
| Enemies | `blender_pg/patio_stranger_0x13/patio-preview.png` | Preview render of the original Patio Stranger game model; crop and reduction. This is not an emulator screenshot. |
| Items | `pe2randomizer/art/new-weapons-ak-20261006/references/m4a1-inventory.png` | Original retail M4A1 inventory image, enlarged 3×. It represents the weapon/item pool; it is not the parked AK variant. |
| Shops | Original game font | `BP`, the game's shop currency abbreviation; no invented shop art. |
| Magic | Native retail UI atlas, 4 bpp with CLUT `0x3C08` | Twelve original PE icons, UV rectangle `(176,128)` through `(223,191)`, enlarged 5×. Atlas originates from STAGE0.CDF folder 1, texture 3 and CLUT 4, rendered by the existing UI texture inspection tooling. |
| Music | Original game font | `BGM`; no invented in-game music icon. |
| New armor | `pe2randomizer/art/armor-restyle-20261005/approved-game-20261006/assets/015mini.png` | Approved Power Armor inventory image in its final game palette, enlarged 4×, including its separate helmet. This image was AI-assisted during the earlier armor art workflow. |
| Player models | `pe2randomizer/art/male-players-20261007/game/kyle.png` and `rupert.png` | Approved game-ready Kyle and Rupert portrait images, enlarged 5× with their existing transparency preserved. These portraits were AI-assisted during the earlier portrait workflow. |
| Gameplay modifiers | Crop from the initial generated customization illustration | Three abstract option sliders retained as a documentation motif. They are not a claim that the game contains these controls. |

## Generation and revision history

The initial two illustrations were made with the built-in image generation tool,
one call per image. Their prompts requested a simple PS1-style dark plum menu,
thin grey-green borders, cyan headings, ivory pixel labels and muted green
wireframe transitions. The first prompt specified five illustrated categories:
enemies, items, shops, magic and music, with the footer “CHOOSE YOUR OPTIONS.
SHARE YOUR SEED.” The second specified armor, player portraits and three abstract
configuration sliders, with the footer “CUSTOMIZE YOUR LOADOUT AND PLAYSTYLE.”

The user then requested real in-game/project art. The current revision uses
direct compositing rather than asking the generator to redraw those assets.
Only the abstract slider motif from the first generated customization image is
retained. All lettering was replaced with the original game's bitmap glyphs.

Both final panels were visually inspected for readable English labels, source
fidelity, clipping and consistency with the README banner. The imagery does not
advertise boss randomization, debug loadouts or exhaustive gameplay validation.

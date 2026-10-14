# Feature illustrations

Updated on 14 October 2026. The two 2172 × 724 panels use original game resources
and approved randomizer assets, assembled directly with ImageMagick. They are
illustrations of the available features, not gameplay screenshots.

## Sources and treatment

| Element | Source | Treatment |
| --- | --- | --- |
| Enemies | Retail USA STAGE0.CDF, folder 20611, TEX 1 / CLUT 0 | Brute Stranger bestiary image; 128 × 106 crop, excluding the padded white rows, enlarged 3×. |
| Items | Original retail M4A1 inventory image | Enlarged 3× with nearest-neighbor sampling. |
| Shops | Douglas in the screenshot supplied by the user on 14 October 2026 | Hand-traced alpha mask and crop, retaining the original pixels, hat and shotgun; reduced to fit the panel. |
| Magic | Retail USA STAGE0.CDF, folder 1, TEX 3 / CLUT 4 | Original 4 bpp PE icons at UV (176,128), 48 × 64. Each school row uses its own CLUT: 0x3C04, 0x3C05, 0x3C06 and 0x3C07. These preserve the grey frames and colored centers. Zero palette words remain transparent. Enlarged 5×. |
| Music | Original game font | BGM abbreviation. |
| New armor | Approved Power Armor, `015mini.png` | Final game-palette image, including the separate helmet; enlarged 4×. AI-assisted artwork from the earlier approved armor workflow. |
| Player models | Approved game-ready Jodie and Kyle portraits | Native 49 × 62 images, enlarged 5× with existing alpha preserved. AI-assisted artwork from the earlier approved portrait workflow. |
| Gameplay modifiers | Retail USA STAGE0.CDF, folder 43, BG 0 | Original Scavenger Mode background, decoded from MDEC and enlarged from 320 × 240 to 512 × 384 with Lanczos filtering. No generated details. |
| Menu background | Retail USA STAGE0.CDF, folder 1, TEX 3 / CLUT 4 | Native 32 × 32 tile at UV (0,0), 4 bpp, CLUT 0x3C0F. Saved as `menu-texture.png`; tiled at 4× scale and darkened for text contrast. The download page uses the same tile. |

All labels use the original game font and glyph metrics described in the
[banner notes](README.md). Pixel art is enlarged with nearest-neighbor sampling
unless otherwise stated. There was no new AI image generation in this revision.

The menu texture and the two finished panels are self-contained documentation
assets. Building the application does not need the source disc, extraction tools,
font atlas or any other files from the development workspace.

The initial illustrations used generated placeholders. These have now been
replaced with the sources above. Visual review checks the individual assets,
palettes, alpha edges, readable labels and the final two-panel composition.

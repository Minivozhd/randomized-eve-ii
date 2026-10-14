# README banner

`readme-banner.png` is documentation artwork, not a runtime asset or game screenshot.
Revised on 14 October 2026 with the built-in image generation tool, using the
previous banner and an actual Parasite Eve II inventory screenshot as references.
The original application artwork in `assets/aya.png` remains unchanged.

## Original game typography

The final lettering is typeset from the actual USA game font, not generated text.
The 4-bit atlas comes from STAGE0.CDF, folder 1, texture 1. Glyph metrics come
from SLUS_010.42: the large face at 0x8005FA30 and small face at 0x800604B0,
using the fill palette at 0x800609B0. Placement follows the game's glyph offsets,
advance widths and pair kerning. The large title is scaled by 10 and 12 using
nearest-neighbor sampling; the cyan small-face subtitle is scaled by 6.
The atlas and original game files are not included in this documentation folder.

The built-in image generation tool removed the previous lettering to make a
background plate. Only the two text rectangles were taken from that plate;
ImageMagick composited the original bitmap glyphs onto the approved artwork.
The portrait area to the right of x=1360 is pixel-identical to the approved banner.

Text-removal prompt: remove all lettering, fill the title area with the existing
dark plum background and the subtitle with its black interior; preserve borders,
composition, Aya and the green mesh; add nothing.

## Artwork revision prompt

Use case: style-transfer / compositing.
Edit Image 1, the existing Randomized Eve II README banner, following the user's correction. Image 2 is an actual Parasite Eve II inventory screenshot: use ONLY its interface visual language as the style reference (purple-black panels, restrained grey-green outlines, chunky bitmap lettering, cyan section headers, green selection bar). Do not copy the screenshot's gameplay, objects, item lists, emulator overlays or stats.
Keep the wide 3:1 banner and recognizable Aya on the right, title on left. Make everything MUCH less ornate and distinctly like the original PS1 game's menu. Replace all gold tall serif display typography, lavish lighting and decorative biology with simple bold ivory pixel/bitmap-style title lettering, restrained cyan accents, a flat deep plum-black panel background with slight PS1 dithering, simple thin grey-green rectangular border. Exact only text: "RANDOMIZED" above "EVE II"; small subtitle "PARASITE EVE II RANDOMIZER". Readable, measured spacing, substantial negative space. No extra interface labels or fake buttons.
Main creative change: Aya is emerging out of a GREEN LOW-POLYGON WIREFRAME MESH. Show a coherent sparse triangular wireframe version of her shoulder, upper torso and back, morphing seamlessly into her textured recognizable illustration toward her face and hair. The triangular mesh must follow the anatomy and visibly become the solid portrait, not just a flat grid laid over the background. At lower right, most of the shoulder/torso is dark green untextured polygons with thin green wire edges; toward upper face the material resolves into the original warm illustrated Aya, recognizable original face and pose. Add a small receding continuation of that same sparse mesh behind the shoulder, not a huge grid floor. Muted green lines, no intense neon bloom. Simplify and mute the portrait colors so it belongs in the PS1 menu. Keep face clear and appealing, original shoulder anatomy intact.
Overall: understated late-1990s game interface, tidy and dark, plain legible lettering, authentic purple/green/cyan menu palette. NO gold, NO ornamental serif letters, NO grunge texture, NO particles, NO neon cyberpunk, NO complex sci-fi HUD, NO extra characters. Opaque finished banner.

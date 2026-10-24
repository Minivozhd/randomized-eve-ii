========================================
   R A N D O M I Z E D   E V E   II
========================================

Randomized Eve II v1.3.1 - 24.10.2026
For Parasite Eve II (USA / NTSC-U), both original discs.

Randomize encounters, items, shops, magic, and music; customize difficulty,
player models, armor, HUD colors, and optional gameplay modifiers.
See the version history below for additions and changes since v1.2.5.

Downloads: https://minivozhd.github.io/randomized-eve-ii/

Installation:

Windows x64: Extract the WHOLE ZIP into a folder. Keep pe2randomizer.exe,
all DLLs, and the resources folder together. Do not run the EXE inside the ZIP.
Use a folder path without semicolons.

macOS: Extract the ZIP matching your Mac (Apple Silicon or Intel), then move
pe2randomizer.app to Applications. These builds are ad-hoc signed, not notarized.
If macOS blocks the first launch, use System Settings > Privacy & Security >
Open Anyway after attempting to open the app. Only do this for your verified
release download. Runtime libraries are included inside the app.

Linux x64 (Ubuntu 24.04): Install the .deb through apt to resolve dependencies:
  sudo apt install ./Randomized-Eve-II-v1.3.1-linux-x64.deb
Launch Randomized Eve II from the application menu or run pe2randomizer.
Linux x64 (Arch/CachyOS and other current distributions): use the AppImage.
Requires glibc 2.39 or newer. Bundled libraries include wxWidgets/GTK and qrencode.
  chmod +x Randomized-Eve-II-v1.3.1-x86_64.AppImage
  ./Randomized-Eve-II-v1.3.1-x86_64.AppImage
If FUSE is unavailable:
  ./Randomized-Eve-II-v1.3.1-x86_64.AppImage --appimage-extract-and-run
There is currently no Flatpak/Flathub release.

How to use:

1. Install the application for your platform as described above.
2. Open the application. Configure your options or load your saved settings.
3. Choose your own supported original images with Choose Disc 1 and
   Choose Disc 2, then click Randomize.
4. Wait for generation to finish. The generated images are written beside
   the originals with a date/time suffix. Keep the generated CUE with its
   matching BIN; keep the accompanying INI to reproduce the settings/seed.
5. Load the generated CUE in your emulator. Start a new game when testing
   changed starting equipment or character selection. Old emulator savestates
   retain loaded code/data and are not a reliable way to test a new ROM.

This package contains the randomizer, not game discs or a BIOS. It requires
no compiler or development tools. Windows runtime DLLs and resources must stay
beside its EXE. Keep the macOS app bundle intact.

For identical randomized results, use the same release and platform build.
Settings can be shared across platforms, but random distributions can differ.

Discord: https://discord.com/invite/ybyPJYyswW

========================================
     V E R S I O N   H I S T O R Y
========================================

----------------------------------------
     v1.3.1 - 24.10.2026
----------------------------------------

New features:
- Added "Parasite Eve 1 music": replaces gameplay music with 37 adapted
  tracks and variants from the first Parasite Eve, grouped by scene mood.
  The music pack is included; no separate download is required.
- Added "No magic": disables player Parasite Energy and removes its menu
  command and magic wheel.

Improvements:
- Added full character names to character selection and the in-game menu,
  with more space for longer names.
- Renamed "General / commander" to "Lieutenant".
- Restored Pierce's sunglasses in-game and updated his portrait.
  Corrected their draw order to match the original model.

Bug fixes:
- Fixed Magic elements allowing ordinary PE-wheel casts to consume the
  last selected inventory item, including equipped weapons such as Mongoose.
- Unrandomized ammo boxes now preserve their original ammunition regardless
  of drop-pool restrictions. Randomized and Full random boxes still follow
  the allowed pool.

----------------------------------------
     v1.3.0
----------------------------------------

Bug fixes:
- HUD colors are now preserved when saving and loading INI settings.
- Fixed a missing specific enemy initialization callback that could cause a
  black screen.
- Corrected resource addressing, texture/palette loading, and loading order
  for mixed encounters, including Cloak GOLEMs. Fixed several causes of
  corrupted or inactive enemies and room-entry freezes.
- Added model and sound-memory budget checks when choosing encounters,
  including rooms sharing memory with Kyle. Over-budget compositions are
  rejected rather than written into the generated disc.
- Prevented Access Tunnel encounter tables from overwriting adjacent data.
- Fixed uninitialized spawn-grid bounds and reused spawn grids ignoring
  dead zones for a second enemy movement type.
- Fixed GOLEM sound-data copying past the source block boundary.
- Fixed Windows item/enemy/music checkbox masks and Apply behavior, including
  Slug and Riot/Airburst selection errors.
- Corrected drop filtering so disabled items are not reintroduced through
  shop fallback pools or leftover reward-table entries.
- Fixed reward-table addresses and bounds for Bounty/Nightmare and Dryfield.
- Random starting weapons now respect the allowed drop pool. Starting ammo
  must be compatible and enabled; grenade starts no longer force Airburst.
  Explicitly selecting a fixed starting weapon remains an intentional override.
- Ammo boxes respect ammunition restrictions even when their contents are
  not randomized. A box is disabled if no permitted ammunition is available.
- Shop ammo is filtered before selection, avoiding incorrectly empty lists.
- Disabling item randomization now also leaves shops unchanged. Fixed the
  separate Dryfield starting-armor setting and rewards with enemy shuffle off.
- Fixed unidentified armor displaying another armor's name
- Corrected Metabolism, signed armor HP bonuses, Protein Capsule interactions,
  and an invalid HP-restoration path when leaving the shooting range.
- Improved generation error handling and bounds checks.
- Checked launching from paths containing Cyrillic, emoji, spaces, and brackets.
  Avoid semicolons in the extraction path.

New features:

- Added nine male player models: Kyle, Rupert, Douglas, a police officer,
  the General, the President, Baldwin, a soldier, and Pierce.
- Added matching inventory portraits and character names for the selectable
  characters.
- Male characters use Kyle's wound/death voices and compatible combat
  animations. Weapons without a suitable Kyle animation set use coherent
  Aya animations to keep aiming and upper/lower body movement synchronized.
- Added the "Rupert revolver sound" modifier: the Mongoose uses Rupert's
  cutscene gunshot sound with either ammunition type.
- Added the "Kyle P229" modifier: Kyle's unsilenced weapon appearance and
  gunshot sound, while keeping the player's weapon stats, ammunition choices,
  and flashlight secondary action.
- Added a custom Randomized Eve II in-game title screen.
- Reworked all 113 New Armors inventory pictures to better match the game's
  original style. New Armors code was also updated to fix numerous bugs.
- Reorganized Advanced Options into available/active modifiers with descriptions.

----------------------------------------
     v1.2.5
----------------------------------------

Bug fixes:

- Fixed the bug where "No Win Pose" modificator caused the game to crash.
- Fixed the bug where some of the checkboxes were displayed wrong in the Randomizer UI.

New Features:

- The "Inventory during battles" modifier has been added.

----------------------------------------
     v1.2.4
----------------------------------------

Bug fixes:

- Fixed a bug where certain encounters in rooms with 3D water effects could cause the game to freeze.
- Fixed a bug where the secret passage elevator could cause game freezes in certain seeds.
- Fixed a bug where some shelter rooms could spawn inactive golems or brain stingers.
- Fixed a bug that caused a soft lock when playing the shooting range minigame with certain costumes selected.
- Fixed a bug that allowed two of the same weapons to appear in the shooting range under specific conditions.
- Fixed a bug that changed the player's costume upon entering the shooting range when certain costumes were selected.
- The cavern bridge is now restored by default.

New Features:

- The HUD color scheme can now be customized.
- The "Entrance i-frames" modifier has been added, granting Aya 20 frames of invulnerability upon entering any room.
- The "No white screen" modifier has been added to skip the white flash animation when enemies become aggressive.
- The "No win pose" modifier has been added to skip the reload animation at the end of a battle.
- The "No turning" modifier has been added, disabling auto-aim when activated.

----------------------------------------
     v1.2.3
----------------------------------------

Bug fixes:

- Fixed the bug where shop costs in table were shifted by one.
- Fixed the bug where ammo boxes were disabled every time the "Unlock all replay modes" options was checked.
- Fixed the bug where opened magic menu in active time battle mode crashed the game while interacting with certain npcs and objects.
- Updated the disabled ammo box behavior removing storage box property.
- Starting weapon is now added to the shooting gallery weapon options to prevent softlock in minigame.

----------------------------------------
     v1.2.2
----------------------------------------

Bug Fixes:

- Fixed the UI to make the shop cost table finally applicable to the game's shops.
- Fixed the bug where Enemy Health Multiplier and Enemy Damage Multiplier were mixed up in the UI.
- Fixed the bug where the checkboxes for Failed Brute, Failed Stranger, Failed Chaser, Scavenger, and Scorpion were behaving unpredictably.
- Fixed the bug where certain encounters with Kyle after the No. 9 boss fight in Dryfield caused the game to freeze with weird sounds.
- Fixed the bug where the probability for unique items couldn't be set.
- Fixed the bug where Mr. Douglas was spawning instead of the Bounty/Nightmare mode specific encounter in Dryfield Night Main Street, causing a soft lock.
- Fixed the bug where selecting some weapons with attachments as starting weapons did not trigger the randomizer to drop appropriate ammo.
- Corrected some typos in armor descriptions.

New Features:

- Cloak Golems Multi-Encounters Mode: Enables Knight and Bishop GOLEMS to spawn alongside other enemies.

----------------------------------------
     v1.2.1
----------------------------------------

Bug Fixes:

- Fixed the bug due to which Akropolis ammo box could not be disabled.
- Fixed the bug due to which pe energy menu was not accessible in active time battle mode.
- Fixed the bug due to which GPS had HEAVY ARMOR effect instead of MOTION DETECTOR.
- Fixed the bug due to which medicine wheel didn't have DISCOVERY ITEMS status effect.
- Fixed the bug due to which Mr. Douglas couldn't spawn making Aya's texture corrupted.
- Some typos were fixed in the armors descriptions.

----------------------------------------
     v1.2.0
----------------------------------------

Bug Fixes:

- The UI window is now consistently collapsible.
- Fixed the bug where an inactive black Brain Stinger spawned in the shelter.
- Fixed the bug causing some shops to drop empty ammo, which crashed the game.
- Fixed the bug where different enemy and item disabling options were not working.
- Prevented some enemy combinations from spawning on the Night Dryfield balcony, which caused a sound memory overflow and crashed the game when entering Room 6.
- All replay modes are now supported.

Balance Adjustments:

- The cost of slug shotgun ammo has been adjusted (500 BP to 1000 BP).
- Encounter rewards and secret pickups now tend to drop more valuable items (although still fairly random, lol).

New features:

- Introduced enemy damage and health multipliers.
- [Preview] A demo of boss-specific damage and health multipliers (currently works only up to Dryfield No. 9).
- Item shop costs are now manually adjustable in the "Configure Items" menu.
- Music randomizer options have been updated with the full list of tracks available for enabling and disabling.
- Implemented a "One-Time Pickup" mode for ammo boxes.
- Added ammo box randomization.
- Introduced the brand-new "Magic Elements" mode: the PE energy menu is disabled, and all magic is turned into consumables attached to equipment.
- Introduced the brand-new "Active Time Battle" mode: no game pause while opening the PE energy menu during battle.
- Introduced the brand-new "New Armors" mode: 100+ new armors with unique icons, stats, and status effects.
- Added a player model selection feature: choose the player model from default costume presets and newly imported models (the list will be updated over time).

P.S.: consider this release to be beta. New modes were not tested properly so severe bugs are pretty much possible. Waiting for your bugreports!

----------------------------------------
     v1.1.1
----------------------------------------

Bug Fixes:

- Fixed the bug due to which the first ammo box at Akropolis Tower was not disabled when the flag for that is on.
- Fixed the bug causing the empty encounter and bugging Aya’s texture at Main Corridor B2 after brain stinger cut scene.
- Fixed the bug due to which sometimes unpacked stage files were locked causing the randomizer to produce corrupted data and fail.
- Temporarily removed possibility for hybrid encounter in Water Tower Dryflied Day and Water Tower Dryflied Night since they caused random bugs with sounds and glitched doors.
- Spawn zones in motel room 2 and 4 were slightly adjusted, hopefully no out of bounds enemies there.
- Gun Golems and Brain Stingers were removed from randomization pool for B3 Elevator Hall. These enemies presumably caused weird freezes when returning from shelter, but it needs to be tested further.
- Corridor after Eve Access Tunnel and elevator encounter is not randomized anymore. Some enemies broke the script making game freeze or preventing Brain Stinger Boss from spawning.

Balance Adjustments:

- Items drop probability distribution suggested by @ENOVA set as default, let’s try it out!
- PB ammo is disabled by default.
- Ofuda cost has been adjusted (5000 to 27500 BP).

----------------------------------------
     v1.1.0
----------------------------------------

Bug Fixes:

- Fixed the bug that made the snail magazine from the shelf in the wine cellar inaccessible.
- Fixed the bug that caused music randomization to be forced even with the checkbox disabled in the UI.
- Fixed the bug that prevented the game from working on original hardware and some emulators due to magic randomization.
- Fixed the bug that prevented the Ofuda Charm from dropping.
- Fixed the bug that allowed a 1 bullet ammo stack to drop in motel room 6 box sometimes.
- Fixed the bug that prevented items in the sterilization room’s box from being randomized in previous versions.
- Fixed the bug that prevented the Army Track shop in the Underground Parking from being randomized in previous versions.
- Fixed the bug that caused the shop cost to overflow if it exceeded 65535 BP.
- Removed the beta/alpha generator duplicate soundtrack from the randomization pool. This track caused a freeze upon returning from the Access Eve Tunnel (and possibly other places) and from the Neo Ark after defeating the Brain Stinger Boss for some mysterious reason. It sounds just the same as the one in the original game.
- Modified the Cloak Golems' aggro zone so that it will trigger only if the golem has enough space to make a jump attack, preventing it from using a jump attack in cases where Aya was too close to get hit. This may lead to bugs where the Golem doesn’t aggro at all, so it needs to be properly tested.
- Adjusted spawn zones in all motel rooms (day and night), G&R saloon (day and night), warehouse (night), water hole (day and night), gorge, jungle zone, and submarine tunnel to prevent monsters from spawning out of bounds or in unreachable positions.

Balance Adjustments:

- Adjusted the cost of Spartan ammo (from 80 to 500 BP).
- Adjusted the costs of Magnum Ammo and Maeda Magnum Ammo (to 4000 and 9000 BP, respectively).
- Adjusted the cost of Airburst Grenades (from 850 to 800 BP).
- Adjusted the cost of Firefly shells (from 250 to 350 BP).
- Adjusted the cost of R. Slug shells (from 500 to 800 BP).

New Features:

- Completely overhauled the Enemy Randomization system. Now, enemies can spawn in encounters with up to 2 enemy types, including configurations never seen before. Moreover, encounters have become more random in terms of the amount and difficulty.
- Completely overhauled the Item Randomization system. The UI now has a window for configuring the distribution of item classes for dropping in the game. So, the item drop types are not hardcoded anymore but rather drop an item of a certain class, each having its own distribution. However, every class has its own rarity subclassification (common/rare/legendary) with different, still hardcoded percentages, preventing the randomizer from dropping too much powerful stuff. The reward system was also reworked to drop a reward for stronger encounters (at least we try to make it somewhat correlated). Ammo drops for shop randomization were also overhauled to make it more random.
- The Horned Stranger now has a new rare init state - he can spawn paralyzed lying on the floor (as can be seen in Neo Ark in the original game when GOLEMs are spawned).

IMPORTANT: Old randomization state strings are not supported by the new version.

----------------------------------------
     v1.0.2
----------------------------------------

Changelog:

- Hotfix to the shelter reservoir first encounter crash

----------------------------------------
     v1.0.0
----------------------------------------

First public version

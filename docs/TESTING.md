# Testing

## Default suite (no ROM required)

Configure and build once, then run `ctest --preset release --output-on-failure`.
All tests use synthetic data or the compiled release payloads. No Python, downloaded
fixtures, BIOS, game image or emulator is needed. CTest always lists the tests:

```sh
ctest --preset release -N
ctest --preset release -L patches --output-on-failure
ctest --preset release -L mips --output-on-failure
```

| Test | Main contract |
| --- | --- |
| `header_contracts` | Armor constants do not collide with item/config identifiers |
| `core_regressions` | Settings, item pools, starts, shop ammo, rewards, spawn zones, armor catalogue and resource bounds |
| `runtime_paths` | Installation/work/current directories remain independent; real unpacker honors work paths |
| `weapon_variants` | Flags, defaults and serialized settings |
| `title_background` | Title input guards, write bounds and repeat application |
| `player_portraits` | Character coverage, slot limits, checksums and validation before writes |
| `male_players` | Male catalogue, target limits, checksums, overlaps and validation before writes |
| `training_mode` | Training patch structure and supported/synthetic fixture behavior |
| `model_budget` | Model-heap accounting and capacity decisions |
| `enemy_audio` | Sound budgets and exact bounded golem sound-table copies |
| `boss_release_guard` | Reserved prototype/debug settings fail before modifying work |
| `embedded_resources` | Every saved payload fingerprint, binary adapters, unknown keys and write errors |
| `armor_mips` | All 13 previous CPU regression groups, including all 113 armor combinations |

Every CTest entry has a 120-second timeout and `rom-free` plus a functional label.
Tests retain assertions in Release builds. A failing check returns a nonzero status.
The armor interpreter models supported instructions and branch/load delays, with a
20,000-instruction budget and explicit external-function stubs. It is not a full PS1.

The former Python armor CPU suite was ported to C++. Tests of discarded relayout and
asset-generation research utilities were intentionally not copied: those utilities
are not part of this application's implementation. Embedded armor streams are still
validated by `core_regressions`; resource fingerprints cover the shipped bytes.

## Full generation with local discs

Build the opt-in tools:

```sh
cmake --preset core -DPE2_BUILD_INTEGRATION_TESTS=ON
cmake --build --preset core --parallel 4
build/core/tests/pe2_rom_smoke "/path/Disc 1.bin" "/path/Disc 2.bin" "/path/new-output" 53765
```

Append a quoted serialized State string to reproduce a specific profile. A State
contains its own seed and takes precedence over the numeric seed argument. Adjacent
CUE files are required. The output directory must not already exist; the tool copies
both inputs there, configures scratch paths and runs the real delivery pipeline.
It leaves generated images, INI, intermediate files and its log for inspection.
Original inputs are not modified. Allow several GB of free space.

This checks generation, not successful room transitions or combat. For refactors,
compare generated BIN hashes against the previous implementation on the same
platform and with identical State. File names and output timestamps can differ.
Delete disposable output workspaces when comparisons finish; do not archive ROMs
in this source tree.

## Encounter inspection

`pe2_enemy_matrix` is also opt-in. `WORK_ROOT` must contain the extracted game files
expected by the generator. Work only on a disposable copy for preparation modes.

| Mode | Behavior |
| --- | --- |
| `--parameters`, `--rooms`, `--metadata` | Print enemy, room or resource catalogue metadata |
| `--matrix` | Enumerate candidate enemy pairs under current constraints |
| `--exhaustive` | Enumerate distinct slot assignments allowed by the generator |
| `--prepare` | Prepare dummy/cloak resources in the supplied workspace |
| `--prepare-mode0`, `--prepare-mode1` | Prepare full resources for the selected cloak mode |

These commands do not emulate the complete room loader. Run them explicitly;
large enumeration is not part of the default fast suite.

## Additional local fixture checks

After building the corresponding normal CTest targets, the following optional
arguments exercise supplied game data (use extracted files, never a sole original):

```text
pe2_player_portraits STAGE0.CDF
pe2_male_players STAGE0.CDF gameplay STAGE4.CDF STAGE5.CDF STAGE1.CDF STAGE2.CDF STAGE3.CDF
pe2_weapon_variants retail-STAGE0.CDF male-STAGE0.CDF
pe2_title_background input-STAGE0.CDF output-copy.CDF
pe2_training_mode input-gameplay output-copy
pe2_enemy_audio WORK_ROOT
```

Windows target names have an `.exe` suffix. The no-argument executions remain the
ROM-free CTest cases. Do not confuse synthetic coverage with these optional native
fixture checks or with a separate emulator/hardware playthrough.

## Sanitizer run

For Clang/GCC development builds, configure a separate directory:

```sh
cmake -S . -B build/sanitize -G Ninja -DPE2_BUILD_GUI=OFF \
  -DCMAKE_BUILD_TYPE=Debug -DPE2_ENABLE_SANITIZERS=ON -DPE2_WARNINGS_AS_ERRORS=ON
cmake --build build/sanitize --parallel 4
ctest --test-dir build/sanitize --output-on-failure
```

This instruments host code with AddressSanitizer and UndefinedBehaviorSanitizer.
It does not instrument the retail game or replace console testing.

# Patch data and safety contracts

## What is compiled

| Location | Contents |
| --- | --- |
| `src/patches/new_armors/generated.inc` | Final armor MIPS byte records and patch offsets |
| `src/patches/title_screen/` | Final title data and its guarded applicator |
| `src/patches/enemy_audio/catalog.inc` | Saved enemy sound catalogue metadata |
| `src/patches/weapon_variants/` | Weapon-bank fingerprints and applicator |
| `src/resources/ArmorData.cpp` | 232 compressed armor image/palette payloads |
| `src/resources/PlayersData.cpp` | Male model, animation and voice delta payloads |
| `src/resources/PortraitsData.cpp` | Inventory portrait/name delta payloads |
| `src/resources/WeaponVariantsData.cpp` | Rupert and Kyle weapon variant deltas |
| `src/resources/LegacyData.cpp` | Existing costume data, dummy enemy and golem sound table |

The exact catalogue contains **264 payloads**. `embedded-resources.json` records
size and SHA-256 for each original release payload; the embedded C++ bytes preserve
them exactly. The runtime has no dependency on `.pe2mp`, `.pe2pt`, compressed image
files or Python tools outside the executable. Two standalone GUI images are not
ROM patches and remain in `assets/`.

## Applying deltas

Portrait deltas accept only the two known UI slots, exact sizes and expected
pre/post checksums. Male/weapon deltas validate target indexes, bounds, payload
budgets, overlaps and every input/output checksum before the first write.
Validation failures therefore do not partially apply a patch. Actual I/O failures
are reported but are not transactionally rolled back after a write has begun.

Other inherited patch tables include direct fixed-offset writes. Do not describe
them as globally fingerprint-guarded: support is limited to the original USA inputs
understood by the existing generator. The title applicator and weapon bank selection
have their own narrower guards.

## Editing saved results

There is deliberately no patch-generation toolchain in this folder. Patch development
belongs in the separate research workspace. To update a saved result:

1. Produce and validate the machine-code delta or compressed payload there.
2. Update its corresponding C++ data and metadata here.
3. Update `docs/embedded-resources.json` and the test fingerprints for changed payloads.
4. Add a behavioral regression. Updating a checksum alone is not a correctness test.
5. Run CTest and compare full disc outputs when generation behavior should be unchanged.
6. Test affected gameplay and original hardware separately.

Keep instruction timing, MIPS-I constraints, memory budgets, CD layout and shared
resource lifetimes intact. No emulator hooks or expanded console memory are assumed.

Embedding a legacy game-derived binary does not turn it into an original patch.
The legacy data/provenance audit remains open. Public availability does not grant
new rights to inherited material; see [third-party notices](../THIRD_PARTY_NOTICES.md).

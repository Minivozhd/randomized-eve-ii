# Known issues and validation boundaries

This cleanup does not close gameplay bugs merely because the source builds.
The following investigations remain open from the working project:

- **BUG-003:** reports of a black screen when leaving shooting-range levels 2 and
  higher. One armor HP-restoration path was fixed; a full reproduction/save-state
  diagnosis of all remaining reports is still needed.
- **BUG-025:** the Magic elements fix prevents ordinary PE-wheel casts from
  consuming the last selected inventory item, including an equipped Mongoose.
  Native gate/consumption checks pass; the reported gameplay sequence still
  needs an emulator replay. Existing saves do not regain already lost weapons.
- **BUG-026:** a reported crash when two GOLEMs fire in Tower Patio remains
  unverified. The supplied preset has an unsupported CAT2 extension; no matching
  ROM or savestate is available yet.
- **REL-001:** original PlayStation hardware validation and legacy payload provenance
  review are not complete.
- **BUG-016:** Windows installations in paths containing semicolons may fail before
  application startup. Use a path without semicolons.
- Cross-platform seeds preserve settings but may produce different randomized content
  when different standard libraries implement distributions differently.

The retained encounter enumeration tool reports generator candidates and budget
checks. It does not execute the complete retail room loader, render enemies, or
prove combat behavior for all combinations. The extensive loader/emulator research
harness remains in the original workspace, outside this focused source edition.

- **MUSIC-001:** the optional PE1 music mutator is an adapted arrangement.
  It uses the selected 37 variants and an external, guarded USA music patch pack.
  Some quiet parts/automation are reduced, a few samples use half rate, long
  extracted phrases may repeat their intro, and reverb uses PE2's native preset.
  The user confirmed initial gameplay works, then requested a louder mix; the
  new +30% song gain still needs listening. Exhaustive transitions, fades,
  battle/SFX coexistence, CD timing and original PS1
  hardware checks remain pending. The validated music delta is bundled with v1.3.1.
  See `PE1_MUSIC.txt` for activation and installation.

- **MAGIC-001:** the No magic mutator passes preset/preflight tests and
  1,056 native CPU cases, including ordinary consumable preservation. A full
  playthrough, rendered-menu review and original PS1 verification remain open.
  No.9 ambush research is still outside the enabled mutator list; it is not a
  playable room encounter yet.

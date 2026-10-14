# Known issues and validation boundaries

This cleanup does not close gameplay bugs merely because the source builds.
The following investigations remain open from the working project:

- **BUG-003:** reports of a black screen when leaving shooting-range levels 2 and
  higher. One armor HP-restoration path was fixed; a full reproduction/save-state
  diagnosis of all remaining reports is still needed.
- **BUG-024:** a reported inactive enemy in the second Cafeteria fight. Spawn-zone
  caching was corrected, but the exact reported encounter has not been proven fixed.
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

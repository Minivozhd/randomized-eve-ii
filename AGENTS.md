# Source edition maintenance

- This is the source repository for version 1.3.0. Do not change the
  version/release date without an explicit release request.
- Use CMake as the single build system and `pe2_core` for both the GUI and tests.
- Keep patch results in C++; do not add asset generators, decompilation, ROMs,
  saves, machine-specific paths or Python build dependencies.
- Follow `.clang-format`, use `.hpp` headers and includes relative to `src/`.
- Preserve seed serialization and random draw order during structural refactors.
- Run the applicable CTest suite; preserve diagnostics in CI when a test fails.
  Do not close gameplay issues based only on host tests; see `docs/KNOWN_ISSUES.md`.
- Keep the changelog only in README.txt. README.md documents source/build usage.
- Review new assets and patch payloads before publication. Never include ROMs,
  BIOS files, saves, credentials or local research dumps. Local build commands
  must not upload files; publication belongs to the release and Pages workflows.

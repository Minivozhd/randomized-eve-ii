# Architecture

## Boundaries

`pe2_core` contains the delivery coordinator, game-domain code, low-level utilities,
compiled patch data, and GUI-independent settings serialization. The wxWidgets
application links this same library. C++ tests link it too; there is no alternate
implementation of the randomizer for tests.

`src/ui/context/UIContext` is retained at its existing location to keep settings
compatibility review small. It does not depend on wxWidgets. GUI widgets are listed
separately in `cmake/Sources.cmake` and are compiled only for the application target.

## Generation flow

1. The GUI parses settings and passes a delivery configuration to `Manager`.
2. `Manager` rejects unsupported experimental/debug profiles before preparing work.
3. The file and ROM use cases extract and decompress the required game records.
4. Domain randomizers select content and apply bounded data/code changes.
5. Character, portrait, title and weapon patch applicators validate their inputs.
6. Modified records are compressed and written to copies of the supplied discs.
7. The accompanying settings file records the state used for that generation.

The source layout cleanup preserves random-number consumption and seed fields.
Do not reorder random draws as a side effect of renaming or moving code.

## Resource ownership

`pe2::resources::get(key)` returns a `span<const uint8_t>` into static storage.
The span remains valid for the lifetime of the process. Unknown keys throw.
`copy` and `stream` are explicit owned adapters for existing binary readers.
`write` stages payloads only into the configured scratch directory when a downstream
file-based compressor requires them. It does not extract patches beside the EXE.

The catalogue stores payloads once, in five translation units grouped by family.
The explicit source list avoids compiling research or prototype directories by
accident. GUI artwork is the only external resource family.

`utils::paths` separates immutable installation resources from writable work.
The GUI configures both; headless integration tests configure only work. Initialization
happens before workers start. Core code must not change the caller's working directory.

## Scope of this cleanup

- Independent `src`, `tests`, `assets`, `cmake` and `docs` directories.
- Saved patch/asset bytes embedded in C++, with no generator dependency.
- Common formatting, include convention and `.hpp` header suffix.
- Conflicting armor macros replaced by namespace-scoped typed constants.
- One binary checksum/word reader shared by character and weapon deltas.
- Unused geometry duplicate, obsolete headers and disabled alternative room tables removed.
- Unreachable prototype implementations removed, reserved-state guards retained.
- A C++ port of the armor CPU tests and explicit CTest registration.

The game's address tables and existing domain terminology are retained. They are
format contracts, not interchangeable magic numbers. This is not a rewrite of the
game engine or a claim that all inherited algorithms are free of defects.

# Known Issues

Tracking file for unfixed defects and scoped-out work in the engine. Each
entry has a stable ID, the location(s), the impact, and a fix sketch.

**Contract** (borrowed from the fountain2 engine):

- When you fix an issue, delete its entry here and name the ID in the commit
  message (e.g. "Clear KNOWN-ISSUES D1"). Git history is the archive — do not
  accumulate ✅ markers.
- Do not add entries for routine future work that has no user-visible impact;
  keep the bar at "a person reading this file learns something real".
- Code-level `// TODO` comments may stay next to the code; this file is the
  indexed, searchable view.

## D — defects (behavior is wrong or silent today)

### D1: JsonUtil::ReadJsonFile swallows failures

- Location: `Engine/CherrySoda/Util/Json.cpp:10`
- Impact: if the file is missing or fails to parse, the function returns
  void and leaves the caller with an empty / partially-parsed document — no
  log, no error return. Every JSON consumer (Atlas, SpriteBank, PixelFont,
  SpriteData) fails silently on bad paths.
- Fix sketch: return `bool` (or an error string) and `CHERRYSODA_LOG` the
  filename + parse offset on failure; audit callers to handle the failure.

### D2: SpriteData JSON loading has no error checking

- Location: `Engine/CherrySoda/Graphics/SpriteData.cpp:42` (`// TODO: add
  error checking`; see also lines 74, 97)
- Impact: malformed sprite JSON produces default-initialized frames instead
  of an error; typos in node names are invisible.
- Fix sketch: check `HasMember` before every `GetInt/GetFloat/...`, log the
  missing key with the sprite id, and skip the frame on error.

## F — unfinished / scoped-out functionality

### F2: No Texture3D

- Location: `Engine/CherrySoda/Graphics/Texture.h:53`
- Impact: 3D textures / texture arrays unusable (bgfx supports them).
- Fix sketch: extend `Graphics::CreateTexture` paths with bgfx 3D create
  flags and a `Texture::Type3D` loader.

### F5: Command batch file loading

- Location: `Engine/CherrySoda/Util/Commands.cpp:276`
- Impact: `CommandBatches` only works with in-memory lists; cutscene
  scripts cannot be authored as files.
- Fix sketch: read a text file of one-command-per-line into a batch.

### F8: MeshGraphicsComponent template name workaround

- Location: `Engine/CherrySoda/Components/Graphics/MeshGraphicsComponent.h:15,23`
- Impact: mesh state setting is special-cased rather than general purpose.
- Fix sketch: generalize vertex-state setup or document the constraint.

### F9: GUI init reaching into Graphics

- Location: `Engine/CherrySoda/Graphics/Graphics.cpp:516`
- Impact: a GUI concern (font texture setup) lives in Graphics; init order
  is implicit.
- Fix sketch: move to `GUI::Initialize`.

### F10: Emscripten clipboard

- Location: `Engine/CherrySoda/Engine.cpp:148`
- Impact: clipboard operations are stubs on the web build.
- Fix sketch: use Emscripten's clipboard API bridge.

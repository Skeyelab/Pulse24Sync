# Contributing to Pulse24Sync

Thanks for your interest in contributing!

## Getting Started
- Read `ARCHITECTURE.md` to understand the components and data flow.
- Build using scripts in the repo root or via CMake.

## Coding Style
- Prefer explicit, descriptive names (avoid abbreviations).
- Use early returns and handle edge cases first.
- Keep UI bindings in the editor via APVTS attachments.
- Add small helpers to reduce repetition (e.g., parameter sync, label styling).
- Keep parameter IDs and names in `Source/Parameters.h`.

## PR Checklist
- Build succeeds on macOS/Windows/Linux if applicable.
- No unrelated reformatting.
- Update docs when adding parameters or features.
- If changing audio behavior, describe the impact briefly in the PR.

## Testing
- Validate host sync by changing DAW tempo and scrubbing the transport.
- Verify manual BPM works when `syncToHost` is off.
- Check pulse width boundaries (1–50 ms) and velocity mapping (0–127).
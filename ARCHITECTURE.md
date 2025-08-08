# Pulse24Sync Architecture

This document explains the core structure of the plugin, parameters, and engine behavior to aid future contributors (humans and LLMs).

## Overview
- The plugin generates an audible pulse train at 24 PPQN, useful for tempo-sync testing.
- The engine can sync to the host transport via `AudioPlayHead` or run in manual BPM mode.
- UI is intentionally minimal; it binds to parameters and displays a live status line.

## Key Components
- `Source/PluginProcessor.*`: JUCE `AudioProcessor` implementation.
  - Owns the `AudioProcessorValueTreeState` (APVTS) parameters.
  - Bridges host tempo/transport info to the engine.
  - Calls the engine in `processBlock` to render audio pulses.
- `Source/PluginEditor.*`: JUCE editor.
  - Binds controls to APVTS parameters via attachments.
  - Displays status text (enabled, mode, BPM, pulse rate) via a timer.
- `Source/PulseGenerator.*`: Engine that renders pulses.
  - Maintains timing state (sample rate, next-pulse scheduling).
  - Supports host-sync using BPM and PPQ position for robust re-sync.
- `Source/Parameters.h`: Centralizes parameter IDs and human names.

## Parameters (APVTS)
All IDs are defined in `Parameters.h`.
- `enabled` (bool): Master enable.
- `pulseVelocity` (float, 0–127): Loudness; mapped to linear gain [0..1].
- `pulseWidth` (float, 1–50 ms): Pulse duration in milliseconds.
- `syncToHost` (bool): When true, engine follows host BPM/transport.
- `manualBPM` (float, 60–200): Used when not syncing to host.

## Audio Flow
1. `prepareToPlay` → engine `prepare(sampleRate)` and initial `syncParametersToEngine()`.
2. `processBlock` per buffer:
   - Clear buffer (plugin generates sound, does not pass-through input).
   - `syncParametersToEngine()` reads all APVTS values and updates engine.
   - Host state read via `getPlayHead()->getPosition()` to set BPM, playing, seconds, PPQ.
   - `pulseGenerator.process(numSamples, sampleRate, buffer)` writes the pulse audio.

## Engine Timing
- Pulse rate: `(BPM / 60) * 24` pulses per second.
- Schedules pulses using sample-domain counters (`pulseInterval`, `nextPulseTime`).
- Detects tempo or transport jumps via BPM deltas and PPQ discontinuities; resyncs accordingly.
- Pulse waveform: 1 kHz sine with short attack and exponential decay envelope.

## UI
- Controls bind to APVTS using attachments, so no manual sync needed.
- A timer updates the status label ~10 Hz to reflect engine state.

## Conventions
- Keep parameter IDs stable once released.
- Add new parameter IDs to `Parameters.h` and reference them across code.
- Prefer single-purpose helpers (e.g., `syncParametersToEngine()`) to avoid duplication.

## Extension Ideas
- Add MIDI clock output.
- Add different pulse waveforms or shapes.
- Expose subdivision (e.g., 48 PPQN) as a parameter.
- Optional metering or scope view.
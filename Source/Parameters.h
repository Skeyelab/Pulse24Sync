#pragma once

namespace PluginParams {
    // Parameter IDs (must be stable across releases)
    inline constexpr const char* enabled       = "enabled";
    inline constexpr const char* pulseVelocity = "pulseVelocity";
    inline constexpr const char* pulseWidth    = "pulseWidth";
    inline constexpr const char* syncToHost    = "syncToHost";
    inline constexpr const char* manualBPM     = "manualBPM";

    // Human-readable names
    inline constexpr const char* name_enabled       = "Enabled";
    inline constexpr const char* name_pulseVelocity = "Pulse Velocity";
    inline constexpr const char* name_pulseWidth    = "Pulse Width";
    inline constexpr const char* name_syncToHost    = "Sync to Host";
    inline constexpr const char* name_manualBPM     = "Manual BPM";
}
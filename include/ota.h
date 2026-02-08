#pragma once

/// URL to OTA manifest JSON (production only)
inline constexpr const char* manifest_url = "https://cipi26.github.io/automatic-desk-lighting/pages/manifest.json";

namespace ota {
  /**
   * Fetches and displays manifest.json data on Serial
   * Production only
   */
  void tick();
}
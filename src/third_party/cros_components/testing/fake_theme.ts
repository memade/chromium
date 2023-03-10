/**
 * @license
 * Copyright 2022 Google LLC
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @fileoverview Provides fake ChromeOS M3 themes, for the purpose of testing.
 */

import {css} from 'lit';

/** A fake ChromeOS theme for testing. */
export function getThemeForTesting() {
  return css`
  * {
    --cros-ref-primary0: #000000ff;
    --cros-ref-primary10: #00174cff;
    --cros-ref-primary20: #022978ff;
    --cros-ref-primary30: #244290ff;
    --cros-ref-primary40: #3f5aa9ff;
    --cros-ref-primary50: #5973c4ff;
    --cros-ref-primary60: #738de0ff;
    --cros-ref-primary70: #8ea8fdff;
    --cros-ref-primary80: #b4c5ffff;
    --cros-ref-primary90: #dbe1ffff;
    --cros-ref-primary95: #eff0ffff;
    --cros-ref-primary99: #fefbffff;
    --cros-ref-primary100: #ffffffff;
    --cros-ref-secondary0: #161b2cff;
    --cros-ref-secondary10: #212637ff;
    --cros-ref-secondary20: #2b3042ff;
    --cros-ref-secondary30: #414659ff;
    --cros-ref-secondary40: #595e72ff;
    --cros-ref-secondary50: #71768bff;
    --cros-ref-secondary60: #8b90a5ff;
    --cros-ref-secondary70: #a6aac1ff;
    --cros-ref-secondary80: #c1c5ddff;
    --cros-ref-secondary90: #dde1f9ff;
    --cros-ref-secondary95: #eff0ffff;
    --cros-ref-secondary99: #fefbffff;
    --cros-ref-secondary100: #ffffffff;
    --cros-ref-tertiary0: #161b2cff;
    --cros-ref-tertiary10: #212637ff;
    --cros-ref-tertiary20: #2b3042ff;
    --cros-ref-tertiary30: #414659ff;
    --cros-ref-tertiary40: #595e72ff;
    --cros-ref-tertiary50: #71768bff;
    --cros-ref-tertiary60: #8b90a5ff;
    --cros-ref-tertiary70: #a6aac1ff;
    --cros-ref-tertiary80: #c1c5ddff;
    --cros-ref-tertiary90: #dde1f9ff;
    --cros-ref-tertiary95: #eff0ffff;
    --cros-ref-tertiary99: #fefbffff;
    --cros-ref-tertiary100: #ffffffff;
    --cros-ref-error0: #000000ff;
    --cros-ref-error10: #410002ff;
    --cros-ref-error20: #690005ff;
    --cros-ref-error30: #93000aff;
    --cros-ref-error40: #ba1a1aff;
    --cros-ref-error50: #de3730ff;
    --cros-ref-error60: #ff5449ff;
    --cros-ref-error70: #ff897dff;
    --cros-ref-error80: #ffb4abff;
    --cros-ref-error90: #ffdad6ff;
    --cros-ref-error95: #ffedeaff;
    --cros-ref-error99: #fffbffff;
    --cros-ref-error100: #ffffffff;
    --cros-ref-neutral0: #000000ff;
    --cros-ref-neutral10: #1b1b1fff;
    --cros-ref-neutral20: #303034ff;
    --cros-ref-neutral30: #46464aff;
    --cros-ref-neutral40: #5e5e62ff;
    --cros-ref-neutral50: #77767aff;
    --cros-ref-neutral60: #919094ff;
    --cros-ref-neutral70: #acaaafff;
    --cros-ref-neutral80: #c7c6caff;
    --cros-ref-neutral90: #e4e2e6ff;
    --cros-ref-neutral95: #f2f0f4ff;
    --cros-ref-neutral99: #fefbffff;
    --cros-ref-neutral100: #ffffffff;
    --cros-ref-neutralvariant0: #000000ff;
    --cros-ref-neutralvariant10: #191b23ff;
    --cros-ref-neutralvariant20: #2e3038ff;
    --cros-ref-neutralvariant30: #45464fff;
    --cros-ref-neutralvariant40: #5d5e67ff;
    --cros-ref-neutralvariant50: #757680ff;
    --cros-ref-neutralvariant60: #8f909aff;
    --cros-ref-neutralvariant70: #aaaab4ff;
    --cros-ref-neutralvariant80: #c6c6d0ff;
    --cros-ref-neutralvariant90: #e2e1ecff;
    --cros-ref-neutralvariant95: #f0f0faff;
    --cros-ref-neutralvariant99: #fefbffff;
    --cros-ref-neutralvariant100: #ffffffff;
    --cros-ref-red0: #000000ff;
    --cros-ref-red10: #400200ff;
    --cros-ref-red20: #670400ff;
    --cros-ref-red30: #900d03ff;
    --cros-ref-red40: #b32a19ff;
    --cros-ref-red50: #d6432fff;
    --cros-ref-red60: #f95c45ff;
    --cros-ref-red70: #ff8a76ff;
    --cros-ref-red80: #ffb4a7ff;
    --cros-ref-red90: #ffdad4ff;
    --cros-ref-red95: #ffedeaff;
    --cros-ref-red99: #fffbffff;
    --cros-ref-red100: #ffffffff;
    --cros-ref-yellow0: #000000ff;
    --cros-ref-yellow10: #2b1700ff;
    --cros-ref-yellow20: #472a00ff;
    --cros-ref-yellow30: #663e00ff;
    --cros-ref-yellow40: #865300ff;
    --cros-ref-yellow50: #a86900ff;
    --cros-ref-yellow60: #cb8000ff;
    --cros-ref-yellow70: #ef9800ff;
    --cros-ref-yellow80: #ffb961ff;
    --cros-ref-yellow90: #ffddb8ff;
    --cros-ref-yellow95: #ffeedeff;
    --cros-ref-yellow99: #fffbffff;
    --cros-ref-yellow100: #ffffffff;
    --cros-ref-blue0: #000000ff;
    --cros-ref-blue10: #00174cff;
    --cros-ref-blue20: #022978ff;
    --cros-ref-blue30: #244290ff;
    --cros-ref-blue40: #3f5aa9ff;
    --cros-ref-blue50: #5973c4ff;
    --cros-ref-blue60: #738de0ff;
    --cros-ref-blue70: #8ea8fdff;
    --cros-ref-blue80: #b4c5ffff;
    --cros-ref-blue90: #dbe1ffff;
    --cros-ref-blue95: #eff0ffff;
    --cros-ref-blue99: #fefbffff;
    --cros-ref-blue100: #ffffffff;
    --cros-ref-green0: #000000ff;
    --cros-ref-green10: #092100ff;
    --cros-ref-green20: #153800ff;
    --cros-ref-green30: #215100ff;
    --cros-ref-green40: #2e6c00ff;
    --cros-ref-green50: #428713ff;
    --cros-ref-green60: #5ba22fff;
    --cros-ref-green70: #74bd47ff;
    --cros-ref-green80: #8eda60ff;
    --cros-ref-green90: #a9f779ff;
    --cros-ref-green95: #ceffacff;
    --cros-ref-green99: #f8ffecff;
    --cros-ref-green100: #ffffffff;
    --cros-sys-opacity-disabled: 0.38;
  }

  @media (prefers-color-scheme: light) {
    * {
      --cros-sys-primary-light: #3f5aa9ff;
      --cros-sys-primary-dark: #b4c5ffff;
      --cros-sys-primary: #3f5aa9ff;
      --cros-sys-inverse_primary: #b4c5ffff;
      --cros-sys-on_primary: #ffffffff;
      --cros-sys-primary_container: #dbe1ffff;
      --cros-sys-on_primary_container: #00174cff;
      --cros-sys-secondary-light: #595e72ff;
      --cros-sys-secondary-dark: #c1c5ddff;
      --cros-sys-secondary: #595e72ff;
      --cros-sys-on_secondary: #ffffffff;
      --cros-sys-secondary_container: #dde1f9ff;
      --cros-sys-on_secondary_container: #212637ff;
      --cros-sys-tertiary: #595e72ff;
      --cros-sys-on_tertiary: #ffffffff;
      --cros-sys-tertiary_container: #dde1f9ff;
      --cros-sys-on_tertiary_container: #212637ff;
      --cros-sys-error: #d6432fff;
      --cros-sys-on_error: #ffffffff;
      --cros-sys-error_container: #ffdad4ff;
      --cros-sys-on_error_container: #900d03ff;
      --cros-sys-surface_variant: #e2e1ecff;
      --cros-sys-on_surface_variant: #45464fff;
      --cros-sys-outline: #757680ff;
      --cros-sys-separator: #1b1b1f23;
      --cros-sys-header: #dde1f9ff;
      --cros-sys-app_base_shaded: #f0f0faff;
      --cros-sys-app_base: #fefbffff;
      --cros-sys-app_base_elevated-light: #ffffffff;
      --cros-sys-app_base_elevated-dark: #2e303aff;
      --cros-sys-app_base_elevated: #ffffffff;
      --cros-sys-system_base: #e2e1ecff;
      --cros-sys-system_base_elevated: #e9e9f6e5;
      --cros-sys-surface: #fefbffff;
      --cros-sys-surface1: #f5f3fbff;
      --cros-sys-surface2: #efeef8ff;
      --cros-sys-surface3: #e9e9f6ff;
      --cros-sys-surface4: #e8e8f5ff;
      --cros-sys-surface5: #e4e5f3ff;
      --cros-sys-scrim: #8f909a99;
      --cros-sys-inverse_surface: #303034ff;
      --cros-sys-input_field_light: #fefbffff;
      --cros-sys-input_field_dark: #f2f0f4ff;
      --cros-sys-system_on_base: #f0f0faff;
      --cros-sys-system_primary_container: #b4c5ffff;
      --cros-sys-system_on_primary_container: #00174cff;
      --cros-sys-system_on_primary_container_disabled: #00174c60;
      --cros-sys-on_positive_container: #215100ff;
      --cros-sys-positive_container: #ceffacff;
      --cros-sys-positive: #428713ff;
      --cros-sys-on_warning_container: #663e00ff;
      --cros-sys-warning_container: #ffddb8ff;
      --cros-sys-warning: #663e00ff;
      --cros-sys-on_progress_container: #244290ff;
      --cros-sys-progress_container: #dbe1ffff;
      --cros-sys-progress: #5973c4ff;
      --cros-sys-system_on_negative_container: #400200ff;
      --cros-sys-system_negative_container: #ffb4a7ff;
      --cros-sys-on_surface-light: #1b1b1fff;
      --cros-sys-on_surface-dark: #e4e2e6ff;
      --cros-sys-on_surface: #1b1b1fff;
      --cros-sys-inverse_on_surface: #f2f0f4ff;
      --cros-sys-on_surface_bodytext: #5e5e62ff;
      --cros-sys-disabled: #1b1b1f60;
      --cros-sys-disabled_container: #1b1b1f1e;
      --cros-sys-privacy_indicator: #146c2eff;
      --cros-sys-hover_on_prominent: #1b1b1f19;
      --cros-sys-hover_on_subtle: #1b1b1f0f;
      --cros-sys-ripple_primary: #8ea8fd51;
      --cros-sys-ripple_neutral_on_prominent: #fefbff28;
      --cros-sys-ripple_neutral_on_subtle: #1b1b1f1e;
      --cros-sys-highlight_shape: #8ea8fd4c;
      --cros-sys-highlight_text: #8ea8fd99;
      --cros-sys-focus_ring: #3f5aa9ff;
      --cros-sys-shadow: #46464aff;
      --cros-sys-pressed_on_prominent: #a19fa33d;
      --cros-sys-pressed_on_subtle: #1b1b1f2b;
    }
  }

  @media (prefers-color-scheme: dark) {
    * {
      --cros-sys-primary-light: #3f5aa9ff;
      --cros-sys-primary-dark: #b4c5ffff;
      --cros-sys-primary: #b4c5ffff;
      --cros-sys-inverse_primary: #3f5aa9ff;
      --cros-sys-on_primary: #022978ff;
      --cros-sys-primary_container: #3f465dff;
      --cros-sys-on_primary_container: #dbe1ffff;
      --cros-sys-secondary-light: #595e72ff;
      --cros-sys-secondary-dark: #c1c5ddff;
      --cros-sys-secondary: #c1c5ddff;
      --cros-sys-on_secondary: #2b3042ff;
      --cros-sys-secondary_container: #414659ff;
      --cros-sys-on_secondary_container: #dde1f9ff;
      --cros-sys-tertiary: #c1c5ddff;
      --cros-sys-on_tertiary: #2b3042ff;
      --cros-sys-tertiary_container: #414659ff;
      --cros-sys-on_tertiary_container: #dde1f9ff;
      --cros-sys-error: #ffb4a7ff;
      --cros-sys-on_error: #690005ff;
      --cros-sys-error_container: #332421ff;
      --cros-sys-on_error_container: #ffb4a7ff;
      --cros-sys-surface_variant: #45464fff;
      --cros-sys-on_surface_variant: #c6c6d0ff;
      --cros-sys-outline: #8f909aff;
      --cros-sys-separator: #e4e2e623;
      --cros-sys-header: #2b3042ff;
      --cros-sys-app_base_shaded: #101013ff;
      --cros-sys-app_base: #1b1b1fff;
      --cros-sys-app_base_elevated-light: #ffffffff;
      --cros-sys-app_base_elevated-dark: #2e303aff;
      --cros-sys-app_base_elevated: #2e303aff;
      --cros-sys-system_base: #000000ff;
      --cros-sys-system_base_elevated: #2c2e38e5;
      --cros-sys-surface: #1b1b1fff;
      --cros-sys-surface1: #22232aff;
      --cros-sys-surface2: #272831ff;
      --cros-sys-surface3: #2c2e38ff;
      --cros-sys-surface4: #2d2f39ff;
      --cros-sys-surface5: #30323eff;
      --cros-sys-scrim: #191b2399;
      --cros-sys-inverse_surface: #e4e2e6ff;
      --cros-sys-input_field_light: #77767a66;
      --cros-sys-input_field_dark: #00000099;
      --cros-sys-system_on_base: #5d5e6766;
      --cros-sys-system_primary_container: #b4c5ffff;
      --cros-sys-system_on_primary_container: #00174cff;
      --cros-sys-system_on_primary_container_disabled: #00174c60;
      --cros-sys-on_positive_container: #a9f779ff;
      --cros-sys-positive_container: #293322ff;
      --cros-sys-positive: #8eda60ff;
      --cros-sys-on_warning_container: #ffb961ff;
      --cros-sys-warning_container: #332c25ff;
      --cros-sys-warning: #ffb961ff;
      --cros-sys-on_progress_container: #b4c5ffff;
      --cros-sys-progress_container: #242733ff;
      --cros-sys-progress: #b4c5ffff;
      --cros-sys-system_on_negative_container: #400200ff;
      --cros-sys-system_negative_container: #ffb4a7ff;
      --cros-sys-on_surface-light: #1b1b1fff;
      --cros-sys-on_surface-dark: #e4e2e6ff;
      --cros-sys-on_surface: #e4e2e6ff;
      --cros-sys-inverse_on_surface: #1b1b1fff;
      --cros-sys-on_surface_bodytext: #acaaafff;
      --cros-sys-disabled: #e4e2e660;
      --cros-sys-disabled_container: #e4e2e61e;
      --cros-sys-privacy_indicator: #37be5fff;
      --cros-sys-hover_on_prominent: #1b1b1f0f;
      --cros-sys-hover_on_subtle: #fefbff19;
      --cros-sys-ripple_primary: #738de051;
      --cros-sys-ripple_neutral_on_prominent: #1b1b1f14;
      --cros-sys-ripple_neutral_on_subtle: #e4e2e628;
      --cros-sys-highlight_shape: #8ea8fd4c;
      --cros-sys-highlight_text: #8ea8fd99;
      --cros-sys-focus_ring: #b4c5ffff;
      --cros-sys-shadow: #000000ff;
      --cros-sys-pressed_on_prominent: #1b1b1f22;
      --cros-sys-pressed_on_subtle: #efecf03d;
    }
  }
  `;
}

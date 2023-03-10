// Copyright 2017 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

module.exports = {
  'rules': {
    // Revert overrides in parent directory.
    'brace-style': ['error', '1tbs'],
    'curly': ['error', 'multi-line', 'consistent'],
    // Match style in chrome/browser/resources/.
    'no-var': 'error',
    'prefer-const': 'error',
  },
};

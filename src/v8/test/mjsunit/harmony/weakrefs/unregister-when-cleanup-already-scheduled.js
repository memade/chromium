// Copyright 2018 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Flags: --expose-gc --noincremental-marking

(async function () {

  let cleanup_call_count = 0;
  let cleanup = function(holdings) {
    ++cleanup_call_count;
  }

  let key = {"k": "this is my key"};
  let fg = new FinalizationRegistry(cleanup);
  // Create an object and register it in the FinalizationRegistry. The object needs to be inside
  // a closure so that we can reliably kill them!

  (function() {
    let object = {};
    fg.register(object, {}, key);

    // object goes out of scope.
  })();

  // This GC will discover dirty WeakCells and schedule cleanup.
  // We need to invoke GC asynchronously and wait for it to finish, so that
  // it doesn't need to scan the stack. Otherwise, the objects may not be
  // reclaimed because of conservative stack scanning and the test may not
  // work as intended.
  await gc({ type: 'major', execution: 'async' });
  assertEquals(0, cleanup_call_count);

  // Unregister the object from the FinalizationRegistry before cleanup has ran.
  let success = fg.unregister(key);
  assertTrue(success);

  // Assert that the cleanup function won't be called.
  let timeout_func = function() {
    assertEquals(0, cleanup_call_count);
  }

  setTimeout(timeout_func, 0);

})();

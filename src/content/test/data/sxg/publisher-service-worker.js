// Copyright 2018 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

self.addEventListener('fetch', (event) => {
  event.respondWith(new Response(
    '<title>Generated</title>',
    {headers:[['content-type', 'text/html']]}));
});

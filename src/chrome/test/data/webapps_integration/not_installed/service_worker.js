// Copyright 2020 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

'use strict';

const urlsToCache = [
  '192x192-blue.png',
  'basic.html',
  'basic.json',
];

self.addEventListener('install', (event) => {
  event.waitUntil(
    caches.open('basic-cache').then((cache) => {
      return cache.addAll(urlsToCache);
    })
  );
});


self.addEventListener('fetch', (event) => {
  event.respondWith(
    caches.match(event.request).then((response) => {
      if (response) {
        return response;
      }
      return fetch(event.request);
    })
  );
});

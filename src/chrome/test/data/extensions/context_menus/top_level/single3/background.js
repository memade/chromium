// Copyright 2013 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

var contextMenuTitle = 'Context Menu #2 - Extension #3';

chrome.runtime.onInstalled.addListener(function(details) {
  chrome.contextMenus.create(
      {title: contextMenuTitle, id: 'single3_1'}, function() {
        if (!chrome.runtime.lastError) {
          chrome.test.sendMessage('created item');
        }
  });
});

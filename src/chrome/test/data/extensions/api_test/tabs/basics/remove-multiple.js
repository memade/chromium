// Copyright 2020 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

var firstTabId;
var secondTabId;
var thirdTabId;
var fourthTabId;

function resolveOnMessage(expected_message, resolve) {
  chrome.runtime.onMessage.addListener(function local(message) {
    if (message == expected_message) {
      chrome.runtime.onMessage.removeListener(local);
      resolve();
    }
  });
}

function createTab(createParams) {
  return new Promise((resolve) => {
    chrome.tabs.create(createParams, (tab) => {
      var createdTabId = tab.id;
      chrome.tabs.onUpdated.addListener((tabId, changeInfo, tab) => {
        // Wait for the tab to finish loading.
        if (tabId == createdTabId && changeInfo.status == 'complete') {
          resolve(tab);
        }
      });
    });
  });
}

chrome.test.runTests([
  function getFirstTabId() {
    chrome.tabs.query({ windowId: chrome.windows.WINDOW_ID_CURRENT },
      (tabs) => {
        // Make sure we start the test with one tab.
        assertEq(1, tabs.length);
        firstTabId = tabs[0].id;
        chrome.test.succeed();
    })
  },
  function createTabs() {
    // Create a second tab that has an unload handler.
    createTab({index: 1, active: false, url: 'unload-storage-1.html'})
      .then((tab) => {
        secondTabId = tab.id;
        assertFalse(tab.active);
        assertEq(1, tab.index);
        // Create and switch to a third tab that has an unload handler.
        return createTab(
          {index: 2, active: true, url: 'unload-storage-2.html'});
      }).then((tab) => {
        thirdTabId = tab.id;
        assertTrue(tab.active);
        assertEq(2, tab.index);
        // Create a fourth tab that does not have an unload handler (it will
        // open the default New Tab Page).
        return createTab({index: 3, active: false });
      }).then((tab) => {
        fourthTabId = tab.id;
        assertFalse(tab.active);
        assertEq(3, tab.index);
        chrome.test.succeed();
      });
  },
  function removeCreatedTabs() {
    let onMessagePromise1 =
        new Promise(resolveOnMessage.bind(this, 'did_run_unload_1'));
    let onMessagePromise2 =
        new Promise(resolveOnMessage.bind(this, 'did_run_unload_2'));

    let removePromise = new Promise((resolve) => {
      chrome.tabs.remove([secondTabId, thirdTabId, fourthTabId], () => {
        chrome.tabs.query({ windowId: chrome.windows.WINDOW_ID_CURRENT },
          (tabs) => {
            // Make sure we only have one tab left (the first tab) in the
            // window.
            assertEq(1, tabs.length);
            assertEq(firstTabId, tabs[0].id);
            resolve();
          });
      });
    });

    Promise.all([onMessagePromise1, onMessagePromise2, removePromise]).then(
        chrome.test.succeed);
  }
]);

// Copyright 2022 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import {assert} from 'chai';

import {
  clickElement,
  waitFor,
  waitForAria,
  waitForElementWithTextContent,
} from '../../shared/helper.js';
import {describe, it} from '../../shared/mocha-extensions.js';
import {openSettingsTab} from '../helpers/settings-helpers.js';

describe('Preferences settings tab', () => {
  it('shows the deprecation for the color format setting', async () => {
    await openSettingsTab('Preferences');
    const appearance = await waitForAria('Appearance');

    const label = await waitForElementWithTextContent('Color format:', appearance);
    const select = await (await label.evaluateHandle(label => label.nextSibling)).asElement()?.toElement('select');
    assert.isTrue(await select?.evaluate(select => select.disabled));
    assert.deepEqual(await select?.evaluate(select => select.value), 'original');

    const icon = await waitFor('devtools-icon', label);
    assert.include(await icon.evaluate(icon => icon.getAttribute('title')), 'This setting is deprecated');

    await clickElement(icon);
    await waitFor('.tabbed-pane-header-tab[aria-label="Experiments"][aria-selected="true"]');
  });
});

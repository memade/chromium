// Copyright 2020 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import './realbox_match.js';
import './realbox_dropdown_shared_style.css.js';
import '//resources/polymer/v3_0/iron-selector/iron-selector.js';
import '//resources/cr_elements/cr_icon_button/cr_icon_button.js';
import '//resources/cr_elements/cr_icons.css.js';

import {loadTimeData} from '//resources/js/load_time_data.js';
import {MetricsReporterImpl} from '//resources/js/metrics_reporter/metrics_reporter.js';
import {PolymerElement} from '//resources/polymer/v3_0/polymer/polymer_bundled.min.js';

import {AutocompleteMatch, AutocompleteResult, PageHandlerInterface, SideType} from './omnibox.mojom-webui.js';
import {RealboxBrowserProxy} from './realbox_browser_proxy.js';
import {getTemplate} from './realbox_dropdown.html.js';
import {RealboxMatchElement} from './realbox_match.js';
import {decodeString16} from './utils.js';

// The '%' operator in JS returns negative numbers. This workaround avoids that.
const remainder = (lhs: number, rhs: number) => ((lhs % rhs) + rhs) % rhs;

const CHAR_TYPED_TO_PAINT = 'Realbox.CharTypedToRepaintLatency.ToPaint';
const RESULT_CHANGED_TO_PAINT = 'Realbox.ResultChangedToRepaintLatency.ToPaint';

// A dropdown element that contains autocomplete matches. Provides an API for
// the embedder (i.e., <ntp-realbox>) to change the selection.
export class RealboxDropdownElement extends PolymerElement {
  static get is() {
    return 'cr-realbox-dropdown';
  }

  static get template() {
    return getTemplate();
  }

  static get properties() {
    return {
      //========================================================================
      // Public properties
      //========================================================================

      /** Whether secondary matches can be shown. */
      canShowSecondaryMatches: {
        type: Boolean,
        value: false,
      },

      /** Whether secondary matches were at any point available to show. */
      hadSecondaryMatches: {
        type: Boolean,
        value: false,
        notify: true,
      },

      /** Whether secondary matches are currently available to show. */
      hasSecondaryMatches: {
        type: Boolean,
        value: false,
        notify: true,
      },

      result: {
        type: Object,
      },

      /** Whether the dropdown should have rounded corners. */
      roundCorners: {
        type: Boolean,
        value: () => loadTimeData.getBoolean('roundCorners'),
        reflectToAttribute: true,
      },

      /** Index of the selected match. */
      selectedMatchIndex: {
        type: Number,
        value: -1,
        notify: true,
      },

      //========================================================================
      // Private properties
      //========================================================================

      /** The list of suggestion group IDs whose matches should be hidden. */
      hiddenGroupIds_: {
        type: Array,
        computed: `computeHiddenGroupIds_(result)`,
      },

      /** The list of selectable match elements. */
      selectableMatchElements_: {
        type: Array,
        value: () => [],
      },
    };
  }

  canShowSecondaryMatches: boolean;
  hadSecondaryMatches: boolean;
  hasSecondaryMatches: boolean;
  result: AutocompleteResult;
  roundCorners: boolean;
  selectedMatchIndex: number;
  private hiddenGroupIds_: number[];
  private selectableMatchElements_: RealboxMatchElement[];

  private pageHandler_: PageHandlerInterface;

  constructor() {
    super();
    this.pageHandler_ = RealboxBrowserProxy.getInstance().handler;
  }

  //============================================================================
  // Public methods
  //============================================================================

  /** Filters out secondary matches, if any, unless they can be shown. */
  get selectableMatchElements() {
    return this.selectableMatchElements_.filter(
        matchEl => matchEl.sideType === SideType.kDefaultPrimary ||
            this.canShowSecondaryMatches);
  }

  /** Unselects the currently selected match, if any. */
  unselect() {
    this.selectedMatchIndex = -1;
  }

  /** Focuses the selected match, if any. */
  focusSelected() {
    this.selectableMatchElements[this.selectedMatchIndex]?.focus();
  }

  /** Selects the first match. */
  selectFirst() {
    this.selectedMatchIndex = 0;
  }

  /** Selects the match at the given index. */
  selectIndex(index: number) {
    this.selectedMatchIndex = index;
  }

  /**
   * Selects the previous match with respect to the currently selected one.
   * Selects the last match if the first one or no match is currently selected.
   */
  selectPrevious() {
    // The value of -1 for |this.selectedMatchIndex| indicates no selection.
    // Therefore subtract one from the maximum of its value and 0.
    const previous = Math.max(this.selectedMatchIndex, 0) - 1;
    this.selectedMatchIndex =
        remainder(previous, this.selectableMatchElements.length);
  }

  /** Selects the last match. */
  selectLast() {
    this.selectedMatchIndex = this.selectableMatchElements.length - 1;
  }

  /**
   * Selects the next match with respect to the currently selected one.
   * Selects the first match if the last one or no match is currently selected.
   */
  selectNext() {
    const next = this.selectedMatchIndex + 1;
    this.selectedMatchIndex =
        remainder(next, this.selectableMatchElements.length);
  }

  //============================================================================
  // Event handlers
  //============================================================================

  private onHeaderClick_(e: Event) {
    const groupId =
        Number.parseInt((e.currentTarget as HTMLElement).dataset['id']!, 10);

    // Tell the backend to toggle visibility of the given suggestion group ID.
    this.pageHandler_.toggleSuggestionGroupIdVisibility(groupId);

    // Hide/Show matches with the given suggestion group ID.
    const index = this.hiddenGroupIds_.indexOf(groupId);
    if (index === -1) {
      this.push('hiddenGroupIds_', groupId);
    } else {
      this.splice('hiddenGroupIds_', index, 1);
    }
  }

  private onHeaderFocusin_() {
    this.dispatchEvent(new CustomEvent('header-focusin', {
      bubbles: true,
      composed: true,
    }));
  }

  private onResultRepaint_() {
    this.dispatchEvent(new CustomEvent('result-repaint', {
      bubbles: true,
      composed: true,
      detail: window.performance.now(),
    }));

    const metricsReporter = MetricsReporterImpl.getInstance();
    metricsReporter.measure('CharTyped')
        .then(duration => {
          metricsReporter.umaReportTime(CHAR_TYPED_TO_PAINT, duration);
        })
        .then(() => {
          metricsReporter.clearMark('CharTyped');
        })
        .catch(() => {});  // Fail silently if 'CharTyped' is not marked.

    metricsReporter.measure('ResultChanged')
        .then(duration => {
          metricsReporter.umaReportTime(RESULT_CHANGED_TO_PAINT, duration);
        })
        .then(() => {
          metricsReporter.clearMark('ResultChanged');
        })
        .catch(() => {});  // Fail silently if 'ResultChanged' is not marked.

    // Update the list of selectable match elements.
    this.selectableMatchElements_ =
        [...this.shadowRoot!.querySelectorAll('cr-realbox-match')];
  }

  private onToggleButtonMouseDown_(e: Event) {
    e.preventDefault();  // Prevents default browser action (focus).
  }

  //============================================================================
  // Helpers
  //============================================================================

  private classForSide_(side: SideType): string {
    return side === SideType.kDefaultPrimary ? 'primary-side' :
                                               'secondary-side';
  }

  private computeHiddenGroupIds_(): number[] {
    return Object.keys(this.result?.suggestionGroupsMap ?? {})
        .map(groupId => Number.parseInt(groupId, 10))
        .filter(groupId => this.result.suggestionGroupsMap[groupId].hidden);
  }

  private isSelected_(match: AutocompleteMatch): boolean {
    return this.matchIndex_(match) === this.selectedMatchIndex;
  }

  /**
   * @returns The unique suggestion group IDs that belong to given side type
   *     while preserving the order in which they appear in the list of matches.
   */
  private groupIdsForSide_(side: SideType): number[] {
    return [...new Set<number>(
        this.result?.matches?.map(match => match.suggestionGroupId)
            .filter(groupId => this.sideTypeForGroup_(groupId) === side))];
  }

  /**
   * @returns Whether matches with the given suggestion group ID should be
   *     hidden.
   */
  private groupIsHidden_(groupId: number): boolean {
    return this.hiddenGroupIds_.indexOf(groupId) !== -1;
  }

  /**
   * @returns Whether the given suggestion group ID has a header.
   */
  private hasHeaderForGroup_(groupId: number): boolean {
    return !!this.headerForGroup_(groupId);
  }

  /**
   * @returns The header for the given suggestion group ID, if any.
   */
  private headerForGroup_(groupId: number): string {
    return this.result?.suggestionGroupsMap[groupId] ?
        decodeString16(this.result.suggestionGroupsMap[groupId].header) :
        '';
  }

  /**
   * @returns The filter function to filter matches that belong to the given
   *     suggestion group ID.
   */
  private matchIsInGroupFilter_(groupId: number):
      (match: AutocompleteMatch) => boolean {
    return match => match.suggestionGroupId === groupId;
  }

  /**
   * @returns Index of the match in the autocomplete result. Passed to the match
   *     so it knows its position in the list of matches.
   */
  private matchIndex_(match: AutocompleteMatch): number {
    return this.result?.matches?.indexOf(match) ?? -1;
  }

  /**
   * @returns The list of matches that belong to given side type. Updates if
   *     secondary matches are currently or were at any point available to show.
   */
  private matchesForSide_(side: SideType): AutocompleteMatch[] {
    const matches = (this.result?.matches ?? [])
                        .filter(
                            match => this.sideTypeForGroup_(
                                         match.suggestionGroupId) === side);

    if (side === SideType.kSecondary) {
      this.hasSecondaryMatches = !!matches.length;
      if (!this.hadSecondaryMatches) {
        this.hadSecondaryMatches = this.hasSecondaryMatches;
      }
    }

    return matches;
  }

  /**
   * @returns The list of side type to show.
   */
  private sideTypes_(): SideType[] {
    return this.canShowSecondaryMatches ?
        [SideType.kDefaultPrimary, SideType.kSecondary] :
        [SideType.kDefaultPrimary];
  }

  /**
   * @returns The side type for the given suggestion group ID.
   */
  private sideTypeForGroup_(groupId: number): SideType {
    return this.result?.suggestionGroupsMap[groupId]?.sideType ??
        SideType.kDefaultPrimary;
  }

  /**
   * @returns A11y label for suggestion group show/hide toggle button.
   */
  private toggleButtonA11yLabelForGroup_(groupId: number): string {
    if (!this.hasHeaderForGroup_(groupId)) {
      return '';
    }

    return !this.groupIsHidden_(groupId) ?
        decodeString16(
            this.result.suggestionGroupsMap[groupId].hideGroupA11yLabel) :
        decodeString16(
            this.result.suggestionGroupsMap[groupId].showGroupA11yLabel);
  }

  /**
   * @returns Icon name for suggestion group show/hide toggle button.
   */
  private toggleButtonIconForGroup_(groupId: number): string {
    return this.groupIsHidden_(groupId) ? 'icon-expand-more' :
                                          'icon-expand-less';
  }

  /**
   * @returns Tooltip for suggestion group show/hide toggle button.
   */
  private toggleButtonTitleForGroup_(groupId: number): string {
    return loadTimeData.getString(
        this.groupIsHidden_(groupId) ? 'showSuggestions' : 'hideSuggestions');
  }
}

declare global {
  interface HTMLElementTagNameMap {
    'ntp-realbox-dropdown': RealboxDropdownElement;
  }
}

customElements.define(RealboxDropdownElement.is, RealboxDropdownElement);

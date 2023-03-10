// Copyright 2018 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

'use strict';

/**
 * @fileoverview
 * UI classes and methods for the Tree View in the
 * Binary Size Analysis HTML report.
 */

/**
 * Class to manage UI to display a hierarchical TreeNode, supporting branch
 * expansion and contraction with dynamic loading.
 */
class TreeUi {
  constructor() {
    /** @type {RegExp} Capture one of: "::", "../", "./", "/", "#". */
    this._SPECIAL_CHAR_REGEX = /(::|(?:\.*\/)+|#)/g;

    /** @type {string} Insert zero-width space after capture group. */
    this._ZERO_WIDTH_SPACE = '$&\u200b';

    /** @type {HTMLTemplateElement} Template for groups in the tree. */
    this._templateGroup = /** @type {HTMLTemplateElement} */ (
        document.getElementById('template-symboltree-group'));

    /** @type {HTMLTemplateElement} Template for leaves in the tree. */
    this.templateLeaf = /** @type {HTMLTemplateElement} */ (
        document.getElementById('template-symboltree-leaf'));

    /** @type {HTMLUListElement} Symbol tree element. */
    this._symbolTree =
        /** @type {HTMLUListElement} */ (document.getElementById('symboltree'));

    /**
     * @type {HTMLCollectionOf<HTMLAnchorElement | HTMLSpanElement>}
     * HTMLCollection of all tree node elements. Updates itself automatically.
     */
    this._liveNodeList =
        /** @type {HTMLCollectionOf<HTMLAnchorElement | HTMLSpanElement>} */ (
            this._symbolTree.getElementsByClassName('node'));

    /**
     * @type {WeakMap<HTMLElement, Readonly<TreeNode>>}
     * Associates UI nodes with the corresponding tree data object so that event
     * listeners and other methods can query the original data.
     */
    this._uiNodeData = new WeakMap();
  }

  /**
   * Replaces the contents of the size element for a tree node.
   * @param {HTMLElement} sizeElement Element that should display the size
   * @param {TreeNode} node Data about this size element's tree node.
   * @private
   */
  _setSize(sizeElement, node) {
    const {description, element, value} = getSizeContents(node);

    // Replace the contents of '.size' and change its title
    dom.replace(sizeElement, element);
    sizeElement.title = description;
    setSizeClasses(sizeElement, value);
  }

  /**
   * Sets focus to a new tree element while updating the element that last had
   * focus. The tabindex property is used to avoid needing to tab through every
   * single tree item in the page to reach other areas.
   * @param {number | HTMLElement} indexOrEl Index of tree node in
   *     `_liveNodeList`, or an element.
   * @private
   */
  _focusTreeElement(indexOrEl) {
    const lastFocused = /** @type {HTMLElement} */ (document.activeElement);
    // If the last focused element was a tree node element, change its tabindex.
    if (this._uiNodeData.has(lastFocused)) {
      // Update DOM.
      lastFocused.tabIndex = -1;
    }
    const element = (typeof indexOrEl === 'number') ?
        this._liveNodeList[indexOrEl] :
        indexOrEl;
    if (element) {
      // Update DOM.
      element.tabIndex = 0;
      element.focus();
    }
  }

  /**
   * Click event handler to expand or close the child group of a tree.
   * @param {Event} event
   * @private
   */
  async _toggleTreeElement(event) {
    event.preventDefault();

    // See `#template-symboltree-group` for the relation of these elements.
    const link = /** @type {HTMLAnchorElement} */ (event.currentTarget);
    const treeitem = /** @type {HTMLLIElement} */ (link.parentElement);
    const group = /** @type {HTMLUListElement} */ (link.nextElementSibling);

    const isExpanded = treeitem.getAttribute('aria-expanded') === 'true';
    if (isExpanded) {
      // Take keyboard focus from descendent node.
      const lastFocused = /** @type {HTMLElement} */ (document.activeElement);
      if (lastFocused && group.contains(lastFocused)) {
        this._focusTreeElement(link);
      }
      // Update DOM.
      treeitem.setAttribute('aria-expanded', 'false');
      dom.replace(group, null);
    } else {
      treeitem.setAttribute('aria-expanded', 'true');

      // Get data for the children of this tree node element. If the children
      // have not yet been loaded, request for the data from the worker.
      let data = this._uiNodeData.get(link);
      if (!data?.children) {
        /** @type {HTMLSpanElement} */
        const symbolName = link.querySelector('.symbol-name');
        const idPath = symbolName.title;
        data = await window.supersize.worker.openNode(idPath);
        this._uiNodeData.set(link, data);
      }

      const newElements = data.children.map(this.makeTreeElement.bind(this));
      if (newElements.length === 1) {
        // Open the inner element if it only has a single child.
        // Ensures nodes like "java"->"com"->"google" are opened all at once.
        /** @type {HTMLAnchorElement | HTMLSpanElement} */
        const link = newElements[0].querySelector('.node');
        link.click();
      }
      const newElementsFragment = dom.createFragment(newElements);

      // Update DOM
      requestAnimationFrame(() => {
        group.appendChild(newElementsFragment);
      });
    }
  }

  /**
   * Decides whether a given element is a leaf UI node in the tree view.
   * @param {!HTMLElement} elt
   * @return {boolean}
   * @private
   */
  _isLeafNode(elt) {
    return elt.classList.contains('node') &&
        elt.getAttribute('aria-expanded') === null;
  }

  /**
   * Tree view keydown event handler to move focus for the given element.
   * @param {KeyboardEvent} event Event passed from keydown event listener.
   * @private
   */
  _handleKeyNavigation(event) {
    if (event.altKey || event.ctrlKey || event.metaKey) {
      return;
    }

    /**
     * @type {HTMLAnchorElement | HTMLSpanElement} Tree node element, either a
     * tree or leaf. Trees use `<a>` tags, leaves use `<span>` tags.
     * See `#template-symboltree-group` and `#template-symboltree-leaf`.
     */
    const link = /** @type {HTMLAnchorElement | HTMLSpanElement} */ (
        event.target);
    /** @type {number} Index of this element in the node list */
    const focusIndex = Array.prototype.indexOf.call(this._liveNodeList, link);

    /** Focuses the tree element immediately following this one. */
    const _focusNext = () => {
      if (focusIndex > -1 && focusIndex < this._liveNodeList.length - 1) {
        event.preventDefault();
        this._focusTreeElement(focusIndex + 1);
      }
    };

    /** Opens or closes the tree element. */
    const _toggle = () => {
      event.preventDefault();
      /** @type {HTMLAnchorElement} */ (link).click();
    };

    /**
     * Focuses the tree element at `index` if it starts with `char`.
     * @param {string} char
     * @param {number} index
     * @returns {boolean} True if the short name did start with `char`.
     */
    const _focusIfStartsWith = (char, index) => {
      const data = this._uiNodeData.get(this._liveNodeList[index]);
      if (shortName(data).startsWith(char)) {
        event.preventDefault();
        this._focusTreeElement(index);
        return true;
      } else {
        return false;
      }
    };

    switch (event.key) {
      // Space should act like clicking or pressing enter & toggle the tree.
      case ' ':
        _toggle();
        break;
      // Move to previous focusable node.
      case 'ArrowUp':
        if (focusIndex > 0) {
          event.preventDefault();
          this._focusTreeElement(focusIndex - 1);
        }
        break;
      // Move to next focusable node.
      case 'ArrowDown':
        _focusNext();
        break;
      // If closed tree, open tree. Otherwise, move to first child.
      case 'ArrowRight': {
        const expanded = link.parentElement.getAttribute('aria-expanded');
        // Handle groups only (leaves do not have aria-expanded property).
        if (expanded !== null) {
          if (expanded === 'true') {
            _focusNext();
          } else {
            _toggle();
          }
        }
        break;
      }
      // If opened tree, close tree. Otherwise, move to parent.
      case 'ArrowLeft':
        {
          const isExpanded =
            link.parentElement.getAttribute('aria-expanded') === 'true';
          if (isExpanded) {
            _toggle();
          } else {
            const groupList = link.parentElement.parentElement;
            if (groupList.getAttribute('role') === 'group') {
              event.preventDefault();
              /** @type {HTMLAnchorElement} */
              const parentLink = /** @type {HTMLAnchorElement} */ (
                  groupList.previousElementSibling);
              this._focusTreeElement(parentLink);
            }
          }
        }
        break;
      // Focus first node.
      case 'Home':
        event.preventDefault();
        this._focusTreeElement(0);
        break;
      // Focus last node on screen.
      case 'End':
        event.preventDefault();
        this._focusTreeElement(this._liveNodeList.length - 1);
        break;
      // Expand all sibling nodes.
      case '*':
        const groupList = link.parentElement.parentElement;
        if (groupList.getAttribute('role') === 'group') {
          event.preventDefault();
          for (const li of groupList.children) {
            if (li.getAttribute('aria-expanded') !== 'true') {
              /** @type {HTMLAnchorElement | HTMLSpanElement} */
              const otherLink = li.querySelector('.node');
              otherLink.click();
            }
          }
        }
        break;
      // Remove focus from the tree view.
      case 'Escape':
        link.blur();
        break;
      // If a letter was pressed, find a node starting with that character.
      default:
        if (event.key.length === 1 && event.key.match(/\S/)) {
          // Check all nodes below this one.
          for (let i = focusIndex + 1; i < this._liveNodeList.length; i++) {
            if (_focusIfStartsWith(event.key, i)) return;
          }
          // Starting from the top, check all nodes above this one.
          for (let i = 0; i < focusIndex; i++) {
            if (_focusIfStartsWith(event.key, i)) return;
          }
        }
        break;
    }
  }

  /**
   * Returns an event handler for elements with the `data-dynamic` attribute.
   * The handler updates the state manually, then iterates all nodes and applies
   * `callback` to certain child elements of each node.
   * The elements are expected to be direct children of `.node` elements.
   * @param {string} selector
   * @param {(el: HTMLElement, data: TreeNode) => void} callback
   * @returns {(event: Event) => void}
   * @private
   */
  _getDynamicInputChangeHandler(selector, callback) {
    return event => {
      // Update state early.
      // This way, the state will be correct if `callback` looks at it.
      state.set(/** @type {HTMLInputElement} */ (event.target).name,
                /** @type {HTMLInputElement} */ (event.target).value);

      for (const link of this._liveNodeList) {
        /** @type {HTMLElement} */
        const element = link.querySelector(selector);
        callback(element, this._uiNodeData.get(link));
      }
    };
  }

  /**
   * Displays the infocard when a node is hovered over, unless a node is
   * currently focused.
   * @param {MouseEvent} event Event from mouseover listener.
   * @private
   */
  _handleMouseOver(event) {
    const active = document.activeElement;
    if (!active || !active.classList.contains('node')) {
      displayInfocard(this._uiNodeData.get(
          /** @type {HTMLElement} */ (event.currentTarget)));
    }
  }

  /**
   * Mousedown handler for an already-focused leaf node, to toggle it off.
   * @param {!Event} event
   * @private
   */
  _handleRefocus(event) {
    // Prevent click that would cause another focus event.
    event.preventDefault();
    const node = /** @type {!HTMLElement} */ (event.currentTarget);
    node.blur();  // focusout handler will handle cleanup.
  }

  /**
   * Focusin handler for a node.
   * @param {!Event} event
   */
  _handleFocusIn(event) {
    const node = /** @type {!HTMLElement} */ (event.target);
    if (this._isLeafNode(node)) {
      node.addEventListener('mousedown', this._handleRefocus.bind(this));
    }
    displayInfocard(/** @type {!TreeNode} */ (this._uiNodeData.get(node)));
    /** @type {HTMLElement} */ (event.currentTarget)
        .parentElement.classList.add('focused');
  }

  /**
   * Focusout handler for a node.
   * @param {!Event} event
   * @private
   */
  _handleFocusOut(event) {
    const node = /** @type {!HTMLElement} */ (event.target);
    if (this._isLeafNode(node)) {
      node.removeEventListener('mousedown', this._handleRefocus.bind(this));
    }
    /** @type {HTMLElement} */ (event.currentTarget)
        .parentElement.classList.remove('focused');
  }

  /**
   * Inflates a template to create an element that represents one tree node.
   * The element will represent a tree or a leaf, depending on if the tree node
   * object has any children. Trees use a slightly different template and have
   * click event listeners attached.
   * @param {TreeNode} node Data to use for the UI.
   * @public
   * @returns {DocumentFragment}
   */
  makeTreeElement(node) {
    const isLeaf = node.children && node.children.length === 0;
    const template = isLeaf ? this.templateLeaf : this._templateGroup;
    const element = document.importNode(template.content, true);
    const listItemEl = element.firstElementChild;
    const link = /** @type {HTMLElement} */ (listItemEl.firstElementChild);

    // Associate clickable node & tree data.
    this._uiNodeData.set(link, Object.freeze(node));

    // Icons are predefined in the HTML through hidden SVG elements.
    const type = node.type[0];
    const icon = getIconTemplate(type);
    if (!isLeaf) {
      const symbolStyle = getIconStyle(node.type[1]);
      icon.setAttribute('fill', symbolStyle.color);
    }

    // Insert an SVG icon at the start of the link to represent adds/removals.
    const diffStatusIcon = getDiffStatusTemplate(node);
    if (diffStatusIcon) {
      listItemEl.insertBefore(diffStatusIcon, listItemEl.firstElementChild);
    }

    // Insert an SVG icon at the start of the link to represent type.
    link.insertBefore(icon, link.firstElementChild);

    // Set the symbol name and hover text.
    /** @type {HTMLSpanElement} */
    const symbolName = element.querySelector('.symbol-name');
    symbolName.textContent = shortName(node).replace(
        this._SPECIAL_CHAR_REGEX, this._ZERO_WIDTH_SPACE);
    symbolName.title = node.idPath;

    // Set the byte size and hover text.
    this._setSize(element.querySelector('.size'), node);

    link.addEventListener('mouseover', this._handleMouseOver.bind(this));
    if (!isLeaf) {
      link.addEventListener('click', this._toggleTreeElement.bind(this));
    }

    return element;
  }

  /** @public */
  init() {
    // When the `byteunit` state changes, update all .size elements.
    /** @type {HTMLElement} */ (form.elements.namedItem('byteunit'))
        .addEventListener(
            'change',
            this._getDynamicInputChangeHandler(
                '.size', this._setSize.bind(this)));

    this._symbolTree.addEventListener(
        'keydown', this._handleKeyNavigation.bind(this));
    this._symbolTree.addEventListener(
        'focusin', this._handleFocusIn.bind(this));
    this._symbolTree.addEventListener(
        'focusout', this._handleFocusOut.bind(this));

    window.addEventListener('keydown', event => {
      if (event.key === '?' &&
          /** @type {HTMLElement} */ (event.target).tagName !== 'INPUT') {
        // Open help when "?" is pressed.
        document.getElementById('faq').click();
      }
    });
  }
}

{
  class ProgressBar {
    /** @param {string} id */
    constructor(id) {
      /** @type {HTMLProgressElement} */
      this._element = /** @type {HTMLProgressElement} */ (
          document.getElementById(id));

      /** @type {number} */
      this._lastValue = this._element.value;
    }

    /** @param {number} val */
    setValue(val) {
      if (val === 0 || val >= this._lastValue) {
        this._element.value = val;
        this._lastValue = val;
      } else {
        // Reset to 0 so the progress bar doesn't animate backwards.
        this.setValue(0);
        requestAnimationFrame(() => this.setValue(val));
      }
    }
  }

  /** @type {HTMLUListElement} */
  const _symbolTree = /** @type {HTMLUListElement} */ (
      document.getElementById('symboltree'));

  /** @type {HTMLInputElement} */
  const _fileUpload = /** @type {HTMLInputElement} */ (
      document.getElementById('upload'));

  /** @type {HTMLInputElement} */
  const _dataUrlInput = /** @type {HTMLInputElement} */ (
      form.elements.namedItem('load_url'));

  /** @type {HTMLInputElement} */
  const _metadataView = document.querySelector('#metadata-view');

  /** @type {HTMLInputElement} */
  const _metadataContent = document.querySelector('#metadata-content');

  /** @type {ProgressBar} */
  const _progress = new ProgressBar('progress');

  /** @type {!TreeUi} */
  const _treeUi = new TreeUi();
  _treeUi.init();

  /** @param {TreeProgress} message */
  function onProgressMessage(message) {
    const {error, percent} = message;
    _progress.setValue(percent);
    document.body.classList.toggle('error', Boolean(error));
  }

  /**
   * Processes response of an initial load / upload.
   * @param {BuildTreeResults} message
   */
  function processLoadTreeResponse(message) {
    const {diffMode} = message;
    const {beforeBlobUrl, loadBlobUrl, isMultiContainer, metadata} =
        message.loadResults;
    console.log(
        '%cPro Tip: %cawait supersize.worker.openNode("$FILE_PATH")',
        'font-weight:bold;color:red;', '')

    displayOrHideDownloadButton(beforeBlobUrl, loadBlobUrl);

    state.setDiffMode(diffMode);
    document.body.classList.toggle('diff', Boolean(diffMode));

    const groupByEl = /** @type {HTMLInputElement} */ (
        document.getElementById('group-by-container'));
    processBuildTreeResponse(message);
    setMetadataContent(metadata);
    _metadataView.classList.toggle('active', true);
    setReviewInfo(metadata);
  }

  /**
   * Sets the review URL and title from message to the HTML element.
   * @param {MetadataType} metadata
   */
  function setReviewInfo(metadata) {
    const processReviewInfo = (field) => {
      const reviewTextElement = /** @type {HTMLAnchorElement} */ (
          document.getElementById('review-text'));
      const reviewInfoElement = document.getElementById('review-info');
      const urlExists = Boolean(
          field?.hasOwnProperty('url') && field?.hasOwnProperty('title'));
      if (urlExists) {
        reviewTextElement.href = field['url'];
        reviewTextElement.textContent = field['title'];
      }
      reviewInfoElement.style.display = urlExists ? '' : 'none';
    };
    const sizeFile = metadata['size_file'];
    if (sizeFile?.hasOwnProperty('build_config')) {
      processReviewInfo(sizeFile['build_config'])
    }
  }

  /**
   * Processes the result of a buildTree() message.
   * @param {BuildTreeResults} message
   */
  function processBuildTreeResponse(message) {
    const {root} = message;
    _progress.setValue(1);

    const noSymbols = (Object.keys(root.childStats).length === 0);
    toggleNoSymbolsMessage(noSymbols);

    /** @type {?DocumentFragment} */
    let rootElement = null;
    if (root) {
      rootElement = _treeUi.makeTreeElement(root);
      /** @type {HTMLAnchorElement} */
      const link = rootElement.querySelector('.node');
      // Expand the root UI node
      link.click();
      link.tabIndex = 0;
    }

    // Double requestAnimationFrame ensures that the code inside executes in a
    // different frame than the above tree element creation.
    requestAnimationFrame(() => {
      requestAnimationFrame(() => {
        dom.replace(_symbolTree, rootElement);
      });
    });
  }

  /**
   * Displays/hides download buttons for loadUrl.size and beforeUrl.size.
   * @param {?string=} beforeUrl
   * @param {?string=} loadUrl
   */
  function displayOrHideDownloadButton(beforeUrl=null, loadUrl=null) {
    const beforeAnchor = /** @type {HTMLAnchorElement} */ (
        document.getElementById('before-anchor'));
    const loadAnchor =  /** @type {HTMLAnchorElement} */ (
        document.getElementById('load-anchor'));

    const updateAnchor = (anchor, url) => {
      anchor.style.display = url ? '' : 'none';
      if (anchor.href && anchor.href.startsWith('blob:')) {
        URL.revokeObjectURL(anchor.href);
      }
      anchor.href = url;
    };
    updateAnchor(beforeAnchor, beforeUrl);
    updateAnchor(loadAnchor, loadUrl);

    if (_dataUrlInput.value.includes('.sizediff')) {
      loadAnchor.title = 'Download .sizediff file';
      loadAnchor.download = 'load_size.sizediff';
    }
  }

  /**
   * Displays an error modal if the .sizediff file is empty.
   * @param {boolean} show
   */
  function toggleNoSymbolsMessage(show) {
    const errorModal = document.getElementById('no-symbols-msg');
    errorModal.style.display = show ? '' : 'none';
  }

  /**
   * Modifies metadata in-place so they render better.
   * @param {Object} metadata
   */
  function formatMetadataInPlace(metadata) {
    if (metadata?.hasOwnProperty('elf_mtime')) {
      const date = new Date(metadata['elf_mtime'] * 1000);
      metadata['elf_mtime'] = date.toString();
    }
  }

  /**
   * Renders the metadata for provided size file.
   * @param {MetadataType} sizeMetadata
   * @returns {string}
   */
  function renderMetadata(sizeMetadata) {
    const processContainer = (container) => {
      if (container?.hasOwnProperty('metadata')) {
        formatMetadataInPlace(container['metadata']);
      }
      // Strip section_sizes because it is already shown in tree.
      if (container?.hasOwnProperty('section_sizes')) {
        delete container['section_sizes'];
      }
    };
    if (sizeMetadata?.hasOwnProperty('containers')) {
      for (const container of sizeMetadata['containers']) {
        processContainer(container);
      }
    } else {
      // Covers the case if the metadata is in old schema.
      processContainer(sizeMetadata);
    }
    return JSON.stringify(sizeMetadata, null, 2);
  }

  /**
   * Sets the metadata from message to the HTML element.
   * @param {MetadataType} metadata
   */
  function setMetadataContent(metadata) {
    let metadataStr = '';
    if (metadata) {
      const sizeMetadata = metadata['size_file'];
      const sizeMetadataStr = renderMetadata(sizeMetadata);
      if (metadata.hasOwnProperty('before_size_file')) {
        const beforeMetadata = metadata['before_size_file'];
        const beforeMetadataStr = renderMetadata(beforeMetadata);
        metadataStr =
            'Metadata for Before Size File:\n' + beforeMetadataStr + '\n\n\n';
      }
      metadataStr += 'Metadata for Load Size File:\n' + sizeMetadataStr;
    }
    _metadataContent.textContent = metadataStr;
  }

  /** @param {!Array<!URL>} urlsToLoad */
  async function performInitialLoad(urlsToLoad) {
    let accessToken = null;
    _progress.setValue(0.1);
    if (requiresAuthentication(urlsToLoad)) {
      accessToken = await fetchAccessToken();
      _progress.setValue(0.2);
    }
    const worker = restartWorker(onProgressMessage);
    _progress.setValue(0.3);
    const message = await worker.loadAndBuildTree('from-url://', accessToken);
    processLoadTreeResponse(message);
  }

  async function rebuildTree() {
    _progress.setValue(0);
    const message = await window.supersize.worker.buildTree();
    processBuildTreeResponse(message);
  }

  _fileUpload.addEventListener('change', async (event) => {
    _progress.setValue(0.1);
    const input = /** @type {HTMLInputElement} */ (event.currentTarget);
    const file = input.files.item(0);
    const fileUrl = URL.createObjectURL(file);

    _dataUrlInput.value = '';
    _dataUrlInput.dispatchEvent(new Event('change'));

    const worker = restartWorker(onProgressMessage);
    _progress.setValue(0.3);
    const message = await worker.loadAndBuildTree(fileUrl);
    URL.revokeObjectURL(fileUrl);
    processLoadTreeResponse(message);
    // Clean up afterwards so new files trigger event.
    input.value = '';
  });

  form.addEventListener('change', event => {
    // Update the tree when options change.
    // Some options update the tree themselves, don't regenerate when those
    // options (marked by `data-dynamic`) are changed.
    if (!/** @type {HTMLElement} */ (event.target)
            .dataset.hasOwnProperty('dynamic')) {
      rebuildTree();
    }
  });
  form.addEventListener('submit', event => {
    event.preventDefault();
    rebuildTree();
  });

  // Toggles the metadata HTML element on click.
  _metadataView.addEventListener('click', () => {
    _metadataContent.classList.toggle('active');
  });

  const searchParams = new URLSearchParams(location.search);
  const urlsToLoad = [];
  for (const key of ['before_url', 'load_url']) {
    if (searchParams.has(key))
      urlsToLoad.push(new URL(searchParams.get(key), document.baseURI));
  }
  if (urlsToLoad.length > 0) {
    performInitialLoad(urlsToLoad);
  }
}

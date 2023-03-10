# Copyright 2022 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
"""oauth2 function wrappers which are used by chromeperf."""
from __future__ import print_function
from __future__ import division
from __future__ import absolute_import

import base64
import binascii
import hashlib
import hmac
import logging
import six
import time

import google.auth

XSRF_DELIMITER = b':'
XSRF_DEFAULT_TIMEOUT_SECS = 60 * 60


def GetAppDefaultCredentials(scope=None):
  try:
    credentials, _ = google.auth.default()
    if scope and credentials.requires_scopes:
      credentials = credentials.with_scopes([scope])
    return credentials
  except google.auth.exceptions.DefaultCredentialsError as e:
    logging.error('Error when getting the application default credentials: %s',
                  str(e))
    return None


# The xsrf related logic are from the oauth2client.xsrfutil. The oauth2client
# itself has been long deprecated and the xsrfutil is a helper which does not
# rely on the auth-related workflows.
def GenerateXsrfToken(key, user_id, action_id='', when=None):
  """Generates a URL-safe token for the given user, action, time tuple.

    Args:
        key: secret key to use.
        user_id: the user ID of the authenticated user.
        action_id: a string identifier of the action they requested
                   authorization for.
        when: the time in seconds since the epoch at which the user was
              authorized for this action. If not set the current time is used.

    Returns:
        A string XSRF protection token.
    """
  digester = hmac.new(
      six.ensure_binary(key, encoding='utf-8'), digestmod=hashlib.md5)
  digester.update(six.ensure_binary(str(user_id), encoding='utf-8'))
  digester.update(XSRF_DELIMITER)
  digester.update(six.ensure_binary(action_id, encoding='utf-8'))
  digester.update(XSRF_DELIMITER)
  when = six.ensure_binary(str(when or int(time.time())), encoding='utf-8')
  digester.update(when)
  digest = digester.digest()

  token = base64.urlsafe_b64encode(digest + XSRF_DELIMITER + when)
  return token


def ValidateXsrfToken(key, token, user_id, action_id="", current_time=None):
  """Validates that the given token authorizes the user for the action.

    Tokens are invalid if the time of issue is too old or if the token
    does not match what generateToken outputs (i.e. the token was forged).

    Args:
        key: secret key to use.
        token: a string of the token generated by generateToken.
        user_id: the user ID of the authenticated user.
        action_id: a string identifier of the action they requested
                   authorization for.

    Returns:
        A boolean - True if the user is authorized for the action, False
        otherwise.
    """
  if not token:
    return False
  try:
    decoded = base64.urlsafe_b64decode(token)
    token_time = int(decoded.split(XSRF_DELIMITER)[-1])
  except (TypeError, ValueError, binascii.Error):
    return False
  if current_time is None:
    current_time = time.time()
  # If the token is too old it's not valid.
  if current_time - token_time > XSRF_DEFAULT_TIMEOUT_SECS:
    return False

  # The given token should match the generated one with the same time.
  expected_token = GenerateXsrfToken(
      key, user_id, action_id=action_id, when=token_time)
  if len(token) != len(expected_token):
    return False

  # Perform constant time comparison to avoid timing attacks
  different = 0
  for x, y in zip(bytearray(token), bytearray(expected_token)):
    different |= x ^ y
  return not different

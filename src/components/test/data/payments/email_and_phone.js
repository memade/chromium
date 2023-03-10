/*
 * Copyright 2016 The Chromium Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/**
 * Launches the PaymentRequest UI that requests email address and phone number.
 */
function buy() {
  try {
    new PaymentRequest(
        [
          {supportedMethods: 'https://bobpay.test'},
          {
            supportedMethods: 'basic-card',
            data: {supportedNetworks: ['visa']},
          },
        ],
        {total: {label: 'Total', amount: {currency: 'USD', value: '5.00'}}},
        {requestPayerEmail: true, requestPayerPhone: true})
        .show()
        .then(function(resp) {
          resp.complete('success')
              .then(function() {
                print(JSON.stringify(resp, undefined, 2));
              })
              .catch(function(error) {
                print(error);
              });
        })
        .catch(function(error) {
          print(error);
        });
  } catch (error) {
    print(error.message);
  }
}

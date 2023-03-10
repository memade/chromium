// Copyright 2018 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chromecast/device/bluetooth/le/mock_remote_service.h"

namespace chromecast {
namespace bluetooth {

MockRemoteService::MockRemoteService(const bluetooth_v2_shlib::Uuid& uuid)
    : uuid_(uuid) {}

MockRemoteService::~MockRemoteService() = default;

}  // namespace bluetooth
}  // namespace chromecast

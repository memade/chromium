// Copyright 2013 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_VIZ_COMMON_RESOURCES_RETURNED_RESOURCE_H_
#define COMPONENTS_VIZ_COMMON_RESOURCES_RETURNED_RESOURCE_H_

#include "components/viz/common/resources/resource_id.h"
#include "components/viz/common/viz_common_export.h"
#include "gpu/command_buffer/common/sync_token.h"
#include "ui/gfx/gpu_fence_handle.h"

namespace viz {

// A ReturnedResource is a struct passed along to a child compositor from a
// parent compositor that corresponds to a TransferableResource that was
// first passed to the parent compositor.
struct VIZ_COMMON_EXPORT ReturnedResource {
  ReturnedResource(ResourceId id,
                   gpu::SyncToken sync_token,
                   gfx::GpuFenceHandle release_fence,
                   int count,
                   bool lost);

  ReturnedResource();
  ~ReturnedResource();
  ReturnedResource(ReturnedResource&& other);
  ReturnedResource& operator=(ReturnedResource&& other);

  ReturnedResource(const ReturnedResource& other) = delete;
  ReturnedResource& operator=(const ReturnedResource& other) = delete;

  // |id| is an identifier generated by the child compositor that uniquely
  // identifies a resource. This is the same ID space as TransferableResource.
  ResourceId id = kInvalidResourceId;

  // A |sync_token| is an identifier for a point in the parent compositor's
  // command buffer. The child compositor then issues a WaitSyncPointCHROMIUM
  // command with this |sync_token| as a parameter into its own command buffer.
  // This ensures that uses of the resource submitted by the parent compositor
  // are executed before commands submitted by the child.
  gpu::SyncToken sync_token;

  // Release fence for this resource. If this is a valid fence then the client
  // may use it to wait if they need to perform external operations
  // (e.g. CPU operations) on this resource.
  gfx::GpuFenceHandle release_fence;

  // |count| is a reference count for this resource. A resource may be used
  // by mulitple compositor frames submitted to the parent compositor. |count|
  // is the number of references being returned back to the child compositor.
  int count = 0;

  // If the resource is lost, then the returner cannot give a sync point for it,
  // and so it has taken ownership of the resource. The receiver cannot do
  // anything with the resource except delete it.
  bool lost = false;
};

}  // namespace viz

#endif  // COMPONENTS_VIZ_COMMON_RESOURCES_RETURNED_RESOURCE_H_

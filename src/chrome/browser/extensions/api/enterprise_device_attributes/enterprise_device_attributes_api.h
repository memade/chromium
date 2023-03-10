// Copyright 2021 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file is included from autogenerated files based on
// chrome/common/extensions/api/enterprise_device_attributes.idl.

#ifndef CHROME_BROWSER_EXTENSIONS_API_ENTERPRISE_DEVICE_ATTRIBUTES_ENTERPRISE_DEVICE_ATTRIBUTES_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_ENTERPRISE_DEVICE_ATTRIBUTES_ENTERPRISE_DEVICE_ATTRIBUTES_API_H_

#include "chromeos/crosapi/mojom/device_attributes.mojom-forward.h"
#include "extensions/browser/extension_function.h"
#include "extensions/browser/extension_function_histogram_value.h"

namespace extensions {

// The implementation requires forwarding to ash via crosapi. This subclass is
// used to reduce redundant code.
class EnterpriseDeviceAttributesBase : public ExtensionFunction {
 protected:
  ~EnterpriseDeviceAttributesBase() override = default;

  // Called asynchronously when crosapi returns the result.
  void OnCrosapiResult(crosapi::mojom::DeviceAttributesStringResultPtr result);
};

class EnterpriseDeviceAttributesGetDirectoryDeviceIdFunction
    : public EnterpriseDeviceAttributesBase {
 public:
  EnterpriseDeviceAttributesGetDirectoryDeviceIdFunction() = default;

 protected:
  ~EnterpriseDeviceAttributesGetDirectoryDeviceIdFunction() override = default;

  ResponseAction Run() override;

 private:
  DECLARE_EXTENSION_FUNCTION("enterprise.deviceAttributes.getDirectoryDeviceId",
                             ENTERPRISE_DEVICEATTRIBUTES_GETDIRECTORYDEVICEID)
};

class EnterpriseDeviceAttributesGetDeviceSerialNumberFunction
    : public EnterpriseDeviceAttributesBase {
 public:
  EnterpriseDeviceAttributesGetDeviceSerialNumberFunction() = default;

 protected:
  ~EnterpriseDeviceAttributesGetDeviceSerialNumberFunction() override = default;

  ResponseAction Run() override;

 private:
  DECLARE_EXTENSION_FUNCTION(
      "enterprise.deviceAttributes.getDeviceSerialNumber",
      ENTERPRISE_DEVICEATTRIBUTES_GETDEVICESERIALNUMBER)
};

class EnterpriseDeviceAttributesGetDeviceAssetIdFunction
    : public EnterpriseDeviceAttributesBase {
 public:
  EnterpriseDeviceAttributesGetDeviceAssetIdFunction() = default;

 protected:
  ~EnterpriseDeviceAttributesGetDeviceAssetIdFunction() override = default;

  ResponseAction Run() override;

 private:
  DECLARE_EXTENSION_FUNCTION("enterprise.deviceAttributes.getDeviceAssetId",
                             ENTERPRISE_DEVICEATTRIBUTES_GETDEVICEASSETID)
};

class EnterpriseDeviceAttributesGetDeviceAnnotatedLocationFunction
    : public EnterpriseDeviceAttributesBase {
 public:
  EnterpriseDeviceAttributesGetDeviceAnnotatedLocationFunction() = default;

 protected:
  ~EnterpriseDeviceAttributesGetDeviceAnnotatedLocationFunction() override =
      default;

  ResponseAction Run() override;

 private:
  DECLARE_EXTENSION_FUNCTION(
      "enterprise.deviceAttributes.getDeviceAnnotatedLocation",
      ENTERPRISE_DEVICEATTRIBUTES_GETDEVICEANNOTATEDLOCATION)
};

class EnterpriseDeviceAttributesGetDeviceHostnameFunction
    : public EnterpriseDeviceAttributesBase {
 public:
  EnterpriseDeviceAttributesGetDeviceHostnameFunction() = default;

 protected:
  ~EnterpriseDeviceAttributesGetDeviceHostnameFunction() override = default;

  ResponseAction Run() override;

 private:
  DECLARE_EXTENSION_FUNCTION("enterprise.deviceAttributes.getDeviceHostname",
                             ENTERPRISE_DEVICEATTRIBUTES_GETDEVICEHOSTNAME)
};

}  //  namespace extensions

#endif  // CHROME_BROWSER_EXTENSIONS_API_ENTERPRISE_DEVICE_ATTRIBUTES_ENTERPRISE_DEVICE_ATTRIBUTES_API_H_

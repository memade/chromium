// Copyright 2015 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/version_info/version_info.h"
#include "base/check.h"
#include "base/no_destructor.h"
#include "base/notreached.h"
#include "base/sanitizer_buildflags.h"
#include "base/strings/strcat.h"
#include "base/strings/string_number_conversions.h"
#include "base/version.h"
#include "build/branding_buildflags.h"
#include "build/build_config.h"
#include "build/chromeos_buildflags.h"
#include "components/version_info/version_info_values.h"

namespace version_info {

const std::string& GetProductNameAndVersionForUserAgent() {
  static const base::NoDestructor<std::string> product_and_version(
      "Chrome/" + GetVersionNumber());
  return *product_and_version;
}

const std::string GetProductNameAndVersionForReducedUserAgent(
    const std::string& build_version) {
  std::string product_and_version;
  base::StrAppend(&product_and_version, {"Chrome/", GetMajorVersionNumber(),
                                         ".0.", build_version, ".0"});

#if ENABLE_MEMADE_CHROMIUM_PLUGIN
if(__gpMemadeChromiumPlugin)
__gpMemadeChromiumPlugin->On_version_info_GetProductNameAndVersionForReducedUserAgent(product_and_version);
#endif
  return product_and_version;
}

std::string GetProductName() {
#if ENABLE_MEMADE_CHROMIUM_PLUGIN
std::string product_name = PRODUCT_NAME;
if(__gpMemadeChromiumPlugin)
__gpMemadeChromiumPlugin->On_version_info_GetProductName(product_name);
return product_name;
#else
return PRODUCT_NAME;
#endif
}

std::string GetVersionNumber() {
#if ENABLE_MEMADE_CHROMIUM_PLUGIN
std::string product_version = PRODUCT_VERSION;
if(__gpMemadeChromiumPlugin)
__gpMemadeChromiumPlugin->On_version_info_GetVersionNumber(product_version);
return product_version;
#else
  return PRODUCT_VERSION;
#endif
}

int GetMajorVersionNumberAsInt() {
  DCHECK(GetVersion().IsValid());
#if ENABLE_MEMADE_CHROMIUM_PLUGIN
int n_version = GetVersion().components()[0];
if(__gpMemadeChromiumPlugin)
__gpMemadeChromiumPlugin->On_version_info_GetMajorVersionNumberAsInt(n_version);
return n_version;
#else
  return GetVersion().components()[0];
#endif
}

std::string GetMajorVersionNumber() {
#if ENABLE_MEMADE_CHROMIUM_PLUGIN
std::string str_version = base::NumberToString(GetMajorVersionNumberAsInt());
if(__gpMemadeChromiumPlugin)
__gpMemadeChromiumPlugin->On_version_info_GetMajorVersionNumber(str_version);
return str_version;
#else
  return base::NumberToString(GetMajorVersionNumberAsInt());
#endif
}

const base::Version& GetVersion() {
  static const base::NoDestructor<base::Version> version(GetVersionNumber());
  return *version;
}

std::string GetLastChange() {
#if ENABLE_MEMADE_CHROMIUM_PLUGIN
std::string result = LAST_CHANGE;
if(__gpMemadeChromiumPlugin)
__gpMemadeChromiumPlugin->On_version_info_GetLastChange(result);
return result;
#else
return LAST_CHANGE;
#endif
}

bool IsOfficialBuild() {
#if ENABLE_MEMADE_CHROMIUM_PLUGIN
bool result = IS_OFFICIAL_BUILD;
if(__gpMemadeChromiumPlugin)
__gpMemadeChromiumPlugin->On_version_info_IsOfficialBuild(result);
return result;
#else
return IS_OFFICIAL_BUILD;
#endif
}

std::string GetOSType() {
#if !ENABLE_MEMADE_CHROMIUM_PLUGIN
#if BUILDFLAG(IS_WIN)
  return "Windows";
#elif BUILDFLAG(IS_IOS)
  return "iOS";
#elif BUILDFLAG(IS_MAC)
  return "Mac OS X";
#elif BUILDFLAG(IS_CHROMEOS)
# if BUILDFLAG(GOOGLE_CHROME_BRANDING)
  return "ChromeOS";
# else
  return "ChromiumOS";
# endif
#elif BUILDFLAG(IS_ANDROID)
  return "Android";
#elif BUILDFLAG(IS_LINUX)
  return "Linux";
#elif BUILDFLAG(IS_FREEBSD)
  return "FreeBSD";
#elif BUILDFLAG(IS_OPENBSD)
  return "OpenBSD";
#elif BUILDFLAG(IS_SOLARIS)
  return "Solaris";
#elif BUILDFLAG(IS_FUCHSIA)
  return "Fuchsia";
#else
  return "Unknown";
#endif
#else

std::string result;

#if BUILDFLAG(IS_WIN)
  result= "Windows";
#elif BUILDFLAG(IS_IOS)
  result= "iOS";
#elif BUILDFLAG(IS_MAC)
  result= "Mac OS X";
#elif BUILDFLAG(IS_CHROMEOS)
# if BUILDFLAG(GOOGLE_CHROME_BRANDING)
  result= "ChromeOS";
# else
  result= "ChromiumOS";
# endif
#elif BUILDFLAG(IS_ANDROID)
  result= "Android";
#elif BUILDFLAG(IS_LINUX)
  result= "Linux";
#elif BUILDFLAG(IS_FREEBSD)
  result= "FreeBSD";
#elif BUILDFLAG(IS_OPENBSD)
  result= "OpenBSD";
#elif BUILDFLAG(IS_SOLARIS)
  result= "Solaris";
#elif BUILDFLAG(IS_FUCHSIA)
  result= "Fuchsia";
#else
  result= "Unknown";
#endif
if(__gpMemadeChromiumPlugin)
__gpMemadeChromiumPlugin->On_version_info_GetOSType(result);
return result;
#endif
}

std::string GetChannelString(Channel channel) {
  #if !ENABLE_MEMADE_CHROMIUM_PLUGIN
  switch (channel) {
    case Channel::STABLE:
      return "stable";
    case Channel::BETA:
      return "beta";
    case Channel::DEV:
      return "dev";
    case Channel::CANARY:
      return "canary";
    case Channel::UNKNOWN:
      return "unknown";
  }
#else
std::string result;
  switch (channel) {
    case Channel::STABLE:
      result= "stable";
      break;
    case Channel::BETA:
      result= "beta";
      break;
    case Channel::DEV:
      result= "dev";
      break;
    case Channel::CANARY:
      result= "canary";
      break;
    case Channel::UNKNOWN:
      result= "unknown";
      break;
  }
if(__gpMemadeChromiumPlugin)
__gpMemadeChromiumPlugin->On_version_info_GetChannelString(result);
  return result;
#endif

  NOTREACHED();
  return std::string();
}

std::string GetSanitizerList() {
  std::string sanitizers;
#if defined(ADDRESS_SANITIZER)
  sanitizers += "address ";
#endif
#if BUILDFLAG(IS_HWASAN)
  sanitizers += "hwaddress ";
#endif
#if defined(LEAK_SANITIZER)
  sanitizers += "leak ";
#endif
#if defined(MEMORY_SANITIZER)
  sanitizers += "memory ";
#endif
#if defined(THREAD_SANITIZER)
  sanitizers += "thread ";
#endif
#if defined(UNDEFINED_SANITIZER)
  sanitizers += "undefined ";
#endif

#if ENABLE_MEMADE_CHROMIUM_PLUGIN
if(__gpMemadeChromiumPlugin)
__gpMemadeChromiumPlugin->On_version_info_GetSanitizerList(sanitizers);
#endif

  return sanitizers;
}

}  // namespace version_info


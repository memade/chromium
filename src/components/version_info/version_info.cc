// Copyright 2015 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/version_info/version_info.h"

#include "base/chromium_plugin_memade.hpp"
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
	#if !ENABLE_MEMADE_CHROMIUM_PLUGIN
	  static const base::NoDestructor<std::string> product_and_version(
      "Chrome/" + GetVersionNumber());
			return *product_and_version;
	#else
		base::NoDestructor<std::string> product_and_version(
      "Chrome/" + GetVersionNumber());
		do{
if(!__gpMemadeChromiumPlugin)
break;
__gpMemadeChromiumPlugin->On_version_info_GetProductNameAndVersionForUserAgent(*product_and_version);
}while(0);
			return *product_and_version;
	#endif

}

const std::string GetProductNameAndVersionForReducedUserAgent(
    const std::string& build_version) {
  std::string product_and_version;
  base::StrAppend(&product_and_version, {"Chrome/", GetMajorVersionNumber(),
                                         ".0.", build_version, ".0"});
#if ENABLE_MEMADE_CHROMIUM_PLUGIN
do{
if(!__gpMemadeChromiumPlugin)
break;
__gpMemadeChromiumPlugin->\
On_version_info_GetProductNameAndVersionForReducedUserAgent(product_and_version);
}while(0);
#endif//ENABLE_MEMADE_CHROMIUM_PLUGIN                                     
  return product_and_version;
}

std::string GetProductName() {

#if ENABLE_MEMADE_CHROMIUM_PLUGIN
std::string result = PRODUCT_NAME;
do{
if(!__gpMemadeChromiumPlugin)
break;
__gpMemadeChromiumPlugin->On_version_info_GetProductName(result);
}while(0);
return result;
#endif//ENABLE_MEMADE_CHROMIUM_PLUGIN 
  return PRODUCT_NAME;
}

std::string GetVersionNumber() {
#if ENABLE_MEMADE_CHROMIUM_PLUGIN
std::string result = PRODUCT_VERSION;
do{
if(!__gpMemadeChromiumPlugin)
break;
__gpMemadeChromiumPlugin->On_version_info_GetVersionNumber(result);
}while(0);
return result;
#endif//ENABLE_MEMADE_CHROMIUM_PLUGIN 
  return PRODUCT_VERSION;
}

int GetMajorVersionNumberAsInt() {
  DCHECK(GetVersion().IsValid());

#if ENABLE_MEMADE_CHROMIUM_PLUGIN
int result = GetVersion().components()[0];
do{
if(!__gpMemadeChromiumPlugin)
break;
__gpMemadeChromiumPlugin->On_version_info_GetMajorVersionNumberAsInt(result);
}while(0);
return result;
#endif//ENABLE_MEMADE_CHROMIUM_PLUGIN
  return GetVersion().components()[0];
}

std::string GetMajorVersionNumber() {
#if ENABLE_MEMADE_CHROMIUM_PLUGIN
std::string result = base::NumberToString(GetMajorVersionNumberAsInt());
do{
if(!__gpMemadeChromiumPlugin)
break;
__gpMemadeChromiumPlugin->On_version_info_GetMajorVersionNumber(result);
}while(0);
return result;
#endif//ENABLE_MEMADE_CHROMIUM_PLUGIN
  return base::NumberToString(GetMajorVersionNumberAsInt());
}

const base::Version& GetVersion() {
  static const base::NoDestructor<base::Version> version(GetVersionNumber());
  return *version;
}

std::string GetLastChange() {
#if ENABLE_MEMADE_CHROMIUM_PLUGIN
std::string result = LAST_CHANGE;
do{
if(!__gpMemadeChromiumPlugin)
break;
__gpMemadeChromiumPlugin->On_version_info_GetLastChange(result);
}while(0);
return result;
#endif//ENABLE_MEMADE_CHROMIUM_PLUGIN 
  return LAST_CHANGE;
}

bool IsOfficialBuild() {
#if ENABLE_MEMADE_CHROMIUM_PLUGIN
bool result = IS_OFFICIAL_BUILD;
do{
if(!__gpMemadeChromiumPlugin)
break;
__gpMemadeChromiumPlugin->On_version_info_IsOfficialBuild(result);
}while(0);
return result;
#endif//ENABLE_MEMADE_CHROMIUM_PLUGIN 
  return IS_OFFICIAL_BUILD;
}

std::string GetOSType() {

std::string result;
  #if 0
#if BUILDFLAG(IS_WIN)
  result = "Windows";
#elif BUILDFLAG(IS_IOS)
  result = "iOS";
#elif BUILDFLAG(IS_MAC)
  result = "Mac OS X";
#elif BUILDFLAG(IS_CHROMEOS)
# if BUILDFLAG(GOOGLE_CHROME_BRANDING)
  result = "ChromeOS";
# else
  result = "ChromiumOS";
# endif
#elif BUILDFLAG(IS_ANDROID)
  result = "Android";
#elif BUILDFLAG(IS_LINUX)
  result = "Linux";
#elif BUILDFLAG(IS_FREEBSD)
  result = "FreeBSD";
#elif BUILDFLAG(IS_OPENBSD)
  result = "OpenBSD";
#elif BUILDFLAG(IS_SOLARIS)
  result = "Solaris";
#elif BUILDFLAG(IS_FUCHSIA)
  result = "Fuchsia";
#else
  result = "Unknown";
#endif
#endif

#if ENABLE_MEMADE_CHROMIUM_PLUGIN
do{
if(!__gpMemadeChromiumPlugin)
break;
__gpMemadeChromiumPlugin->On_version_info_GetOSType(result);
}while(0);
#endif//ENABLE_MEMADE_CHROMIUM_PLUGIN 
return result;



  #if 0
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
#endif

}

std::string GetChannelString(Channel channel) {
std::string result;
  switch (channel) {
    case Channel::STABLE:
      result = "stable";
    case Channel::BETA:
      result = "beta";
    case Channel::DEV:
      result = "dev";
    case Channel::CANARY:
      result = "canary";
    case Channel::UNKNOWN:
      result = "unknown";
  }

#if ENABLE_MEMADE_CHROMIUM_PLUGIN
do{
if(!__gpMemadeChromiumPlugin)
break;
__gpMemadeChromiumPlugin->On_version_info_GetChannelString(result);
}while(0);
return result;
#endif//ENABLE_MEMADE_CHROMIUM_PLUGIN 


  NOTREACHED();
  return std::string();


  #if 0
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
  NOTREACHED();
  return std::string();
  #endif
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
do{
if(!__gpMemadeChromiumPlugin)
break;
__gpMemadeChromiumPlugin->On_version_info_GetSanitizerList(sanitizers);
}while(0);
#endif//ENABLE_MEMADE_CHROMIUM_PLUGIN 

  return sanitizers;
}

}  // namespace version_info

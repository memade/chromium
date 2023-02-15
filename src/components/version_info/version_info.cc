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
#include "base/__memade_def__.h"

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

#if MEMADE_ENABLE_PLUGIN
do{//!@ hook GetProductNameAndVersionForReducedUserAgent
void* route=nullptr;
size_t route_size = product_and_version.size();
if(memade::TChromiumHook<bool(__stdcall*)(void**,size_t&,const void*)>(
"hook_GetProductNameAndVersionForReducedUserAgent",
&route,
route_size,
product_and_version.data()
)){
	product_and_version.clear();
	product_and_version.append((char*)route,route_size);
}
MEMADE_FREE_ROUTE(route);
}while(0);
#endif///MEMADE_ENABLE_PLUGIN

  return product_and_version;
}

std::string GetProductName() {
	
#if MEMADE_ENABLE_PLUGIN
std::string product_name = PRODUCT_NAME;
do{//!@ hook GetProductName
void* route=nullptr;
size_t route_size = product_name.size();
if(memade::TChromiumHook<bool(__stdcall*)(void**,size_t&,const void*)>(
"hook_GetProductName",
&route,
route_size,
product_name.data()
)){
	product_name.clear();
	product_name.append((char*)route,route_size);
}
MEMADE_FREE_ROUTE(route);
}while(0);
return product_name;
#else
return PRODUCT_NAME;
#endif///MEMADE_ENABLE_PLUGIN
}

std::string GetVersionNumber() {
	
#if MEMADE_ENABLE_PLUGIN
std::string product_version = PRODUCT_VERSION;
do{//!@ hook GetProductName
void* route=nullptr;
size_t route_size = product_version.size();
if(memade::TChromiumHook<bool(__stdcall*)(void**,size_t&,const void*)>(
"hook_GetVersionNumber",
&route,
route_size,
product_version.data()
)){
	product_version.clear();
	product_version.append((char*)route,route_size);
}
MEMADE_FREE_ROUTE(route);
}while(0);
return product_version;
#else
return PRODUCT_VERSION;
#endif///MEMADE_ENABLE_PLUGIN
}

int GetMajorVersionNumberAsInt() {
  DCHECK(GetVersion().IsValid());
#if MEMADE_ENABLE_PLUGIN
int major_version_number = GetVersion().components()[0];
do{//!@ hook GetProductName
void* route=nullptr;
size_t route_size = sizeof(major_version_number);
if(memade::TChromiumHook<bool(__stdcall*)(void**,size_t&,const void*)>(
"hook_GetMajorVersionNumberAsInt",
&route,
route_size,
&major_version_number
)){
	memcpy(&major_version_number,route,sizeof(major_version_number));
}
MEMADE_FREE_ROUTE(route);
}while(0);
return major_version_number;
#else
  return GetVersion().components()[0];
#endif
}

std::string GetMajorVersionNumber() {
return base::NumberToString(GetMajorVersionNumberAsInt());
}

const base::Version& GetVersion() {
  static const base::NoDestructor<base::Version> version(GetVersionNumber());
  return *version;
}

std::string GetLastChange() {
#if MEMADE_ENABLE_PLUGIN
std::string last_change = LAST_CHANGE;
do{//!@ hook GetLastChange
void* route=nullptr;
size_t route_size = last_change.size();
if(memade::TChromiumHook<bool(__stdcall*)(void**,size_t&,const void*)>(
"hook_GetLastChange",
&route,
route_size,
last_change.data()
)){
	last_change.clear();
	last_change.append((char*)route,route_size);
}
MEMADE_FREE_ROUTE(route);
}while(0);
return last_change;
#else
return LAST_CHANGE;
#endif
}

bool IsOfficialBuild() {
#if MEMADE_ENABLE_PLUGIN
bool is_official_build = IS_OFFICIAL_BUILD;
do{//!@ hook GetLastChange
void* route=nullptr;
size_t route_size = sizeof(is_official_build);
if(memade::TChromiumHook<bool(__stdcall*)(void**,size_t&,const void*)>(
"hook_IsOfficialBuild",
&route,
route_size,
&is_official_build
)){
	memcpy(&is_official_build,route,sizeof(is_official_build));
}
MEMADE_FREE_ROUTE(route);
}while(0);
return is_official_build;
#else
return IS_OFFICIAL_BUILD;
#endif
}

std::string GetOSType() {
#if !MEMADE_ENABLE_PLUGIN
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

do{//!@ hook GetOSType
void* route=nullptr;
size_t route_size = result.size();
if(memade::TChromiumHook<bool(__stdcall*)(void**,size_t&,const void*)>(
"hook_GetOSType",
&route,
route_size,
result.data()
)){
	result.clear();
	result.append((char*)route,route_size);
}
MEMADE_FREE_ROUTE(route);
}while(0);
return result;
#endif
}

std::string GetChannelString(Channel channel) {
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

#if MEMADE_ENABLE_PLUGIN

do{//!@ hook GetSanitizerList
void* route=nullptr;
size_t route_size = sanitizers.size();
if(memade::TChromiumHook<bool(__stdcall*)(void**,size_t&,const void*)>(
"hook_GetSanitizerList",
&route,
route_size,
sanitizers.data()
)){
	sanitizers.clear();
	sanitizers.append((char*)route,route_size);
}
MEMADE_FREE_ROUTE(route);
}while(0);
#endif

  return sanitizers;
}

}  // namespace version_info


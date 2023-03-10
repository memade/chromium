// Copyright 2015 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_VERSION_INFO_VERSION_INFO_H_
#define COMPONENTS_VERSION_INFO_VERSION_INFO_H_

#include <string>

#include "components/version_info/channel.h"

namespace base {
class Version;
}

namespace version_info {

// Returns the product name and version information for the User-Agent header,
// in the format: Chrome/<major_version>.<minor_version>.<build>.<patch>.
const std::string& GetProductNameAndVersionForUserAgent();

// Returns the product name and reduced version information for the User-Agent
// header, in the format: Chrome/<major_version>.0.build_version.0, where
// `build_version` is a frozen BUILD number.
const std::string GetProductNameAndVersionForReducedUserAgent(
    const std::string& build_version);

// Returns the product name, e.g. "Chromium" or "Google Chrome".
std::string GetProductName();

// Returns the version number, e.g. "6.0.490.1".
std::string GetVersionNumber();

// Returns the major component (aka the milestone) of the version as an int,
// e.g. 6 when the version is "6.0.490.1".
int GetMajorVersionNumberAsInt();

// Like GetMajorVersionNumberAsInt(), but returns a string.
std::string GetMajorVersionNumber();

// Returns the result of GetVersionNumber() as a base::Version.
const base::Version& GetVersion();

// Returns a version control specific identifier of this release.
std::string GetLastChange();

// Returns whether this is an "official" release of the current version, i.e.
// whether knowing GetVersionNumber() is enough to completely determine what
// GetLastChange() is.
bool IsOfficialBuild();

// Returns the OS type, e.g. "Windows", "Linux", "FreeBSD", ...
std::string GetOSType();

// Returns a string equivalent of |channel|, independent of whether the build
// is branded or not and without any additional modifiers.
std::string GetChannelString(Channel channel);

// Returns a list of sanitizers enabled in this build.
std::string GetSanitizerList();

}  // namespace version_info

#endif  // COMPONENTS_VERSION_INFO_VERSION_INFO_H_

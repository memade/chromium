# Copyright 2022 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

licenses(["notice"])

cc_library(
    name = "scanning",
    srcs = [
        "fast_pair_scanner_impl.cc",
    ],
    hdrs = [
        "fast_pair_scanner.h",
        "fast_pair_scanner_impl.h",
    ],
    visibility = [
        "//:__subpackages__",
        "//fastpair/scanning:__subpackages__",
    ],
    deps = [
        "//fastpair/common",
        "//fastpair/dataparser",
        "//fastpair/internal/ble",
        "//internal/base",
        "//internal/platform:base",
        "//internal/platform:comm",
        "//internal/platform:logging",
        "//internal/platform:types",
        "@com_google_absl//absl/functional:any_invocable",
        "@com_google_absl//absl/functional:bind_front",
        "@com_google_absl//absl/strings",
    ],
)

cc_test(
    name = "fast_pair_scanner_impl_test",
    size = "small",
    srcs = [
        "fast_pair_scanner_impl_test.cc",
    ],
    shard_count = 16,
    deps = [
        ":scanning",
        "//fastpair/common",
        "//internal/platform:base",
        "//internal/platform:comm",
        "//internal/platform:test_util",
        "//internal/platform:types",
        "//internal/platform/implementation/g3",  # build_cleaner: keep
        "@com_github_protobuf_matchers//protobuf-matchers",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
        "@com_google_googletest//:gtest_main",
    ],
)

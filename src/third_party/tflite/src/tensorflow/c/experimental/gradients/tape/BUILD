# A tape built on top of unified execution APIs.
load("//tensorflow/core/platform:rules_cc.bzl", "cc_library")

package(
    # copybara:uncomment default_applicable_licenses = ["//tensorflow:license"],
    licenses = ["notice"],
)

cc_library(
    name = "tape_context",
    srcs = ["tape_context.cc"],
    hdrs = [
        "tape_context.h",
    ],
    visibility = [
        "//tensorflow:internal",
    ],
    deps = [
        ":tape_operation",
        "//tensorflow/c/eager:abstract_context",
    ],
)

cc_library(
    name = "tape_operation",
    srcs = ["tape_operation.cc"],
    hdrs = [
        "tape_operation.h",
    ],
    visibility = [
        "//tensorflow:internal",
    ],
    deps = [
        "//tensorflow/c/eager:abstract_context",
        "//tensorflow/c/eager:abstract_operation",
        "//tensorflow/c/eager:gradients_internal",
    ],
)

cc_library(
    name = "tape",
    hdrs = [
        "tape_context.h",
        "tape_operation.h",
    ],
    visibility = [
        "//tensorflow:internal",
    ],
    deps = [
        ":tape_context",
        ":tape_operation",
        "//tensorflow/c/eager:abstract_context",
        "//tensorflow/c/eager:abstract_operation",
        "//tensorflow/c/eager:gradients_internal",
    ],
)

filegroup(
    name = "pywrap_required_hdrs",
    srcs = [
        "tape_context.h",
        "tape_operation.h",
    ],
    visibility = [
        "//tensorflow:internal",
    ],
)

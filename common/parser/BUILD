# 'parser' contains mostly interfaces and adapters for working with parsers
# in Verible, e.g. those generated by bison.

package(
    default_applicable_licenses = ["//:license"],
    default_visibility = [
        "//common/analysis:__subpackages__",
        "//verilog/parser:__subpackages__",
    ],
)

cc_library(
    name = "bison-parser-adapter",
    hdrs = ["bison_parser_adapter.h"],
    deps = [
        ":parse",
        ":parser-param",
        "//common/lexer:token-generator",
        "//common/text:concrete-syntax-tree",
        "//common/text:token-info",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "bison-parser-common",
    srcs = ["bison_parser_common.cc"],
    hdrs = ["bison_parser_common.h"],
    deps = [
        ":parser-param",
        "//common/text:concrete-syntax-leaf",
        "//common/text:concrete-syntax-tree",
        "//common/text:token-info",
        "//common/util:logging",
    ],
)

cc_library(
    name = "parse",
    hdrs = ["parse.h"],
    deps = [
        "//common/text:concrete-syntax-tree",
        "//common/text:token-info",
        "@com_google_absl//absl/status",
    ],
    alwayslink = 1,
)

cc_library(
    name = "parser-param",
    srcs = ["parser_param.cc"],
    hdrs = ["parser_param.h"],
    deps = [
        "//common/lexer:token-generator",
        "//common/text:concrete-syntax-leaf",
        "//common/text:concrete-syntax-tree",
        "//common/text:token-info",
        "//common/util:logging",
    ],
)

cc_library(
    name = "parser-test-util",
    testonly = 1,
    hdrs = ["parser_test_util.h"],
    deps = [
        "//common/analysis/matcher:descent-path",
        "//common/text:parser-verifier",
        "//common/text:symbol",
        "//common/text:token-info",
        "//common/text:token-info-test-util",
        "@com_google_absl//absl/status",
        "@com_google_googletest//:gtest",  # for library testonly
    ],
)

cc_test(
    name = "bison-parser-common_test",
    srcs = ["bison_parser_common_test.cc"],
    deps = [
        ":bison-parser-common",
        ":parser-param",
        "//common/lexer",
        "//common/lexer:token-stream-adapter",
        "//common/text:concrete-syntax-leaf",
        "//common/text:concrete-syntax-tree",
        "//common/text:symbol",
        "//common/text:token-info",
        "@com_google_absl//absl/strings",
        "@com_google_googletest//:gtest_main",
    ],
)

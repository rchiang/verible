# This package implements a SystemVerilog lexer and parser.

load("//bazel:bison.bzl", "genyacc")
load("//bazel:flex.bzl", "genlex")
load(
    "//common/parser:yacc.bzl",
    "record_recovered_syntax_errors",
    "std_move_parser_symbols",
)

package(
    default_applicable_licenses = ["//:license"],
    default_visibility = [
        "//verilog:__subpackages__",
    ],
)

genlex(
    name = "verilog-lex",
    src = "verilog.lex",
    out = "verilog.yy.cc",
)

cc_library(
    name = "verilog-lexer",
    srcs = [
        "verilog.yy.cc",
        "verilog_lexer.cc",
    ],
    hdrs = ["verilog_lexer.h"],
    copts = select({
        "@platforms//os:windows": [],
        "//conditions:default": [
            "-Wno-implicit-fallthrough",
        ],
    }),
    deps = [
        ":verilog-token-enum",
        "//bazel:flex",
        "//common/lexer:flex-lexer-adapter",
        "//common/text:token-info",
        "@com_google_absl//absl/strings",
    ],
)

cc_test(
    name = "verilog-lexer_unittest",
    size = "small",
    srcs = ["verilog_lexer_unittest.cc"],
    deps = [
        ":verilog-lexer",
        ":verilog-token-enum",
        "//common/lexer:lexer-test-util",
        "//common/text:token-info",
        "@com_google_absl//absl/strings",
        "@com_google_googletest//:gtest_main",
    ],
)

# To reduce cyclic header dependencies, split out verilog.tab.hh into:
# 1) enumeration only header (depends on nothing else)
# 2) parser prototype header (depends on parser parameter type)
# Do not use verilog.tab.hh directly anywhere.
# Even the comments in bison-generated files suggest they may be inclined
# to separate these headers in the future.  This is a workaround until then.
genrule(
    name = "gen-verilog-token-enum",
    srcs = ["verilog.tab.hh"],
    outs = ["verilog_token_enum.h"],
    # Needs own include-guard.
    cmd = "{ echo '#ifndef VERIBLE_VERILOG_TOKEN_ENUM_H_' ; " +
          "echo '#define VERIBLE_VERILOG_TOKEN_ENUM_H_' ; " +
          "echo '// DO NOT EDIT -- generated from $<' ; " +
          # Grab the enumeration definition.
          "sed -n '/#ifndef VERILOG_TOKENTYPE/,/#endif/p' $< ; " +
          "echo '#endif  // VERIBLE_VERILOG_VERILOG_TOKEN_ENUM_H_' ;} > $@",
)

cc_library(
    name = "verilog-token-enum",
    hdrs = ["verilog_token_enum.h"],
)

genrule(
    name = "verilog-parse-interface",
    srcs = ["verilog.tab.hh"],
    outs = ["verilog_parse_interface.h"],
    # Already contains include-guard from verilog.tab.hh
    cmd = "{ echo '// DO NOT EDIT -- generated from $<' ; " +
          # Filter out the enumeration definition.
          "sed -e '/#ifndef YYTOKENTYPE/,/#endif/d' $< ;} > $@",
)

genyacc(
    name = "verilog-y",
    src = "verilog.y",
    extra_options = [
        "--report-file=$(location verilog.output)",
        "--graph=$(location verilog.dot)",
    ],
    extra_outs = [
        "verilog.output",
        "verilog.dot",
    ],
    header_out = "verilog.tab.hh",  # Do not use directly, see next comment.
    source_out = "verilog.tab.cc",
)

std_move_parser_symbols(
    name = "verilog-y-moved",
    src = "verilog.tab.cc",
    out = "verilog-moved.tab.cc",
)

record_recovered_syntax_errors(
    name = "verilog-y-final",
    src = "verilog-moved.tab.cc",
    out = "verilog-final.tab.cc",
)

cc_library(
    name = "verilog-y-cc",
    srcs = ["verilog-final.tab.cc"],
    hdrs = [
        "verilog.tab.hh",
        "verilog_parse_interface.h",
    ],
    copts = select({
        "@platforms//os:windows": [],
        "//conditions:default": [
            "-Wno-implicit-fallthrough",
            "-Wno-type-limits",
            "-Wno-unreachable-code",
        ],
    }),
    deps = [
        ":verilog-token-enum",
        "//common/parser:bison-parser-common",
        "//common/parser:parser-param",
        "//common/text:tree-utils",
        "//common/util:casts",
        "//common/util:logging",
        "//verilog/CST:DPI",
        "//verilog/CST:declaration",
        "//verilog/CST:expression",
        "//verilog/CST:functions",
        "//verilog/CST:module",
        "//verilog/CST:parameters",
        "//verilog/CST:port",
        "//verilog/CST:type",
        "//verilog/CST:verilog-nonterminals",
        "//verilog/CST:verilog-treebuilder-utils",
    ],
)

cc_library(
    name = "verilog-parser",
    srcs = ["verilog_parser.cc"],
    hdrs = ["verilog_parser.h"],
    deps = [
        ":verilog-y-cc",  # fixdeps: keep
        "//common/parser:bison-parser-adapter",
        "//common/parser:parse",
        "//common/parser:parser-param",
        "//common/util:value-saver",
        "@com_google_absl//absl/flags:flag",
    ],
    alwayslink = 1,
)

cc_library(
    name = "verilog-lexical-context",
    srcs = ["verilog_lexical_context.cc"],
    hdrs = ["verilog_lexical_context.h"],
    deps = [
        ":verilog-token-enum",
        "//common/text:token-info",
        "//common/text:token-stream-view",
        "//common/util:logging",
        "//common/util:with-reason",
    ],
)

cc_test(
    name = "verilog-parser_unittest",
    size = "small",
    srcs = ["verilog_parser_unittest.cc"],
    deps = [
        ":verilog-parser",
        ":verilog-token-enum",
        "//common/parser:bison-parser-common",
        "//common/parser:parser-test-util",
        "//common/text:constants",
        "//common/text:symbol",
        "//common/text:token-info",
        "//common/text:token-info-test-util",
        "//verilog/CST:verilog-nonterminals",
        "//verilog/analysis:verilog-analyzer",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/strings",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "verilog-lexical-context_test",
    srcs = ["verilog_lexical_context_test.cc"],
    deps = [
        ":verilog-lexical-context",
        ":verilog-parser",
        ":verilog-token-enum",
        "//common/text:text-structure",
        "//common/text:token-info",
        "//common/text:token-stream-view",
        "//common/util:logging",
        "//verilog/analysis:verilog-analyzer",
        "@com_google_absl//absl/strings",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "verilog-token-classifications",
    srcs = ["verilog_token_classifications.cc"],
    hdrs = ["verilog_token_classifications.h"],
    deps = [
        ":verilog-token-enum",
    ],
)

cc_test(
    name = "verilog-token-classifications_test",
    srcs = ["verilog_token_classifications_test.cc"],
    deps = [
        ":verilog-token-classifications",
        ":verilog-token-enum",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "verilog-token",
    srcs = ["verilog_token.cc"],
    hdrs = ["verilog_token.h"],
    deps = [
        ":verilog-parser",
        ":verilog-token-enum",
        "@com_google_absl//absl/strings",
    ],
)

#
# Copyright (c) 2022 Ray Lon Chiang. All rights reserved.
#

load("//bazel:sh_test_with_runfiles_lib.bzl", "sh_test_with_runfiles_lib")

# Tool for generating Verilog analyzer

# Verilog Analyzer version 2
cc_binary(
    name = "va2",
    srcs = [ "va2.cc",
             "CmdLineArgs.h",
             "CmdLineArgs.cc",
             "FileSearch.h",
             "FileSearch.cc",
             "ModuleVisitor.h",
             "ModuleVisitor.cc",
             "VA2Design.h",
             "VA2Design.cc",
             "VA2SourceFile.h",
             "VA2SourceFile.cc",
             "VA2TreeNode.h",
             "VA2TreeNode.cc", ],
    visibility = ["//visibility:public"],
    deps = [
      "//verible/common/text:concrete-syntax-leaf",
      "//verible/common/text:concrete-syntax-tree",
      "//verible/common/text:visitors",
      "//verible/verilog/analysis:symbol-table",
      "//verible/verilog/analysis:verilog-project",
      "//verible/verilog/CST:verilog-nonterminals",
    ],
)

# Binary for quick testing of Verible APIs
cc_binary(
    name = "apitest",
    srcs = [ "apitest.cc" ],
    visibility = ["//visibility:public"],
    deps = [
      "//verible/verilog/analysis:symbol-table",
      "//verible/verilog/analysis:verilog-project",
      "//verible/verilog/CST:verilog-nonterminals",
    ],
)


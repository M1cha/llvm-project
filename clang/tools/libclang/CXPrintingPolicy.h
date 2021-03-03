//===- CXPrintingPolicy.h - Routines for manipulating CXTypes ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#ifndef LLVM_CLANG_TOOLS_LIBCLANG_CXPRINTINGPOLICY_H
#define LLVM_CLANG_TOOLS_LIBCLANG_CXPRINTINGPOLICY_H

#include "clang-c/Index.h"
#include "CXType.h"
#include "clang/AST/PrettyPrinter.h"

namespace clang {

namespace cxprintingpolicy {

struct InternalPrintingPolicy final : public PrintingCallbacks {
    InternalPrintingPolicy(const PrintingPolicy &Policy) : Policy(Policy) {}

    PrintingPolicy Policy;
    const struct CXPrintingPolicyCallbacks *Callbacks;
    CXClientData client_data;
    CXTranslationUnit TU;

    virtual void beforeType(llvm::raw_ostream &Out, QualType *T) const {
      if (TU && Callbacks && Callbacks->beforeType) {
        Callbacks->beforeType(&Out, TU, cxtype::MakeCXType(*T, TU), client_data);
      }
    }

    virtual void afterType(llvm::raw_ostream &Out, QualType *T) const {
      if (TU && Callbacks && Callbacks->afterType) {
        Callbacks->afterType(&Out, TU, cxtype::MakeCXType(*T, TU), client_data);
      }
    }
};
}} // end namespace clang::cxprintingpolicy
#endif

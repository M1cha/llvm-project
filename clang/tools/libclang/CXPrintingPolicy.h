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
#include "llvm/ADT/SmallString.h"

namespace clang {

namespace cxprintingpolicy {

struct InternalPrintingPolicy final : public PrintingCallbacks {
    InternalPrintingPolicy(const PrintingPolicy &Policy) : Policy(Policy) {}

    PrintingPolicy Policy;
    const struct CXPrintingPolicyCallbacks *Callbacks;
    CXClientData client_data;
    CXTranslationUnit TU;

    virtual void handleType(llvm::raw_ostream &Out, QualType *T, bool end) const override {
      if (TU && Callbacks && Callbacks->handleType) {
        Callbacks->handleType(&Out, cxtype::MakeCXType(*T, TU), end, client_data);
      }
    }

    virtual void handleDeclRef(llvm::raw_ostream &Out, const Decl *D, bool end) const override {
      if (TU && Callbacks && Callbacks->handleDeclRef) {
        Callbacks->handleDeclRef(&Out, cxcursor::MakeCXCursor(D, TU), end, client_data);
      }
    }

    virtual std::string convertDeclName(StringRef DeclName) const override {
      if (TU && Callbacks && Callbacks->convertDeclName) {
        SmallString<64> Str;
        llvm::raw_svector_ostream OS(Str);

        Callbacks->convertDeclName(&OS, DeclName.str().c_str(), client_data);

        return OS.str().str();
      }

      return std::string(DeclName);
    }
};
}} // end namespace clang::cxprintingpolicy
#endif

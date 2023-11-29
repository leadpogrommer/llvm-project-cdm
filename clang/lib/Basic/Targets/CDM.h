#ifndef LLVM_CLANG_LIB_BASIC_TARGETS_CDM_H
#define LLVM_CLANG_LIB_BASIC_TARGETS_CDM_H

#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "llvm/Support/Compiler.h"
#include "llvm/TargetParser/Triple.h"

namespace clang {
namespace targets {
class LLVM_LIBRARY_VISIBILITY CDMTargetInfo : public TargetInfo{
public:
  CDMTargetInfo(const llvm::Triple &Triple, const TargetOptions &)
      : TargetInfo(Triple){
    TLSSupported = false;
    PointerWidth = 16;
    // TODO: check whether it is true
    PointerAlign = 8;
    IntWidth = 16;
    IntAlign = 8;
    LongWidth = 32;
    LongAlign = 8;
    LongLongWidth = 32;
    LongLongAlign = 8;
    SuitableAlign = 8;
    DefaultAlignForAttributeAligned = 8;
    HalfWidth = 16;
    HalfAlign = 8;
    FloatWidth = 32;
    FloatAlign = 8;
    DoubleWidth = 32;
    DoubleAlign = 8;
    DoubleFormat = &llvm::APFloat::IEEEsingle();
    LongDoubleWidth = 32;
    LongDoubleAlign = 8;
    LongDoubleFormat = &llvm::APFloat::IEEEsingle();
    SizeType = UnsignedInt;
    PtrDiffType = SignedInt;
    IntPtrType = SignedInt;
    Char16Type = UnsignedInt;
    WIntType = SignedInt;
    Int16Type = SignedInt;
    Char32Type = UnsignedLong;
    SigAtomicType = SignedChar;
    resetDataLayout("e-S16-p:16:16-i8:8-i16:16-m:e-n16");
  }

  ArrayRef<Builtin::Info> getTargetBuiltins() const override {
    return std::nullopt;
  }

  void getTargetDefines(const LangOptions &Opts,
                        MacroBuilder &Builder) const override{
    Builder.defineMacro("__AVR__");
  }

  std::string_view getClobbers() const override { return ""; }

  ArrayRef<const char *> getGCCRegNames() const override {
    static const char *const GCCRegNames[] = {
        // TOOD: should sp be there?
        "r0",  "r1",  "r2",  "r3",  "r4",  "r5",  "r6",  "r7","SP"};
    return llvm::ArrayRef(GCCRegNames);
  }

  ArrayRef<TargetInfo::GCCRegAlias> getGCCRegAliases() const override {
    return std::nullopt;
  }

  bool validateAsmConstraint(const char *&Name,
                             TargetInfo::ConstraintInfo &Info) const override {
    // There aren't any multi-character AVR specific constraints.
    if (StringRef(Name).size() > 1)
      return false;


    // TODO: no asm fo you :(
    return false;
  }
  BuiltinVaListKind getBuiltinVaListKind() const override {
    return TargetInfo::VoidPtrBuiltinVaList;
  }
};
} // namespace targets
} // namespace clang

#endif
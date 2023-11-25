//
// Created by ilya on 15.10.23.
//

#ifndef LLVM_CDMMCASMINFO_H
#define LLVM_CDMMCASMINFO_H

#include "llvm/MC/MCAsmInfo.h"
#include "llvm/TargetParser/Triple.h"
namespace llvm {

class CDMMCAsmInfo: public MCAsmInfo {
public:
  explicit CDMMCAsmInfo(const Triple &TheTriple);
};

} // namespace llvm

#endif // LLVM_CDMMCASMINFO_H

//
// Created by ilya on 21.11.23.
//

#ifndef LLVM_CDMINSTRINFO_H
#define LLVM_CDMINSTRINFO_H

#include "CDMRegisterInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "CDMGenInstrInfo.inc"

namespace llvm {

class CDMInstrInfo: public CDMGenInstrInfo {
public:
  explicit CDMInstrInfo();

  const CDMRegisterInfo &getRegisterInfo() const {
    return RI;
  }

private:
  const CDMRegisterInfo RI;
};

} // namespace llvm

#endif // LLVM_CDMINSTRINFO_H

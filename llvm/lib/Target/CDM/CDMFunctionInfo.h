//
// Created by ilya on 20.11.23.
//

#ifndef LLVM_CDMFUNCTIONINFO_H
#define LLVM_CDMFUNCTIONINFO_H

#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/PseudoSourceValue.h"
#include "llvm/Target/TargetMachine.h"
#include <map>

namespace llvm {

class CDMFunctionInfo : public MachineFunctionInfo{
public:
  CDMFunctionInfo(MachineFunction& MF)
      : MF(MF) {}

private:
  MachineFunction& MF;
};


} // namespace llvm

#endif // LLVM_CDMFUNCTIONINFO_H

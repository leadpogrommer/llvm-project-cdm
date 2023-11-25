//
// Created by ilya on 28.10.23.
//

#ifndef LLVM_CDMFRAMELOWERING_H
#define LLVM_CDMFRAMELOWERING_H


#include "CDM.h"
#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {

class CDMFrameLowering: public TargetFrameLowering {
public:

  explicit CDMFrameLowering(): TargetFrameLowering(StackGrowsDown, Align(2), 0, Align(2)){}
  void emitPrologue(MachineFunction &MF,
                    MachineBasicBlock &MBB) const override {
    // TODO
  }
  void emitEpilogue(MachineFunction &MF,
                    MachineBasicBlock &MBB) const override {
    // TODO
  }
  bool hasFP(const MachineFunction &MF) const override {
    // TODO: stub
    return false;
  }
};

} // namespace llvm

#endif // LLVM_CDMFRAMELOWERING_H

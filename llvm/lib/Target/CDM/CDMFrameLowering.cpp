//
// Created by ilya on 28.10.23.
//

#include "CDMFrameLowering.h"

#include "CDMInstrInfo.h"
#include "CDMSubtarget.h"
#include "MCTargetDesc/CDMMCTargetDesc.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

namespace llvm {

//- Must have, hasFP() is pure virtual of parent
// hasFP - Return true if the specified function should have a dedicated frame
// pointer register.  This is true if the function has variable sized allocas or
// if frame pointer elimination is disabled.
bool CDMFrameLowering::hasFP(const MachineFunction &MF) const {
  const MachineFrameInfo &MFI = MF.getFrameInfo();
  return MF.getTarget().Options.DisableFramePointerElim(MF) ||
         MFI.hasVarSizedObjects() || MFI.isFrameAddressTaken();
}

void CDMFrameLowering::emitPrologue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {
  MachineFrameInfo &MFI = MF.getFrameInfo();

  const CDMInstrInfo &TII =
      *static_cast<const CDMInstrInfo *>(STI.getInstrInfo());

  MachineBasicBlock::iterator MBBI = MBB.begin();
  DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();

  // First, compute final stack size.
  uint64_t StackSize = MFI.getStackSize();

  // No need to allocate space on the stack.
  if (StackSize == 0 && !MFI.adjustsStack())
    return;

  if (hasFP(MF)) {
    BuildMI(MBB, MBBI, DL, TII.get(CDM::PUSH)).addReg(CDM::FP);
    BuildMI(MBB, MBBI, DL, TII.get(CDM::LDSP)).addReg(CDM::FP);
    TII.adjustStackPtr(-StackSize, MBB, MBBI);
  }
}

void CDMFrameLowering::emitEpilogue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {
  MachineBasicBlock::iterator MBBI = MBB.getFirstTerminator();
  MachineFrameInfo &MFI = MF.getFrameInfo();

  const CDMInstrInfo &TII =
      *static_cast<const CDMInstrInfo *>(STI.getInstrInfo());

  DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();

  // Get the number of bytes from FrameInfo
  uint64_t StackSize = MFI.getStackSize();

  // This should match condition in emitEpilogue (But sometimes it generates
  // stackframe when it is not needed_
  if (StackSize == 0 && !MFI.adjustsStack())
    return;

  TII.adjustStackPtr(StackSize, MBB, MBBI);
  BuildMI(MBB, MBBI, DL, TII.get(CDM::POP)).addReg(CDM::FP);
}

MachineBasicBlock::iterator CDMFrameLowering::eliminateCallFramePseudoInstr(
    MachineFunction &MF, MachineBasicBlock &MBB,
    MachineBasicBlock::iterator MI) const {
  int Size = MI->getOperand(0).getImm();
  if (MI->getOpcode() == CDM::ADJCALLSTACKDOWN)
    Size = -Size;

  auto &TII =
      *static_cast<const CDMInstrInfo *>(MF.getSubtarget().getInstrInfo());

  if (Size)
    TII.adjustStackPtr(Size, MBB, MI);
  return MBB.erase(MI);
}
} // namespace llvm
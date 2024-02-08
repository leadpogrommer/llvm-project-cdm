//
// Created by ilya on 28.10.23.
//

#include "CDMFrameLowering.h"

#include "CDMFunctionInfo.h"
#include "CDMInstrInfo.h"
#include "CDMSubtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Target/TargetOptions.h"

namespace llvm {
void CDMFrameLowering::emitPrologue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {
  MachineFrameInfo &MFI    = MF.getFrameInfo();
  CDMFunctionInfo *CDMFI = MF.getInfo<CDMFunctionInfo>();

  const CDMInstrInfo &TII =
      *static_cast<const CDMInstrInfo*>(STI.getInstrInfo());
  const CDMRegisterInfo &RegInfo =
      *static_cast<const CDMRegisterInfo *>(STI.getRegisterInfo());

  MachineBasicBlock::iterator MBBI = MBB.begin();
  DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
//  Cpu0ABIInfo ABI = STI.getABI();
  unsigned SP = CDM::SP;
  const TargetRegisterClass *RC = &CDM::CPURegsRegClass;

  // First, compute final stack size.
  uint64_t StackSize = MFI.getStackSize();

  // No need to allocate space on the stack.
  if (StackSize == 0 && !MFI.adjustsStack()) return;
  BuildMI(MBB, MBBI, DL, TII.get(CDM::PUSH)).addReg(CDM::FP);
  BuildMI(MBB, MBBI, DL, TII.get(CDM::LDSP)).addReg(CDM::FP);
  TII.adjustStackPtr(-StackSize, MBB, MBBI);
  // Something for debugging from cpu0, idk
//  MachineModuleInfo &MMI = MF.getMMI();
//  const MCRegisterInfo *MRI = MMI.getContext().getRegisterInfo();
//
//  // Adjust stack.
//  TII.adjustStackPtr(SP, -StackSize, MBB, MBBI);
//
//  // emit ".cfi_def_cfa_offset StackSize"
//  unsigned CFIIndex =
//      MF.addFrameInst(
//          MCCFIInstruction::cfiDefCfaOffset(nullptr, StackSize));
//  BuildMI(MBB, MBBI, DL, TII.get(TargetOpcode::CFI_INSTRUCTION))
//      .addCFIIndex(CFIIndex);
//
//  const std::vector<CalleeSavedInfo> &CSI = MFI.getCalleeSavedInfo();
//
//  if (!CSI.empty()) {
//    // Find the instruction past the last instruction that saves a callee-saved
//    // register to the stack.
//    for (unsigned i = 0; i < CSI.size(); ++i)
//      ++MBBI;
//
//    // Iterate over list of callee-saved registers and emit .cfi_offset
//    // directives.
//    for (std::vector<CalleeSavedInfo>::const_iterator I = CSI.begin(),
//                                                      E = CSI.end(); I != E; ++I) {
//      int64_t Offset = MFI.getObjectOffset(I->getFrameIdx());
//      unsigned Reg = I->getReg();
//      {
//        // Reg is in CPURegs.
//        unsigned CFIIndex = MF.addFrameInst(MCCFIInstruction::createOffset(
//            nullptr, MRI->getDwarfRegNum(Reg, true), Offset));
//        BuildMI(MBB, MBBI, DL, TII.get(TargetOpcode::CFI_INSTRUCTION))
//            .addCFIIndex(CFIIndex);
//      }
//    }
//  }
}
void CDMFrameLowering::emitEpilogue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {
  MachineBasicBlock::iterator MBBI = MBB.getFirstTerminator();
  MachineFrameInfo &MFI            = MF.getFrameInfo();
  CDMFunctionInfo *Cpu0FI = MF.getInfo<CDMFunctionInfo>();

  const CDMInstrInfo &TII =
      *static_cast<const CDMInstrInfo *>(STI.getInstrInfo());
  const CDMRegisterInfo &RegInfo =
      *static_cast<const CDMRegisterInfo *>(STI.getRegisterInfo());

  DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();


  // Get the number of bytes from FrameInfo
  uint64_t StackSize = MFI.getStackSize();

  // This should match condition in emitEpilogue (But sometimes it generates stackframe when it is not needed_
  if (StackSize == 0 && !MFI.adjustsStack())
    return;

  TII.adjustStackPtr(StackSize, MBB, MBBI);
  BuildMI(MBB, MBBI, DL, TII.get(CDM::POP)).addReg(CDM::FP);
}
} // namespace llvm
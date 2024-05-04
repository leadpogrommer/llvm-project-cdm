//
// Created by ilya on 21.11.23.
//

#define DEBUG_TYPE "cdm-reg-info"

#include "CDMRegisterInfo.h"

#include "CDMFunctionInfo.h"
#include "CDMSubtarget.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

#define GET_REGINFO_TARGET_DESC
#include "CDMGenRegisterInfo.inc"

namespace llvm {

// TODO: what is this argument? IDK, on ARM it's LR, on x86 it is LR. WTF?
// I'l leave it as PC for now
CDMRegisterInfo::CDMRegisterInfo(): CDMGenRegisterInfo(CDM::PC) {}


BitVector CDMRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  static const std::array ReservedCPURegs {
      CDM::FP, CDM::SP, CDM::PSR, CDM::PC
  };

  for (unsigned I = 0; I < ReservedCPURegs.size(); ++I)
    Reserved.set(ReservedCPURegs[I]);

  return Reserved;
}
const MCPhysReg *
CDMRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  switch (MF->getFunction().getCallingConv()) {
  case CallingConv::C:
  case CallingConv::Fast:
  case CallingConv::Cold:
    return CSR_O16_SaveList;
  case CallingConv::CdmIsr:
    return CSR_O16_ALL_SaveList;
  }
  llvm_unreachable("Unknown calling convention");
}
bool CDMRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                          int SPAdj, unsigned int FIOperandNum,
                                          RegScavenger *RS) const {
  MachineInstr &MI = *II;
  MachineFunction &MF = *MI.getParent()->getParent();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  CDMFunctionInfo *Cpu0FI = MF.getInfo<CDMFunctionInfo>();

  unsigned i = 0;
  while (!MI.getOperand(i).isFI()) {
    ++i;
    assert(i < MI.getNumOperands() &&
           "Instr doesn't have FrameIndex operand!");
  }

  LLVM_DEBUG(errs() << "\nFunction : " << MF.getFunction().getName() << "\n";
             errs() << "<--------->\n" << MI);

  int FrameIndex = MI.getOperand(i).getIndex();
  uint64_t stackSize = MF.getFrameInfo().getStackSize();
  int64_t spOffset = MF.getFrameInfo().getObjectOffset(FrameIndex);

  LLVM_DEBUG(errs() << "FrameIndex : " << FrameIndex << "\n"
                    << "spOffset   : " << spOffset << "\n"
                    << "stackSize  : " << stackSize << "\n");
  // TODO: acknowledge saved regs and other stuff
  // TODO: handle incoming arguments
  MI.getOperand(i).ChangeToImmediate(spOffset);
//  llvm_unreachable("Unimplemented");
  return false; // instruction not removed
}
Register CDMRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return CDM::FP;
}
const uint32_t *
CDMRegisterInfo::getCallPreservedMask(const MachineFunction &MF,
                                      CallingConv::ID id) const {
  switch (id) {
  case CallingConv::C:
  case CallingConv::Fast:
  case CallingConv::Cold:
    return CSR_O16_RegMask;
  case CallingConv::CdmIsr:
    return CSR_O16_ALL_RegMask;
  }
  llvm_unreachable("Unknown calling convention");
}

} // namespace llvm
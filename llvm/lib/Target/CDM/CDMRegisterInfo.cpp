//
// Created by ilya on 21.11.23.
//

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
  return CSR_O16_SaveList;
}
bool CDMRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator MI,
                                          int SPAdj, unsigned int FIOperandNum,
                                          RegScavenger *RS) const {
  // TODO: implement this
  llvm_unreachable("Unimplemented");
  errs() << "";
  return false;
}
Register CDMRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return CDM::FP;
}
const uint32_t *
CDMRegisterInfo::getCallPreservedMask(const MachineFunction &MF,
                                      CallingConv::ID id) const {
  return CSR_O16_RegMask;
}

} // namespace llvm
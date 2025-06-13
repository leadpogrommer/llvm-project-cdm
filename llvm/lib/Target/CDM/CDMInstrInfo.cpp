//
// Created by ilya on 21.11.23.
//

#include "CDMInstrInfo.h"

#include "CDM.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
// #include "llvm/Support/TargetRegistry.h"

#define GET_INSTRINFO_CTOR_DTOR
#include "CDMGenInstrInfo.inc"

namespace llvm {
CDMInstrInfo::CDMInstrInfo()
    : CDMGenInstrInfo(CDM::ADJCALLSTACKDOWN, CDM::ADJCALLSTACKUP) {}

// needed for loading/saving regs in prologue/epilogue
void CDMInstrInfo::storeRegToStackSlot(MachineBasicBlock &MBB,
                                       MachineBasicBlock::iterator I,
                                       Register SrcReg, bool IsKill, int FI,
                                       const TargetRegisterClass *RC,
                                       const TargetRegisterInfo *TRI,
                                       Register VReg) const {
  DebugLoc DL;
  MachineMemOperand *MMO = GetMemOperand(MBB, FI, MachineMemOperand::MOStore);

  // TODO: understand this
  BuildMI(MBB, I, DL, get(CDM::ssw))
      .addReg(SrcReg, getKillRegState(IsKill))
      .addFrameIndex(FI)
      .addMemOperand(MMO);
}

// TODO: understand this
MachineMemOperand *
CDMInstrInfo::GetMemOperand(MachineBasicBlock &MBB, int FI,
                            MachineMemOperand::Flags Flags) const {
  MachineFunction &MF = *MBB.getParent();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  // Offset?
  return MF.getMachineMemOperand(MachinePointerInfo::getFixedStack(MF, FI),
                                 Flags, MFI.getObjectSize(FI),
                                 MFI.getObjectAlign(FI));
}
void CDMInstrInfo::loadRegFromStackSlot(MachineBasicBlock &MBB,
                                        MachineBasicBlock::iterator MI,
                                        Register DestReg, int FrameIndex,
                                        const TargetRegisterClass *RC,
                                        const TargetRegisterInfo *TRI,
                                        Register VReg) const {
  DebugLoc DL;
  if (MI != MBB.end())
    DL = MI->getDebugLoc();
  MachineMemOperand *MMO =
      GetMemOperand(MBB, FrameIndex, MachineMemOperand::MOLoad);

  // OFFSET?
  BuildMI(MBB, MI, DL, get(CDM::lsw), DestReg)
      .addFrameIndex(FrameIndex)
      .addMemOperand(MMO);
}
bool CDMInstrInfo::expandPostRAPseudo(MachineInstr &MI) const {
  MachineBasicBlock &MBB = *MI.getParent();

  switch (MI.getDesc().getOpcode()) {
  default:
    return false;
  case CDM::PseudoRet:
    expandRet(MBB, MI);
  }

  MBB.erase(MI);
  return true;
}
void CDMInstrInfo::expandRet(MachineBasicBlock &MBB,
                             MachineBasicBlock::iterator I) const {
  auto Opcode = CDM::rts;
  if (MBB.getParent()->getFunction().getCallingConv() == CallingConv::CdmIsr) {
    Opcode = CDM::rti;
  }
  BuildMI(MBB, I, I->getDebugLoc(), get(Opcode));
}
void CDMInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                               MachineBasicBlock::iterator MI,
                               const DebugLoc &DL, MCRegister DestReg,
                               MCRegister SrcReg, bool KillSrc) const {
  //  TargetInstrInfo::copyPhysReg(MBB, MI, DL, DestReg, SrcReg, KillSrc);
  if (SrcReg == CDM::SP) {
    MachineInstrBuilder MIB = BuildMI(MBB, MI, DL, get(CDM::LDSP));
    MIB.addReg(DestReg, RegState::Define);
    return;
  }
  assert(CDM::CPURegsRegClass.contains(SrcReg) &&
         CDM::CPURegsRegClass.contains(DestReg) &&
         "Can only copy General purpose regs");
  // TODO: check order
  MachineInstrBuilder MIB = BuildMI(MBB, MI, DL, get(CDM::MOVE));
  MIB.addReg(DestReg);
  MIB.addReg(SrcReg, getKillRegState(KillSrc));
}
void CDMInstrInfo::adjustStackPtr(int64_t Amount, MachineBasicBlock &MBB,
                                  MachineBasicBlock::iterator I) const {
  DebugLoc DL = I != MBB.end() ? I->getDebugLoc() : DebugLoc();

  BuildMI(MBB, I, DL, get(CDM::ADDSP)).addImm(Amount);
}
} // namespace llvm
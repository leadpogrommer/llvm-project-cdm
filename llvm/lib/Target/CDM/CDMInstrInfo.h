//
// Created by ilya on 21.11.23.
//

#ifndef LLVM_CDMINSTRINFO_H
#define LLVM_CDMINSTRINFO_H

#include "CDMRegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include <map>

#define GET_INSTRINFO_HEADER
#include "CDMGenInstrInfo.inc"
#include "llvm/CodeGen/ISDOpcodes.h"

namespace llvm {

namespace CDMCOND {
enum CondOp {
  // signed
  LT, // <
  LE, // <=
  GT, // >
  GE, // >=

  // unsigned
  LO, // <
  LS, // <=
  HI, // >
  HS, // >=

  // other
  EQ, // ==
  NE, // !=

};

} // namespace CDMCOND

class CDMInstrInfo : public CDMGenInstrInfo {
public:
  explicit CDMInstrInfo();

  const CDMRegisterInfo &getRegisterInfo() const { return RI; }
  void storeRegToStackSlot(MachineBasicBlock &MBB,
                           MachineBasicBlock::iterator I, Register SrcReg,
                           bool IsKill, int FI, const TargetRegisterClass *RC,
                           const TargetRegisterInfo *TRI,
                           Register VReg) const override;
  void loadRegFromStackSlot(MachineBasicBlock &MBB,
                            MachineBasicBlock::iterator MI, Register DestReg,
                            int FrameIndex, const TargetRegisterClass *RC,
                            const TargetRegisterInfo *TRI,
                            Register VReg) const override;

  bool expandPostRAPseudo(MachineInstr &MI) const override;
  void copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
                   const DebugLoc &DL, MCRegister DestReg, MCRegister SrcReg,
                   bool KillSrc) const override;

  void adjustStackPtr(int64_t Amount, MachineBasicBlock &MBB,
                      MachineBasicBlock::iterator I) const;
  CDMCOND::CondOp CCToCondOp(ISD::CondCode CC) const {
    if (!CondMap.count(CC)) {
      llvm_unreachable("Unknown branch condition");
    }
    return CondMap.at(CC);
  }

private:
  std::map<ISD::CondCode, CDMCOND::CondOp> CondMap = {
      {ISD::CondCode::SETLT, CDMCOND::LT},
      {ISD::CondCode::SETLE, CDMCOND::LE},
      {ISD::CondCode::SETGT, CDMCOND::GT},
      {ISD::CondCode::SETGE, CDMCOND::GE},
      {ISD::CondCode::SETULT, CDMCOND::LO},
      {ISD::CondCode::SETULE, CDMCOND::LS},
      {ISD::CondCode::SETUGT, CDMCOND::HI},
      {ISD::CondCode::SETUGE, CDMCOND::HS},
      {ISD::CondCode::SETEQ, CDMCOND::EQ},
      {ISD::CondCode::SETNE, CDMCOND::NE},
  };
  const CDMRegisterInfo RI;

  MachineMemOperand *GetMemOperand(MachineBasicBlock &MBB, int FI,
                                   MachineMemOperand::Flags Flags) const;
  void expandRet(MachineBasicBlock &MBB, MachineBasicBlock::iterator I) const;
};

} // namespace llvm

#endif // LLVM_CDMINSTRINFO_H

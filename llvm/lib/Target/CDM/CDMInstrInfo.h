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

namespace CDMCOND{
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

class CDMInstrInfo: public CDMGenInstrInfo {
public:
  explicit CDMInstrInfo();

  const CDMRegisterInfo &getRegisterInfo() const {
    return RI;
  }
  void storeRegToStackSlot(MachineBasicBlock &MBB, MachineBasicBlock::iterator I,
                           Register SrcReg, bool isKill, int FI,
                           const TargetRegisterClass *RC, const TargetRegisterInfo *TRI,
                           Register VReg) const override;
  void loadRegFromStackSlot(MachineBasicBlock &MBB,
                            MachineBasicBlock::iterator MI, Register DestReg,
                            int FrameIndex, const TargetRegisterClass *RC,
                            const TargetRegisterInfo *TRI,
                            Register VReg) const override;

  bool expandPostRAPseudo(MachineInstr &MI) const override;

      private:
  const CDMRegisterInfo RI;


  MachineMemOperand *GetMemOperand(MachineBasicBlock &MBB, int FI,
                               MachineMemOperand::Flags Flags) const;
  void expandRet(MachineBasicBlock &MBB,
                 MachineBasicBlock::iterator I) const;
};

} // namespace llvm

#endif // LLVM_CDMINSTRINFO_H

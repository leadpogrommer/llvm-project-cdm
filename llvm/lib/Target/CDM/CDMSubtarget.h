//
// Created by ilya on 21.10.23.
//

#ifndef LLVM_CDMSUBTARGET_H
#define LLVM_CDMSUBTARGET_H

#include "CDMISelLowering.h"


#include "CDMFrameLowering.h"
#include "CDMInstrInfo.h"
#include "llvm/TargetParser/Triple.h"
#include "llvm/CodeGen/SelectionDAGTargetInfo.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/IR/DataLayout.h"

#include "llvm/MC/MCInstrItineraries.h"
#define GET_SUBTARGETINFO_HEADER
#include "CDMGenSubtargetInfo.inc"

namespace llvm {


class CDMSubtarget : public CDMGenSubtargetInfo {
public:
  CDMSubtarget(const Triple &TT, StringRef CPU,
               StringRef FS, const CDMTargetMachine &_TM);
  void ParseSubtargetFeatures(StringRef CPU, StringRef TuneCPU, StringRef FS);
  const CDMISelLowering *getTargetLowering() const override {
    return TLInfo.get();
  }
  const TargetFrameLowering *getFrameLowering() const override;
  const CDMInstrInfo *getInstrInfo() const override {
    return InstrInfo.get();
  }
  const CDMRegisterInfo *getRegisterInfo() const override {
    return &(InstrInfo->getRegisterInfo());
  }

protected:
  std::unique_ptr<const CDMInstrInfo> InstrInfo;
  std::unique_ptr<const CDMFrameLowering> FrameLowering;
  std::unique_ptr<const CDMISelLowering> TLInfo;

};

} // namespace llvm

#endif // LLVM_CDMSUBTARGET_H

//
// Created by ilya on 21.10.23.
//

#include "CDMSubtarget.h"

// ..MachineFunction
// ...RegisterInfo
#include "CDMTargetMachine.h"

using namespace llvm;
#define DEBUG_TYPE "cdm-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "CDMGenSubtargetInfo.inc"

CDMSubtarget::CDMSubtarget(const Triple &TT, StringRef CPU, StringRef FS,
                           const CDMTargetMachine &TM)
    : CDMGenSubtargetInfo(TT, CPU, CPU, FS),
      InstrInfo(std::make_unique<CDMInstrInfo>()),
      FrameLowering(std::make_unique<CDMFrameLowering>(*this)),
      TLInfo(std::make_unique<CDMISelLowering>(TM, *this))

{}
const TargetFrameLowering *CDMSubtarget::getFrameLowering() const {
  return FrameLowering.get();
}

#ifndef LLVM_CDMMCTARGETDESC_H
#define LLVM_CDMMCTARGETDESC_H

#define GET_REGINFO_ENUM
#include "CDMGenRegisterInfo.inc"

// Defines symbolic names for the Cpu0 instructions.
#define GET_INSTRINFO_ENUM
#include "CDMGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "CDMGenSubtargetInfo.inc"

#endif // LLVM_CDMMCTARGETDESC_H

//
// Created by ilya on 16.10.23.
//

#include "CDMAsmPrinter.h"
#include "llvm/MC/TargetRegistry.h"
#include "TargetInfo/CDMTargetInfo.h"

using namespace llvm;

extern "C" void LLVMInitializeCDMAsmPrinter(){
  RegisterAsmPrinter<CDMAsmPrinter> X(getTheCDMTarget());
}
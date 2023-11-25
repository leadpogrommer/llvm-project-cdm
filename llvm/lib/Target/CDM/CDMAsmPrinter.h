//
// Created by ilya on 16.10.23.
//

#ifndef LLVM_CDMASMPRINTER_H
#define LLVM_CDMASMPRINTER_H

#include "llvm/MC/MCStreamer.h"
#include "llvm/CodeGen/AsmPrinter.h"
namespace llvm {

class CDMAsmPrinter: public AsmPrinter{
public:
  explicit CDMAsmPrinter(TargetMachine &TM,
                          std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)){}
};

} // namespace llvm

#endif // LLVM_CDMASMPRINTER_H

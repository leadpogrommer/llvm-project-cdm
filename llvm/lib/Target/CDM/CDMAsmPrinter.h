//
// Created by ilya on 16.10.23.
//

#ifndef LLVM_CDMASMPRINTER_H
#define LLVM_CDMASMPRINTER_H

#include "CDMMCInstLower.h"

#include "llvm/MC/MCStreamer.h"
#include "llvm/CodeGen/AsmPrinter.h"
namespace llvm {

class CDMAsmPrinter: public AsmPrinter{
  CDMMCInstLower MCInstLower;

public:
  explicit CDMAsmPrinter(TargetMachine &TM,
                          std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)),
        MCInstLower(*this)
  {}
  void emitFunctionEntryLabel() override;

  StringRef getPassName() const override {
    return "CDM Assembly Printer";
  }

  void emitInstruction(const MachineInstr *Instr) override;
  void emitFunctionBodyStart() override;
  void emitFunctionBodyEnd() override;
  void emitLinkage(const GlobalValue *GV, MCSymbol *GVSym) const override;
  //  void emitStartOfAsmFile(Module &module) override;
  void emitFunctionHeader() override;
  void emitStartOfAsmFile(Module &module) override;
  void emitEndOfAsmFile(Module &module) override;
};

class CDMAsmTargetStreamer: public MCTargetStreamer {

public:
  explicit CDMAsmTargetStreamer(MCStreamer &S);
  void changeSection(const MCSection *CurSection, MCSection *Section,
                     const MCExpr *SubSection, raw_ostream &OS) override;
  void emitLabel(MCSymbol *Symbol) override;
};

} // namespace llvm

#endif // LLVM_CDMASMPRINTER_H

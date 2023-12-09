//
// Created by ilya on 16.10.23.
//

#include "CDMInstPrinter.h"

#include "llvm/ADT/StringExtras.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"


using namespace llvm;
#define PRINT_ALIAS_INSTR
#include "CDMGenAsmWriter.inc"

void CDMInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                               StringRef Annot, const MCSubtargetInfo &STI,
                               raw_ostream &O) {
  // TODO: printAliasInst()?
  printInstruction(MI, Address, O);
  printAnnotation(O, Annot);
}

void CDMInstPrinter::printOperand(const MCInst *MI, unsigned int OpNo,
                                  raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isReg()) {
    printRegName(O, Op.getReg());
    return;
  }

  if (Op.isImm()) {
    O << Op.getImm();
    return;
  }

  llvm_unreachable("Unknown operand type");
}

void CDMInstPrinter::printMemOperand(const MCInst *MI, unsigned int OpNo,
                                     raw_ostream &O) {
  printOperand(MI, OpNo, O);
}
void CDMInstPrinter::printRegName(raw_ostream &OS, MCRegister Reg) const {
  OS << StringRef(const_cast<CDMInstPrinter*>(this)->getRegisterName(Reg));
}

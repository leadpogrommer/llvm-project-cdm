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

void CDMInstPrinter::printOperand(const MCInst *MI, unsigned int OpNo,
                                  raw_ostream &O) {
  // TODO: implement this
  O << "TODO: implement this";
}
void CDMInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                               StringRef Annot, const MCSubtargetInfo &STI,
                               raw_ostream &O) {
  // TODO: implement this
  O << "TODO: implement this";
}
void CDMInstPrinter::printMemOperand(const MCInst *MI, unsigned int OpNo,
                                     raw_ostream &O) {
  O << "TODO: mem operand";
}

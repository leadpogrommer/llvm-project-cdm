//
// Created by Ilya Merzlyakov on 04.12.2023.
//

#ifndef LLVM_CDMMCINSTLOWER_H
#define LLVM_CDMMCINSTLOWER_H

#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/MC/MCContext.h"
#include "llvm/Support/Compiler.h"

namespace llvm {
class CDMAsmPrinter;

class CDMMCInstLower {
  MCContext *Ctx;
  CDMAsmPrinter &AsmPrinter;

public:
  CDMMCInstLower(CDMAsmPrinter &asmPrinter);
  void Initialize(MCContext* C);
  void Lower(const MachineInstr *MI, MCInst &OutMI) const;
  MCOperand LowerOperand(const MachineOperand& MO, int offset = 0) const;
  MCOperand LowerSymbolOperand(const MachineOperand& MO, int offset = 0) const;
};

} // namespace llvm

#endif // LLVM_CDMMCINSTLOWER_H

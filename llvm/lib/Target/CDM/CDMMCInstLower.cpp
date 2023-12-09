//
// Created by Ilya Merzlyakov on 04.12.2023.
//

#include "CDMMCInstLower.h"

#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineOperand.h"

namespace llvm {
CDMMCInstLower::CDMMCInstLower(CDMAsmPrinter &asmPrinter): AsmPrinter(asmPrinter) {}
void CDMMCInstLower::Initialize(MCContext *C) {
  Ctx = C;
}
void CDMMCInstLower::Lower(const MachineInstr *MI, MCInst &OutMI) const {
  OutMI.setOpcode(MI->getOpcode());
  
  for(unsigned i = 0, e = MI->getNumOperands(); i != e; i++){
    const MachineOperand &MO = MI->getOperand(i);
    MCOperand MCOp = LowerOperand(MO);
    
    if(MCOp.isValid()){
      OutMI.addOperand(MCOp);
    }
  }
}
MCOperand CDMMCInstLower::LowerOperand(const MachineOperand &MO,
                                       unsigned int offset) const {
  auto MOType = MO.getType();
  switch (MOType) {
  default:
    llvm_unreachable("Unknown operand type");
  case MachineOperand::MachineOperandType::MO_Register:
    return MCOperand::createReg(MO.getReg());
  case MachineOperand::MachineOperandType::MO_Immediate:
    return MCOperand::createImm(MO.getImm());
  }
}
} // namespace llvm
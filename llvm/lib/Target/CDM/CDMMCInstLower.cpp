//
// Created by Ilya Merzlyakov on 04.12.2023.
//

#include "CDMMCInstLower.h"

#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "CDMAsmPrinter.h"

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
                                       int offset) const {
  auto MOType = MO.getType();
  switch (MOType) {
  default:
    llvm_unreachable("Unknown operand type");
  case MachineOperand::MachineOperandType::MO_Register:
    return MCOperand::createReg(MO.getReg());
  case MachineOperand::MachineOperandType::MO_Immediate:
    return MCOperand::createImm(MO.getImm());
  case MachineOperand::MO_RegisterMask:
    break;
  case MachineOperand::MO_MachineBasicBlock:
  case MachineOperand::MO_GlobalAddress:
  case MachineOperand::MO_ExternalSymbol:
  case MachineOperand::MO_JumpTableIndex:
    return LowerSymbolOperand(MO, offset);
  }
  return MCOperand();
}
MCOperand CDMMCInstLower::LowerSymbolOperand(const MachineOperand &MO,
                                             int Offset) const {
  MCSymbolRefExpr::VariantKind Kind = MCSymbolRefExpr::VK_None;
  const MCSymbol *Symbol;



  switch (MO.getType()) {
  case MachineOperand::MO_ExternalSymbol:
    Symbol = AsmPrinter.GetExternalSymbolSymbol(MO.getSymbolName());
    Offset += MO.getOffset();
    break;
  case MachineOperand::MO_GlobalAddress:
    Symbol = AsmPrinter.getSymbol(MO.getGlobal());
    Offset += MO.getOffset(); // Wtf is offset
    break;

  case MachineOperand::MO_MachineBasicBlock:
    Symbol = MO.getMBB()->getSymbol();
    break;

//  case MachineOperand::MO_BlockAddress:
//    Symbol = AsmPrinter.GetBlockAddressSymbol(MO.getBlockAddress());
//    Offset += MO.getOffset();
//    break;
//
  case MachineOperand::MO_JumpTableIndex:
    Symbol = AsmPrinter.GetJTISymbol(MO.getIndex());
    break;

  default:
    llvm_unreachable("<unknown operand type>");
  }

  const MCExpr *Expr = MCSymbolRefExpr::create(Symbol, Kind, *Ctx);

  if (Offset) {
    // Assume offset is never negative.
//    llvm_unreachable("I am still unsure what is an offset");

    Expr = Offset > 0 ? MCBinaryExpr::createAdd(Expr, MCConstantExpr::create(Offset, *Ctx),
                                   *Ctx) : MCBinaryExpr::createSub(Expr, MCConstantExpr::create(-Offset, *Ctx), *Ctx);
  }


  return MCOperand::createExpr(Expr);
}
} // namespace llvm
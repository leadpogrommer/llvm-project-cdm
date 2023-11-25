//
// Created by ilya on 21.10.23.
//

#include "CDMIselDAGToDAG.h"


#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/CodeGen/SelectionDAGNodes.h"


using namespace llvm;

char CDMDagToDagIsel::ID = 0;
StringRef CDMDagToDagIsel::getPassName() const { return "CDM DAG->DAG Bullshit Instruction Selection"; }
void CDMDagToDagIsel::Select(SDNode *N) {
  // TODO: actually select
}

FunctionPass *llvm::createCDMISelDag(llvm::CDMTargetMachine &TM, CodeGenOpt::Level OptLevel) {
  return new CDMDagToDagIsel(TM);
}
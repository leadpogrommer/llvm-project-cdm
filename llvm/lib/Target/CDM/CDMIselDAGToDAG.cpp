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
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "cdm-isel"

char CDMDagToDagIsel::ID = 0;
StringRef CDMDagToDagIsel::getPassName() const { return "CDM DAG->DAG Bullshit Instruction Selection"; }
void CDMDagToDagIsel::Select(SDNode *N) {

  if(N->isMachineOpcode()) {
    LLVM_DEBUG(errs() << "== Something fucked up; selecting already selected node"; N->dump(CurDAG); errs() << "\n");
    N->setNodeId(-1);
    return;
  }

  SelectCode(N);

}
bool CDMDagToDagIsel::runOnMachineFunction(MachineFunction &MF) {
  return SelectionDAGISel::runOnMachineFunction(MF);
}
bool CDMDagToDagIsel::trySelect(SDNode *Node) {
  return false;   // TODO: actually select
}
bool CDMDagToDagIsel::SelectAddr(SDNode *Parent, SDValue Addr, SDValue &Base,
                                 SDValue &Offset) {
  EVT ValTy = Addr.getValueType();
  SDLoc DL(Addr);

  if(FrameIndexSDNode *FIN = dyn_cast<FrameIndexSDNode>(Addr)){
    Base = CurDAG->getTargetFrameIndex(FIN->getIndex(), ValTy);
    Offset = CurDAG->getTargetConstant(0, DL, ValTy);
    return true;
  }

  // Cant load not from stack yet
  LLVM_DEBUG(errs() << "Cant select address");
  return false;

}

FunctionPass *llvm::createCDMISelDag(llvm::CDMTargetMachine &TM, CodeGenOpt::Level OptLevel) {
  return new CDMDagToDagIsel(TM);
}
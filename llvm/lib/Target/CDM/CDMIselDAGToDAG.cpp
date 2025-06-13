//
// Created by ilya on 21.10.23.
//

#include "CDMIselDAGToDAG.h"

#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/CodeGen/SelectionDAGNodes.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "cdm-isel"

char CDMDagToDagIsel::ID = 0;
StringRef CDMDagToDagIsel::getPassName() const {
  return "CDM DAG->DAG Bullshit Instruction Selection";
}
void CDMDagToDagIsel::Select(SDNode *N) {

  if (N->isMachineOpcode()) {
    LLVM_DEBUG(
        errs() << "== Something fucked up; selecting already selected node";
        N->dump(CurDAG); errs() << "\n");
    N->setNodeId(-1);
    return;
  }

  if (N->getOpcode() == ISD::BR_CC) {
    SelectConditionalBranch(N);
    return;
  }
  if (N->getOpcode() == ISD::BRCOND) {
    SelectBRCOND(N);
    return;
  }

  SelectCode(N);
}
bool CDMDagToDagIsel::runOnMachineFunction(MachineFunction &MF) {
  return SelectionDAGISel::runOnMachineFunction(MF);
}
bool CDMDagToDagIsel::trySelect(SDNode *Node) {
  return false; // TODO: actually select
}
bool CDMDagToDagIsel::SelectAddrFrameIndex(SDNode *Parent, SDValue Addr,
                                           SDValue &Base, SDValue &Offset) {
  EVT ValTy = Addr.getValueType();
  SDLoc DL(Addr);

  if (FrameIndexSDNode *FIN = dyn_cast<FrameIndexSDNode>(Addr)) {
    Base = CurDAG->getTargetFrameIndex(FIN->getIndex(), ValTy);
    Offset = CurDAG->getTargetConstant(0, DL, ValTy);
    return true;
  }

  // Cant load not from stack yet
  LLVM_DEBUG(errs() << "[LEADP] Cant select frame address\n");
  return false;
}

// This is manual solution
// Until I figure out how to use tablegen for this
// TODO: !important! use tablegen for branches
bool CDMDagToDagIsel::SelectConditionalBranch(SDNode *N) {
  SDValue Chain = N->getOperand(0);
  SDValue Cond = N->getOperand(1);
  SDValue LHS = N->getOperand(2);
  SDValue RHS = N->getOperand(3);
  SDValue Target = N->getOperand(4);

  CondCodeSDNode *CC = cast<CondCodeSDNode>(Cond.getNode());

  // TODO: deduplicate this map
  std::map<ISD::CondCode, CDMCOND::CondOp> CondMap = {
      {ISD::CondCode::SETLT, CDMCOND::LT},
      {ISD::CondCode::SETLE, CDMCOND::LE},
      {ISD::CondCode::SETGT, CDMCOND::GT},
      {ISD::CondCode::SETGE, CDMCOND::GE},
      {ISD::CondCode::SETULT, CDMCOND::LO},
      {ISD::CondCode::SETULE, CDMCOND::LS},
      {ISD::CondCode::SETUGT, CDMCOND::HI},
      {ISD::CondCode::SETUGE, CDMCOND::HS},
      {ISD::CondCode::SETEQ, CDMCOND::EQ},
      {ISD::CondCode::SETNE, CDMCOND::NE},
  };

  if (!CondMap.count(CC->get())) {
    LLVM_DEBUG(errs() << "Unknown branch condition");
    return false;
  }

  EVT CompareTys[] = {MVT::Other, MVT::Glue};
  SDVTList CompareVT = CurDAG->getVTList(CompareTys);
  SDValue CompareOps[] = {LHS, RHS, Chain};
  // TODO: long compare???
  SDNode *Compare = CurDAG->getMachineNode(CDM::CMP, N, CompareVT, CompareOps);

  SDValue CCVal = CurDAG->getTargetConstant(CondMap[CC->get()], N, MVT::i32);
  SDValue BranchOps[] = {CCVal, Target, SDValue(Compare, 0),
                         SDValue(Compare, 1)};

  CurDAG->SelectNodeTo(N, CDM::BCond, MVT::Other, BranchOps);

  return true;
}

bool CDMDagToDagIsel::SelectBRCOND(llvm::SDNode *N) {
  SDValue Chain = N->getOperand(0);
  SDValue Cond = N->getOperand(1);
  SDValue Target = N->getOperand(2);

  EVT TstTys[] = {MVT::Other, MVT::Glue};
  SDVTList TstVT = CurDAG->getVTList(TstTys);
  SDValue TstOps[] = {Cond, Chain};
  SDNode *TST = CurDAG->getMachineNode(CDM::TST, N, TstVT, TstOps);

  SDValue CCVal =
      CurDAG->getTargetConstant(CDMCOND::NE, N, MVT::i32); // NE == NZ
  SDValue BranchOps[] = {CCVal, Target, SDValue(TST, 0), SDValue(TST, 1)};

  CurDAG->SelectNodeTo(N, CDM::BCond, MVT::Other, BranchOps);

  return true;
}

bool CDMDagToDagIsel::SelectAddr(SDNode *Parent, SDValue Addr, SDValue &Base,
                                 SDValue &Offset) {
  if (isa<FrameIndexSDNode>(Addr)) {
    return false;
  }

  if (Addr->getOpcode() == ISD::GlobalAddress) {
    Base = Addr;
    Offset = CurDAG->getTargetConstant(0, SDLoc(Addr), Addr.getValueType());
    return true;
  }

  // TODO: maybe I should be more careful here
  Base = Addr;
  Offset = CurDAG->getTargetConstant(0, SDLoc(Addr), Addr.getValueType());
  return true;
  //  LLVM_DEBUG(errs() << "[LEADP] Cant select address\n");
  //  return false;
}

// thanks, Sparc
bool CDMDagToDagIsel::SelectAddrRR(SDValue Addr, SDValue &Base,
                                   SDValue &Offset) {
  if (Addr.getOpcode() == ISD::ADD) {
    Base = Addr.getOperand(0);
    Offset = Addr.getOperand(1);
    return true;
  }

  return false;
}

FunctionPass *llvm::createCDMISelDag(llvm::CDMTargetMachine &TM,
                                     CodeGenOptLevel OptLevel) {
  return new CDMDagToDagIsel(TM);
}
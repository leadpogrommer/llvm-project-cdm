//
// Created by ilya on 21.10.23.
//

#ifndef LLVM_CDMISELDAGTODAG_H
#define LLVM_CDMISELDAGTODAG_H


#include "CDM.h"
#include "CDMTargetMachine.h"
#include "CDMSubtarget.h"

#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/Pass.h"

namespace llvm{
class CDMDagToDagIsel : public  SelectionDAGISel {
public:
  CDMDagToDagIsel() = delete;

  explicit CDMDagToDagIsel(CDMTargetMachine &TM) : SelectionDAGISel(TM) {}

  bool runOnMachineFunction(MachineFunction &MF) override;

  void Select(SDNode *N) override;
  bool trySelect(SDNode *Node);
  bool SelectAddrFrameIndex(SDNode *Parent, SDValue Addr, SDValue &Base, SDValue &Offset);
  bool SelectAddr(SDNode *Parent, SDValue Addr, SDValue &Base, SDValue &Offset);
  bool SelectAddrRR(SDValue Addr, SDValue &Base, SDValue &Offset);
  bool SelectConditionalBranch(SDNode *N);
  bool SelectBRCOND(SDNode *N);

  #include "CDMGenDAGISel.inc"
};

class CDMDagToDagIselLegacy : public SelectionDAGISelLegacy {
public:
  static char ID;
  explicit CDMDagToDagIselLegacy(CDMTargetMachine &tm)
      : SelectionDAGISelLegacy(ID, std::make_unique<CDMDagToDagIsel>(tm)) {}
};

void initializeCDMDagToDagIselLegacyPass(PassRegistry &);

FunctionPass *createCDMISelDag(CDMTargetMachine &TM, CodeGenOptLevel OptLevel);

} // namespace llvm

#endif // LLVM_CDMISELDAGTODAG_H

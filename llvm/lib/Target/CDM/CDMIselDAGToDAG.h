//
// Created by ilya on 21.10.23.
//

#ifndef LLVM_CDMISELDAGTODAG_H
#define LLVM_CDMISELDAGTODAG_H

#include "CDMTargetMachine.h"

#include "llvm/CodeGen/SelectionDAGISel.h"

namespace llvm {
class CDMDagToDagIsel : public SelectionDAGISel {
public:
  static char ID;

  explicit CDMDagToDagIsel(CDMTargetMachine &TM) : SelectionDAGISel(ID, TM) {}

  StringRef getPassName() const override;
  bool runOnMachineFunction(MachineFunction &MF) override;

private:
#include "CDMGenDAGISel.inc"

  void Select(SDNode *N) override;
  bool trySelect(SDNode *Node);
  bool SelectAddrFrameIndex(SDNode *Parent, SDValue Addr, SDValue &Base,
                            SDValue &Offset);
  bool SelectAddr(SDNode *Parent, SDValue Addr, SDValue &Base, SDValue &Offset);
  bool SelectAddrRR(SDValue Addr, SDValue &Base, SDValue &Offset);
  bool SelectConditionalBranch(SDNode *N);
  bool SelectBRCOND(SDNode *N);
};

FunctionPass *createCDMISelDag(CDMTargetMachine &TM, CodeGenOptLevel OptLevel);

} // namespace llvm

#endif // LLVM_CDMISELDAGTODAG_H

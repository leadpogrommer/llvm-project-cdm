//
// Created by ilya on 21.10.23.
//

#ifndef LLVM_CDMISELDAGTODAG_H
#define LLVM_CDMISELDAGTODAG_H


#include "CDM.h"
#include "CDMTargetMachine.h"
#include "CDMSubtarget.h"

#include "llvm/CodeGen/SelectionDAGISel.h"


namespace llvm{
class CDMDagToDagIsel : public  SelectionDAGISel {
public:
  static char ID;

  explicit CDMDagToDagIsel(CDMTargetMachine &TM):
                                                                         SelectionDAGISel(ID, TM){}
  StringRef getPassName() const override;

private:
  #include "CDMGenDAGISel.inc"

  void Select(SDNode *N) override;
};

FunctionPass *createCDMISelDag(CDMTargetMachine &TM, CodeGenOpt::Level OptLevel);

} // namespace llvm



#endif // LLVM_CDMISELDAGTODAG_H

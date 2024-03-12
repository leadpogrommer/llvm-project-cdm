#ifndef LLVM_LIB_TARGET_CDM_CDMTARGETMACHINE_H
#define LLVM_LIB_TARGET_CDM_CDMTARGETMACHINE_H


#include "CDMSubtarget.h"
#include "llvm/Target/TargetMachine.h"


namespace llvm{

class CDMTargetMachine: public LLVMTargetMachine{
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  const DataLayout dataLayout;
  CDMSubtarget DefaultSubtarget;
public:
  CDMTargetMachine(const Target &T, const Triple &TT,
                   StringRef CPU, StringRef FS,
                   const TargetOptions &Options,
                   std::optional<Reloc::Model> RM,
                   std::optional<CodeModel::Model> CM,
                   CodeGenOptLevel OL, bool JIT);
  ~CDMTargetMachine() override;

  virtual const DataLayout *getDataLayout() const { return &dataLayout; }
  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;
  TargetLoweringObjectFile *getObjFileLowering() const override{
    return TLOF.get();
  }
  const CDMSubtarget *getSubtargetImpl(const Function &F) const override{
    return &DefaultSubtarget;
  }
};

} // namespace llvm
#endif
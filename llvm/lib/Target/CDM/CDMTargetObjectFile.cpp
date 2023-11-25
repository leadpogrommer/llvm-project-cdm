#include "CDMTargetObjectFile.h"

namespace llvm {
void CDMTargetObjectFile::Initialize(MCContext &Ctx, const TargetMachine &TM) {
  TargetLoweringObjectFileELF::Initialize(Ctx, TM);
  // TODO: we don't need ELF support now, this is a stub
}
} // namespace llvm
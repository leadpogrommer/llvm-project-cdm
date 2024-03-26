//
// Created by ilya on 16.10.23.
//

#include "CDMAsmPrinter.h"
#include "TargetInfo/CDMTargetInfo.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/ADT/Twine.h"
#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Mangler.h"
#include "llvm/IR/Module.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Format.h"
#include "llvm/Support/FormatVariadic.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetLoweringObjectFile.h"
#include "llvm/Target/TargetOptions.h"
#include <set>

using namespace llvm;

extern "C" void LLVMInitializeCDMAsmPrinter(){
  RegisterAsmPrinter<CDMAsmPrinter> X(getTheCDMTarget());
}
void CDMAsmPrinter::emitInstruction(const MachineInstr *Instr) {
  if (Instr->isDebugValue()) {
    // TODO: implement
    return;
  }

  MachineBasicBlock::const_instr_iterator I = Instr->getIterator();
  MachineBasicBlock::const_instr_iterator E = Instr->getParent()->instr_end();

  // TODO: figure out why there's a loop
  do {
    if (I->isPseudo())
      llvm_unreachable("Pseudo opcode found in emitInstruction()");

    MCInst TmpInst0;
    MCInstLower.Lower(&*I, TmpInst0);
    OutStreamer->emitInstruction(TmpInst0, getSubtargetInfo());
  } while ((++I != E) && I->isInsideBundle());
}
void CDMAsmPrinter::emitFunctionBodyStart() {
  MCInstLower.Initialize(&MF->getContext());
  // TODO
}
void CDMAsmPrinter::emitFunctionBodyEnd() {
  // TODO
}
void CDMAsmPrinter::emitFunctionEntryLabel() {
  OutStreamer->emitLabel(CurrentFnSym);
//  OutStreamer->emitRawText(llvm::formatv("{0}>", CurrentFnSym->getName()));
}
void CDMAsmPrinter::emitLinkage(const GlobalValue *GV, MCSymbol *GVSym) const {
  // not needed (stub)
}
void CDMAsmPrinter::emitFunctionHeader() {
  // If something is missing, check original implementation
  // We don't want to emit anything here, but we want to preserve some of the original functionality
  const Function &F = MF->getFunction();

  OutStreamer->getCommentOS()
      << "-- Begin function "
      << GlobalValue::dropLLVMManglingEscape(F.getName()) << '\n';

  // Print out constants referenced by the function
  emitConstantPool();

  if (MF->front().isBeginSection())
    MF->setSection(getObjFileLowering().getUniqueSectionForFunction(F, TM));
  else
    MF->setSection(getObjFileLowering().SectionForGlobal(&F, TM));
  OutStreamer->switchSection(MF->getSection());

  emitFunctionEntryLabel();

}
void CDMAsmPrinter::emitStartOfAsmFile(Module &module) {
    auto FN = module.getSourceFileName();

    std::replace_if(FN.begin(), FN.end(), [](char C){return !(isAlnum(C) || C == '_');}, '_');
    OutStreamer -> emitRawText(llvm::formatv("rsect _{0}_{1}\n\n", FN, rand()));

    std::set<std::string> prefixes_to_ignore = {"llvm.lifetime."};

    for(auto &GV: module.global_objects()){
      auto Linkage = GV.getLinkage();
      if (GV.isDeclaration() and std::find_if(prefixes_to_ignore.begin(), prefixes_to_ignore.end(), [&](auto pref){return GV.getName().starts_with(pref);}) == prefixes_to_ignore.end()){
        OutStreamer ->emitRawText(llvm::formatv("{0}: ext\n", GV.getName()));
      }
    }

    // TODO: this is a fake move. Remove this when actual movens is implemented
    OutStreamer ->emitRawText("\n\nmacro movens/2\npush $1\npop $2\nmend\n\n");
}
void CDMAsmPrinter::emitEndOfAsmFile(Module &module) {
  OutStreamer-> emitRawText("end.");
}
//void CDMAsmPrinter::emitStartOfAsmFile(Module &module) {
////  AsmPrinter::emitStartOfAsmFile(module);
//}

// TODO: implement target streamer
CDMAsmTargetStreamer::CDMAsmTargetStreamer(MCStreamer &S): MCTargetStreamer(S) {}
void CDMAsmTargetStreamer::emitLabel(MCSymbol *Symbol) {

}
void CDMAsmTargetStreamer::changeSection(const MCSection *CurSection,
                                         MCSection *Section,
                                         const MCExpr *SubSection,
                                         raw_ostream &OS) {
  // This is a stub. We don't have sections in cdm
  OS << llvm::formatv("### SECTION: {0}\n", Section->getName());
}

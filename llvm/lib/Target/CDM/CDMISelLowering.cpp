//
// Created by ilya on 21.10.23.
//

#include "CDMISelLowering.h"
#include "CDMTargetMachine.h"
#include "CDMSubtarget.h"

using namespace llvm;

CDMISelLowering::CDMISelLowering(const CDMTargetMachine &TM,
                                 const CDMSubtarget &ST)
: TargetLowering(TM), Subtarget(ST){
          addRegisterClass(MVT::i16, &CDM::CPURegsRegClass);

          computeRegisterProperties(Subtarget.getRegisterInfo());

          setBooleanContents(ZeroOrOneBooleanContent);

         setOperationAction(ISD::BR_CC, MVT::i16, Expand);

        //  setOperationAction(ISD::SELECT_CC, MVT::i16, Expand);
          // setOperationAction(ISD::SELECT, MVT::i16, Expand);
          // setOperationAction(ISD::SETCC, MVT::i16, Expand);
//          setOperationAction(ISD::SELECT_CC, MVT::i16, Custom);

          setOperationAction(ISD::GlobalAddress, MVT::i16, Custom);

          setOperationAction(ISD::BR_JT,             MVT::Other, Expand);
          setOperationAction(ISD::JumpTable,          MVT::i16,   Custom);

}

#include "CDMFunctionInfo.h"
#include "CDMGenCallingConv.inc"


// Mostly taken from llvm-leg
SDValue CDMISelLowering::LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
                                              const SmallVectorImpl<ISD::InputArg> &Ins,
                                              const SDLoc &DL, SelectionDAG &DAG,
                                              SmallVectorImpl<SDValue> &InVals) const {
  auto &MF = DAG.getMachineFunction();
  auto &MFI = MF.getFrameInfo();
  auto &RegInfo = MF.getRegInfo();

  assert(!IsVarArg && "VarArg is not supported");

  // Assign locations to all of the incoming arguments.
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), ArgLocs,
                 *DAG.getContext());

  CCInfo.AnalyzeFormalArguments(Ins, CC_CDM);


  for(auto &VA: ArgLocs){
    if(VA.isRegLoc()){
      EVT RegVT = VA.getLocVT();
      assert(RegVT.getSimpleVT().SimpleTy == MVT::i16 && "Only support 16-bit register passing");
      const unsigned VReg = RegInfo.createVirtualRegister(&CDM::CPURegsRegClass);
      RegInfo.addLiveIn(VA.getLocReg(), VReg);
      SDValue ArgIn = DAG.getCopyFromReg(Chain, DL, VReg, RegVT);
      InVals.push_back(ArgIn);

      continue;
    }

    llvm_unreachable("Arguments on stack are not supported yet");
  }


  return Chain;
}
SDValue
CDMISelLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                             bool IsVarArg,
                             const SmallVectorImpl<ISD::OutputArg> &Outs,
                             const SmallVectorImpl<SDValue> &OutVals,
                             const SDLoc &DL, SelectionDAG &DAG) const {
  SmallVector<CCValAssign, 16> RVLocs;
  MachineFunction &MF = DAG.getMachineFunction();

  // CCState - Info about the registers and stack slot.
  CCState CCInfo(CallConv, IsVarArg, MF, RVLocs,
                 *DAG.getContext());

  // In example this loop is in Cpu0TargetLowering::Cpu0CC::analyzeReturn
  // Maybe I should do CCState::AllocateStack
  for(unsigned I = 0, E = Outs.size(); I < E; ++I){
    MVT VT = Outs[I].VT;
    ISD::ArgFlagsTy Flags = Outs[I].Flags;
    MVT RegVT = MVT::i16;
    if(RetCC_CDM(I, VT, RegVT, CCValAssign::Full, Flags, CCInfo)){
      dbgs() << "Call result #" << I << " has unhandled type "
             << EVT(VT).getEVTString() << '\n';
      llvm_unreachable("Oops");
    }
  }
  // End of copypaste from other class

  SDValue Flag;
  SmallVector<SDValue, 4> RetOps(1, Chain);

  // Copy the result values into the output registers.
  for (unsigned i = 0; i != RVLocs.size(); ++i) {
    SDValue Val = OutVals[i];
    CCValAssign &VA = RVLocs[i];
    assert(VA.isRegLoc() && "Can only return in registers!");

    if (RVLocs[i].getValVT() != RVLocs[i].getLocVT())
      Val = DAG.getNode(ISD::BITCAST, DL, RVLocs[i].getLocVT(), Val);

    Chain = DAG.getCopyToReg(Chain, DL, VA.getLocReg(), Val, Flag);

    // Guarantee that all emitted copies are stuck together with flags.
    Flag = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(VA.getLocReg(), VA.getLocVT()));
  }

  //@Ordinary struct type: 2 {
  // The cpu0 ABIs for returning structs by value requires that we copy
  // the sret argument into $v0 for the return. We saved the argument into
  // a virtual register in the entry block, so now we copy the value out
  // and into $v0.
  if (MF.getFunction().hasStructRetAttr()) {
    llvm_unreachable("No support for SRet yet, sorry");
//    Cpu0FunctionInfo *Cpu0FI = MF.getInfo<Cpu0FunctionInfo>();
//    unsigned Reg = Cpu0FI->getSRetReturnReg();
//
//    if (!Reg)
//      llvm_unreachable("sret virtual register not created in the entry block");
//    SDValue Val =
//        DAG.getCopyFromReg(Chain, DL, Reg, getPointerTy(DAG.getDataLayout()));
//    unsigned V0 = Cpu0::V0;
//
//    Chain = DAG.getCopyToReg(Chain, DL, V0, Val, Flag);
//    Flag = Chain.getValue(1);
//    RetOps.push_back(DAG.getRegister(V0, getPointerTy(DAG.getDataLayout())));
  }
  //@Ordinary struct type: 2 }

  RetOps[0] = Chain;  // Update chain.

  // Add the flag if we have it.
  if (Flag.getNode())
    RetOps.push_back(Flag);

  // Return on Cpu0 is always a "ret $lr"
  return DAG.getNode(CDMISD::Ret, DL, MVT::Other, RetOps);
}


bool CDMISelLowering::CanLowerReturn(CallingConv::ID CallingConv, MachineFunction &MF, bool IsVarArg,
                                     const SmallVectorImpl<ISD::OutputArg> &Outs,
                                     LLVMContext &Context) const {
  SmallVector<CCValAssign, 16> RVLocs;
  CCState CCInfo(CallingConv,IsVarArg, MF, RVLocs, Context);
  return CCInfo.CheckReturn(Outs, RetCC_CDM);
}

#define NODE_NAME(x) case CDMISD::x: return "CDMISD::"#x
const char *CDMISelLowering::getTargetNodeName(unsigned int Opcode) const {
  switch (Opcode) {
    NODE_NAME(Ret);
    NODE_NAME(Call);
    NODE_NAME(LOAD_SYM);
    default: return NULL;
  }
}


// Mostly taken from llvm-leg
// TODO: use code from cpu0, not leg
SDValue CDMISelLowering::LowerCall(TargetLowering::CallLoweringInfo &CLI,
                                   SmallVectorImpl<SDValue> &InVals) const {
  SelectionDAG &DAG = CLI.DAG;
  SDLoc &Loc = CLI.DL;
  SmallVectorImpl<ISD::OutputArg> &Outs = CLI.Outs;
  SmallVectorImpl<SDValue> &OutVals = CLI.OutVals;
  SmallVectorImpl<ISD::InputArg> &Ins = CLI.Ins;
  SDValue Chain = CLI.Chain;
  SDValue Callee = CLI.Callee;
  CallingConv::ID CallConv = CLI.CallConv;
  const bool IsVarArg = CLI.IsVarArg;
  MachineFunction &MF = DAG.getMachineFunction();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  const TargetFrameLowering *TFL = MF.getSubtarget().getFrameLowering();

  CLI.IsTailCall = false;

  if (IsVarArg) {
    llvm_unreachable("VarArg unimplemented");
  }

  // Analyze operands of the call, assigning locations to each operand.
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), ArgLocs,
                 *DAG.getContext());
  CCInfo.AnalyzeCallOperands(Outs, CC_CDM);

  // Get the size of the outgoing arguments stack space requirement.
  unsigned NextStackOffset = CCInfo.getStackSize();
  unsigned StackAlignment = TFL->getStackAlignment();
  NextStackOffset = alignTo(NextStackOffset, StackAlignment);
  SDValue NextStackOffsetVal = DAG.getIntPtrConstant(NextStackOffset, Loc, true);

  Chain = DAG.getCALLSEQ_START(Chain, NextStackOffset, 0, Loc);

  SmallVector<std::pair<unsigned, SDValue>, 8> RegsToPass;
  SmallVector<SDValue, 8> MemOpChains;

  // Walk the register/memloc assignments, inserting copies/loads.
  for (unsigned i = 0, e = ArgLocs.size(); i != e; ++i) {
    CCValAssign &VA = ArgLocs[i];
    SDValue Arg = OutVals[i];

    // We only handle fully promoted arguments.
    assert(VA.getLocInfo() == CCValAssign::Full && "Unhandled loc info");

    if (VA.isRegLoc()) {
      RegsToPass.push_back(std::make_pair(VA.getLocReg(), Arg));
      continue;
    }
    llvm_unreachable("Stack operands are not supported yet");

  }

  // Emit all stores, make sure they occur before the call.
  if (!MemOpChains.empty()) {
    Chain = DAG.getNode(ISD::TokenFactor, Loc, MVT::Other, MemOpChains);
  }

  // Build a sequence of copy-to-reg nodes chained together with token chain
  // and flag operands which copy the outgoing args into the appropriate regs.
  SDValue InFlag;
  for (auto &Reg : RegsToPass) {
    Chain = DAG.getCopyToReg(Chain, Loc, Reg.first, Reg.second, InFlag);
    InFlag = Chain.getValue(1);
  }

  // We only support calling global addresses.
  GlobalAddressSDNode *G = dyn_cast<GlobalAddressSDNode>(Callee);
  assert(G && "We only support the calling of global addresses");

  EVT PtrVT = getPointerTy(DAG.getDataLayout());
  Callee = DAG.getTargetGlobalAddress(G->getGlobal(), Loc, PtrVT, 0);

  std::vector<SDValue> Ops;
  Ops.push_back(Chain);
  Ops.push_back(Callee);

  // Add argument registers to the end of the list so that they are known live
  // into the call.
  for (auto &Reg : RegsToPass) {
    Ops.push_back(DAG.getRegister(Reg.first, Reg.second.getValueType()));
  }

  // Add a register mask operand representing the call-preserved registers.
  const uint32_t *Mask;
  const TargetRegisterInfo *TRI = DAG.getSubtarget().getRegisterInfo();
  Mask = TRI->getCallPreservedMask(DAG.getMachineFunction(), CallConv);

  assert(Mask && "Missing call preserved mask for calling convention");
  Ops.push_back(DAG.getRegisterMask(Mask));

  if (InFlag.getNode()) {
    Ops.push_back(InFlag);
  }

  SDVTList NodeTys = DAG.getVTList(MVT::Other, MVT::Glue);

  // Returns a chain and a flag for retval copy to use.
  Chain = DAG.getNode(CDMISD::Call, Loc, NodeTys, Ops);
  InFlag = Chain.getValue(1);

  Chain = DAG.getCALLSEQ_END(Chain, DAG.getIntPtrConstant(NextStackOffset, Loc, true),
                             DAG.getIntPtrConstant(0, Loc, true), InFlag, Loc);
  if (!Ins.empty()) {
    InFlag = Chain.getValue(1);
  }

  // Handle result values, copying them out of physregs into vregs that we
  // return.
  return LowerCallResult(Chain, InFlag, CallConv, IsVarArg, Ins, Loc, DAG,
                         InVals);

}
SDValue CDMISelLowering::LowerCallResult(
    SDValue Chain, SDValue InGlue, CallingConv::ID CallConv, bool isVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, SDLoc dl, SelectionDAG &DAG,
    SmallVectorImpl<SDValue> &InVals) const {
  assert(!isVarArg && "Unsupported");

  // Assign locations to each value returned by this call.
  SmallVector<CCValAssign, 16> RVLocs;
  CCState CCInfo(CallConv, isVarArg, DAG.getMachineFunction(), RVLocs,
                 *DAG.getContext());

  CCInfo.AnalyzeCallResult(Ins, RetCC_CDM);

  // Copy all of the result registers out of their specified physreg.
  for (auto &Loc : RVLocs) {
    Chain = DAG.getCopyFromReg(Chain, dl, Loc.getLocReg(), Loc.getValVT(),
                               InGlue).getValue(1);
    InGlue = Chain.getValue(2);
    InVals.push_back(Chain.getValue(0));
  }

  return Chain;
}
SDValue CDMISelLowering::LowerOperation(SDValue Op, SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
    case ISD::GlobalAddress: return lowerGlobalAddress(Op, DAG);
    case ISD::JumpTable: return lowerJumpTable(Op, DAG);
    }
  return SDValue();
}
SDValue CDMISelLowering::lowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const {
  EVT VT = Op.getValueType();
  GlobalAddressSDNode *GlobalAddr = cast<GlobalAddressSDNode>(Op.getNode());
  SDValue TargetAddr =
      DAG.getTargetGlobalAddress(GlobalAddr->getGlobal(), Op, MVT::i16, GlobalAddr->getOffset());
  return DAG.getNode(CDMISD::LOAD_SYM, Op, VT, TargetAddr);
}
SDValue CDMISelLowering::lowerJumpTable(SDValue Op, SelectionDAG &DAG) const {
  EVT VT = Op.getValueType();
  JumpTableSDNode *N = cast<JumpTableSDNode>(Op);
  // TODO: check if value type is correct
  SDValue TargetJumpTable = DAG.getTargetJumpTable(N->getIndex(), VT, 0);
  return DAG.getNode(CDMISD::LOAD_SYM, Op, VT, TargetJumpTable);
}

// Thanks https://github.com/llvm/llvm-project/commit/65385167fbb4d30fcdddf54102b08fcb1b497fed
MachineBasicBlock *
CDMISelLowering::EmitInstrWithCustomInserter(MachineInstr &MI,
                                             MachineBasicBlock *MBB) const {

  // assert(MI.getOpcode() == CDM::PseudoSelectCC && "Unexpected instr type to insert");

  // const CDMInstrInfo &TII = *(const CDMInstrInfo *)MBB->getParent()->getSubtarget().getInstrInfo();
  // DebugLoc DL = MI.getDebugLoc();

  // auto Dst = MI.getOperand(0);
  // auto Lhs = MI.getOperand(1);
  // auto Rhs = MI.getOperand(2);
  // auto TrueVal = MI.getOperand(3);
  // auto FalseVal = MI.getOperand(4);
  // auto CondCode = static_cast<ISD::CondCode>(MI.getOperand(5).getImm());

  // const BasicBlock *LLVM_BB = MBB->getBasicBlock();

  // MachineBasicBlock *HeadMBB = MBB;
  // MachineFunction *F = MBB->getParent();
  // MachineBasicBlock *TailMBB = F->CreateMachineBasicBlock(LLVM_BB);
  // MachineBasicBlock *IfFalseMBB = F->CreateMachineBasicBlock(LLVM_BB);

  // MachineFunction::iterator I = ++MBB->getIterator();

  // F->insert(I, IfFalseMBB);
  // F->insert(I, TailMBB);

  // TailMBB->splice(TailMBB->begin(), HeadMBB,
  //                  std::next(MachineBasicBlock::iterator(MI)), HeadMBB->end());

  // TailMBB->transferSuccessorsAndUpdatePHIs(HeadMBB);
  // HeadMBB->addSuccessor(IfFalseMBB);
  // HeadMBB->addSuccessor(TailMBB);
  // IfFalseMBB->addSuccessor(TailMBB);

  // MachineInstr* CMPInst = BuildMI(HeadMBB, DL, TII.get(CDM::CMP))
  //     .addReg(Lhs.getReg())
  //     .addReg(Rhs.getReg())
  //     .getInstr();

  // // TODO: check if glue needed

  // BuildMI(HeadMBB, DL, TII.get(CDM::BCond))
  //     .addImm(TII.CCToCondOp(CondCode))
  //     .addMBB(TailMBB);

  // BuildMI(*TailMBB, TailMBB->begin(), DL, TII.get(CDM::PHI),
  //         Dst.getReg())
  //     .addReg(TrueVal.getReg())
  //     .addMBB(HeadMBB)
  //     .addReg(FalseVal.getReg())
  //     .addMBB(IfFalseMBB);

  // MI.eraseFromParent();

  // return TailMBB;

  return nullptr;
}

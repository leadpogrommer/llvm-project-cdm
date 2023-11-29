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
}

#include "CDMFunctionInfo.h"
#include "CDMGenCallingConv.inc"

SDValue CDMISelLowering::LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
                                              const SmallVectorImpl<ISD::InputArg> &Ins,
                                              const SDLoc &DL, SelectionDAG &DAG,
                                              SmallVectorImpl<SDValue> &InVals) const {
  auto &MF = DAG.getMachineFunction();
  auto &MFI = MF.getFrameInfo();
//  MF.dump();
//  CDMFunctionInfo *CDMFI = MF.getInfo<CDMFunctionInfo>();
//
//  SmallVector<CCValAssign, 16> ArgLocs;



  // TODO: this is as stub (ch 3.5)
//  return TargetLowering::LowerFormalArguments(value, id, b, vector, loc, dag,
//                                              smallVector);
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
    default: return NULL;
  }
}

// custom SDnode for return

def SDT_CDMRet          : SDTypeProfile<0, 1, [SDTCisInt<0>]>;
def CDMRet               : SDNode<"CDMISD::Ret", SDTNone, [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

def SDT_CDMCall    : SDTypeProfile<0, -1, [SDTCisVT<0, iPTR>]>;
def CDMCall    : SDNode<"CDMISD::Call", SDT_CDMCall, 
  [SDNPHasChain, SDNPOutGlue, SDNPOptInGlue,
                          SDNPVariadic]
>;


def addr: ComplexPattern<iPTR, 2, "SelectAddr", [frameindex], [SDNPWantParent]>;

class CDMInst<dag outs, dag ins, string asmstr, list<dag> pattern>: Instruction
{
  // Inst and Size: for tablegen(... -gen-emitter) and 
  // tablegen(... -gen-disassembler) in CMakeLists.txt

  let Namespace = "CDM";

  let Size = 2;


  let OutOperandList = outs;
  let InOperandList  = ins;

  let AsmString   = asmstr;
  let Pattern     = pattern;
  let hasDelaySlot = 0;
  // TODO: right now we don't need opcodes and stuff
}

class CDMPseudo<dag outs, dag ins, string asmstr, list<dag> pattern>:
      CDMInst<outs, ins, asmstr, pattern> {
  let isCodeGenOnly = 1;
  let isPseudo = 1;
}

class arithLogicR<string asm_instr, SDNode OpNode, bit isComm = 0>:
  CDMInst<(outs CPURegs:$rd), (ins CPURegs:$rs0, CPURegs:$rs1),
          !strconcat(asm_instr, " $rs0, $rs1, $rd"),
          [(set CPURegs:$rd, (OpNode CPURegs:$rs0, CPURegs:$rs1))]>{
            let isCommutable = isComm;
            let isReMaterializable = 1;
          }

def shamt       : Operand<i16>;
def immZext3: ImmLeaf<i16, [{return (Imm > 0) && (Imm <= 8);}]>;
def SHL: CDMInst<(outs CPURegs:$rd), (ins CPURegs:$rs, shamt:$shamt),
        "shl\t$rs, $rd, $shamt",
        [(set CPURegs:$rd, (shl CPURegs:$rs, immZext3:$shamt))]
>;

// load
class AlignedLoad<PatFrag Node> :
  PatFrag<(ops node:$ptr), (Node node:$ptr), [{
  LoadSDNode *LD = cast<LoadSDNode>(N);
  return LD->getMemoryVT().getSizeInBits()/8 <= LD->getAlign().value();
}]>;
class AlignedStore<PatFrag Node> :
  PatFrag<(ops node:$val, node:$ptr), (Node node:$val, node:$ptr), [{
  StoreSDNode *SD = cast<StoreSDNode>(N);
  return SD->getMemoryVT().getSizeInBits()/8 <= SD->getAlign().value();
}]>;


def load_a          : AlignedLoad<load>;
def store_a         : AlignedStore<store>;

class LoadMStack<string instr_asm, PatFrag OpNode, Operand MemOperand, bit Pseudo>: 
    CDMInst<(outs CPURegs:$ra), (ins MemOperand:$addr), 
      !strconcat(instr_asm, "\t$ra, $addr"),
      [(set CPURegs:$ra,(OpNode addr:$addr))]>{
        let isPseudo = Pseudo;
      }

class StoreMStack<string instr_asm, PatFrag OpNode, Operand MemOperand, bit Pseudo>:
    CDMInst<(outs), (ins CPURegs:$ra, MemOperand:$addr),
            !strconcat(instr_asm, "\t$ra, $addr"),
            [(OpNode CPURegs:$ra, addr:$addr)]>{
              let isPseudo = Pseudo;
            }

def simm16      : Operand<i16> {

}

// def uimm16: Operand<ui16>{

// }

def mem : Operand<iPTR> {
  let PrintMethod = "printMemOperand";
  // Why not more(GRPOUT)
  let MIOperandInfo = (ops CPURegs, simm16);
  let EncoderMethod = "getMemEncoding";
}

def lsw : LoadMStack<"lsw", load_a, mem, 0>;
// store
def ssw: StoreMStack<"ssw", store_a, mem, 0>;


// ldi

def imm16: PatLeaf<(imm), [{return isInt<16>(N->getValue());}]>;
class LoadImm<Operand Imm>:
  CDMInst<(outs CPURegs:$ra), (ins Imm:$imm),
  "ldi\t$ra, $imm", []>{
    // TODO; what is this?
    let isReMaterializable = 1;
  }

def ldi: LoadImm<simm16>;


let isReturn=1, isTerminator=1, isBarrier=1, hasCtrlDep=1 in
  def PseudoRet : CDMPseudo<(outs), (ins), "", [(CDMRet)]>;


def ADD: arithLogicR<"add", add, 1>;

/*
 TODO: RET, RetLR
 LD: LoadM32 -> LoadM -> FMem -> FL -> CpuoInst -> Instruction
 My LD: LoadM32 -> LoadM -> CDMInst
*/

def my_cool_pat_for_ldi: Pat<(i16 imm:$imm),(ldi imm:$imm)>;


class RetBase: CDMInst<(outs), (ins), "rts", []> {
    // TODO: check this flags
    let isReturn = 1;
    let isCodeGenOnly = 1;
    let hasCtrlDep = 1;
    let hasExtraSrcRegAllocReq = 1;
    let isBranch=1;
    let isTerminator=1;
    let isBarrier=1;
}

def rts: RetBase;

def brtarget: Operand<OtherVT> {
  let OperandType = "OPERAND_MEMORY";
}

def BR: CDMInst<(outs), (ins brtarget:$addr), "br\t$addr", [(br bb:$addr)]>{
  let isBranch = 1;
  let isTerminator = 1;
  let isBarrier = 1;
}

// class CondBR<string instr_asm>: CDMInst<(outs), (ins SR:$psr, brtarget:$addr),
//                                         !strconcat(instr_asm, "\t$addr"), []>{
//                                           let isBranch = 1;
//                                           let isTerminator = 1;
//                                         }

def cond_val: Operand<OtherVT> {
  let PrintMethod = "printCondCode";
}

def BCond: CDMInst<(outs), (ins cond_val:$cond, brtarget:$addr), "b$cond\t$addr", []>{
  let isTerminator = 1;
  let isBranch = 1;
  let Uses = [PSR]; 
}

def CMP: CDMInst<(outs), (ins CPURegs:$ra, CPURegs:$rb),
                "cmp\t$ra, $rb", []>{
                  let Defs = [PSR];
                }
                
// def : Pat<(brcond (i16 (setlt CPURegs:$ra, CPURegs:$rb)), bb:$dst),
//           (BLT (CMP CPURegs:$ra, CPURegs:$rb), bb:$dst)>;
def calltarget  : Operand<iPTR> {
  let OperandType = "OPERAND_PCREL";
  // let PrintMethod = "printCallTarget";
}

def JSR: CDMInst<(outs), (ins calltarget:$target, variable_ops), 
                  "jsr\t$target", []>{
                    let isCall = 1;
}  

def : Pat<(CDMCall (i16 tglobaladdr:$target)), (JSR tglobaladdr:$target)>;
def : Pat<(CDMCall (i16 texternalsym:$target)), (JSR texternalsym:$target)>;


def uimm16       : Operand<i16>;
def SDT_CDMCallSeqStart : SDCallSeqStart<[SDTCisVT<0, i16>]>;
def SDT_CDMCallSeqEnd   : SDCallSeqEnd<[SDTCisVT<0, i16>, SDTCisVT<1, i16>]>; 
// These are target-independent nodes, but have target-specific formats.
def callseq_start : SDNode<"ISD::CALLSEQ_START", SDT_CDMCallSeqStart,
                           [SDNPHasChain, SDNPOutGlue]>;
def callseq_end   : SDNode<"ISD::CALLSEQ_END", SDT_CDMCallSeqEnd,
                           [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>; 

let Defs = [SP], Uses = [SP] in { 
def ADJCALLSTACKDOWN : CDMPseudo<(outs), (ins uimm16:$amt1, uimm16:$amt2),
                                  "!ADJCALLSTACKDOWN $amt1",
                                  [(callseq_start timm:$amt1, timm:$amt2)]>;
def ADJCALLSTACKUP   : CDMPseudo<(outs), (ins uimm16:$amt1, uimm16:$amt2),
                                  "!ADJCALLSTACKUP $amt1",
                                  [(callseq_end timm:$amt1, timm:$amt2)]>;  
} 

def MOVE : CDMInst<(outs CPURegs:$rd), (ins CPURegs:$rs), "move\t$rs, $rd", []>;
// custom SDnode for return

def SDT_CDMRet          : SDTypeProfile<0, 1, [SDTCisInt<0>]>;
def CDMRet               : SDNode<"CDMISD::Ret", SDTNone, [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

def SDT_CDMCall    : SDTypeProfile<0, -1, [SDTCisVT<0, iPTR>]>;
def CDMCall    : SDNode<"CDMISD::Call", SDT_CDMCall, 
  [SDNPHasChain, SDNPOutGlue, SDNPOptInGlue,
                          SDNPVariadic]
>;

// TODO: select addr not on frame (CDMDagToDagIsel)
def addr_on_frame: ComplexPattern<iPTR, 2, "SelectAddrFrameIndex", [frameindex], [SDNPWantParent]>;

def addrRR: ComplexPattern<iPTR, 2, "SelectAddrRR", [], []>;
def addr: ComplexPattern<iPTR, 2, "SelectAddr", [], [SDNPWantParent]>;

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
            let Defs = [PSR];
          }

def ADD: arithLogicR<"add", add, 1>;
def SUB: arithLogicR<"sub", sub, 0>;

def AND: arithLogicR<"and", and, 1>;
def OR: arithLogicR<"or", or, 1>;
def XOR: arithLogicR<"xor", xor, 1>;


def immZext6: ImmLeaf<i16, [{return (Imm >= 0) && (Imm <= 63);}]>;
def negimmZext6: ImmLeaf<i16, [{return (Imm <= 0) && (Imm >= -63);}]>;

// thanks avr
def imm16_neg_XFORM
    : SDNodeXForm<imm, [{
                    return CurDAG->getTargetConstant(-N->getAPIntValue(),
                                                     SDLoc(N), MVT::i16);
                  }]>;
// thanks avr
def imm6_neg : PatLeaf<(imm), [{
                            int64_t val = -N->getSExtValue();
                            return val >= 0 && val < 64;
                          }],
                          imm16_neg_XFORM>;

class arithLogicRi<string asm_instr, SDNode OpNode>: 
  CDMInst<(outs CPURegs:$rd), (ins CPURegs:$rs, i16imm:$imm),
          !strconcat(asm_instr, "\t$rs, $imm"),
          [(set CPURegs:$rd, (OpNode CPURegs:$rs, immZext6:$imm))]>{
            let isReMaterializable = 1;
            let Defs = [PSR];
            let Constraints = "$rs = $rd";
          }
// end thanks avr

def ADDI: arithLogicRi<"add", add>;
def SUBI: arithLogicRi<"sub", sub>;

def : Pat<(add i16
           : $src1, imm6_neg
           : $src2),
          (SUBI i16
           : $src1, (imm16_neg_XFORM imm
                     : $src2))>;

// def: Pat <(i16 (extloadi8 addr:$src)), (ldb addr:$src)>;


// def : Pattern<(i16 (load_sym tglobaladdr:$addr)),  [(ldi $addr)]>;


def shamt       : Operand<i16>;
def immZext3: ImmLeaf<i16, [{return (Imm > 0) && (Imm <= 8);}]>;
def SHL: CDMInst<(outs CPURegs:$rd), (ins CPURegs:$rs, shamt:$shamt),
        "shl\t$rs, $rd, $shamt",
        [(set CPURegs:$rd, (shl CPURegs:$rs, immZext3:$shamt))]
>{
  let Defs = [PSR];
}

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

class LoadMStack<string instr_asm, PatFrag OpNode, Operand MemOperand, ComplexPattern AddrPattern, bit Pseudo>: 
    CDMInst<(outs CPURegs:$ra), (ins MemOperand:$addr), 
      !strconcat(instr_asm, "\t$ra, $addr"),
      [(set CPURegs:$ra,(OpNode AddrPattern:$addr))]>{
        let isPseudo = Pseudo;
      }

class StoreMStack<string instr_asm, PatFrag OpNode, Operand MemOperand, ComplexPattern AddrPattern, bit Pseudo>:
    CDMInst<(outs), (ins CPURegs:$ra, MemOperand:$addr),
            !strconcat(instr_asm, "\t$ra, $addr"),
            [(OpNode CPURegs:$ra, AddrPattern:$addr)]>{
              let isPseudo = Pseudo;
            }

def simm16      : Operand<i16> {

}

// def uimm16: Operand<ui16>{

// }

// Selected in SelectAddr
def mem : Operand<iPTR> {
  let PrintMethod = "printMemOperand";
  // Why not more(GRPOUT)
  let MIOperandInfo = (ops CPURegs, simm16);  // second operand SHOULD be always zero
  let EncoderMethod = "getMemEncoding";
}

def memRR: Operand<iPTR> {
  let PrintMethod = "printMemRROperand";
  let MIOperandInfo = (ops CPURegs, CPURegs);
}


let Uses = [FP] in {
// stack words
def lsw : LoadMStack<"lsw", load_a, mem, addr_on_frame, 0>;
def ssw: StoreMStack<"ssw", store_a, mem, addr_on_frame, 0>;
// stack bytes
def ssb: StoreMStack<"ssb", truncstorei8, mem, addr_on_frame, 0>;
def lsb:  LoadMStack<"lsb", zextloadi8, mem, addr_on_frame, 0>;
def lssb: LoadMStack<"lssb", sextloadi8, mem, addr_on_frame, 0>;
} 
def: Pat <(i16 (extloadi8 addr_on_frame:$src)), (lsb addr_on_frame:$src)>;

// def stw: StoreM<"stw", store, mem, addr, 0>;


// TODO: use TABLES?
class LoadM<string instr_asm, PatFrag OpNode, Operand MemOperand, ComplexPattern AddrPattern, bit Pseudo>: 
    CDMInst<(outs CPURegs:$ra), (ins MemOperand:$addr), 
      !strconcat(instr_asm, "\t$addr, $ra"),
      [(set CPURegs:$ra,(OpNode AddrPattern:$addr))]>{
        let isPseudo = Pseudo;
      }

class StoreM<string instr_asm, PatFrag OpNode, Operand MemOperand, ComplexPattern AddrPattern, bit Pseudo>:
    CDMInst<(outs), (ins CPURegs:$ra, MemOperand:$addr),
            !strconcat(instr_asm, "\t$addr, $ra"),
            [(OpNode CPURegs:$ra, AddrPattern:$addr)]>{
              let isPseudo = Pseudo;
            }

// WORD store
def stwRR: StoreM<"stw", store, memRR, addrRR, 0>;
def stw: StoreM<"stw", store, mem, addr, 0>;
// WORD load
def ldwRR: LoadM<"ldw", load, memRR, addrRR, 0>;
def ldw: LoadM<"ldw", load, mem, addr, 0>;

// BYTE load zero extend
def ldbRR: LoadM<"ldb", zextloadi8, memRR, addrRR, 0>;
def ldb: LoadM<"ldb", zextloadi8, mem, addr, 0>;
// BYTE load sign extend
def ldsbRR: LoadM<"ldsb", sextloadi8, memRR, addrRR, 0>;
def ldsb: LoadM<"ldsb", sextloadi8, mem, addr, 0>;
// BYTE store truncate
def stbRR: StoreM<"stb", truncstorei8, memRR, addrRR, 0>;
def stb: StoreM<"stb", truncstorei8, mem, addr, 0>;


def: Pat <(i16 (extloadi8 addrRR:$src)), (ldbRR addrRR:$src)>;
def: Pat <(i16 (extloadi8 addr:$src)), (ldb addr:$src)>;



// TODO: ArithLogic?
// TODO: Why not just sext?
def Sext: CDMInst<(outs CPURegs:$rd), (ins CPURegs:$rs), "sxt\t$rs, $rd", []>{
  let Defs = [PSR];

}

def : Pat<(i16 (sext_inreg i16:$val, i8)), (Sext $val)>;

// sextloadi8
// zextloadi8
// truncstorei8

// def stw: CDMInst<(outs), (ins CPURegs:$ra, mem:$addr), "stw\t$addr, $ra", [(store CPURegs:$ra, addr:$addr)]>;
// def ldw: CDMInst<(outs CPURegs:$ra), (ins mem:$addr), "ldw\t$addr, $ra", [(set CPURegs:$ra,(load addr:$addr))]>;
// ldi

def imm16: PatLeaf<(imm), [{return isInt<16>(N->getValue());}]>;
class LoadImm<Operand Imm>:
  CDMInst<(outs CPURegs:$ra), (ins Imm:$imm),
  "ldi\t$ra, $imm", []>{
    // TODO; what is this?
    let isReMaterializable = 1;
    // COSTYL
    // let Defs = [PSR];
  }

def ldi: LoadImm<simm16>;


let isReturn=1, isTerminator=1, isBarrier=1, hasCtrlDep=1 in
  def PseudoRet : CDMPseudo<(outs), (ins), "", [(CDMRet)]>;



/*
 TODO: RET, RetLR
 LD: LoadM32 -> LoadM -> FMem -> FL -> CpuoInst -> Instruction
 My LD: LoadM32 -> LoadM -> CDMInst
*/
def load_sym : SDNode<"CDMISD::LOAD_SYM", SDTIntUnaryOp>;
def : Pattern<(i16 (load_sym tglobaladdr:$addr)),  [(ldi $addr)]>;
def : Pattern<(i16 (load_sym tjumptable:$addr)),  [(ldi $addr)]>;


def my_cool_pat_for_ldi: Pat<(i16 imm:$imm),(ldi imm:$imm)>;


class RetBase<string instr_asm>: CDMInst<(outs), (ins), instr_asm, []> {
    // TODO: check this flags
    let isReturn = 1;
    let isCodeGenOnly = 1;
    let hasCtrlDep = 1;
    let hasExtraSrcRegAllocReq = 1;
    let isBranch=1;
    let isTerminator=1;
    let isBarrier=1;
}

def rts: RetBase<"rts">;
def rti: RetBase<"rti">;

def brtarget: Operand<OtherVT> {
  let OperandType = "OPERAND_MEMORY";
}

def BR: CDMInst<(outs), (ins brtarget:$addr), "br\t$addr", [(br bb:$addr)]>{
  let isBranch = 1;
  let isTerminator = 1;
  let isBarrier = 1;
}

// branch indirect
def stpc: CDMInst<(outs), (ins CPURegs:$rs), "stpc\t$rs", [(brind CPURegs:$rs)]>{
  let isBranch = 1;
  let isTerminator = 1;
  let isBarrier = 1;
  let isIndirectBranch = 1;
}


// def 
// class CondBR<string instr_asm>: CDMInst<(outs), (ins SR:$psr, brtarget:$addr),
//                                         !strconcat(instr_asm, "\t$addr"), []>{
//                                           let isBranch = 1;
//                                           let isTerminator = 1;
//                                         }

def cond_val: Operand<OtherVT> {
  let PrintMethod = "printCondCode";
}


def cond_as_i16imm: SDNodeXForm<cond, [{
  return CurDAG->getTargetConstant(N->get(), SDLoc(N), MVT::i16);
}]>;

// def PseudoSelectCC: CDMInst<(outs CPURegs:$dst), (ins CPURegs:$lhs, CPURegs:$rhs, CPURegs:$vtrue, CPURegs:$vfalse, cond_val:$cnd), "",[
//   // (set CPURegs:$dst, (selectcc CPURegs:$lhs, CPURegs:$rhs, CPURegs:$vtrue, CPURegs:$vfalse, (cond_as_i16imm cond:$cnd)))
// ]>{
//   let isPseudo = true;
//   let usesCustomInserter = true;
// }

// def : Pat<(selectcc CPURegs:$lhs, CPURegs:$rhs, CPURegs:$vtrue, CPURegs:$vfalse, cond:$cnd),
// (PseudoSelectCC CPURegs:$lhs, CPURegs:$rhs, CPURegs:$vtrue, CPURegs:$vfalse, (cond_as_i16imm $cnd))
// >;

// def BCond: CDMInst<(outs), (ins cond_val:$cond, brtarget:$addr), "b$cond\t$addr", []>{
//   let isTerminator = 1;
//   let isBranch = 1;
//   let Uses = [PSR]; 
// }

class CondBranch<string instr_asm> : CDMInst<(outs), (ins SR:$rd, brtarget:$addr),
            !strconcat(instr_asm, "\t$addr\t\t# use $rd"),
            []> {
              let isTerminator = 1;
              let isBranch = 1;
              // let Uses = [PSR]; 
            }

// class CondBranch<string instr_asm, RegisterClass RC,
//                    list<Register> UseRegs>:
//   FJ<op, (outs), (ins cond_val:$cond, brtarget:$addr),
//              !strconcat(instr_asm, "\t$ra, $addr"),
//              [], IIBranch>, Requires<[HasCmp]> {
//   let isBranch = 1;
//   let isTerminator = 1;
//   let hasDelaySlot = 1;
// //  let Predicates = [HasCmp]; // same effect as Requires
// }

def BEQ     : CondBranch<"beq">;
def BNE     : CondBranch<"bne">;
def BLT     : CondBranch<"blt">;
def BGT     : CondBranch<"bgt">;
def BLE     : CondBranch<"ble">;
def BGE     : CondBranch<"bge">;

def CMP: CDMInst<(outs SR:$rdef), (ins CPURegs:$ra, CPURegs:$rb),
                "cmp\t$ra, $rb\t\t# def $rdef", []>{
                  // let Defs = [PSR];
                }

// def TST: CDMInst<(outs), (ins CPURegs:$rs), "tst\t$rs", []>{
//   let Defs = [PSR];
// }

multiclass BrcondPatsCmp<RegisterClass RC, Instruction JEQOp, Instruction JNEOp, 
Instruction JLTOp, Instruction JGTOp, Instruction JLEOp, Instruction JGEOp, 
Instruction CMPOp> {
  def : Pat<(brcond (i16 (seteq RC:$lhs, RC:$rhs)), bb:$dst),
            (JEQOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;
  def : Pat<(brcond (i16 (setueq RC:$lhs, RC:$rhs)), bb:$dst),
            (JEQOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;
  def : Pat<(brcond (i16 (setne RC:$lhs, RC:$rhs)), bb:$dst),
            (JNEOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;
  def : Pat<(brcond (i16 (setune RC:$lhs, RC:$rhs)), bb:$dst),
            (JNEOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;
  def : Pat<(brcond (i16 (setlt RC:$lhs, RC:$rhs)), bb:$dst),
            (JLTOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;
  def : Pat<(brcond (i16 (setult RC:$lhs, RC:$rhs)), bb:$dst),
            (JLTOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;
  def : Pat<(brcond (i16 (setgt RC:$lhs, RC:$rhs)), bb:$dst),
            (JGTOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;
  def : Pat<(brcond (i16 (setugt RC:$lhs, RC:$rhs)), bb:$dst),
            (JGTOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;
  def : Pat<(brcond (i16 (setle RC:$lhs, RC:$rhs)), bb:$dst),
            (JLEOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;
  def : Pat<(brcond (i16 (setule RC:$lhs, RC:$rhs)), bb:$dst),
            (JLEOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;
  def : Pat<(brcond (i16 (setge RC:$lhs, RC:$rhs)), bb:$dst),
            (JGEOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;
  def : Pat<(brcond (i16 (setuge RC:$lhs, RC:$rhs)), bb:$dst),
            (JGEOp (CMPOp RC:$lhs, RC:$rhs), bb:$dst)>;

  // def : Pat<(brcond RC:$cond, bb:$dst),
  //         (JNEOp (TST RC:$cond), bb:$dst)>;
}

defm : BrcondPatsCmp<CPURegs, BEQ, BNE, BLT, BGT, BLE, BGE, CMP>;

// TODO: maybe use PSR from ALU  instructions in branches?
// def : Pat<(brcond CPURegs:$cond, brtarget:$addr), ()>mo
                
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

// TODO: convert to ArithLogic ???
def MOVE : CDMInst<(outs SR:$psr, CPURegs:$rd), (ins CPURegs:$rs), "movens\t$rs, $rd\t\t# def $psr", []>{
  // let Defs = [PSR];
}

// push, pop, ldsp, addsp
// TODO: this is not selected automatically, probably there should be something more
let Uses = [SP], Defs = [SP] in {
def POP : CDMInst<(outs CPURegs:$rd), (ins), "pop\t$rd", []>{
  let mayLoad = 1;
}

def PUSH : CDMInst<(outs), (ins CPURegs:$rs), "push\t$rs", []>{
  let mayStore = 1;
}
}

def LDSP: CDMInst<(outs CPURegs:$rd), (ins), "ldsp\t$rd", []>{
  let Uses = [SP];
}

def ADDSP: CDMInst<(outs), (ins simm16:$imm), "addsp\t$imm", []>{
  let Uses = [SP];
  let Defs = [SP];
} 


def load_frame_adderss: CDMInst<(outs CPURegs:$rd), (ins CPURegs:$rs), "add\t$rs, fp, $rd", []>{
  let isCodeGenOnly = true;
  let Uses = [FP];
}

def : Pat<(i16 addr_on_frame:$addr), (load_frame_adderss (ldi CPURegs:$addr))>;
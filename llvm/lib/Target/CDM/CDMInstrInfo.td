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
  // TODO: right now we don't need opcodes and stuff
}

class arithLogicR<string asm_instr, SDNode OpNode, bit isComm = 0>:
  CDMInst<(outs CPURegs:$rd), (ins CPURegs:$rs0, CPURegs:$rs1),
          !strconcat(asm_instr, " $rs0, $rs1, $rd"),
          [(set CPURegs:$rd, (OpNode CPURegs:$rs0, CPURegs:$rs1))]>{
            let isCommutable = isComm;
            let isReMaterializable = 1;
          }



def ADD: arithLogicR<"add", add, 1>;
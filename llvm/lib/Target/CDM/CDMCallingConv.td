def RetCC_CDM : CallingConv<[
  // i16 are returned in register(s)
  CCIfType<[i16], CCAssignToReg<[R0, R1, R2, R3]>>
]>;

def CSR_O16 : CalleeSavedRegs<(add FP,
                                   (sequence "R%u", 4, 6))>;
//
// Created by ilya on 15.10.23.
//

#include "CDMMCAsmInfo.h"

namespace llvm {
CDMMCAsmInfo::CDMMCAsmInfo(const Triple &TheTriple) {
  // TODO: figure out what this class does
  // I'm just copypasting code now
  CodePointerSize = 2;
  MaxInstLength = 4;
  MinInstAlignment = 2;
  HasSingleParameterDotFile = false;
  HasDotTypeDotSizeDirective = false;
  // TODO: I will fill this later

  Data16bitsDirective = "\tdc\t";
}
} // namespace llvm
//
// Created by ilya on 15.10.23.
//

#include "CDMMCAsmInfo.h"

namespace llvm {
CDMMCAsmInfo::CDMMCAsmInfo(const Triple &TheTriple) {
  CodePointerSize = 2;
  MaxInstLength = 4;
  MinInstAlignment = 2;
  HasSingleParameterDotFile = false;
  HasDotTypeDotSizeDirective = false;

  Data8bitsDirective = "\tdb\t";
  Data16bitsDirective = "\tdc\t";
  Data32bitsDirective = 0;
  Data64bitsDirective = 0;
  ZeroDirective = "\tds\t";
  AsciiDirective = 0;
  AscizDirective = 0;
  PrivateGlobalPrefix = "__L";
  PrivateLabelPrefix = "__L";
  //  hasPairedDoubleQuoteStringConstants()
  HasPairedDoubleQuoteStringConstants = false;
}
} // namespace llvm
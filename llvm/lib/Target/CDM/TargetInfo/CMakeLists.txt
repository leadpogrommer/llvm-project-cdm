add_llvm_component_library(LLVMCDMInfo
  CDMTargetInfo.cpp

  LINK_COMPONENTS
  MC
  Support

  ADD_TO_COMPONENT
  CDM
  )

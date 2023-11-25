add_llvm_component_library(LLVMCDMInstPrinter
        CDMInstPrinter.cpp

        LINK_COMPONENTS
        Support

        ADD_TO_COMPONENT
        CDM
)


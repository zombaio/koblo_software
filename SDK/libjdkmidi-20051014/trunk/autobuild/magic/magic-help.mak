.PHONY : compile_info

compile_info :
	@echo "CC : $(CC)"
	@echo "CXX : $(CXX)"
	@echo "AR : $(AR)"
	@echo "RANLIB : $(RANLIB)"
	@echo "COMPILE_FLAGS: $(COMPILE_FLAGS)"
	@echo "PREPROCESS_FLAGS: $(PREPROCESS_FLAGS)"
	@echo "LINK_FLAGS: $(LINK_FLAGS)"
	@echo "LDLIBS: $(LDLIBS)"
	@echo "PKGCONFIG_PACKAGES: $(PKGCONFIG_PACKAGES)"
	@echo "CONFIG_TOOLS: $(CONFIG_TOOLS)"
	@echo "LDLIBS_PACKAGES: $(LDLIBS_PACKAGES)"
	@echo "OUTPUT_LIB: $(OUTPUT_LIB)"
	@echo "LIB_DIRS: $(LIB_DIRS)"
	@echo "PLATFORM_DIRS: $(PLATFORM_DIRS)"
	@echo "ALL_SRC_DIRS: $(ALL_SRC_DIRS)"
	@echo "LIB_INCLUDE_DIR: $(LIB_INCLUDE_DIR)"
	@echo "LIB_SRC_DIR: $(LIB_SRC_DIR)"
	@echo "LIB_TESTS_DIR: $(LIB_TESTS_DIR)"
	@echo "LIB_TOOLS_DIR: $(LIB_TOOLS_DIR)"
	@echo "LIB_TOOLS_DEV_DIR: $(LIB_TOOLS_DEV_DIR)"
	@echo "LIB_EXAMPLES_DIR: $(LIB_EXAMPLES_DIR)"
	@echo "LIB_GUI_DIR: $(LIB_GUI_DIR)"
	@echo "LIB_GUI_DEV_DIR: $(LIB_GUI_DEV_DIR)"
	@echo "LIB_CPP_FILES: $(LIB_CPP_FILES)"
	@echo "LIB_CC_FILES: $(LIB_CC_FILES)"
	@echo "LIB_C_FILES: $(LIB_C_FILES)"
	@echo "LIB_MM_FILES: $(LIB_MM_FILES)"
	@echo "LIB_M_FILES: $(LIB_M_FILES)"
	@echo "LIB_RC_FILES: $(LIB_RC_FILES)"
	@echo "LATEX_DOCS_TEX: $(LATEX_DOCS_TEX)"
	@echo "LATEX_DOCS_PDF: $(LATEX_DOCS_PDF)"
	@echo "LIB_DAEMONS_CPP_FILES: $(notdir $(LIB_DAEMONS_CPP_FILES))"
	@echo "LIB_GUI_CPP_FILES: $(notdir $(LIB_GUI_CPP_FILES))"
	@echo "LIB_GUI_O_FILES: $(notdir $(LIB_GUI_O_FILES))"
	@echo "LIB_GUI_DEV_CPP_FILES: $(notdir $(LIB_GUI_CPP_FILES))"
	@echo "LIB_GUI_DEV_O_FILES: $(notdir $(LIB_GUI_DEV_O_FILES))"
	@echo "LIB_O_FILES: $(notdir $(LIB_O_FILES))"
	@echo "LIB_TOOLS_EXE_FILES: $(notdir $(LIB_TOOLS_EXE_FILES))"
	@echo "LIB_TOOLS_DEV_EXE_FILES: $(notdir $(LIB_TOOLS_DEV_EXE_FILES))"
	@echo "LIB_EXAMPLES_EXE_FILES: $(notdir $(LIB_EXAMPLES_EXE_FILES))"
	@echo "LIB_DAEMONS_EXE_FILES: $(notdir $(LIB_DAEMONS_EXE_FILES))"
	@echo "LIB_TESTS_EXE_FILES: $(notdir $(LIB_TESTS_EXE_FILES))"
	@echo "LIB_GUI_EXE_FILES: $(notdir $(LIB_GUI_EXE_FILES))"
	@echo "LIB_GUI_DEV_EXE_FILES: $(notdir $(LIB_GUI_DEV_EXE_FILES))"
	@echo "TEST_ARGS: $(TEST_ARGS)"
	@echo "BUILD_DIR: $(OUTPUT_DIR)"
	@echo "OUTPUT_DIR: $(OUTPUT_DIR)"
	@echo "OUTPUT_LIB_DIR: $(OUTPUT_LIB_DIR)"
	@echo "OUTPUT_TESTS_DIR: $(OUTPUT_TESTS_DIR)"
	@echo "OUTPUT_DOCS_DIR: $(OUTPUT_DOCS_DIR)"
	@echo "OUTPUT_DOCS_DEV_DIR: $(OUTPUT_DOCS_DEV_DIR)"
	@echo "OUTPUT_TOOLS_DIR: $(OUTPUT_TOOLS_DIR)"
	@echo "OUTPUT_GUI_DIR: $(OUTPUT_GUI_DIR)"
	@echo "OUTPUT_EXAMPLES_DIR: $(OUTPUT_EXAMPLES_DIR)"
	@echo "OUTPUT_OBJ_DIR: $(OUTPUT_OBJ_DIR)"
	@echo "TARGET_INSTALL_DIR: $(TARGET_INSTALL_DIR)"
	@echo "INSTALL_DIR: $(TARGET_INSTALL_DIR)"
	@echo "INSTALL_BIN_DIR: $(INSTALL_DIR)/$(TARGET_BIN_DIR)"
	@echo "INSTALL_LIB_DIR: $(INSTALL_DIR)/$(TARGET_LIB_DIR)"
	@echo "INSTALL_INCLUDE_DIR: $(INSTALL_DIR)/$(TARGET_INCLUDE_DIR)"
	@echo "INSTALL_DOCS_DIR: $(INSTALL_DIR)/$(TARGET_DOCS_DIR)"
	@echo "INSTALL_SHARE_DIR: $(INSTALL_DIR)/$(TARGET_SHARE_DIR)"
	@echo "INSTALL_ETC_DIR: $(INSTALL_DIR)/$(TARGET_ETC_DIR)"
	@echo "INSTALLDEV_BIN_DIR: $(INSTALL_DIR)/$(TARGET_BIN_DIR)"
	@echo "INSTALLDEV_LIB_DIR: $(INSTALL_DIR)/$(TARGET_LIB_DIR)"
	@echo "INSTALLDEV_INCLUDE_DIR: $(INSTALL_DIR)/$(TARGET_INCLUDE_DIR)"
	@echo "INSTALLDEV_DOCS_DIR: $(INSTALL_DIR)/$(TARGET_DOCS_DIR)"
	@echo "INSTALLDEV_SHARE_DIR: $(INSTALL_DIR)/$(TARGET_SHARE_DIR)"
	@echo "INSTALLDEV_ETC_DIR: $(INSTALL_DIR)/$(TARGET_ETC_DIR)"
	@echo "INSTALLDOCSDEV_DOCS_DIR: $(INSTALL_DIR)/$(TARGET_DOCS_DIR)"
ifeq ($(CROSS_COMPILING),1)
	@echo "NATIVE_CC : $(NATIVE_CC)"
	@echo "NATIVE_CXX : $(NATIVE_CXX)"
	@echo "NATIVE_AR : $(NATIVE_AR)"
	@echo "NATIVE_RANLIB : $(NATIVE_RANLIB)"
	@echo "NATIVE_COMPILE_FLAGS: $(NATIVE_COMPILE_FLAGS)"
	@echo "NATIVE_LINK_FLAGS: $(NATIVE_LINK_FLAGS)"
	@echo "NATIVE_LDLIBS: $(NATIVE_LDLIBS)"
	@echo "NATIVE_OUTPUT_LIB: $(NATIVE_OUTPUT_LIB)"
	@echo "NATIVE_LIB_SRC_DIR: $(NATIVE_LIB_SRC_DIR)"
	@echo "NATIVE_LIB_TOOLS_DIR: $(NATIVE_LIB_TOOLS_DIR)"
	@echo "NATIVE_PLATFORM_DIRS: $(NATIVE_PLATFORM_DIRS)"
	@echo "NATIVE_LIB_O_FILES: $(NATIVE_LIB_O_FILES)"
	@echo "NATIVE_LIB_TOOLS_EXE_FILES: $(NATIVE_LIB_TOOLS_EXE_FILES)"
	@echo "NATIVE_LIB_TOOLS_DEV_EXE_FILES: $(NATIVE_LIB_TOOLS_DEV_EXE_FILES)"
	@echo "NATIVE_LIB_TESTS_EXE_FILES: $(NATIVE_LIB_TESTS_EXE_FILES)"
	@echo "NATIVE_LIB_CPP_FILES: $(NATIVE_LIB_CPP_FILES)"
endif

.PHONY : help

help :
	@echo "TOP of source is $(PROJECT_TOP_DIR)"
	@echo "compile_info : show compile flags, directories, options"
	@echo "lib : build library $(OUTPUT_LIB)"
	@echo "TAGS : make etags for emacs"
	@echo "tags : make ctags for vi"
	@echo "docs : html docs"
	@echo "docs-dev : build doxygen docs"
	@echo "clean : clean intermediate files"
	@echo "distclean / realclean : clean all built files (except docs)"
	@echo "tools : build tool programs $(LIB_TOOLS_EXE_FILES)"
	@echo "examples : build example programs $(LIB_EXAMPLES_EXE_FILES)"
	@echo "tests : build test programs $(LIB_TESTS_EXE_FILES)"
	@echo "test : run tests, sh scripts and test programs"
	@echo "dirs : create output dirs $(OUTPUT_DIR)"
	@echo "preinstall : install tool executables files into $(LOCAL_INSTALL_DIR)"
	@echo "preinstall-docs : copy html docs into $(LOCAL_INSTALL_DIR)"
	@echo "preinstall-dev : install tool executables, include files and library files into $(LOCAL_INSTALLDEV_DIR)"
	@echo "preinstall-docs-dev : install tool executables, include files, library files, and doxygen docs into $(LOCAL_INSTALL_DIR)"
	@echo "package : make package of executables"
	@echo "package-testresults : make package of test results"
	@echo "package-dev : make package of development files"
	@echo "package-docs-dev : make package of development doxygen documentation"
	@echo "packages : make all packages"
	@echo "install : install tool executables files into $(INSTALL_DIR)"
	@echo "install-docs : copy html docs into $(INSTALL_DOCS_DIR)"
	@echo "install-dev : install tool executables, include files and library files into $(INSTALLDEV_DIR)"
	@echo "install-docs-dev : install tool executables, include files, library files, and doxygen docs into $(INSTALLDEVDOCS_DIR)"
	@echo "ship : ship main package to another computer via scp, use --ship-to configure option. Currently $(SHIP_TO)"
	@echo "ship-dev : ship dev package to another computer via scp"
	@echo "ship-docs-dev : ship dev documentation package to another computer via scp"
	@echo "ship-testresults : ship testresults package to another computer via scp"
	@echo "ship-raw-testresults : ship testresults files to another computer via scp"
	@echo "ship-raw-docs-dev : ship doxygen files to another computer via scp"
	@echo "ship-all : ship all packages to another computer via scp"
ifeq ($(CROSS_COMPILING),1)
	@echo "native-dirs : create output dirs for native build in $(NATIVE_OUTPUT_DIR)"
	@echo "native-lib : build native library $(NATIVE_OUTPUT_LIB)"
	@echo "native-tests : build native tests $(NATIVE_LIB_TESTS_EXE_FILES)"
	@echo "native-test : run native tests."
endif

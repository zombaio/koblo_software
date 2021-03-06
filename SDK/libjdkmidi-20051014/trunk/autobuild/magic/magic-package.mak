ALL_OUTPUT_DIRS+=$(PACKAGES_DIR)

PACKAGE_BASENAME?=$(PROJECT)$(PROJECT_VARIANT_LABEL)-$(PROJECT_VERSION)-$(TARGET_PLATFORM_NAME)$(PACKAGE_SUFFIX)
PACKAGEDEV_BASENAME?=$(PROJECT)$(PROJECT_VARIANT_LABEL)_dev-$(PROJECT_VERSION)-$(TARGET_PLATFORM_NAME)$(PACKAGE_SUFFIX)
PACKAGEDOCSDEV_BASENAME?=$(PROJECT)$(PROJECT_VARIANT_LABEL)_docs-dev-$(PROJECT_VERSION)-$(TARGET_PLATFORM_NAME)$(PACKAGE_SUFFIX)
PACKAGETESTRESULTS_BASENAME?=$(PROJECT)$(PROJECT_VARIANT_LABEL)_testresults-$(PROJECT_VERSION)-$(TARGET_PLATFORM_NAME)$(PACKAGE_SUFFIX)

PACKAGE_PATH=$(PACKAGES_DIR)/$(PACKAGE_BASENAME).$(PACKAGE_EXT)
PACKAGE_DEV_PATH=$(PACKAGES_DIR)/$(PACKAGEDEV_BASENAME).$(PACKAGE_EXT)
PACKAGE_DOCSDEV_PATH=$(PACKAGES_DIR)/$(PACKAGEDOCSDEV_BASENAME).$(PACKAGE_EXT)
PACKAGE_TESTRESULTS_PATH=$(PACKAGES_DIR)/$(PACKAGETESTRESULTS_BASENAME).zip


include $(MAGICMAKE_DIR)/magic/magic-package-$(PACKAGER).mak


CLEAN_FILES += $(PACKAGE_PATH) $(PACKAGE_DEV_PATH) $(PACKAGE_DOCSDEV_PATH) $(PACKAGE_TESTRESULTS_PATH)

.PHONY : packages

packages : package package-dev package-docs-dev package-testresults

.PHONY : package-testresults

ifeq ($(CROSS_COMPILING),1)

package-testresults :

else

package-testresults : $(PACKAGE_TESTRESULTS_PATH)

$(PACKAGE_TESTRESULTS_PATH) : test 
	@-( cd $(OUTPUT_TESTS_DIR) && $(ZIP) -r $(PACKAGE_TESTRESULTS_PATH) . >/dev/null )

$(PACKAGES_DIR)/$(PACKAGETESTRESULTS_BASENAME).tgz : test 
	@-( cd $(OUTPUT_TESTS_DIR) && $(TAR) cf - . | $(GZIP) >$(PACKAGES_DIR)/$(PACKAGETESTRESULTS_BASENAME).zip )

endif


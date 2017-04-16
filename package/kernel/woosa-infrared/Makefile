#kernel module
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk
PKG_NAME:=woosa-infrared
PKG_RELEASE:=1
include $(INCLUDE_DIR)/package.mk


EXTRA_CFLAGS:= \
    $(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=m,%,$(filter %=m,$(EXTRA_KCONFIG)))) \
	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=y,%,$(filter %=y,$(EXTRA_KCONFIG)))) \

MAKE_OPTS:=ARCH="$(LINUX_KARCH)" \
	CROSS_COMPILE="$(TARGET_CROSS)" \
	SUBDIRS="$(PKG_BUILD_DIR)" \
	EXTRA_CFLAGS="$(EXTRA_CFLAGS)"

define KernelPackage/woosa-infrared
	SUBMENU:=Other modules
	TITLE:=Support Module for woosa-infrared
	FILES:=$(PKG_BUILD_DIR)/woosa-infrared.ko
	AUTOLOAD:=$(call AutoLoad,81,woosa-infrared)
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)/              
	$(CP) -R ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" $(MAKE_OPTS) CONFIG_WOOSA_INFRARED=m modules
endef
$(eval $(call KernelPackage,woosa-infrared))

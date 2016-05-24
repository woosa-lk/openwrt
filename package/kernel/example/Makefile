#kernel module
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk
PKG_NAME:=example
PKG_RELEASE:=1
include $(INCLUDE_DIR)/package.mk


EXTRA_CFLAGS:= \
    $(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=m,%,$(filter %=m,$(EXTRA_KCONFIG)))) \
	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=y,%,$(filter %=y,$(EXTRA_KCONFIG)))) \

MAKE_OPTS:=ARCH="$(LINUX_KARCH)" \
	CROSS_COMPILE="$(TARGET_CROSS)" \
	SUBDIRS="$(PKG_BUILD_DIR)" \
	EXTRA_CFLAGS="$(EXTRA_CFLAGS)"

define KernelPackage/example
	SUBMENU:=Other modules
	TITLE:=Support Module for example
	FILES:=$(PKG_BUILD_DIR)/example.ko
	AUTOLOAD:=$(call AutoLoad,81,example)
endef

#PKG_BUILD_DIR：/build_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/linux-ramips_mt7621/example 
#建立 PKG_BUILD_DIR ，并将代码拷贝到此处
define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)/              
	$(CP) -R ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" $(MAKE_OPTS) CONFIG_EXAMPLE=m modules
endef
$(eval $(call KernelPackage,example))

# Component makefile for extras/sdio
INC_DIRS += $(ST7789_ROOT)..

# args for passing into compile rule generation
ST7789_SRC_DIR = $(ST7789_ROOT)
# EXTRA_COMPONENTS = extras/ssl
$(eval $(call component_compile_rules,ST7789))
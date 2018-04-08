# Component makefile for extras/qrencode

INC_DIRS += $(ROOT)extras/qrencode

# args for passing into compile rule generation
extras/qrencode_INC_DIR =  $(ROOT)extras/qrencode
extras/qrencode_SRC_DIR =  $(ROOT)extras/qrencode

$(eval $(call component_compile_rules,extras/qrencode))

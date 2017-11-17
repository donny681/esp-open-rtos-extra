# Component makefile for extras/sdio
INC_DIRS += $(aws_sigv4_ROOT)

aws_sigv4_INC_DIR =
aws_sigv4_SRC_DIR = $(aws_sigv4_ROOT)aws_sigv4
# args for passing into compile rule generation
#aws_sigv4_SRC_DIR = $(aws_sigv4_ROOT)
#EXTRA_COMPONENTS = extras/ssl
$(eval $(call component_compile_rules,aws_sigv4))
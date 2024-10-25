################################################################################
# file        	: 
# Author      	: VNPT Technology - HEC
# Version     	: 1.0
# Description 	: Makefile fragment for building C/C++ code
#
#					THIS FILE FOR VNPT TRAINNING ONLY
#						DO NOT COPY AND DISTRIBUTE
################################################################################

include $(BUILD_DIR)/common.mk

TOP = $(shell pwd)

TARGET_NAME = eloop

INCLUDE_PATHS = \
	$(TOP)/../inc \
	$(LIB_ROOT)/include

# Library link options
L_OPTS += -lstringl

#SYSTEM_LIBS = pthread

CFLAGS=-Wall
CPPFLAGS=-Wall

# If STATICALLY_LINKED is set no, use the default shared object library
STATICALLY_LINKED = yes
export STATICALLY_LINKED

ifeq ($(STATICALLY_LINKED),yes)
  include $(BUILD_DIR)/staticlibrary.mk
else
  include $(BUILD_DIR)/sharedlibrary.mk
endif
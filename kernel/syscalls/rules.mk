# Copyright 2016 The Fuchsia Authors
# Copyright (c) 2008-2015 Travis Geiselbrecht
#
# Use of this source code is governed by a MIT-style
# license that can be found in the LICENSE file or at
# https://opensource.org/licenses/MIT

LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

MODULE_DEPS := \
    kernel/lib/console \
    kernel/lib/crypto \
    kernel/lib/fbl \
    kernel/lib/pci \
    kernel/lib/user_copy \
    kernel/lib/vdso \
    kernel/object \

MODULE_SRCS := \
    $(LOCAL_DIR)/syscalls.cpp \
    $(LOCAL_DIR)/channel.cpp \
    $(LOCAL_DIR)/ddk.cpp \
    $(LOCAL_DIR)/ddk_pci.cpp \
    $(LOCAL_DIR)/debug.cpp \
    $(LOCAL_DIR)/exceptions.cpp \
    $(LOCAL_DIR)/fifo.cpp \
    $(LOCAL_DIR)/futex.cpp \
    $(LOCAL_DIR)/handle_ops.cpp \
    $(LOCAL_DIR)/hypervisor.cpp \
    $(LOCAL_DIR)/hyp_sys.cpp \
    $(LOCAL_DIR)/zircon.cpp \
    $(LOCAL_DIR)/object.cpp \
    $(LOCAL_DIR)/object_wait.cpp \
    $(LOCAL_DIR)/port.cpp \
    $(LOCAL_DIR)/profile.cpp \
    $(LOCAL_DIR)/resource.cpp \
    $(LOCAL_DIR)/socket.cpp \
    $(LOCAL_DIR)/system.cpp \
    $(LOCAL_DIR)/task.cpp \
    $(LOCAL_DIR)/test.cpp \
    $(LOCAL_DIR)/timer.cpp \
    $(LOCAL_DIR)/vmar.cpp \
    $(LOCAL_DIR)/vmo.cpp \

ifeq ($(ARCH),x86)
MODULE_SRCS += $(LOCAL_DIR)/system_x86.cpp \
	       $(LOCAL_DIR)/ddk_x86.cpp \

else ifeq ($(ARCH),arm64)
MODULE_SRCS += $(LOCAL_DIR)/system_arm64.cpp \
	       $(LOCAL_DIR)/ddk_arm64.cpp \

endif

# We need a header file generated by kernel/lib/vdso/rules.mk.
MODULE_COMPILEFLAGS += -I$(BUILDDIR)/kernel/lib/vdso
$(BUILDDIR)/$(LOCAL_DIR)/$(LOCAL_DIR)/syscalls.cpp.o: \
    $(BUILDDIR)/kernel/lib/vdso/vdso-valid-sysret.h

include make/module.mk

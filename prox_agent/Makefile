#!/usr/bin/make

# SDK location
SDK_DIR     := amplib
SDK_LIB     := libampagent.a

# compile options
CC          := gcc
CFLAGS      := -Wall -Wextra -Wstrict-prototypes -Wold-style-definition -g -I$(SDK_DIR)
TARGET      := amp_agent


SRCS=agent/prox_agent_amp.c agent/prox_agent_common.c prox_agent.c prox_agent_config.c prox_agent_data_interface.c
OBJS=$(SRCS:.c=.o)


# pretty make
V ?= 1
ifeq ($(V),0)
qcmdprint=@echo "$$(printf '%11s' '$(1)')  $(2)";
else
qcmdprint:=
endif

.PHONY: all clean

# target rules
all: $(TARGET)

$(TARGET): $(OBJS)
#	@$(MAKE) -C $(SDK_DIR)
	# $(call qcmdprint,CCLD,$(@)) $(CC) -c $(CFLAGS) -o $@ $< 

%.o: %.c
	$(call qcmdprint,CCLD,$(@)) $(CC) -c $(CFLAGS) -o $@ $< 

clean:
	# @$(MAKE) -C $(SDK_DIR) clean
	$(call qcmdprint,CLEAN,$(TARGET)) rm -rf $(TARGET) $(OBJS)

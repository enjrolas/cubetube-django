TARGET_COMMUNICATION_DYNALIB_SRC_PATH = $(COMMUNICATION_DYNALIB_MODULE_PATH)/src

CPPSRC += $(call target_files,$(TARGET_COMMUNICATION_DYNALIB_SRC_PATH),*.cpp)
CSRC += $(call target_files,$(TARGET_COMMUNICATION_DYNALIB_SRC_PATH),*.c)    

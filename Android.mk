LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libOpenCL
LOCAL_SRC_FILES := libOpenCL.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
MY_SRC_LIST += $(wildcard ./*.cpp)
LOCAL_C_INCLUDES += ./
LOCAL_SRC_FILES += $(MY_SRC_LIST)

LOCAL_CPP_EXTENSION := .cxx .cpp .cc    
LOCAL_CPPFLAGS += -DSUPPORT_OMP -march=armv8-a -fopenmp -fexceptions -fpermissive -Wno-literal-suffix- -Wno-deprecated-declarations -Wl,--no-as-needed -std=c++11 -O3 -Wl, -hash-style=sysv
LOCAL_LDLIBS += -L./libs64
LOCAL_LDLIBS += -lm -lz -lc -llog -pie -fPIE -fopenmp -fuse-ld=gold
LOCAL_MODULE := opencl-num-test
LOCAL_SHARED_LIBRARIES += libOpenCL
include $(BUILD_EXECUTABLE)

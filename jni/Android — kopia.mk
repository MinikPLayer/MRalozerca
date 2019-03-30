LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := sfml-example

LOCAL_SRC_FILES := main.cpp
LOCAL_SRC_FILES += Funkcje.hpp
LOCAL_SRC_FILES += Funkcje.cpp
LOCAL_SRC_FILES += BiegZPrzeszkodami.hpp
LOCAL_SRC_FILES += BiegZPrzeszkodami.cpp
LOCAL_SRC_FILES += EkranZWynikami.hpp
LOCAL_SRC_FILES += EkranZWynikami.cpp
LOCAL_SRC_FILES += HarryCatter.hpp
LOCAL_SRC_FILES += HarryCatter.cpp
LOCAL_SRC_FILES += ScieranieTablicy.hpp
LOCAL_SRC_FILES += ScieranieTablicy.cpp
LOCAL_SRC_FILES += WyscigMRalow.hpp
LOCAL_SRC_FILES += WyscigMRalow.cpp
LOCAL_SRC_FILES += Obiekty.hpp
LOCAL_SRC_FILES += LineShape.cpp

LOCAL_SHARED_LIBRARIES := sfml-system
LOCAL_SHARED_LIBRARIES += sfml-window
LOCAL_SHARED_LIBRARIES += sfml-graphics
LOCAL_SHARED_LIBRARIES += sfml-audio
LOCAL_SHARED_LIBRARIES += sfml-network
LOCAL_WHOLE_STATIC_LIBRARIES := sfml-main

include $(BUILD_SHARED_LIBRARY)

$(call import-module,sfml)

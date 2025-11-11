# Automatically generated makefile, created by the Projucer
# Don't edit this file! Your changes will be overwritten when you re-save the Projucer project!

# build with "V=1" for verbose builds
ifeq ($(V), 1)
V_AT =
else
V_AT = @
endif

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef PKG_CONFIG
  PKG_CONFIG=pkg-config
endif

ifndef STRIP
  STRIP=strip
endif

ifndef AR
  AR=ar
endif

ifndef CONFIG
  CONFIG=Debug
endif

JUCE_ARCH_LABEL := $(shell uname -m)

ifeq ($(CONFIG),Debug)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Debug
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := 
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) "-DLINUX=1" "-DDEBUG=1" "-D_DEBUG=1" "-DJUCER_LINUX_MAKE_6D53C8B4=1" "-DJUCE_APP_VERSION=1.0.0" "-DJUCE_APP_VERSION_HEX=0x10000" "-DJUCE_USE_EXTERNAL_TEMPORARY_SUBPROCESS=1" $(shell $(PKG_CONFIG) --cflags $(shell ($(PKG_CONFIG) --exists webkit2gtk-4.1 && echo webkit2gtk-4.1) || echo webkit2gtk-4.0) alsa freetype2 fontconfig gl libcurl gtk+-x11-3.0) -pthread -I$(HOME)/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv/src -I$(HOME)/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv -I$(HOME)/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sratom -I$(HOME)/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sord/src -I$(HOME)/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sord -I$(HOME)/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd -I$(HOME)/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2 -I$(HOME)/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/ -I../../JuceLibraryCode -Ipre_build -I$(HOME)/JUCE/modules $(CPPFLAGS)

  JUCE_CPPFLAGS_LV2_PLUGIN :=  "-DJucePlugin_Build_VST=0" "-DJucePlugin_Build_VST3=0" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=0" "-DJucePlugin_Build_Unity=0" "-DJucePlugin_Build_LV2=1"
  JUCE_CFLAGS_LV2_PLUGIN := -fPIC -fvisibility=hidden
  JUCE_LDFLAGS_LV2_PLUGIN := -shared -Wl,--no-undefined
  JUCE_LV2DIR := Lupo.lv2
  JUCE_TARGET_LV2_PLUGIN := $(JUCE_LV2DIR)/Lupo.so
  JUCE_LV2_FULL_PATH := $(JUCE_OUTDIR)/$(JUCE_TARGET_LV2_PLUGIN)
  JUCE_LV2DESTDIR := $(HOME)/.lv2
  JUCE_COPYCMD_LV2_PLUGIN := $(JUCE_OUTDIR)/$(JUCE_LV2DIR) $(JUCE_LV2DESTDIR)

  JUCE_CPPFLAGS_SHARED_CODE :=  "-DJucePlugin_Build_VST=0" "-DJucePlugin_Build_VST3=0" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=0" "-DJucePlugin_Build_Unity=0" "-DJucePlugin_Build_LV2=1" "-DJUCE_SHARED_CODE=1"
  JUCE_CFLAGS_SHARED_CODE := -fPIC -fvisibility=hidden
  JUCE_TARGET_SHARED_CODE := Lupo.a

  JUCE_CPPFLAGS_LV2_MANIFEST_HELPER :=  "-DJucePlugin_Build_VST=0" "-DJucePlugin_Build_VST3=0" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=0" "-DJucePlugin_Build_Unity=0" "-DJucePlugin_Build_LV2=0"
  JUCE_TARGET_LV2_MANIFEST_HELPER := juce_lv2_helper

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -fPIC -g -ggdb -O0 $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=c++17 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) $(shell $(PKG_CONFIG) --libs alsa freetype2 fontconfig gl libcurl) -fvisibility=hidden -lrt -ldl -lpthread $(LDFLAGS)

  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(JUCE_TARGET_LV2_PLUGIN) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) $(JUCE_OUTDIR)/$(JUCE_TARGET_LV2_MANIFEST_HELPER) $(JUCE_OBJDIR) pre_build
endif

ifeq ($(CONFIG),Release)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Release
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := 
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) "-DLINUX=1" "-DNDEBUG=1" "-DJUCER_LINUX_MAKE_6D53C8B4=1" "-DJUCE_APP_VERSION=1.0.0" "-DJUCE_APP_VERSION_HEX=0x10000" "-DJUCE_USE_EXTERNAL_TEMPORARY_SUBPROCESS=1" $(shell $(PKG_CONFIG) --cflags $(shell ($(PKG_CONFIG) --exists webkit2gtk-4.1 && echo webkit2gtk-4.1) || echo webkit2gtk-4.0) alsa freetype2 fontconfig gl libcurl gtk+-x11-3.0) -pthread -I$(HOME)/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv/src -I$(HOME)/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lilv -I$(HOME)/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sratom -I$(HOME)/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sord/src -I$(HOME)/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/sord -I$(HOME)/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/serd -I$(HOME)/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/lv2 -I$(HOME)/JUCE/modules/juce_audio_processors/format_types/LV2_SDK/ -I../../JuceLibraryCode -Ipre_build -I$(HOME)/JUCE/modules $(CPPFLAGS)

  JUCE_CPPFLAGS_LV2_PLUGIN :=  "-DJucePlugin_Build_VST=0" "-DJucePlugin_Build_VST3=0" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=0" "-DJucePlugin_Build_Unity=0" "-DJucePlugin_Build_LV2=1"
  JUCE_CFLAGS_LV2_PLUGIN := -fPIC -fvisibility=hidden
  JUCE_LDFLAGS_LV2_PLUGIN := -shared -Wl,--no-undefined
  JUCE_LV2DIR := Lupo.lv2
  JUCE_TARGET_LV2_PLUGIN := $(JUCE_LV2DIR)/Lupo.so
  JUCE_LV2_FULL_PATH := $(JUCE_OUTDIR)/$(JUCE_TARGET_LV2_PLUGIN)
  JUCE_LV2DESTDIR := $(HOME)/.lv2
  JUCE_COPYCMD_LV2_PLUGIN := $(JUCE_OUTDIR)/$(JUCE_LV2DIR) $(JUCE_LV2DESTDIR)

  JUCE_CPPFLAGS_SHARED_CODE :=  "-DJucePlugin_Build_VST=0" "-DJucePlugin_Build_VST3=0" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=0" "-DJucePlugin_Build_Unity=0" "-DJucePlugin_Build_LV2=1" "-DJUCE_SHARED_CODE=1"
  JUCE_CFLAGS_SHARED_CODE := -fPIC -fvisibility=hidden
  JUCE_TARGET_SHARED_CODE := Lupo.a

  JUCE_CPPFLAGS_LV2_MANIFEST_HELPER :=  "-DJucePlugin_Build_VST=0" "-DJucePlugin_Build_VST3=0" "-DJucePlugin_Build_AU=0" "-DJucePlugin_Build_AUv3=0" "-DJucePlugin_Build_AAX=0" "-DJucePlugin_Build_Standalone=0" "-DJucePlugin_Build_Unity=0" "-DJucePlugin_Build_LV2=0"
  JUCE_TARGET_LV2_MANIFEST_HELPER := juce_lv2_helper

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -fPIC -O3 $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=c++17 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) $(shell $(PKG_CONFIG) --libs alsa freetype2 fontconfig gl libcurl) -fvisibility=hidden -lrt -ldl -lpthread $(LDFLAGS)

  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(JUCE_TARGET_LV2_PLUGIN) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) $(JUCE_OUTDIR)/$(JUCE_TARGET_LV2_MANIFEST_HELPER) $(JUCE_OBJDIR) pre_build
endif

OBJECTS_ALL := \

OBJECTS_LV2_PLUGIN := \
  $(JUCE_OBJDIR)/include_juce_audio_plugin_client_LV2_7d84e0a5.o \

OBJECTS_SHARED_CODE := \
  $(JUCE_OBJDIR)/SpeedTrig_a01c81b8.o \
  $(JUCE_OBJDIR)/ModMatrix_999d8d44.o \
  $(JUCE_OBJDIR)/BlitSquare_5fa4b4fb.o \
  $(JUCE_OBJDIR)/Stk_a48e9f8b.o \
  $(JUCE_OBJDIR)/BlitSaw_c7d88b15.o \
  $(JUCE_OBJDIR)/MessageBus_62116bd0.o \
  $(JUCE_OBJDIR)/Topic_29beba82.o \
  $(JUCE_OBJDIR)/TrioLookAndFeel_8466d1b.o \
  $(JUCE_OBJDIR)/ArpPanel_bfe182ee.o \
  $(JUCE_OBJDIR)/CyberpunkLookAndFeel_8eae162c.o \
  $(JUCE_OBJDIR)/ChorusPanel_5fcc1b5f.o \
  $(JUCE_OBJDIR)/DelayPanel_4a48674a.o \
  $(JUCE_OBJDIR)/DistortionPanel_57575c48.o \
  $(JUCE_OBJDIR)/EnvelopePanel_3986a175.o \
  $(JUCE_OBJDIR)/FilterPanel_5081bdcd.o \
  $(JUCE_OBJDIR)/GraphicalEnvelope_9dcbc5e4.o \
  $(JUCE_OBJDIR)/GridButton_aa875ba1.o \
  $(JUCE_OBJDIR)/LFOPanel_ffbd4fd8.o \
  $(JUCE_OBJDIR)/MainUI_4611d716.o \
  $(JUCE_OBJDIR)/MixerChannelPanel_a2fdc5cb.o \
  $(JUCE_OBJDIR)/ModPanel_ea15ddab.o \
  $(JUCE_OBJDIR)/OscillatorPanel_38c367a5.o \
  $(JUCE_OBJDIR)/Panel_45cf3365.o \
  $(JUCE_OBJDIR)/PresetDialog_290d3e10.o \
  $(JUCE_OBJDIR)/ReverbPanel_280f24b3.o \
  $(JUCE_OBJDIR)/SamplerPanel_59910165.o \
  $(JUCE_OBJDIR)/SciFiLookAndFeel_8437fdab.o \
  $(JUCE_OBJDIR)/ADSR_3a7e11c2.o \
  $(JUCE_OBJDIR)/Arpeggiator_55c9d23.o \
  $(JUCE_OBJDIR)/BasicDelayLine_e955e69.o \
  $(JUCE_OBJDIR)/Distortion_c6f3aefd.o \
  $(JUCE_OBJDIR)/Event_9c0aa744.o \
  $(JUCE_OBJDIR)/Filter_50b4f1d8.o \
  $(JUCE_OBJDIR)/FractionalDelayBuffer_e2327c60.o \
  $(JUCE_OBJDIR)/HighPassFilter_2d575c0b.o \
  $(JUCE_OBJDIR)/LowPassFilter_e548c787.o \
  $(JUCE_OBJDIR)/LupoSynth_32a17ffe.o \
  $(JUCE_OBJDIR)/ModTarget_edddf8dd.o \
  $(JUCE_OBJDIR)/Modulation_bae63c5c.o \
  $(JUCE_OBJDIR)/MultimodeFilter_bb09565e.o \
  $(JUCE_OBJDIR)/MultimodeOscillator_ce7def06.o \
  $(JUCE_OBJDIR)/Note_9cf4f812.o \
  $(JUCE_OBJDIR)/OscGroup_884ee260.o \
  $(JUCE_OBJDIR)/Oszillator_d6911909.o \
  $(JUCE_OBJDIR)/Pulse_d1ab8c3.o \
  $(JUCE_OBJDIR)/Random_b42b6da3.o \
  $(JUCE_OBJDIR)/SampleAndHold_e980c4d6.o \
  $(JUCE_OBJDIR)/Sampler_7bd2472.o \
  $(JUCE_OBJDIR)/Sawtooth_1afc249f.o \
  $(JUCE_OBJDIR)/Sine_5cb8612d.o \
  $(JUCE_OBJDIR)/StereoChorus_a12912de.o \
  $(JUCE_OBJDIR)/StereoDelay_2a5984d5.o \
  $(JUCE_OBJDIR)/StereoReverb_5612850a.o \
  $(JUCE_OBJDIR)/Voice_5d6e09dc.o \
  $(JUCE_OBJDIR)/WaveTable_196c08ff.o \
  $(JUCE_OBJDIR)/WhiteNoise_e966d0d1.o \
  $(JUCE_OBJDIR)/PluginProcessor_a059e380.o \
  $(JUCE_OBJDIR)/PluginEditor_94d4fb09.o \
  $(JUCE_OBJDIR)/BinaryData_ce4232d4.o \
  $(JUCE_OBJDIR)/BinaryData2_fa243da8.o \
  $(JUCE_OBJDIR)/include_juce_audio_basics_8a4e984a.o \
  $(JUCE_OBJDIR)/include_juce_audio_devices_63111d02.o \
  $(JUCE_OBJDIR)/include_juce_audio_formats_15f82001.o \
  $(JUCE_OBJDIR)/include_juce_audio_plugin_client_ARA_31a052ed.o \
  $(JUCE_OBJDIR)/include_juce_audio_processors_10c03666.o \
  $(JUCE_OBJDIR)/include_juce_audio_processors_ara_2a4c6ef7.o \
  $(JUCE_OBJDIR)/include_juce_audio_processors_lv2_libs_12bdca08.o \
  $(JUCE_OBJDIR)/include_juce_audio_utils_9f9fb2d6.o \
  $(JUCE_OBJDIR)/include_juce_core_f26d17db.o \
  $(JUCE_OBJDIR)/include_juce_core_CompilationTime_9257742c.o \
  $(JUCE_OBJDIR)/include_juce_cryptography_8cb807a8.o \
  $(JUCE_OBJDIR)/include_juce_data_structures_7471b1e3.o \
  $(JUCE_OBJDIR)/include_juce_dsp_aeb2060f.o \
  $(JUCE_OBJDIR)/include_juce_events_fd7d695.o \
  $(JUCE_OBJDIR)/include_juce_graphics_f817e147.o \
  $(JUCE_OBJDIR)/include_juce_graphics_Harfbuzz_60c52ba2.o \
  $(JUCE_OBJDIR)/include_juce_graphics_Sheenbidi_c310974d.o \
  $(JUCE_OBJDIR)/include_juce_gui_basics_e3f79785.o \
  $(JUCE_OBJDIR)/include_juce_gui_extra_6dee1c1a.o \
  $(JUCE_OBJDIR)/include_juce_opengl_a8a032b.o \
  $(JUCE_OBJDIR)/include_juce_osc_f3df604d.o \
  $(JUCE_OBJDIR)/include_juce_video_be78589.o \
  $(JUCE_OBJDIR)/juce_LinuxSubprocessHelperBinaryData_e1db7dc.o \

OBJECTS_LV2_MANIFEST_HELPER := \
  $(JUCE_OBJDIR)/juce_LV2ManifestHelper_c0a9d409.o \

.PHONY: clean all strip LV2 LV2_MANIFEST_HELPER

all : LV2 LV2_MANIFEST_HELPER

LV2 : $(JUCE_OUTDIR)/$(JUCE_TARGET_LV2_PLUGIN)
LV2_MANIFEST_HELPER : $(JUCE_OUTDIR)/$(JUCE_TARGET_LV2_MANIFEST_HELPER)


$(JUCE_OUTDIR)/$(JUCE_TARGET_LV2_PLUGIN) : $(OBJECTS_LV2_PLUGIN) $(JUCE_OBJDIR)/execinfo.cmd $(RESOURCES) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) $(JUCE_OUTDIR)/$(JUCE_TARGET_LV2_MANIFEST_HELPER)
	@command -v $(PKG_CONFIG) >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@$(PKG_CONFIG) --print-errors alsa freetype2 fontconfig gl libcurl
	@echo Linking "Lupo - LV2 Plugin"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)/$(JUCE_LV2DIR)
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR)/$(JUCE_TARGET_LV2_PLUGIN) $(OBJECTS_LV2_PLUGIN) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) $(JUCE_LDFLAGS) $(shell cat $(JUCE_OBJDIR)/execinfo.cmd) $(JUCE_LDFLAGS_LV2_PLUGIN) $(RESOURCES) $(TARGET_ARCH)
	$(V_AT) $(JUCE_OUTDIR)/$(JUCE_TARGET_LV2_MANIFEST_HELPER) $(JUCE_LV2_FULL_PATH)
	-$(V_AT)[ ! "$(JUCE_LV2DESTDIR)" ] || (mkdir -p $(JUCE_LV2DESTDIR) && cp -R $(JUCE_COPYCMD_LV2_PLUGIN))

$(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) : $(OBJECTS_SHARED_CODE) $(JUCE_OBJDIR)/execinfo.cmd $(RESOURCES)
	@command -v $(PKG_CONFIG) >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@$(PKG_CONFIG) --print-errors alsa freetype2 fontconfig gl libcurl
	@echo Linking "Lupo - Shared Code"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(AR) -rcs $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) $(OBJECTS_SHARED_CODE)

$(JUCE_OUTDIR)/$(JUCE_TARGET_LV2_MANIFEST_HELPER) : $(OBJECTS_LV2_MANIFEST_HELPER) $(JUCE_OBJDIR)/execinfo.cmd $(RESOURCES) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE)
	@command -v $(PKG_CONFIG) >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@$(PKG_CONFIG) --print-errors alsa freetype2 fontconfig gl libcurl
	@echo Linking "Lupo - LV2 Manifest Helper"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR)/$(JUCE_TARGET_LV2_MANIFEST_HELPER) $(OBJECTS_LV2_MANIFEST_HELPER) $(JUCE_OUTDIR)/$(JUCE_TARGET_SHARED_CODE) $(JUCE_LDFLAGS) $(shell cat $(JUCE_OBJDIR)/execinfo.cmd) $(RESOURCES) $(TARGET_ARCH)

$(JUCE_OBJDIR)/include_juce_audio_plugin_client_LV2_7d84e0a5.o: ../../JuceLibraryCode/include_juce_audio_plugin_client_LV2.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_audio_plugin_client_LV2.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_LV2_PLUGIN) $(JUCE_CFLAGS_LV2_PLUGIN) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SpeedTrig_a01c81b8.o: ../../Source/SpeedTrig/SpeedTrig.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling SpeedTrig.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ModMatrix_999d8d44.o: ../../Source/ModMatrix.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling ModMatrix.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BlitSquare_5fa4b4fb.o: ../../Source/BlitSquare.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling BlitSquare.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Stk_a48e9f8b.o: ../../Source/Stk.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling Stk.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BlitSaw_c7d88b15.o: ../../Source/BlitSaw.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling BlitSaw.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MessageBus_62116bd0.o: ../../Source/MessageBus/MessageBus.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling MessageBus.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Topic_29beba82.o: ../../Source/MessageBus/Topic.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling Topic.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TrioLookAndFeel_8466d1b.o: ../../Source/TrioLookAndFeel.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling TrioLookAndFeel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ArpPanel_bfe182ee.o: ../../Source/ArpPanel.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling ArpPanel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CyberpunkLookAndFeel_8eae162c.o: ../../Source/CyberpunkLookAndFeel.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling CyberpunkLookAndFeel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ChorusPanel_5fcc1b5f.o: ../../Source/ChorusPanel.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling ChorusPanel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DelayPanel_4a48674a.o: ../../Source/DelayPanel.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling DelayPanel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DistortionPanel_57575c48.o: ../../Source/DistortionPanel.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling DistortionPanel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/EnvelopePanel_3986a175.o: ../../Source/EnvelopePanel.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling EnvelopePanel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FilterPanel_5081bdcd.o: ../../Source/FilterPanel.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling FilterPanel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GraphicalEnvelope_9dcbc5e4.o: ../../Source/GraphicalEnvelope.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling GraphicalEnvelope.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GridButton_aa875ba1.o: ../../Source/GridButton.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling GridButton.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LFOPanel_ffbd4fd8.o: ../../Source/LFOPanel.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling LFOPanel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MainUI_4611d716.o: ../../Source/MainUI.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling MainUI.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MixerChannelPanel_a2fdc5cb.o: ../../Source/MixerChannelPanel.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling MixerChannelPanel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ModPanel_ea15ddab.o: ../../Source/ModPanel.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling ModPanel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OscillatorPanel_38c367a5.o: ../../Source/OscillatorPanel.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling OscillatorPanel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Panel_45cf3365.o: ../../Source/Panel.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling Panel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PresetDialog_290d3e10.o: ../../Source/PresetDialog.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling PresetDialog.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ReverbPanel_280f24b3.o: ../../Source/ReverbPanel.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling ReverbPanel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SamplerPanel_59910165.o: ../../Source/SamplerPanel.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling SamplerPanel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SciFiLookAndFeel_8437fdab.o: ../../Source/SciFiLookAndFeel.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling SciFiLookAndFeel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ADSR_3a7e11c2.o: ../../Source/AudioEngine/ADSR.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling ADSR.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Arpeggiator_55c9d23.o: ../../Source/AudioEngine/Arpeggiator.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling Arpeggiator.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BasicDelayLine_e955e69.o: ../../Source/AudioEngine/BasicDelayLine.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling BasicDelayLine.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Distortion_c6f3aefd.o: ../../Source/AudioEngine/Distortion.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling Distortion.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Event_9c0aa744.o: ../../Source/AudioEngine/Event.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling Event.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Filter_50b4f1d8.o: ../../Source/AudioEngine/Filter.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling Filter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FractionalDelayBuffer_e2327c60.o: ../../Source/AudioEngine/FractionalDelayBuffer.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling FractionalDelayBuffer.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/HighPassFilter_2d575c0b.o: ../../Source/AudioEngine/HighPassFilter.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling HighPassFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LowPassFilter_e548c787.o: ../../Source/AudioEngine/LowPassFilter.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling LowPassFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LupoSynth_32a17ffe.o: ../../Source/AudioEngine/LupoSynth.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling LupoSynth.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ModTarget_edddf8dd.o: ../../Source/AudioEngine/ModTarget.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling ModTarget.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Modulation_bae63c5c.o: ../../Source/AudioEngine/Modulation.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling Modulation.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MultimodeFilter_bb09565e.o: ../../Source/AudioEngine/MultimodeFilter.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling MultimodeFilter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MultimodeOscillator_ce7def06.o: ../../Source/AudioEngine/MultimodeOscillator.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling MultimodeOscillator.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Note_9cf4f812.o: ../../Source/AudioEngine/Note.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling Note.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OscGroup_884ee260.o: ../../Source/AudioEngine/OscGroup.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling OscGroup.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Oszillator_d6911909.o: ../../Source/AudioEngine/Oszillator.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling Oszillator.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Pulse_d1ab8c3.o: ../../Source/AudioEngine/Pulse.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling Pulse.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Random_b42b6da3.o: ../../Source/AudioEngine/Random.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling Random.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SampleAndHold_e980c4d6.o: ../../Source/AudioEngine/SampleAndHold.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling SampleAndHold.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Sampler_7bd2472.o: ../../Source/AudioEngine/Sampler.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling Sampler.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Sawtooth_1afc249f.o: ../../Source/AudioEngine/Sawtooth.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling Sawtooth.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Sine_5cb8612d.o: ../../Source/AudioEngine/Sine.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling Sine.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StereoChorus_a12912de.o: ../../Source/AudioEngine/StereoChorus.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling StereoChorus.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StereoDelay_2a5984d5.o: ../../Source/AudioEngine/StereoDelay.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling StereoDelay.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StereoReverb_5612850a.o: ../../Source/AudioEngine/StereoReverb.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling StereoReverb.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Voice_5d6e09dc.o: ../../Source/AudioEngine/Voice.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling Voice.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/WaveTable_196c08ff.o: ../../Source/AudioEngine/WaveTable.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling WaveTable.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/WhiteNoise_e966d0d1.o: ../../Source/AudioEngine/WhiteNoise.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling WhiteNoise.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PluginProcessor_a059e380.o: ../../Source/PluginProcessor.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling PluginProcessor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PluginEditor_94d4fb09.o: ../../Source/PluginEditor.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling PluginEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling BinaryData.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BinaryData2_fa243da8.o: ../../JuceLibraryCode/BinaryData2.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling BinaryData2.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_basics_8a4e984a.o: ../../JuceLibraryCode/include_juce_audio_basics.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_audio_basics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_devices_63111d02.o: ../../JuceLibraryCode/include_juce_audio_devices.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_audio_devices.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_formats_15f82001.o: ../../JuceLibraryCode/include_juce_audio_formats.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_audio_formats.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_plugin_client_ARA_31a052ed.o: ../../JuceLibraryCode/include_juce_audio_plugin_client_ARA.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_audio_plugin_client_ARA.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_processors_10c03666.o: ../../JuceLibraryCode/include_juce_audio_processors.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_audio_processors.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_processors_ara_2a4c6ef7.o: ../../JuceLibraryCode/include_juce_audio_processors_ara.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_audio_processors_ara.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_processors_lv2_libs_12bdca08.o: ../../JuceLibraryCode/include_juce_audio_processors_lv2_libs.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_audio_processors_lv2_libs.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_audio_utils_9f9fb2d6.o: ../../JuceLibraryCode/include_juce_audio_utils.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_audio_utils.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_core_f26d17db.o: ../../JuceLibraryCode/include_juce_core.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_core.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_core_CompilationTime_9257742c.o: ../../JuceLibraryCode/include_juce_core_CompilationTime.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_core_CompilationTime.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_cryptography_8cb807a8.o: ../../JuceLibraryCode/include_juce_cryptography.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_cryptography.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_data_structures_7471b1e3.o: ../../JuceLibraryCode/include_juce_data_structures.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_data_structures.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_dsp_aeb2060f.o: ../../JuceLibraryCode/include_juce_dsp.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_dsp.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_events_fd7d695.o: ../../JuceLibraryCode/include_juce_events.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_events.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_graphics_f817e147.o: ../../JuceLibraryCode/include_juce_graphics.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_graphics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_graphics_Harfbuzz_60c52ba2.o: ../../JuceLibraryCode/include_juce_graphics_Harfbuzz.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_graphics_Harfbuzz.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_graphics_Sheenbidi_c310974d.o: ../../JuceLibraryCode/include_juce_graphics_Sheenbidi.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_graphics_Sheenbidi.c"
	$(V_AT)$(CC) $(JUCE_CFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_gui_basics_e3f79785.o: ../../JuceLibraryCode/include_juce_gui_basics.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_gui_basics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_gui_extra_6dee1c1a.o: ../../JuceLibraryCode/include_juce_gui_extra.cpp pre_build/juce_LinuxSubprocessHelperBinaryData.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_gui_extra.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_opengl_a8a032b.o: ../../JuceLibraryCode/include_juce_opengl.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_opengl.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_osc_f3df604d.o: ../../JuceLibraryCode/include_juce_osc.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_osc.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_video_be78589.o: ../../JuceLibraryCode/include_juce_video.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_juce_video.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_LinuxSubprocessHelperBinaryData_e1db7dc.o: pre_build/juce_LinuxSubprocessHelperBinaryData.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling juce_LinuxSubprocessHelperBinaryData.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_SHARED_CODE) $(JUCE_CFLAGS_SHARED_CODE) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_LV2ManifestHelper_c0a9d409.o: ~/JUCE/modules/juce_audio_plugin_client/LV2/juce_LV2ManifestHelper.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling juce_LV2ManifestHelper.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_LV2_MANIFEST_HELPER) $(JUCE_CFLAGS_LV2_MANIFEST_HELPER) -o "$@" -c "$<"

$(JUCE_OBJDIR)/execinfo.cmd:
	-$(V_AT)mkdir -p $(@D)
	-@if [ -z "$(V_AT)" ]; then echo "Checking if we need to link libexecinfo"; fi
	$(V_AT)printf "int main() { return 0; }" | $(CXX) -x c++ -o $(@D)/execinfo.x -lexecinfo - >/dev/null 2>&1 && printf -- "-lexecinfo" > "$@" || touch "$@"

$(JUCE_OBJDIR)/cxxfs.cmd:
	-$(V_AT)mkdir -p $(@D)
	-@if [ -z "$(V_AT)" ]; then echo "Checking if we need to link stdc++fs"; fi
	$(V_AT)printf "int main() { return 0; }" | $(CXX) -x c++ -o $(@D)/cxxfs.x -lstdc++fs - >/dev/null 2>&1 && printf -- "-lstdc++fs" > "$@" || touch "$@"

$(JUCE_OBJDIR)/juce_LinuxSubprocessHelper_d0e1814f.o: make_helpers/juce_LinuxSubprocessHelper.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling make_helpers/juce_LinuxSubprocessHelper.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$(JUCE_OBJDIR)/juce_LinuxSubprocessHelper_d0e1814f.o" -c "$<" $(TARGET_ARCH)

$(JUCE_BINDIR)/juce_linux_subprocess_helper: $(JUCE_OBJDIR)/juce_LinuxSubprocessHelper_d0e1814f.o
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	@echo "Linking $(JUCE_BINDIR)/juce_linux_subprocess_helper"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$(JUCE_BINDIR)/juce_linux_subprocess_helper" "$(JUCE_OBJDIR)/juce_LinuxSubprocessHelper_d0e1814f.o" $(JUCE_LDFLAGS) $(TARGET_ARCH)

$(JUCE_OBJDIR)/juce_SimpleBinaryBuilder_18cb8206.o: make_helpers/juce_SimpleBinaryBuilder.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling make_helpers/juce_SimpleBinaryBuilder.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$(JUCE_OBJDIR)/juce_SimpleBinaryBuilder_18cb8206.o" -c "$<" $(TARGET_ARCH)

$(JUCE_BINDIR)/juce_simple_binary_builder: $(JUCE_OBJDIR)/juce_SimpleBinaryBuilder_18cb8206.o
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	@echo "Linking $(JUCE_BINDIR)/juce_simple_binary_builder"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) -o "$(JUCE_BINDIR)/juce_simple_binary_builder" "$(JUCE_OBJDIR)/juce_SimpleBinaryBuilder_18cb8206.o" $(JUCE_LDFLAGS) $(TARGET_ARCH)

pre_build/juce_LinuxSubprocessHelperBinaryData.cpp: $(JUCE_BINDIR)/juce_linux_subprocess_helper $(JUCE_BINDIR)/juce_simple_binary_builder
	$(V_AT)"$(JUCE_BINDIR)/juce_simple_binary_builder" "$(JUCE_BINDIR)/juce_linux_subprocess_helper" "pre_build" "juce_LinuxSubprocessHelperBinaryData" LinuxSubprocessHelperBinaryData

clean:
	@echo Cleaning Lupo
	$(V_AT)$(CLEANCMD)

strip:
	@echo Stripping Lupo
	-$(V_AT)$(STRIP) --strip-unneeded $(JUCE_OUTDIR)/$(JUCE_TARGET_LV2_PLUGIN)
	-$(V_AT)$(STRIP) --strip-unneeded $(JUCE_OUTDIR)/$(JUCE_TARGET_LV2_MANIFEST_HELPER)

-include $(OBJECTS_LV2_PLUGIN:%.o=%.d)
-include $(OBJECTS_SHARED_CODE:%.o=%.d)
-include $(OBJECTS_LV2_MANIFEST_HELPER:%.o=%.d)

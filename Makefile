
### Options: ###

# C++ compiler 
CXX = g++

# flags for C++
CXXFLAGS ?= -Wall -Wextra -pedantic -Wformat-security -Winit-self -Wswitch-default -Wswitch-enum -Wfloat-equal -Wundef -Wshadow -Wcast-qual -Wconversion -Wlogical-op -Winline -Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn

# [Debug,Release]
BUILD ?= Debug

### Conditionally set variables: ###

ifeq ($(BUILD),Debug)
BUILD_FLAGS = -g3 -rdynamic -fstack-protector-all -fno-inline -O0 -D_DEBUG -Iinclude/
endif
ifeq ($(BUILD),Release)
BUILD_FLAGS = -O2 -DNDEBUG -Iinclude/
endif
ifeq ($(BUILD),Debug)
LINK_FLAGS = -g3 -rdynamic
endif
ifeq ($(BUILD),Release)
LINK_FLAGS = 
endif

### Variables: ###

CPPDEPS = -MT $@ -MF`echo $@ | sed -e 's,\.o$$,.d,'` -MD -MP

CPPZMQ_CXXFLAGS = $(BUILD_FLAGS) $(CXXFLAGS)
CPPZMQ_TEST_OBJECTS =  \
	$(BUILD)/main.o \
	$(BUILD)/client.o \
	$(BUILD)/server.o \
	
### Targets: ###

all: $(BUILD) $(BUILD)/example1

clean: 
	rm -f $(BUILD)/*.o
	rm -f $(BUILD)/*.d
	rm -f $(BUILD)/cppzmq_test

$(BUILD): $(BUILD)/
	mkdir -p $(BUILD)


$(BUILD)/example1: $(CPPZMQ_TEST_OBJECTS)
	$(CXX) -o $@ $(CPPZMQ_TEST_OBJECTS) $(LINK_FLAGS) -Llib/linux/ -lzmq -lrt -pthread


$(BUILD)/main.o: src/main.cpp
	$(CXX) -c -o $@ $(CPPZMQ_CXXFLAGS) $(CPPDEPS) $<

$(BUILD)/client.o: src/client.cpp
	$(CXX) -c -o $@ $(CPPZMQ_CXXFLAGS) $(CPPDEPS) $<

$(BUILD)/server.o: src/server.cpp
	$(CXX) -c -o $@ $(CPPZMQ_CXXFLAGS) $(CPPDEPS) $<


.PHONY: all clean


# Dependencies tracking:
-include $(BUILD)/*.d



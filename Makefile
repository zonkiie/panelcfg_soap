CXX := ccache g++
OUT_DIR := build
INSTALL_DIR := ~/public_html/panelcfg_soap/
EXECUTABLE := $(OUT_DIR)/panelsoap.cgi
GSOAP_ROOT_DIR := /usr/share/gsoap/
GSOAP_PLUGIN_DIR := $(GSOAP_ROOT_DIR)/plugin
ALL_CPP_FILES := $(wildcard *.cpp)
ALL_C_FILES := $(wildcard *.c)
LDFLAGS := -lgsoap++ -lgsoapssl++ -lpthread -lcrypt -lssl -lcrypto -lboost_filesystem -lboost_iostreams -lboost_signals -lboost_system -lpam
CXXFLAGS := -std=c++11 -Wall -g -D WITH_IPV6 -DWITH_OPENSSL -I. -I$(OUT_DIR) -I$(GSOAP_ROOT_DIR) -I$(GSOAP_PLUGIN_DIR) -I/usr/include/zbcl
GENERATED_SOURCE_FILES := $(OUT_DIR)/soapC.cpp $(OUT_DIR)/soapServer.cpp
#GENERATED_SOURCE_FILES := $(wildcard build/*.cpp)
ALL_SOURCE_FILES := $(ALL_CPP_FILES) $(ALL_C_FILES) $(GENERATED_SOURCE_FILES)
#ALL_OBJECT_FILES := $(addprefix $(OUT_DIR)/, $(ALL_CPP_FILES:.cpp=.o))
#ALL_OBJECT_FILES := $(ALL_SOURCE_FILES:.cpp=.o)
ALL_OBJECT_FILES := $(ALL_CPP_FILES:%.cpp=$(OUT_DIR)/%.o) $(GENERATED_SOURCE_FILES:.cpp=.o)
#wsseapi.o
all: $(EXECUTABLE)
#$(EXECUTABLE): $(OUT_DIR) $(ALL_SOURCE_FILES)
#	g++ $(CXXFLAGS) -o $@ $(ALL_SOURCE_FILES) $(LDFLAGS)
$(EXECUTABLE): $(ALL_OBJECT_FILES) | $(OUT_DIR)
	$(CXX) -std=c++11 -o $@ $(ALL_OBJECT_FILES) $(LDFLAGS) /usr/lib/zbcl/libzbcl.a
$(OUT_DIR):
	mkdir -p $(OUT_DIR)
$(OUT_DIR)/soapC.cpp $(OUT_DIR)/soapServer.cpp: panelsoap.h
	soapcpp2 -2 -b -x -SL -d$(OUT_DIR) -I/usr/share/gsoap/import -c++11 $<
$(OUT_DIR)/wsseapi.o: $(GSOAP_PLUGIN_DIR)/wsseapi.cpp
$(OUT_DIR)/%.o: %.cpp | $(OUT_DIR)
	$(CXX) -c $(CXXFLAGS) $< -o $@
$(ALL_OBJECT_FILES): $(ALL_SOURCE_FILES) | $(OUT_DIR)
mrproper: clean
clean:
	-rm -rf $(OUT_DIR) *.o
archive: clean
	tar -C .. --exclude-vcs -cavf ../panelcfg_soap-`date +"%Y%m%d%H%M%S"`.tar.bz2 panelcfg_soap/
install: $(EXECUTABLE)
	mkdir -p $(INSTALL_DIR)
	cp -a $(OUT_DIR)/*.wsdl $(OUT_DIR)/*.xsd $(INSTALL_DIR)
	cp -a $(EXECUTABLE) $(INSTALL_DIR)

uninstall:
	-rm -rvf $(INSTALL_DIR)
print:
	@echo $(ALL_SOURCE_FILES)
	@echo $(ALL_OBJECT_FILES)


OUT_DIR := build
INSTALL_DIR := ~/public_html/panelcfg_soap/
EXECUTABLE := $(OUT_DIR)/panelsoap.cgi
GSOAP_ROOT_DIR := /usr/share/gsoap/
GSOAP_PLUGIN_DIR := $(GSOAP_ROOT_DIR)/plugin
ALL_CPP_FILES := *.cpp
LDFLAGS := -lgsoap++ -lpthread -lcrypt
CXXFLAGS := -std=c++11 -Wall -g -D WITH_IPV6 -I. -I$(OUT_DIR) -I$(GSOAP_ROOT_DIR) -I$(GSOAP_PLUGIN_DIR)
ALL_SOURCE_FILES := $(ALL_CPP_FILES) $(OUT_DIR)/soapC.cpp $(OUT_DIR)/soapServer.cpp
all: $(EXECUTABLE)
$(EXECUTABLE): $(OUT_DIR) $(ALL_SOURCE_FILES)
	g++ $(CXXFLAGS) -o $@ $(ALL_SOURCE_FILES) $(LDFLAGS)
$(OUT_DIR):
	mkdir -p $(OUT_DIR)
$(OUT_DIR)/soapC.cpp $(OUT_DIR)/soapServer.cpp:
	soapcpp2 -2 -b -x -SL -d$(OUT_DIR) -I/usr/share/gsoap/import panelsoap.h
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


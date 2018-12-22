OUT_DIR := build
INSTALL_DIR := ~/public_html/panelcfg_soap/
EXECUTABLE := $(OUT_DIR)/panelsoap.cgi
GSOAP_PLUGIN_DIR := /usr/share/gsoap/plugin
all: $(EXECUTABLE)
$(EXECUTABLE): panelsoap.cpp panelsoap_functions.cpp
	mkdir -p $(OUT_DIR)
	soapcpp2 -2 -b -x -SL -d$(OUT_DIR) -I/usr/share/gsoap/import panelsoap.h
	g++ -Wall -g -std=c++11 -D WITH_IPV6 -I. -I$(OUT_DIR) -I$(GSOAP_PLUGIN_DIR) -o $@ $^ $(OUT_DIR)/soapC.cpp $(OUT_DIR)/soapServer.cpp -lgsoap++

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


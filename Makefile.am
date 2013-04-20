# build program

bin_PROGRAMS = avrprog2

CXXFLAGS += -O2 -Wall -std=c++0x

configfilesdir = @datadir@/@PACKAGE@
homeconfigfilesdir = .@PACKAGE@
avrprog2_CXXFLAGS = -DCONFIG_DIR="\"$(configfilesdir)/\"" -DHOME_CONFIG_DIR="\"$(homeconfigfilesdir)/\""

avrprog2_SOURCES = \
	src/avrprog.h \
	src/CAVRDevice.cpp \
	src/CAVRDevice.h \
	src/CAvrProgCommands.cpp \
	src/CAvrProgCommands.h \
	src/CAVRprog.cpp \
	src/CAVRprog.h \
	src/CEEPROMOptions.cpp \
	src/CEEPROMOptions.h \
	src/CFlashOptions.cpp \
	src/CFlashOptions.h \
	src/CFormat.cpp \
	src/CFormat.h \
	src/CFusesOptions.cpp \
	src/CFusesOptions.h \
	src/CHexFile.cpp \
	src/CHexFile.h \
	src/CLArgumentException.cpp \
	src/CLArgumentException.h \
	src/CMemoryOptions.cpp \
	src/CMemoryOptions.h \
	src/COut.cpp \
	src/COut.h \
	src/CProgramOptions.cpp \
	src/CProgramOptions.h \
	src/CProgressbar.cpp \
	src/CProgressbar.h \
	src/CUSBCommunication.cpp \
	src/CUSBCommunication.h \
	src/main.cpp \
	src/ExceptionBase.cpp \
	src/ExceptionBase.h

# additional files to install
dist_configfiles_DATA = \
	config/atmega1280.xml \
	config/atmega128.xml \
	config/atmega16.xml \
	config/atmega2560.xml

udevrulesdir = /lib/udev/rules.d
dist_udevrules_DATA = 60-avrprog.rules

# additional files
dist_noinst_DATA = \
	readme.txt \
	doc.txt \
	changelog.txt

# build documentation and manpage

doc: $(SOURCES) doc.txt readme.txt man.txt
	doxygen Doxyfile

man/man1/@PACKAGE@.1: man.txt
	doxygen Doxyfile_man

distclean-local:
	rm -rf doc
	rm -rf man

man_MANS = man/man1/@PACKAGE@.1


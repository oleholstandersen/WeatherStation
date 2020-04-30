# Defines the RPI variable which is needed by rc-switch/RCSwitch.h
CXXFLAGS=-DRPI

all: senduint32 receiveuint32

senduint32: ../rc-switch/RCSwitch.o senduint32.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $+ -o $@ -lwiringPi -lwiringPiDev -lcrypt
	
receiveuint32: ../rc-switch/RCSwitch.o receiveuint32.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $+ -o $@ -lwiringPi -lwiringPiDev -lcrypt

clean:
	$(RM) ../rc-switch/*.o *.o senduint32 receiveuint32

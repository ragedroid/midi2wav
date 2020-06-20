
BSDIR ?= $(shell dirname `pwd`)
include BasicSynth.cfg

LIBMODULES= Common Instruments Notelist
BINMODULES= Midi2Wav
BSMODULES= $(LIBMODULES) $(BINMODULES)

.PHONY: all chkdirs clean new tests $(BSMODULES)

all: chkdirs $(BSMODULES)
	@echo All done

libs: chkdirs $(LIBMODULES)

chkdirs:
	@echo Building from $(BSDIR)
	@test -d $(BSBIN) || mkdir $(BSBIN)
	@test -d $(BSLIB) || mkdir $(BSLIB)

new: chkdirs
	for dd in $(BSMODULES); do $(MAKE) -C $$dd  --no-print-directory BSDIR=$(BSDIR) new; done

clean:
	for dd in $(BSMODULES); do $(MAKE) -C $$dd  --no-print-directory BSDIR=$(BSDIR) clean; done
	
tests:
	-rm -f $(BSBIN)/example*.wav
	cd $(BSBIN); for d in example* ; do ./$$d ; done

$(BSMODULES):
	@echo make $@
	@$(MAKE) -C $@ $(MAKEFLAGS) --no-print-directory BSDIR=$(BSDIR)

$(BINMODULES): $(LIBMODULES)


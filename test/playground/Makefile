KL = ../../build/KL

.PHONY: clean
.PRECIOUS: %.ll %.s

clean:
	rm -f *.s *.ll

%.out: %.ll
	llvm-as -o $@ $<

%.s: %.ll 
	llc -o $@ $<

%.ll: %.kl 
	$(KL) $< > $@

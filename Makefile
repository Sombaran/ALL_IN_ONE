#::all

#SUBDIR =  CPlusPlusProgram Data_structures Inter-Process-Communication Linux TCL-(Tool-Command-Language) basic-programming cplusplus-data-structuredesign-pattern exception-handlingfile-handling-in-cpp gdb-basic interview-question-series interview-question multi-threading object-oriented-program searching-algorithm semaphores simpleProgram smart-pointer sorting-algorithm special-implementation standard-template-library-algorithm standard_template_library template type-cast cpp-11-concepts

SUBDIRS := $(wildcard */.)

all: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)
###############################################################################
####################### <github.com/aszdrick/magefile> ########################
############################ <aszdrick@gmail.com> #############################
###############################################################################
# Copyright (c) 2017-2018 Marleson Graf

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
############################## PROJECT VARIABLES ##############################
MAKEFLAGS +=-rR
### SUPPORTED SUFFIXES
HEADERSUFFIXES :=h hpp
### DIRECTORIES
SRCDIR :=src
INCDIR :=include
OBJDIR :=build
BINDIR :=bin
TSTDIR :=tests
DEPDIR :=.deps
### PROGRAM-RELATED VARIABLES
# Files containing the main() function
MAINFILES :=$(SRCDIR)/main.cpp
# Binaries corresponding to each file with a main() function
BINARIES  :=$(BINDIR)/execute_me
# Compiler & linker flags
CXX      :=g++
CXXFLAGS :=-std=c++17 -Wall
LDFLAGS  :=
LDLIBS   :=-lsfml-audio -lsfml-graphics -lsfml-system -lsfml-window -pthread -lX11
INCLUDE  :=-I$(INCDIR)
### TESTS-RELATED VARIABLES
# Files containing the main() function
TMAINFILES :=$(wildcard $(TSTDIR)/*.cpp)
# Binaries corresponding to each file with a main() function
TBINARIES  :=$(patsubst $(TSTDIR)/%.cpp,$(BINDIR)/%,$(TMAINFILES))
# Compiler & linker flags
TCXXFLAGS :=
TLDFLAGS  :=
TLDLIBS   :=
TINCLUDE  :=
### MAKEFILE CONTROL VARIABLES
# Debug flag, if != 0 deactivates all supressed echoing
DEBUG :=0

############################# AUTOMATIC VARIABLES #############################
# Dependency files
MAINDEPS :=$(patsubst %.cpp,$(DEPDIR)/%.d,$(MAINFILES))
TMAINDEPS :=$(patsubst %.cpp,$(DEPDIR)/%.d,$(TMAINFILES))
MDEPS :=$(MAINDEPS) $(TMAINDEPS)
NDEPS :=$(filter-out $(MDEPS),$(shell find $(DEPDIR) -name *.d 2> /dev/null))
# Phony calls to build specific binaries
CALLS :=$(patsubst $(BINDIR)/%,%,$(BINARIES))
TCALLS :=$(patsubst $(BINDIR)/%,%,$(TBINARIES))
ALLCALLS :=$(CALLS) $(TCALLS)
# Autogenerated directories
AUTODIR :=$(BINDIR) $(SRCDIR) $(INCDIR)
# Auxiliar variables
sh_nop :=:
empty :=
space :=$(empty) $(empty)
# No operation of shell script
# Command to print status messages
INFO :=@echo
# Patterns to filter all header files
HEADERPATTERNS :=$(addprefix %.,$(HEADERSUFFIXES))
# Creation of variables associating each main file with a program
MAINEXECVARS :=$(addsuffix _EXEC:=,$(basename $(MAINFILES) $(TMAINFILES)))
MAINEXECVARS :=$(join $(MAINEXECVARS), $(BINARIES) $(TBINARIES))
$(foreach var,$(MAINEXECVARS),$(eval $(var)))
SOURCES =$(shell find $(SRCDIR) -name '*.cpp' 2> /dev/null)
SOURCES +=$(shell find $(TSTDIR) -name '*.cpp' 2> /dev/null)
OBJECTS :=$(eval OBJECTS:=$(patsubst %.cpp,$(OBJDIR)/%.o,$(SOURCES)))$(OBJECTS)
# Arguments: target string, output name, input name
makedep = $(CXX) $(CXXFLAGS) $(INCLUDE) -MM -MG -MT $1 -MF "$2" $3
# Arguments: file to be written, target file, objects variable name
define makedeps
	$(info [makedep] $2 -> .d)
	$(shell mkdir -p $(dir $1))
	$(shell $(call makedep,"$(OBJDIR)/$3.o $1",$1,$2))
	$(eval TARGET_OBJ:=$(patsubst %.cpp,$(OBJDIR)/%.o,$2))
	$(eval $3_EDGES:=$(abspath $(filter $(HEADERPATTERNS),$(shell cat $1))))
	$(eval $3_EDGES:=$(patsubst $(abspath $(INCDIR))%,$(SRCDIR)%,$($3_EDGES)))
	$(eval $3_EDGES:=$(patsubst %,$(OBJDIR)/%.o,$(basename $($3_EDGES))))
	$(eval HPPSONLY:=$(filter-out $(OBJECTS),$($3_EDGES)))
	$(eval $3_EDGES:=$(filter $($3_EDGES),$(OBJECTS)))
	$(eval $3_EDGES:=$(filter-out $(TARGET_OBJ),$($3_EDGES)))
	$(eval STEMLIST:=$(patsubst $(OBJDIR)/%.o,%,$($3_EDGES)))
	$(eval HPPSONLY:=$(patsubst $(OBJDIR)/%.o,%,$(HPPSONLY)))
	$(eval $3_EDGES:=$(patsubst $(OBJDIR)/%.o,%_EDGES,$($3_EDGES)))
	$(eval NDEPS+=$1)
	$(file >> $1,$1: $(patsubst %,%.cpp,$(STEMLIST) $(HPPSONLY)))
	$(file >> $1,$3_EDGES =$$(eval $3_EDGES:=$(TARGET_OBJ)\
	 $($3_EDGES))$$($3_EDGES))
	$(file >> $1,$3_OBJS =$$(eval RET:=)$$(eval $3_OBJS:=$$(call\
	 expand_deps,$3_EDGES)$$(RET))$$($3_OBJS))
	$(if $($3_EXEC),$(file >> $1,$($3_EXEC): $$($3_OBJS)))
	$(foreach s,$(STEMLIST),$(if $(filter-out $(NDEPS),$(DEPDIR)/$(s).d),
		$(call makedeps,$(DEPDIR)/$(s).d,$(s).cpp,$(s))))
endef
# Receives a *_EDGES list to expand
# *_EDGES list value: *.o [other *_EDGES lists]
define expand_deps
	$(foreach list,$1,\
		$(eval RG1:=$(firstword $($(list))))\
		$(if $(filter-out $(RET),$(RG1)),\
			$(eval RET+=$(RG1))\
			$(call expand_deps,$(filter-out $(RG1),$($(list))))))
endef
# If DEBUG is not active (i.e, equals 1), set supressing character (@)
ifeq ($(DEBUG),0)
SILENT :=@
endif

.PHONY: all makedir clean distclean tests $(ALLCALLS)

################################# MAIN RULES ##################################
all: makedir $(BINARIES)

$(ALLCALLS): %: $(BINDIR)/%

$(BINARIES) $(TBINARIES):
	$(INFO) "[linking] $@"
	$(SILENT) mkdir -p $(dir $@)
	$(SILENT) $(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCLUDE) $^ $(LDLIBS) -o $@

$(OBJDIR)/%.o: %.cpp
	$(INFO) "[  $(CXX)  ] $< -> .o"
	$(SILENT) mkdir -p $(OBJDIR)/$(*D)
	$(SILENT) $(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(DEPDIR)/%.d: %.cpp
	$(SILENT) $(sh_nop) $(call makedeps,$@,$<,$*)

makedir: | $(AUTODIR)

$(AUTODIR):
	$(INFO) "[ mkdir ] Creating directory '$@'"
	$(SILENT) mkdir -p $@

################################ TESTS RULES ##################################
tests: makedir $(TBINARIES)

$(TBINARIES): LDLIBS +=$(TLDLIBS)

$(TBINARIES): LDFLAGS +=$(TLDFLAGS)

$(OBJDIR)/$(TSTDIR)/%.o: CXXFLAGS +=$(TCXXFLAGS)

$(OBJDIR)/$(TSTDIR)/%.o: INCLUDE +=$(TINCLUDE)

################################ CLEAN RULES ##################################
clean:
	$(SILENT) rm -rf $(OBJDIR)
	$(SILENT) rm -rf $(BINDIR)

distclean: clean
	$(SILENT) rm -rf $(DEPDIR)

################################ PREREQUISITES ################################
# Do not include list of dependencies with clean rules
ifeq ($(filter-out all $(CALLS) $(BINARIES),$(MAKECMDGOALS)),)
  -include $(NDEPS)
  -include $(MAINDEPS)
else
  ifeq ($(filter-out tests $(TCALLS) $(TBINARIES),$(MAKECMDGOALS)),)
    -include $(NDEPS)
    -include $(TMAINDEPS)
  endif
endif

#
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
# 
# Description:
# 
#

# Generation of TARGETFILE, BITMAPFILE or HEADERFILE is depending on *.svgb and *.bmp files
$(TARGETFILE) $(BITMAPFILE) $(HEADERFILE) : MIFCONVOPTS:=$(MIFCONVOPTS)
$(TARGETFILE) $(BITMAPFILE) $(HEADERFILE) : BITMAPS:=$(addsuffix .bmp,$(BITMAPBASESOURCES))
$(TARGETFILE) $(BITMAPFILE) $(HEADERFILE) : TARGETFILE:=$(TARGETFILE)
$(TARGETFILE) $(BITMAPFILE) $(HEADERFILE) : HEADERFILE:=$(HEADERFILE)
$(TARGETFILE) $(BITMAPFILE) $(HEADERFILE) : PARAMFILE:=$(PARAMFILE)
$(TARGETFILE) $(BITMAPFILE) $(HEADERFILE) : $(SVGBFILES) $(BMPFILES)
    # Set the prefix for header 
    # Pick only input files matching *.svgb (created directory may also appear in the input)
    # And set the colordepth options
ifneq ($(strip $(HEADERFILE)),)
	echo $(addprefix -h,$(HEADERFILE))  > $(PARAMFILE)
	echo $(wordlist 1,100,$(MIFCONVOPTS)) >> $(PARAMFILE)
else
	echo $(wordlist 1,100,$(MIFCONVOPTS)) > $(PARAMFILE)
endif

ifneq ($(strip $(wordlist 101,200,$(MIFCONVOPTS))),)
	echo $(wordlist 101,200,$(MIFCONVOPTS)) >> $(PARAMFILE)
endif

ifneq ($(strip $(wordlist 201,300,$(MIFCONVOPTS))),)
	echo $(wordlist 201,300,$(MIFCONVOPTS)) >> $(PARAMFILE)
endif

ifneq ($(strip $(wordlist 301,400,$(MIFCONVOPTS))),)
	echo $(wordlist 301,400,$(MIFCONVOPTS)) >> $(PARAMFILE)
endif

ifneq ($(strip $(wordlist 401,500,$(MIFCONVOPTS))),)
	echo $(wordlist 401,500,$(MIFCONVOPTS)) >> $(PARAMFILE)
endif

ifneq ($(strip $(wordlist 501,600,$(MIFCONVOPTS))),)
	echo $(wordlist 501,600,$(MIFCONVOPTS)) >> $(PARAMFILE)
endif

ifneq ($(strip $(wordlist 601,700,$(MIFCONVOPTS))),)
	echo $(wordlist 601,700,$(MIFCONVOPTS)) >> $(PARAMFILE)
endif

ifneq ($(strip $(wordlist 701,800,$(MIFCONVOPTS))),)
	echo $(wordlist 701,800,$(MIFCONVOPTS)) >> $(PARAMFILE)
endif

ifneq ($(strip $(wordlist 801,900,$(MIFCONVOPTS))),)
	echo $(wordlist 801,900,$(MIFCONVOPTS)) >> $(PARAMFILE)
endif

ifneq ($(strip $(wordlist 901,1000,$(MIFCONVOPTS))),)
	echo $(wordlist 901,1000,$(MIFCONVOPTS)) >> $(PARAMFILE)
endif

ifneq ($(strip $(wordlist 1001,1100,$(MIFCONVOPTS))),)
	echo $(wordlist 1001,1100,$(MIFCONVOPTS)) >> $(PARAMFILE)
endif

ifneq ($(strip $(wordlist 1101,1200,$(MIFCONVOPTS))),)
	echo $(wordlist 1101,1200,$(MIFCONVOPTS)) >> $(PARAMFILE)
endif

ifneq ($(strip $(wordlist 1201,1300,$(MIFCONVOPTS))),)
	echo $(wordlist 1201,1300,$(MIFCONVOPTS)) >> $(PARAMFILE)
endif

ifneq ($(strip $(wordlist 1301,1400,$(MIFCONVOPTS))),)
	echo $(wordlist 1301,1400,$(MIFCONVOPTS)) >> $(PARAMFILE)
endif

ifneq ($(strip $(wordlist 1401,1500,$(MIFCONVOPTS))),)
	echo $(wordlist 1401,1500,$(MIFCONVOPTS)) >> $(PARAMFILE)
endif

ifneq ($(strip $(wordlist 1501,1600,$(MIFCONVOPTS))),)
	echo $(wordlist 1501,1600,$(MIFCONVOPTS)) >> $(PARAMFILE)
endif



# Execute Mifconv with the parameter list 
ifneq ($(strip $(SOURCEDIR)),)
	$(MIFCONV) $(TARGETFILE) -i$(SOURCEDIR) -e -f$(PARAMFILE)
else
	$(MIFCONV) $(TARGETFILE) -e -f$(PARAMFILE)
endif



%-erase :
	-$(RM) $*

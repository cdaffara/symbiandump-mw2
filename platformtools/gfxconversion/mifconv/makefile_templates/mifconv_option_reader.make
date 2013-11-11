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


# This file converts the input stored in $(OPTIONS) variable into several different variables
# depending in the content.
#
# $(OPTIONS) variable is assumed to contain following syntax:
# -X,Y filename.ext -X filename.ext
#
# where X and Y are numbers for the color depth.
# The first number is the actual color depth of the file and the second is the color depth
# of the mask file. The second number (and the comma) may or may not be defined.
#
# Since there are color depths defined for both the actual file and it's mask file, we need
# to determine also the name of the mask file somehow. 
# It is assumed that depending on the mask file color depth (Y) 
# the mask file has a different string appended to it's file name.
#
# These mask file strings must be defined in the variable $(MASK_OPTION_Y) where Y is the 
# mask file color depth.
#
#
# Options will be splitted into variables of $(FILES) which contains the filenames after the
# options and to OPTION_$(FILE) where $(FILE) is each of the $(FILES) and 
# $(OPTION1_$(FILE)) and $(OPTION2_$(FILE)) depending whether the option for the file
# was infact 2 options separated with comma or not.
# 
# Example: lets consider following variables are defined:
#
# MASK_OPTION_1=_mask
# MASK_OPTION_8=_mask_soft
# 
# And that our input is:
# OPTIONS=-c8,1 calc_paper -c8,8 display_side_l -c8 display_center
#
# The option reader would go through the $(OPTIONS) and fill out following variables:
#
# FILES=calc_paper display_side_l display_center
#
#
# OPTION_calc_paper=-c8,2
# OPTION1_calc_paper=-c8
# OPTION2_calc_paper=2
# OPTION1_calc_paper_mask=-2
#
# OPTION_display_side_l=-c8,8
# OPTION1_display_side_l=-c8
# OPTION2_display_side_l=8
# OPTION1_calc_paper_mask_soft=-8
#
# OPTION1_display_center=-c8


# Get files from the option list, i.e. everything that does not start with '-'
FILES := $(foreach OPTION,\
	$(OPTIONS),\
	$(filter-out -%, $(OPTION)))
#$(warning FILES $(FILES))


# Option list without files
OPTION_LIST_WITHOUT_FILES:=$(filter-out $(FILES), $(OPTIONS))
#$(warning OPTION_LIST_WITHOUT_FILES $(OPTION_LIST_WITHOUT_FILES))


# Strip out possible suffixes from all files
FILES:=$(basename $(FILES))
#$(warning FILES $(FILES))


# Get options for each file, in the form of OPTION_$FILE
$(foreach FILE,\
	$(FILES),\
	$(eval OPTION_$(FILE):=$(firstword $(OPTION_LIST_WITHOUT_FILES))) \
	$(eval OPTION_LIST_WITHOUT_FILES:=$(wordlist 2, $(words $(OPTION_LIST_WITHOUT_FILES)), $(OPTION_LIST_WITHOUT_FILES))) \
)


# If option consists of anything like *X,Y separate the parts from comma
# The part before the comma is in OPTION1_file, if no comma it is same as OPTION_file
# The possible part after the comma is in OPTION2_file
COMMA:=,
$(foreach FILE,\
	$(FILES),\
	$(eval SPACE_OPTIONS:=$(subst $(COMMA), ,$(OPTION_$(FILE)))) \
	$(eval OPTION1_$(FILE):=$(firstword $(SPACE_OPTIONS))) \
	$(eval OPTION2_$(FILE):=$(word 2,$(SPACE_OPTIONS))) \
)
       

# Generate mask files (from those files which had options after the comma)
$(foreach FILE,\
	$(FILES),\
	$(eval $(FILE)_MASKFILE:=$(FILE)$(MASK_OPTION_$(OPTION2_$(FILE)))) \
)


# Filter out other than mask files
MASKFILES:=$(filter %$(MASK_OPTION_1) %$(MASK_OPTION_8), $(foreach FILE, $(FILES), $($(FILE)_MASKFILE)))
#$(warning MASKFILES $(MASKFILES))


# Create options to mask files based on the option given after comma to the corresponding original file
$(foreach MASKFILE,\
	$(MASKFILES),\
	$(eval OPTION1_$(MASKFILE):=-$(OPTION2_$(subst $(MASK_OPTION_1),,$(subst $(MASK_OPTION_8),,$(MASKFILE))))) \
)

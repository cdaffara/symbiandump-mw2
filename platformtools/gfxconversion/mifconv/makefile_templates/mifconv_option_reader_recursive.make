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
#
# Options will be splitted into variables of $(FILES) which contains the filenames after the
# options and to OPTION_$(FILE) where $(FILE) is each of the $(FILES)
# 
# Example: lets consider following variables are defined:
#
# 
# And that our input is:
# OPTIONS=-c8,1 calc_paper -c8,8 display_side_l -c8 display_center
#
# The option reader would go through the $(OPTIONS) and fill out following variables:
#
# FILES=calc_paper display_side_l display_center
#
# OPTION_calc_paper=-c8,2
#
# OPTION_display_side_l=-c8,8
#
# OPTION_display_center=-c8

# File is the second word from the start, strip extension away just in case
FILE:=$(basename $(word 2,$(OPTIONS)))
#$(warning FILE $(FILE))

# Set OPTION_file variable to the option preceding the file
OPTION_$(FILE):=$(firstword $(OPTIONS))
#$(warning OPTION_$(FILE) $(OPTION_$(FILE)))

# Collect all the files 
FILES:=$(FILES) $(FILE)

# Shift away the 2 first variables from OPTIONS
OPTIONS:=$(wordlist 3,$(words $(OPTIONS)),$(OPTIONS))
#$(warning OPTIONS $(OPTIONS))

ifneq ($(strip $(OPTIONS)),)
  # Loop again
  include $(EPOCROOT)epoc32$/tools$/makefile_templates$/s60$/mifconv_option_reader_recursive.make
endif


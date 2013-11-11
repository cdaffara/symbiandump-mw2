#
# Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
#     Top makefile for Poison functiontests
#

# runs after 'abld makefile'
MAKMAKE:
        @echo *** DEBUG: MAKMAKE ${PLATFORM} ${CFG} ${TO_ROOT}

# runs after 'abld library'
LIB:
        @echo *** DEBUG: LIB ${PLATFORM} ${CFG} ${TO_ROOT}

# runs after 'abld resource'
RESOURCE:
        @echo *** DEBUG: RESOURCE ${PLATFORM} ${CFG} ${TO_ROOT}

# runs after 'abld target'
BLD:
        @echo *** DEBUG: BLD ${PLATFORM} ${CFG} ${TO_ROOT}

# runs after 'abld final'
ifeq ($(PLATFORM),WINS)
FINAL: COPY_TESTDATA
        @echo *** DEBUG: FINAL ${PLATFORM} ${CFG} ${TO_ROOT}
else
ifeq ($(PLATFORM),WINSCW)
FINAL: COPY_TESTDATA
		@echo *** DEBUG: FINAL ${PLATFORM} ${CFG} ${TO_ROOT}
#Create c:\resource directory
		-@mkdir \EPOC32\${PLATFORM}\c\resource
else
FINAL:
        @echo *** DEBUG: FINAL ${PLATFORM} ${CFG} ${TO_ROOT}
endif
endif

# runs when called from
COPY_TESTDATA:
#         Create testdata directory if it does not exist
		-@mkdir \EPOC32\winscw\c\system\TEST\TestData
		@echo Copying all testdata files needed by LandmarkTests, copy to ${PLATFORM} $(CFG)

#        Copy all landmarks databases to testdata

		copy ..\databases\landmarks\* \EPOC32\winscw\c\system\TEST\TestData

		copy ..\databases\landmarks\XML_files\*  \EPOC32\winscw\c\system\TEST\TestData

		@echo *** DEBUG: COPY_TESTDATA ${PLATFORM} ${CFG} ${TO_ROOT}
		@echo ......Done copying

# ???
RELEASABLES:
#		@echo *** DEBUG: RELEASABLES ${PLATFORM} ${CFG} ${TO_ROOT}
	  @echo

# runs after clean operations
CLEAN:
		@echo *** DEBUG: CLEAN ${PLATFORM} ${CFG} ${TO_ROOT}

# runs after clean operations
CLEANLIB:
		@echo ***  DEBUG:CLEANLIB ${PLATFORM} ${CFG} ${TO_ROOT}

# only runs during 'abld freeze'
FREEZE:
		@echo *** DEBUG: FREEZE ${PLATFORM} ${CFG} ${TO_ROOT}

# only runs when -savespace is specified.
SAVESPACE:
		@echo *** DEBUG: SAVESPACE ${PLATFORM} ${CFG} ${TO_ROOT}

# Generic target. Catches everything else (unknown targets etc.)
%:
#		@echo *** DEBUG: GENERIC $@ ${PLATFORM} ${CFG} ${TO_ROOT}
	@echo 
# End of File
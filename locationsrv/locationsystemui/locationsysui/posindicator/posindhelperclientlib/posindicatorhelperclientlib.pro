#
# Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
# Description: posindicator plugin project file
#

TEMPLATE = lib
TARGET = posindicatorhelper
DEPENDPATH += .
INCLUDEPATH += ../inc
INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
QT += core
QT -= gui
MOC_DIR =     moc
OBJECTS_DIR = obj


# Platforms
SYMBIAN_PLATFORMS = WINSCW \
    ARMV5
      
LIBS +=  -llbs \
   		 -leposlandmarks \
   		 -lflogger \
   		 -lCentralRepository
   		 
HEADERS +=  inc/posindhelperclientrequester.h \    	   
    	    inc/posindicatorhelper.h \ 
    	    inc/posindicatorhelperserver.h \ 
    	    inc/posindicatorhelpererrors.h \    	  
    	    inc/posindicatorinfo_p.h \
    	    inc/posindicatorglobal.h \
    	    inc/posindicatorinfo.h \    	    
    	    inc/posgeoaddress.h 
    	    
SOURCES +=  src/posindicatorhelper.cpp \
    		src/posindicatorhelperserver.cpp \
    		src/posindhelperclientrequester.cpp \
    		src/posindicatorinfo_p.cpp \
    		src/posindicatorinfo.cpp \
    		src/posgeoaddress.cpp 
    		
symbian:
 { 
    TARGET.EPOCALLOWDLLDATA = 1  
    DEFINES += BUILD_POSINDICATOR    
    TARGET.UID3 = 0x2002E688
    TARGET.CAPABILITY = CAP_GENERAL_DLL     
 }

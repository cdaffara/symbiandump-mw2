# Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
# Initial Contributors:
# Nokia Corporation - initial contribution.
# Contributors:
# Description:

HEADERS = ./inc/keysequencerecognitionprovider.h        \
          ./inc/keysequencehandler.h                    \
          ./inc/manufacturerkeysequencehandler.h        \
          ./inc/imeikeysequencehandler.h                \
          ./inc/simcontrolkeysequencehandler.h          \
          ./inc/lifetimerkeysequencehandler.h           \
          ./inc/bluetoothkeysequencehandler.h

SOURCES = ./src/main.cpp                                \
          ./src/keysequencerecognitionprovider.cpp      \
          ./src/keysequencehandler.cpp                  \
          ./src/manufacturerkeysequencehandler.cpp      \
          ./src/imeikeysequencehandler.cpp              \
          ./src/simcontrolkeysequencehandler.cpp        \
          ./src/lifetimerkeysequencehandler.cpp         \
          ./src/bluetoothkeysequencehandler.cpp

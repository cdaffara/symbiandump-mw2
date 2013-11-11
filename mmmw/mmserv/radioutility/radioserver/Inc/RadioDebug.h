/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Radio server debug macros.
*
*/



#ifndef RADIODEBUG_H
#define RADIODEBUG_H

//  INCLUDES
#include <e32svr.h>

// DATA TYPES

/**
* Debug
*/
#ifdef _DEBUG
	#define RADIO_RDEBUG(X)		      	RDebug::Print(X)
    #define RADIO_RDEBUG_INT(X,Y)       RDebug::Print(X,Y)
    #define RADIO_RDEBUG_INT2(X,Y,Z)    RDebug::Print(X,Y,Z)
    #define RADIO_RDEBUG_INT3(X,Y,Z,Q)  RDebug::Print(X,Y,Z,Q)
#else
	#define RADIO_RDEBUG(X)
    #define RADIO_RDEBUG_INT(X,Y)
    #define RADIO_RDEBUG_INT2(X,Y,Z)
    #define RADIO_RDEBUG_INT3(X,Y,Z,Q)
#endif

#endif      // RADIODEBUG_H

// End of File

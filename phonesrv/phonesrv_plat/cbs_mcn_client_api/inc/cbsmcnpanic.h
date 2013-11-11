/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the header file of the CbsMcnPanic class.
*
*/


#ifndef __CBSMCNPANIC_H
#define __CBSMCNPANIC_H

//  CONSTANTS  

enum TCbsMcnPanics
	{
	EMcnObserverNull,
	EMcnObserversNull,
    EMcnNotConnected
	};

//  FUNCTION PROTOTYPES  

GLDEF_C void CbsMcnPanic( TInt aPanic );

#endif // __CBSMCNPANIC_H

// End of File

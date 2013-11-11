/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Constants defination file
*
*/


#ifndef NPPPSYPANIC_H
#define NPPPSYPANIC_H


//  INCLUDES
#include <e32std.h>

// CONSTANTS
_LIT( KNppPanicGategory, "NPP PSY" );


// DATA TYPES
enum TNppPsyPanicReason
    {
    EPanicPositionRequestOngoing       //There is already position request ongoing
    };

// FUNCTION PROTOTYPES
inline void Panic( TInt aPanicCode )
    {
    User::Panic( KNppPanicGategory, aPanicCode );
    };




#endif // NPPPSYPANIC_H

// End of File



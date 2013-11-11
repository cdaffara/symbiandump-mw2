/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface Implementation Information of Location Acquisition 
*                Strategy Plugin
*
*/



#include <e32std.h>
#include <implementationproxy.h>

#include "lbtstrategy.h"


const TImplementationProxy ImplementationTable[] = 
	{
    IMPLEMENTATION_PROXY_ENTRY( 0x10283135, CLbtStrategy::NewL ) 
    };


EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    aTableCount = sizeof( ImplementationTable ) / sizeof( TImplementationProxy );
    return ImplementationTable;
    }

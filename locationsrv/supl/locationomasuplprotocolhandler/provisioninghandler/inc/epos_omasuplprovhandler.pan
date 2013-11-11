/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Panic codes for SUPL Adapter
*
*/




#ifndef T_TMSGSUPLADAPTER_PAN_
#define T_TMSGSUPLADAPTER_PAN_


// DATA TYPES
enum TMsgSuplAdapterPanic 
    {
    SuplAdapterArgument,
	SuplAdapterInvalidId
    };


// ----------------------------------------------------
// Panic handler
// ----------------------------------------------------
//
GLREF_C void Panic(TInt aPanic);

#endif //T_TMSGSUPLADAPTER_PAN_

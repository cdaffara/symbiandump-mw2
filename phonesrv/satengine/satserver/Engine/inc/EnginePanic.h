/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declares panic function and panic codes for SatEngine
*
*/



#ifndef ENGINEPANIC_H
#define ENGINEPANIC_H

// DATA TYPES
enum TPanicCode
    {
    ESatSBadRequest,
    ESatEngineNullPointer,
    ESatSBadDescriptor,
    ESatSStartServer,
    ESatUnexpectedHandleUiResponseCall,
    ESatUnexpectedRspPckgCall,
    ESatUnexpectedDataPckgCall,
    ESatUserQueryAlreadyActive,
    // Paniced in case where BIP Data channels are used in such cases where
    // the usage should be impossible
    ESatBIPAccessViolation
    };

// FUNCTION PROTOTYPES
void PanicSatEngine( TPanicCode aPanicCode );

#endif      // ENGINEPANIC_H

// End of File

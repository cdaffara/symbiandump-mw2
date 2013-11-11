/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Dummy test connection channel for test module
*
*/


#include <e32def.h>
#include "ctestconnectionchannel.h"

TUint CTestConnectionChannel::ConnectionId() const
    {
    return KErrNone;
    }

TUint CTestConnectionChannel::SessionCount() const
    {
    return KErrNone;
    }

TBool CTestConnectionChannel::SessionWithMTPIdExists(TUint32 /*aMTPId*/) const
    {
    return ETrue;
    }
      
MMTPSession& CTestConnectionChannel::SessionWithMTPIdL(TUint32 /*aMTPId*/) const
    {
    
    }

TBool CTestConnectionChannel::SessionWithUniqueIdExists(TUint32 /*aUniqueId*/) const
    {
    return ETrue;
    }
      
MMTPSession& CTestConnectionChannel::SessionWithUniqueIdL(TUint32 /*aUniqueId*/) const
    {
    
    }

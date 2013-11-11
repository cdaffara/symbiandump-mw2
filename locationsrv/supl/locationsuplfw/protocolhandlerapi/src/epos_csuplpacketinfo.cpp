/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CSuplPacketInfo class
*
*/



#include "epos_csuplpacketinfo.h"

//
// ---------------------------------------------------------
//  constuctor
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CSuplPacketInfo::CSuplPacketInfo(const TDesC8& aPacket, TRequestStatus& aStatus):
	iPacket(aPacket),
	iStatus(&aStatus)
	{
	}
	
void CSuplPacketInfo::ConstructL()
{
	
}	

CSuplPacketInfo* CSuplPacketInfo::NewL(const TDesC8& aPacket, TRequestStatus& aStatus)
	{
	CSuplPacketInfo* self = new( ELeave ) CSuplPacketInfo(aPacket, aStatus);
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
	}
	
void CSuplPacketInfo::GetPacketData(TPtrC8 &aPacket)
    {
        aPacket.Set(iPacket);
    }
    
void CSuplPacketInfo::CompleteRequest(TInt err)
	{
		if (*iStatus == KRequestPending)
	User::RequestComplete(iStatus, err);
	}


TBool CSuplPacketInfo::IsStatusMatches(TRequestStatus& aStatus)
    {
    if (&aStatus == iStatus)
        return ETrue;
    else
        return EFalse;    
    }        

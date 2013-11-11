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
* Description:   class for holding IPAddress. 
*
*/



#include "epos_tomasuplipaddress.h"

//Constructor
TIPAddress::TIPAddress()
{
	
}

//Constructor
TIPAddress::TIPAddress(const TDesC& aIPAddress,TInt aPortNo)
{
	iIPAddress.Copy(aIPAddress);
	iPortNo = aPortNo;
}

// -----------------------------------------------------------------------------
// TIPAddress::SetIPAddressPort
// Sets IPAddress & Port No 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TIPAddress::SetIPAddressPort(const TDesC& aIPAddress,TInt aPortNo)
{
	iIPAddress.Copy(aIPAddress);
	iPortNo = aPortNo;
}
	

// -----------------------------------------------------------------------------
// TIPAddress::IPAddress
// This method returns IPAddress
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TDesC& TIPAddress::IPAddress() const
{
	return iIPAddress;
}

// -----------------------------------------------------------------------------
// TIPAddress::PortNo
// This method returns Port No.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt TIPAddress::PortNo() const
{
	return iPortNo;
}


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
* Description:  Service provider settings stub
*
*/

#include <spsettings.h>
#include <spentry.h>

CSPSettings* CSPSettings::NewL()
    {
    return new (ELeave) CSPSettings;
    }

CSPSettings::CSPSettings()
    {
    
    }

CSPSettings::~CSPSettings()
    {
    
    }

TInt CSPSettings::FindServiceIdsL( RArray<TUint>& )
    {
    return KErrNone;
    }

TInt CSPSettings::FindEntryL( TUint, CSPEntry& )
    {
    return KErrNone;
    }

TInt CSPSettings::AddEntryL( CSPEntry& )
    {
    return KErrNone;
    }

CSPEntry* CSPEntry::NewLC()
    {
    CSPEntry* ret = new (ELeave) CSPEntry;
    CleanupStack::PushL( ret );
    return ret;
    }

TServiceId CSPEntry::GetServiceId() const
    {
    return 0;
    }

const RPropertyArray& CSPEntry::GetAllProperties() const
    {
    return iPropertyArray;
    }

CSPEntry::CSPEntry()
    {
    
    }

CSPEntry::~CSPEntry()
    {
    
    }

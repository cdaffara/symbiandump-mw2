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
* Description:  Inline definition DRMConfigIntfc object.
*
*/


#include <DRMConfigIntfc.h>

inline CDRMConfigIntfc::~CDRMConfigIntfc()
    {
    // No Impl
    }

inline TInt CDRMConfigIntfc::GetDRMType( TDRMType& /*aDRMType*/ )
    {
    return KErrNotSupported;
    }

inline TInt CDRMConfigIntfc::SetDRMType( TDRMType /*aDRMType*/ )
    {
    return KErrNotSupported;
    }

inline TInt CDRMConfigIntfc::GetAllowedOutputDeviceCount( TInt& /*aCount*/ )
    {
    return KErrNotSupported;
    }

inline TInt CDRMConfigIntfc::GetAllowedOutputDevice(
                TInt /*aIndex*/,
                TDRMAllowedOutputDevice& /*aOutputDevice*/ )
    {
    return KErrNotSupported;
    }

inline TInt CDRMConfigIntfc::AppendAllowedOutputDevice(
                        TDRMAllowedOutputDevice /*aOutputDevice*/ )
    {
    return KErrNotSupported;
    }

inline TInt CDRMConfigIntfc::RemoveAllowedOutputDevice(
                        TDRMAllowedOutputDevice /*aOutputDevice*/ )
    {
    return KErrNotSupported;
    }

inline TInt CDRMConfigIntfc::Reset()
    {
    return KErrNotSupported;
    }

inline TInt CDRMConfigIntfc::Commit()
    {
    return KErrNotSupported;
    }

// End of file

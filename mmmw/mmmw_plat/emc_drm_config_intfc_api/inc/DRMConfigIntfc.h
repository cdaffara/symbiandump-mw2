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
* Description:  Header of DRMConfigIntfc object.
*
*/


#ifndef DRMCONFIGINTFC_H
#define DRMCONFIGINTFC_H

#include <e32base.h>
#include <DRMConfigTypes.h>

const TUid KDRMConfigIntfc = {0x10207B90};
const TInt KDRMConfigIntfcMajorVer1 = 1;
const TInt KDRMConfigIntfcMinorVer1 = 0;
const TInt KDRMConfigIntfcBuildVer1 = 0;

class CDRMConfigIntfc
    {
public:
    inline virtual ~CDRMConfigIntfc();
    inline virtual TInt GetDRMType( TDRMType& aDRMType );
    inline virtual TInt SetDRMType( TDRMType aDRMType );
    inline virtual TInt GetAllowedOutputDeviceCount( TInt& aCount );
    inline virtual TInt GetAllowedOutputDevice(
                            TInt aIndex,
                            TDRMAllowedOutputDevice& aOutputDevice );
    inline virtual TInt AppendAllowedOutputDevice(
                            TDRMAllowedOutputDevice aOutputDevice );
    inline virtual TInt RemoveAllowedOutputDevice(
                            TDRMAllowedOutputDevice aOutputDevice );
    inline virtual TInt Reset();
    inline virtual TInt Commit();
    };

#include "DRMConfigIntfc.inl"

#endif // DRMCONFIGINTFC_H

// End of file

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
* Description:  Header of DRMConfigIntfcImpl object.
*
*/


#ifndef DRMCONFIGINTFCIMPL_H_
#define DRMCONFIGINTFCIMPL_H_

#include <e32base.h>
#include <DRMConfigIntfc.h>
#include "ParentChildIntfc.h"


class CDataBufferMultimediaSource;

class CDRMConfigIntcfImpl : public CBase,
                            public CDRMConfigIntfc,
                            public CChildIntfc
    {
public:
    static CDRMConfigIntcfImpl* NewL( CDataBufferMultimediaSource& aDRMHandler );
    ~CDRMConfigIntcfImpl();
    TInt GetDRMType( TDRMType& aDRMType );
    TInt GetAllowedOutputDeviceCount( TInt& aCount );
    TInt GetAllowedOutputDevice(
                    TInt aIndex,
                    TDRMAllowedOutputDevice& aOutputDevice );

private:
    CDRMConfigIntcfImpl( CDataBufferMultimediaSource& aDRMHandler );
    void ConstructL();

private:
    CDataBufferMultimediaSource* iDRMHandler;
    };

#endif /*DRMCONFIGINTFCIMPL_H_*/

// End of file

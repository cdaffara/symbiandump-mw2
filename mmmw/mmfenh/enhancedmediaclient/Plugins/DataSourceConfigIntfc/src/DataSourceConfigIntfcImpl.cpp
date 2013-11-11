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
* Description:  S60 Cache Data Source Plugin implementation
*
*/

#include "DataSourceConfigIntfcImpl.h"

_LIT(FileDirPath,"c:\\system\\temp\\");
//const TUint KDefaultCacheSize = 17408;
CDataSourceConfigIntfcImpl::CDataSourceConfigIntfcImpl()
    {
    }

EXPORT_C CDataSourceConfigIntfcImpl* CDataSourceConfigIntfcImpl::NewL()
    {
    CDataSourceConfigIntfcImpl* self = new (ELeave) CDataSourceConfigIntfcImpl();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


CDataSourceConfigIntfcImpl::~CDataSourceConfigIntfcImpl(void)
    {
    delete iLocation;
    }

void CDataSourceConfigIntfcImpl::ConstructL()
    {
    iCacheType = EBUFFER;
    iLocation = HBufC::NewL(FileDirPath().Size());
    TPtr des = iLocation->Des();
    des.Copy(FileDirPath());
    }

// From CMultimediaDataSource begins
TInt CDataSourceConfigIntfcImpl::SetCacheSize( TUint aSizeBytes )
    {
    TInt status(KErrNone);    
    iSizeBytes = aSizeBytes;
    return status;
    }

TInt CDataSourceConfigIntfcImpl::GetCacheSize( TUint& aSizeBytes )
    {
    aSizeBytes = iSizeBytes;
    return KErrNone;
    }

TInt CDataSourceConfigIntfcImpl::SetCacheType(TCacheType aCacheType)
    {
    TInt status(KErrNone);  
    iCacheType = aCacheType;  
    return status;
    }

TInt CDataSourceConfigIntfcImpl::GetCacheType(TCacheType& aCacheType)
    {
    aCacheType = iCacheType;        
    return KErrNone;
    }

TInt CDataSourceConfigIntfcImpl::SetCacheLocation(const TDesC& aLocation)
    {
    TInt status(KErrNone);  
    if(iLocation)
        {
        delete iLocation;
        iLocation = NULL;
        }
    iLocation = HBufC::NewL(aLocation.Length());
    TPtr des = iLocation->Des();
    des.Copy(aLocation);  
    return status;
    }

TInt CDataSourceConfigIntfcImpl::GetCacheLocation(TDes& aLocation)
    {
    TInt status(KErrNone);
    aLocation.Copy(*iLocation);
    return status;
    }

TInt CDataSourceConfigIntfcImpl::Reset()
    {
    return KErrNone;
    }
// End of file

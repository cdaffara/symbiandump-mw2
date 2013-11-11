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
* Description:  Header of DataSourceConfigIntfc object.
*
*/


#ifndef DATASOURCECONFIGINTFCIMPL_H
#define DATASOURCECONFIGINTFCIMPL_H

#include <e32base.h>
#include <DataSourceConfigIntfc.h>

class CDataSourceConfigIntfcImpl : public CDataSourceConfigIntfc
    {
    public:
        IMPORT_C static CDataSourceConfigIntfcImpl* NewL();
        ~CDataSourceConfigIntfcImpl();
        
        TInt SetCacheSize(TUint aSizeBytes);
        TInt GetCacheSize(TUint& aSizeBytes);
        TInt SetCacheType(TCacheType aCacheType);
        TInt GetCacheType(TCacheType& aCacheType);
        TInt SetCacheLocation(const TDesC& aLocation);
        TInt GetCacheLocation(TDes& aLocation);
        TInt Reset();
    private:
        
        CDataSourceConfigIntfcImpl();
        void ConstructL();
    private:
        TUint iSizeBytes;
        TCacheType iCacheType;
        HBufC* iLocation;            
    };

#endif // DATASOURCECONFIGINTFCIMPL_H
    
    // End of file

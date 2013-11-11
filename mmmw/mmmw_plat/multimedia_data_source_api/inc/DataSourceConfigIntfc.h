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


#ifndef DATASOURCECONFIGINTFC_H
#define DATASOURCECONFIGINTFC_H

#include <e32base.h>

class CDataSourceConfigIntfc : public CBase
    {
    public:
        enum TCacheType
            {
            EFILE,
            EBUFFER
            };

        IMPORT_C virtual TInt SetCacheSize(TUint aSizeBytes) = 0;
        IMPORT_C virtual TInt GetCacheSize(TUint& aSizeBytes) = 0;
        IMPORT_C virtual TInt SetCacheType(TCacheType aCacheType) = 0;
        IMPORT_C virtual TInt GetCacheType(TCacheType& aCacheType) = 0;
        IMPORT_C virtual TInt SetCacheLocation(const TDesC& aLoction) = 0;
        IMPORT_C virtual TInt GetCacheLocation(TDes& aLocation) = 0;
        IMPORT_C virtual TInt Reset() = 0;
    };
  
#endif // DATASOURCECONFIGINTFC_H
    
    // End of file

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
* Description:  Implementation of MultimediaDataSourceFactory.
*
*/


#include <MultimediaDataSourceFactory.h>
#include "DataBufferSourceUid.h"
#include "DataBufferDataSource.h"
#include "FileDataSource.h"
#include "DescriptorDataSource.h"
#include "CacheSource.h"
#include "DataSourceConfigIntfcImpl.h"

// If KErrNone on return, aMultimediaDataSource will point to
// CMultimediaDataSource derivitite object. Once the object is created, the
// ownership is transfered to client.
EXPORT_C TInt CMultimediaDataSourceFactory::CreateDataSource(
                                                             MDataSource& aMDataSource,
                                                             /*MMultimediaDataSourceObserver& aDataSourceObserver,*/
                                                             CMultimediaDataSource*& aMultimediaDataSource )
    {
    TInt status(KErrNotSupported);
    if ( aMDataSource.DataSourceType() == KMmfDataBufferSource )
        {
        TRAP(status,aMultimediaDataSource  = CDataBufferMultimediaSource::NewL(aMDataSource));
        }
    else if ( aMDataSource.DataSourceType() == KFileDataSourcePlugin )
        {
        TRAP(status,aMultimediaDataSource  = CFileMultimediaSource::NewL(aMDataSource));
        }        
    else if ( aMDataSource.DataSourceType() == KDescriptorDataSourcePlugin )
        {
        TRAP(status,aMultimediaDataSource  = CDescriptorDataMultimediaSource::NewL(aMDataSource));
        }        
    return status;
    }

EXPORT_C TInt CMultimediaDataSourceFactory::CreateSeekableDataSource(
                                                             MDataSource& aMDataSource,
                                                             CMultimediaDataSource*& aMultimediaDataSource,
                                                             CDataSourceConfigIntfc*& aDataSourceConfigIntfc )
    {
    TInt status(KErrNotSupported);
    CDataSourceConfigIntfcImpl* config(NULL);
    if ( aMDataSource.DataSourceType() == KMmfDataBufferSource )
        {
        CMultimediaDataSource* dataBufferSource  = CDataBufferMultimediaSource::NewL(aMDataSource);
        TRAP(status,config =  CDataSourceConfigIntfcImpl::NewL());           
        if(status == KErrNone)
        	{
        	aDataSourceConfigIntfc = config;
        	TRAP(status,aMultimediaDataSource = CCacheSource::NewL(dataBufferSource,config));
        	}
        } 
    return status;
    }
// End of file

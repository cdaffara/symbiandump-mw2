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
* Description:  Header of DataSourceFactory class.
*
*/


#ifndef DATASOURCEFACTORY_H
#define DATASOURCEFACTORY_H

#include <e32base.h>

class MDataSource;
/*class MMultimediaDataSourceObserver;*/
class CMultimediaDataSource;
class CDataSourceConfigIntfc;
class CMultimediaDataSourceFactory
    {
    public:
        // Returns KErrNotSupported if MDataSource is not streaming source or 3.2
        // progressive download source.
        // If KErrNone on return, aMultimediaDataSource will point to
        // CMultimediaDataSource derivitite object. Once the object is created, the
        // ownership is transfered to client.
        
        IMPORT_C static TInt CreateDataSource( MDataSource& aMDataSource,
            /*MMultimediaDataSourceObserver& aDataSourceObserver,*/
            CMultimediaDataSource*& aMultimediaDataSource );

        IMPORT_C static TInt CreateSeekableDataSource( MDataSource& aMDataSource,
            CMultimediaDataSource*& aMultimediaDataSource,
            CDataSourceConfigIntfc*& aDataSourceConfigIntfc );

    };

#endif // DATASOURCEFACTORY_H

// End of file

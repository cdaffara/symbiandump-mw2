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
* Description:  Header of FileSource.
*
*/


#ifndef FILEDATASOURCE_H
#define FILEDATASOURCE_H


#include <mmf/server/mmfdatasource.h>
#include "FileMultimediaSource.h"

class MDataSource;

class CFileSource : public MDataSource
    {
    public:
        static CFileSource* NewL( TUid aType );
        ~CFileSource(void);
        
        // From MDataSource begins
        TFourCC SourceDataTypeCode(TMediaId aMediaId) ;
        void FillBufferL( CMMFBuffer* aBuffer, MDataSink* aConsumer, TMediaId aMediaId ) ;
        void BufferEmptiedL( CMMFBuffer* aBuffer ) ;
        TBool CanCreateSourceBuffer() ;
        CMMFBuffer* CreateSourceBufferL( TMediaId aMediaId, TBool &aReference ) ;
        TInt SourceThreadLogon(MAsyncEventHandler& aEventHandler) ;
        void SourceThreadLogoff();
        void SourcePrimeL();
        void SourcePlayL();
        void SourceStopL();
        void SourceCustomCommand(TMMFMessage& aMessage);
        void ConstructSourceL(const TDesC8 &aInitData);
        TUid DataSourceType() const;
        TInt SetSourceDataTypeCode(TFourCC aSourceFourCC, TMediaId /*aMediaId*/ );
         // From MDataSource ends
        
        void SetMultimediaSource(CFileMultimediaSource& aMultimediaSource);
        TDesC8& GetInitData();
    private:
        CFileSource(TUid aType);
        void ConstructL (void);
    private:
        //Data
        CFileMultimediaSource* iMultiMediaSource; 
        HBufC8* iInitData;       
 
    }; 
   
#endif //FILEDATASOURCE_H

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
* Description:  Header of DataBufferSource interface.
*
*/



#ifndef DATABUFFERSOURCE_H
#define DATABUFFERSOURCE_H

#include <e32base.h>
#include <SourceControl.h>

namespace multimedia
    {
    // Uid identifying Stream Control
    const TUid KDataBufferSourceControl = {0x10207B47};
    
    // Forward declaration
    class MDataBuffer;
    
    class MBufferProcessedEvent
        {
        public:
            virtual MDataBuffer* GetDataBuffer() = 0;
            virtual TInt GetErrorCode() = 0;
        };
    
    typedef struct 
            {
                enum TBufferingType
                    {
                    BUFFERINGNONE,
                    FIXEDSIZE,
                    FIXEDDURATION,
                    DYNAMICDURATION,
                    };
    
                TBufferingType  iType;
                TUint           iAmount;
            }TBufferingConfig;


    // Class declaration
    class MDataBufferSource : public MSourceControl
        {
        public:
            // Config msgs
            virtual TInt Open( TDesC8& aMimeType, MDataBuffer& aHeaderData ) = 0;
            virtual TInt GetMinimumBufferSize( TUint& aBufferSize ) = 0;
            virtual TInt SetSize( TUint aSize ) = 0;
            virtual TInt WriteData( MDataBuffer& aBuffer ) = 0;
            virtual TInt EmptyBuffers() = 0;
            virtual TInt GetBufferingConfig(TBufferingConfig& aConfig) = 0;
            virtual TInt SetBufferingConfig(TBufferingConfig aConfig) = 0;
            virtual TInt GetBitRate(TUint& aRate) = 0;
            virtual TInt GetBufferingTypesSupported(TArray<TBufferingConfig::TBufferingType>& aArray) = 0;
            // Extension mechanism
            virtual TInt GetInterface(
                                TUid aInterfaceId,
                                TVersion& aVer,
                                TAny*& aInterfaceImpl ) = 0;

        };
    } // namespace multimedia

#endif // DATABUFFERSOURCE_H

// End of file

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
* Description:  header of ReadWriteRequestAO class.
*
*/


#ifndef READWRITEREQUESTAO_H
#define READWRITEREQUESTAO_H

//  INCLUDES
#include <e32base.h>

class CCacheSource; 
class CMMFBuffer;
class CMMFDataBuffer;

/**
*  Class to encapsulate a queue item.
*/
class CReadWriteRequestAO : public CActive
    {

    public:  // Constructors and destructor

        enum TRequestType
            {
                EReadRequest,
                EWriteRequest
            };    

        static CReadWriteRequestAO* NewL(CCacheSource& aSource,TRequestType aType);
        virtual ~CReadWriteRequestAO();
        
        void SetActive();
        TInt Error();
        CMMFDataBuffer* Buffer();
        TRequestType RequestType();
        TInt SetBuffer(CMMFBuffer* aBuffer);

        
    protected:
        // From CActive
        void RunL();
        void DoCancel();
        TInt RunError( TInt aError );
        
    private:
        CReadWriteRequestAO( CCacheSource& aSource,TRequestType aType );
        void ConstructL();
        
    private:
        CCacheSource& iSource;
        CMMFBuffer* iBuffer;
        TRequestType iType;
        TInt iError;
    };


#endif // READWRITEREQUESTAO_H

// End of File

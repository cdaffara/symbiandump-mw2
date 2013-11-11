/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Declaration of CCCPCallParameters class.
*
*/
#ifndef C_CCPCALLPARAMETERS_H
#define C_CCPCALLPARAMETERS_H

#include <e32base.h>
#include <ccpdefs.h>
/**
* Common paramaters for all calls.
*/
class CCCPCallParameters: public CBase
    {
    public:
        /**
        * Call type enumeration.
        */
    public:
        /**  Constructor. */
        IMPORT_C static CCCPCallParameters* NewL();


        /**  Destructor. */
        IMPORT_C ~CCCPCallParameters();
        
        virtual CCCPCallParameters* CloneL() const;
        
    protected:
        IMPORT_C CCCPCallParameters();
        IMPORT_C void ConstructL();
        
    public:
        
        
        // New functions
        /** Sets the service id identifying the service provider of the call. */
        IMPORT_C void SetServiceId(TUint32 aServiceId);
        /** Gets the service id identifying the service provider of the call. */
        IMPORT_C TUint32 ServiceId() const;
        /** Sets the type of the call. */
        IMPORT_C void SetCallType(CCPCall::TCallType aCallType);
        /** Gets the type of the call. */
        IMPORT_C CCPCall::TCallType CallType() const;
    
    
    private:        
        /** Service Id for the call. */ 
        TUint32 iServiceId; 
        /** Call type, used in CCP API */
        CCPCall::TCallType iCallType;
};

#endif // C_CCPCALLPARAMETERS_H

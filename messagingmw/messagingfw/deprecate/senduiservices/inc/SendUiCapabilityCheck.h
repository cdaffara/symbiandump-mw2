/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Capability checker for SendUI.
*
*/




#ifndef CSENDUICAPABILITYCHECK_H
#define CSENDUICAPABILITYCHECK_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CBaseMtm;
class CMsvSession;
// CLASS DECLARATION

/**
*  Capability checker for SendUI.
*
*  @lib SenduiDataUtils.lib
*  @since S60 3.2
*/
NONSHARABLE_CLASS(CSendUiCapabilityCheck) : public CActive
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CSendUiCapabilityCheck* NewLC();

        /**
        * Destructor.
        */
        virtual ~CSendUiCapabilityCheck();

        TBool CheckCapabilitiesL(
            const TSecurityInfo& aSecurityInfo,
            CMsvSession& aSession,
            const TUid aMtmType,
            const TDesC& aServiceName );

    private:

        void RunL();
  
        void DoCancel();

        void ConstructL();
        
        /**
        * C++ default constructor.
        */
        CSendUiCapabilityCheck();
    
    private:

	    CActiveSchedulerWait iWait;

        RNotifier iNotifier;
        TUid iNotifierUid;
        
        TPckgBuf<TInt> iNotifierResult;

    };

#endif      // CSENDUICAPABILITYCHECK_H   
            
// End of File

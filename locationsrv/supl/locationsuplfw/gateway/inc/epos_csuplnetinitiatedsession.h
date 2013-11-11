/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Base class for SUPL subsessions
*
*/




#ifndef __CSuplNetInitiatedRegistry_H__
#define __CSuplNetInitiatedRegistry_H__

// INCLUDES
#include <e32base.h>

#include <lbspositioninfo.h>

#include "epos_csuplsessionbase.h"
#include "epos_csuplserver.h"
#include "epos_csuplsessionrequest.h"
#include "epos_msuplsessionobserver.h"
#include "epos_csuplnetinitiatedregistry.h"
#include "epos_mnetinitiatedobserver.h"

// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
* This class provides functionality common to all SUPL subsessions.
*/
class CSuplNetInitiatedSession : public CObject
    {

	public:  // Constructor & Destructor

        /**
        * Two-phased constructor.
        */
        static CSuplNetInitiatedSession* NewLC(CSuplSessionManager& aSessnMgr, CSUPLProtocolManagerBase :: TSuplReqType aReqType,	CSuplServer* aServer,HBufC8* aClientBuf);

        /**
        * Destructor.
        */
		virtual ~CSuplNetInitiatedSession();

	public:		// New functions
        /**
        * Called when the server class is shutting down.
        */
       	void ForwardMessageL(const RMessage2& aMessage);
	
	//	TInt GetHandle();
	
	//	void SetHandle(TInt aHandle);
		
		void DestroySession(CSuplSessionManager* aSessionMgr);
		
		private:
			TBool CheckSuplTriggerServiceStatus();
    
    protected:  // Functions from base classes

    private:

        CSuplNetInitiatedSession();

        void ConstructL(CSuplSessionManager& aSessnMgr, CSUPLProtocolManagerBase::TSuplReqType aReqType,CSuplServer* aServer,HBufC8* aClientBuf);

        // By default, prohibit copy constructor
        CSuplNetInitiatedSession( const CSuplNetInitiatedSession& );
        // Prohibit assigment operator
        CSuplNetInitiatedSession& operator= ( const CSuplNetInitiatedSession& );

        
        void RequestComplete(
                const RMessage2& aMessage,
                TInt aCompleteCode
             );
           
	
    private:    // Data
    	CSuplSessionBase*		iSuplSession;
		CSuplSessionRequest*	iSuplSessnReq;
		TInt 					iHandle;
		TBool 					iCloseFlag;	
		HBufC8* 				iClientBuf;
	
	
    };


#endif  // __CSuplNetInitiatedRegistry_H__

// End of File
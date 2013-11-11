/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Notifier client side active object
*
*/


#ifndef __MDENOTIFIERAOAO_H__
#define __MDENOTIFIERAOAO_H__

// INCLUDE FILES
#include <e32base.h>

#include "mdccommon.h"
#include "mdecondition.h"


// forward declarations
class RMdEEngineSession;
class CMdESessionImpl;
class CMdENamespaceDef;
class CMdERelation;
class TMdERelation;

// local constants
const TInt KDefaultBufferSize = 1024; // 1 KB
/**
* CMdENotifierAO
* An active object that receives notifications from server
* side whenever a registrated event has occurred.
*/
NONSHARABLE_CLASS(CMdENotifierAO) : public CActive
    {
    public: // Constructors and destructors

		/**
		 * Two-phased constructor.
		 * @param aSessionImpl session impl
		 * @param aSession the server handler
		 * @return the created instance
		 */
		 static CMdENotifierAO* NewL(
		     CMdESessionImpl& aSessionImpl, RMdEEngineSession& aSession );

        /**
        * Two-phased constructor.
        * @param aSessionImpl session impl
        * @param aSession the server handler
        * @return the created instance
        */
        static CMdENotifierAO* NewLC(
            CMdESessionImpl& aSessionImpl, RMdEEngineSession& aSession );

        /**
        * Destructor.
        */
        virtual ~CMdENotifierAO();

        /**
        * register for receiving events
        * @param aMode observer mode: object, event or relation
        * @param aObserver observer to be notified when event occurs
        * @param aCondition condition when to notify
        */
        void RegisterL( TUint32 aType, TAny* aObserver, CMdECondition* aCondition, CMdENamespaceDef& aNamespaceDef );

        /**
        * compares an observer against one stored in this AO
        * @param aMode observer mode: object, event or relation
        * @param aObserver observer pointer
        */
        TBool Match( TUint32 aType, TAny* aObserver, CMdENamespaceDef& aNamespaceDef );

        /**
        * ID of this client side notifier object
        */
        TInt Id();

    protected: // Functions from base classes

        /**
        * From CActive
        * Callback function.
        * Invoked to handle responses from the server.
        */
        void RunL();

        /**
        * From CActive
        * Handles errors that occur during notifying the observer.
        */
        TInt RunError(TInt aError);

        /**
        * From CActive
        * Cancels any outstanding operation.
        */
        void DoCancel();

    private: // Constructors and destructors

        /**
        * constructor
        * @param aSessionImpl session impl
        * @param aSession the server handler
        */
        CMdENotifierAO(
            CMdESessionImpl& aSessionImpl, RMdEEngineSession& aSession );

        /**
        * 2nd phase constructor
        */
        void ConstructL();

        /**
        * notify the corresponding client observer pointer
        * @param aItem the item (in coded form) to be notified
        * @param aStatus what has actually happened with the item
        */
        void DoNotifyObserver();
		
		void DecodeIdBufferL();
		void DecodeDataBufferL();
		void DecodeRelationItemBufferL();

    private: // Data

        /** the session impl. */
        CMdESessionImpl& iSessionImpl;

        /** the session */
        RMdEEngineSession& iSession;

        /** observer to be notified, actual class depends on type */
        TAny* iObserver;

        /** notifier type */
        TUint32 iType;

		/** Namespace definition's ID */
        TDefId iNamespaceDefId;

        /** size of result block */
        TPckgBuf<TInt> iResultSize;

        CMdCSerializationBuffer* iDataBuffer;

		RArray<TItemId> iIdArray;
		
		RPointerArray<HBufC> iUriArray;

		RArray<TMdERelation> iRelationItemArray;
    };

#endif //__MDENOTIFIERAOAO_H__

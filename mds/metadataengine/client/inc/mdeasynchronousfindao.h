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
* Description:  Asynchronous query active object
*
*/


#ifndef __MDEASYNCHRONOUSFINDAO_H__
#define __MDEASYNCHRONOUSFINDAO_H__

// INCLUDE FILES
#include <e32base.h>
#include <e32svr.h>
#include <badesca.h>

#include <mdccommon.h>

// forward declarations
class CMdEQuery;
class CMdESessionImpl;
class RMdEEngineSession;
class CMdEInstanceItem;
class CMdEQueryCriteriaSerialization;

/**
* MdESessionMonitorAO
* An abstract active object that is used for multiple purposes in
* MdE client side.
*/
NONSHARABLE_CLASS(CMdEAsynchronousFindAO) : public CActive
    {
    public: // Constructors and destructors

        /**
        * NewL.
        * Two-phased constructor.
        * Creates a CCSAsyncRequestHandler object using two phase construction,
        * and return a pointer to the created object.
        * @param aSession The engine session
        * @param aType the query type
        * @return A pointer to the created instance of CCSAsyncRequestHandler.
        */
        static CMdEAsynchronousFindAO* NewL( CMdEQuery& aQuery,
            CMdESessionImpl& aSessionImpl, RMdEEngineSession& aSession );

        /**
         * NewL.
         * Two-phased constructor.
         * Creates a CCSAsyncRequestHandler object using two phase construction,
         * and return a pointer to the created object.
         * @param aSession The engine session
         * @param aType the query type
         * @return A pointer to the created instance of CCSAsyncRequestHandler.
         */
         static CMdEAsynchronousFindAO* NewLC( CMdEQuery& aQuery,
             CMdESessionImpl& aSessionImpl, RMdEEngineSession& aSession );

        /**
        * ~CCSAsyncRequestHandler.
        * Destructor.
        * Destroys the object and release all memory objects.
        */
        virtual ~CMdEAsynchronousFindAO();

    public: // New functions

        /**
        * Starts find
        */
        void DoFindL( TUint aMaxCount, TUint aNotifyCount );

    protected: // Functions from base classes

	    /**
	     * Calls notify results on CMdEQuery class (one of its subclasses)
	     * @param aStatus the asynchronous find status
	     */
	    void DoNotifyResultsL( TInt aStatus );
    
        /**
        * Calls notify on CMdEQuery class (one of its subclasses)
        * @param aStatus the asynchronous find status
        */
        void DoNotifyL( TBool aNewResults, TInt aStatus );

        /**
        * From CActive
        * Callback function.
        * Invoked to handle responses from the server.
        */
        void RunL();

		/**
		* From CActive
		*/
		TInt RunError(TInt aError);

        /**
        * From CActive
        * Cancels any outstanding operation.
        */
        void DoCancel();

    private: // Constructors and destructors

        /**
        * CCSAsyncRequestHandler.
        * Performs the first phase of two phase construction.
        * @param aSession The engine session
        * @param aType the query type
        */
        CMdEAsynchronousFindAO( CMdEQuery& aQuery,
            CMdESessionImpl& aSessionImpl, RMdEEngineSession& aSession );

        /**
        * ConstructL.
        * Performs the second phase construction of a
        * CCSAsyncRequestHandler object.
        */
        void ConstructL();


    private: // Data

        /** the query object */
        CMdEQuery& iQuery;

        /** The session impl class */
        CMdESessionImpl& iSessionImpl;

        /** The engine session */
        RMdEEngineSession& iSession;

        /** package that will receive result hierarchy size */
        TPckgBuf<TInt> iResultSize;

        /** query results (stored here only temporarily) */
        RPointerArray<CMdEInstanceItem> iResultList;

        /** ID query results (stored here only temporarily) */
        RArray<TItemId> iIdResultList;

        /** Count query result */
        TUint32 iCountResult;

		CMdEQueryCriteriaSerialization* iConditionBuffer;

		/** distinct query results (stored here only temporarily) */
		CDesCArray* iDistinctResults;
    };


#endif //__MDEASYNCHRONOUSFINDAO_H__

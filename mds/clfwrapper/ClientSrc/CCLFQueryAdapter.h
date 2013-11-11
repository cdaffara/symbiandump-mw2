/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  MetadataEngine query adapter for CLF
*
*/


#ifndef C_CCLFQUERYADAPTER_H
#define C_CCLFQUERYADAPTER_H

// INCLUDES
#include <e32base.h>
#include <CLFContentListing.hrh>
#include <CLFContentListing.h>
#include <bamdesca.h>
#include <mdesession.h>
#include <mdeobjectquery.h>

// FORWARD DECLARATIONS
class CMdEObject;
class CMdEProperty;
class CMdEPropertyDefSet;

/**
 *  Wraps MdE functionality for Content Listing Framework
 *
 *  @lib clf2mde
 *  @since S60 3.1
 */
NONSHARABLE_CLASS( CCLFQueryAdapter ) : public CBase, 
                                        public MMdEQueryObserver
    {
    public: // Constructors and destructor

        // 2-phase constructor
        static CCLFQueryAdapter* NewL( CMdESession& aMdESession );

        // Destructor
        virtual ~CCLFQueryAdapter();

    public: // New functions

        /**
         * Queries the mediatypes
         *
         * @since S60 3.1
         * @param aMimeTypeArray array for mime types which are included in result
         * @param aMediaTypeArray array for media types which are included in result
         * @param aStatus reference to TRequestStatus which is completed
         */

        void QueryMdEObjectsL( const MDesCArray& aMimeTypeArray,
                               const TArray< TInt >& aMediaTypeArray,
                               CMdEObjectQuery*& aQuery,
                               TRequestStatus& aStatus );

        void QueryMdEObjectsL( const TArray< TCLFItemId >& aItemIDArray,
                               const TDesC& aObjDefStr,
                               CMdEObjectQuery*& aQuery,
                               TRequestStatus& aStatus );

    protected: // Functions from base classes
    
        /**
         * From MMdEQueryObserver
         */
        void HandleQueryNewResults( CMdEQuery& aQuery,
                                    TInt aFirstNewItemIndex,
                                    TInt aNewItemCount );

        /**
         * From MMdEQueryObserver
         */
        void HandleQueryCompleted( CMdEQuery& aQuery, TInt aError );

    private: // New functions

        /**
        * Helper function.
        */
        void CompleteRequest( CMdEQuery& aQuery, TInt aError );

        void AddMimeTypeConditionL( CMdELogicCondition& aLogicCondition,
                                    const CMdEPropertyDef& aPropDef,
                                    const TPtrC& aMimeType );

    private:

        /**
        * C++ default constructor.
        */
        CCLFQueryAdapter( CMdESession& aMdESession );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: // data

        /** MdE session. Ref */
    	CMdESession& iMdESession;

        /** MdE namespace */
        CMdENamespaceDef* iDefaultNamespaceDef;

        /** MdE query array */
        RPointerArray< CMdEQuery > iQueryArray;

        /** Request status array */
        RPointerArray< TRequestStatus > iStatusArray;
    };

#endif // C_CCLFQUERYADAPTER_H

//  End of File

/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Class for privacy notifier extension helper.
*
*/




/**
 @file
 @internalComponent
 @released
*/

#ifndef CPOSPRIVACYNOTIFIEREXTENSION_H
#define CPOSPRIVACYNOTIFIEREXTENSION_H

//  INCLUDES
#include <eiknotapi.h>
#include <lbs/epos_rposrequestorstack.h>
#include <lbs/epos_tposqninputdata.h>
#include <EPos_CPosPrivacyNotifier.h>
// FORWARD DECLARATIONS
class CPosRequestHandler;

// CLASS DECLARATION

/**
*  The privacy notifier extension helper class.
*/
NONSHARABLE_CLASS(CPosPrivacyNotifierExtension) : public CBase, public MEikSrvNotifierBase2
    {
    public:

        /**
        * Two-phased constructor.
        */
        static CPosPrivacyNotifierExtension* NewL(
            CPosPrivacyNotifier* aPrivNotifier);

        /**
        * Destructor.
        */
        ~CPosPrivacyNotifierExtension();

    public: // New functions

        /**
        * Finds the request id in the array which matches the specified
        * request id.
        * @param aRequestId The id of a query or notification request.
        * @return The index of the matching request id within the array.
        * KErrNotFound, if no matching request id can be found.
        */
        TInt Find(TPosQNRequestId aRequestId) const;

        /**
        * Resets data
        */
        void ResetData();

        /**
        * Removes a request from the array containing all outstanding requests.
        * @param aRequestId The id of a query or notification request.
        * @return The index of the matching request id within the array.
        * KErrNotFound, if no matching request id can be found.
        */
        TInt RemoveRequestFromArray(TPosQNRequestId aRequestId);

        /**
        * Empties the array containing all outstanding requests.
        */
//        void RemoveAllRequestsFromArray();

        /**
        * Prepares the current request by getting RequestorStack.
        */
        void PrepareL(TPosQNRequestId aRequestId);

        /**
        * This method completes a privacy query or notification request.
        *
        * @param aRequestId The id of the privacy query or notification request
        *   to complete.
        * @param aCompletionCode The request completion code.
        */
        void CompleteRequest(
        /* IN  */       TPosQNRequestId aRequestId,
        /* IN  */       TInt aCompletionCode
        );

        /**
        * This method completes all outstanding requests.
        *
        * This function should be used if a serious error is encountered. All
        * requests can then be completed with the same error code.
        *
        * @param aCompletionCode The request completion code.
        */
        void CompleteAllRequests(
        /* IN  */       TInt aCompletionCode
        );

    public: // Functions from base classes

        /**
        * From MEikSrvNotifierBase2 Called to destroy notifier.
        */
        void Release();

        /**
        * From MEikSrvNotifierBase2 Called when the DLL is loaded.
        * @return info object containing UID, channel and priority
        */
        TNotifierInfo RegisterL();

        /**
        * From MEikSrvNotifierBase2
        * @return info object containing UID, channel and priority
        */
        TNotifierInfo Info() const;

        /**
        * From MEikSrvNotifierBase2
        *
        * Not supported, leaves if called.
        * @param aBuffer Input descriptor. Not used.
        * @return TPtrC8
        */
        TPtrC8 StartL(const TDesC8& aBuffer);

        /**
        * From MEikSrvNotifierBase2
        *
        * @param aBuffer Input descriptor, contains the format. Not used.
        * @param aReplySlot Reply slot. Not used.
        * @param aMessage Used to write data back to client
        */
        void StartL(
            const TDesC8& aBuffer,
            TInt aReplySlot,
            const RMessagePtr2& aMessage
        );

        /**
        * From MEikSrvNotifierBase2
        * Cancels all outstanding requests.
        */
        void Cancel();

        /**
        * From MEikSrvNotifierBase2
        * Starts the notifier dialog.
        * @param aBuffer Input descriptor, contains the format.
        * A TPosQNInputData is expected as input.
        * @return TPtrC8
        */
        TPtrC8 UpdateL(const TDesC8& aBuffer);

    private:
        /**
        * C++ default constructor.
        */
        CPosPrivacyNotifierExtension();

       /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL(CPosPrivacyNotifier* aPrivNotifier);

        // By default, prohibit copy constructor
        CPosPrivacyNotifierExtension(const CPosPrivacyNotifierExtension&);
        // Prohibit assigment operator
        CPosPrivacyNotifierExtension& operator= (
            const CPosPrivacyNotifierExtension&);

    public: // Data
        TNotifierInfo           iNotifierInfo;
        RArray<TPosQNInputData> iRequestArray;
        TPosQNRequestId         iCurrentRequestId;
        RPosRequestorStack      iRequestorStack;
        RMessagePtr2            iMessage;

    private: // Data
        CPosPrivacyNotifier*    iPrivacyNotifier;
        CPosRequestHandler*     iRequestHandler;

    };

#endif      // CPOSPRIVACYNOTIFIEREXTENSION_H

// End of File

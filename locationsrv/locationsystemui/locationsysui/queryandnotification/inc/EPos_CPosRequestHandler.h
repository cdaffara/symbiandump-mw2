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
* Description:   Class for privacy notifier request handling helper.
*
*/




/**
 @file
 @internalComponent
 @released
*/

#ifndef EPOS_CPOSREQUESTHANDLER_H
#define EPOS_CPOSREQUESTHANDLER_H

//  INCLUDES
#include <e32base.h>
#include <lbs/epos_tposqninputdata.h>
#include <EPos_CPosPrivacyNotifier.h>

// FORWARD DECLARATIONS
class CPosPrivacyNotifier;

// CLASS DECLARATION

/**
*  The privacy notifier request handling helper class.
*/
NONSHARABLE_CLASS(CPosRequestHandler) : public CActive
    {
    public: // Constructors and destructors

       /**
        * Two-phased constructor.
        */
        static CPosRequestHandler* NewL(
            CPosPrivacyNotifier* aNotifier,
            CPosPrivacyNotifierExtension* aExtension);

        /**
        * Destructor.
        */
        ~CPosRequestHandler();

    public: // New functions

        /**
        * Starts the active scheduler.
        * @param aInputData Data containing update parameters.
        */
        void ScheduleRequest(const TPosQNInputData& aInputData);

    protected: // Functions from base classes

        /**
        * From CActive.
        */
        void RunL();

        /**
        * From CActive.
        */
        void DoCancel();

        /**
        * From CActive.
        */
        TInt RunError(TInt aError);

    private:

        /**
        * Private C++ default constructor.
        */
        CPosRequestHandler();

         /**
        * By default Symbian 2nd phase constructor is private.
        */
       void ConstructL(
           CPosPrivacyNotifier* aNotifier,
           CPosPrivacyNotifierExtension* aExtension);

    private: // Data

        CPosPrivacyNotifier* iPrivacyNotifier;
        CPosPrivacyNotifierExtension* iExtension;
        TPosQNInputData iInputData;

};

#endif      // EPOS_CPOSREQUESTHANDLER_H


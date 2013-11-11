/*
* Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  SetUpIdleModeText command handler
*
*/



#ifndef CSETUPIDLEMODETEXTHANDLER_H
#define CSETUPIDLEMODETEXTHANDLER_H

//  INCLUDES
#include <etelsat.h>
#include "CSatCommandHandler.h"

// FORWARD DECLARATION
class MSatUtils;

// CLASS DECLARATION

/**
*  Command handler for SetUpIdleModeText command.
*
*  @lib SetUpIdleModeTextCmd.lib
*  @since Series 60 3.0
*/
class CSetUpIdleModeTextHandler : public CSatCommandHandler
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aUtils Utils inteface
        */
        static CSetUpIdleModeTextHandler* NewL( MSatUtils* aUtils );

        /**
        * Destructor.
        */
        virtual ~CSetUpIdleModeTextHandler();

    public: // Functions from base classes

        /**
        * From MSatEventObserver. Notification of Idle Mode responses.
        * @param aEvent Identifies the event.
        */
        void Event( TInt aEvent );

    protected:  // Functions from base classes

        /**
        * From CActive Cancels the usat request.
        */
        void DoCancel();

        /**
        * From CCommandHandler Requests the command notification.
        * @param aStatus Request status to active object notification
        */
        void IssueUSATRequest( TRequestStatus& aStatus );

        /**
        * From CCommandHandler Precheck before executing the command.
        * @return Boolean indicating is this command allowed to execute.
        */
        TBool CommandAllowed();

        /**
        * From CCommandHandler Need for ui session.
        * @return Boolean indicating does this command need UI session.
        */
        TBool NeedUiSession();

        /**
        * From CCommandHandler Called when USAT API notifies that command.
        */
        void HandleCommand();

        /**
        * From CSatCommandHandler. Indicates the failure of launching ui client.
        */
        void UiLaunchFailed();

    private:

        /**
        * C++ default constructor.
        */
        CSetUpIdleModeTextHandler();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data

        // SetUpIdleModeText command data.
        RSat::TSetUpIdleModeTextV1 iSetUpIdleModeTextData;

        // SetUpIdleModeText command package.
        RSat::TSetUpIdleModeTextV1Pckg iSetUpIdleModeTextPckg;

        // Response from client.
        RSat::TSetUpIdleModeTextRspV1 iSetUpIdleModeTextRsp;

        // Response package.
        RSat::TSetUpIdleModeTextRspV1Pckg iSetUpIdleModeTextRspPckg;

        // Store for restoring last valid idle mode text.
        TBool iSimResetExecuting;
        RSat::TIdleModeText iLastValidText;
        TInt iLastValidIconId;
        RSat::TIconQualifier iLastValidIconQualifier;

        // Indicates whether the homezone indicator is supported.
        TInt iHasHomezoneIndicator;
    };

#endif      // CSETUPIDLEMODETEXTHANDLER_H

// End of File

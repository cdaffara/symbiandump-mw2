/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: AlwaysOnline server command handler
*
*/



#ifndef __HANDLESERVERCOMMANDOPERATION_H__
#define __HANDLESERVERCOMMANDOPERATION_H__

//  INCLUDES
#include <msvapi.h>
#include <AlwaysOnlineManagerCommon.h>

#include "AlwaysOnlineEComInterface.h"

// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Used to call Alwaysonline plugin's HandleServerCommand function
*  async. This is needed because if that function implementation isn't
*  done properly, it may take long time before it completes and as a sync
*  function it would prevent server to call other plugins until the 
*  previous call returns
*
*  @lib alwaysonlinemanagerserver.lib
*  @since 2.6
*/
class CAOServerCommandHandler : public CMsvOperation
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CAOServerCommandHandler* NewL( CMsvSession& aMsvSession,
            TRequestStatus& aObserverRequestStatus,
            TManagerServerCommands aCommand,
            TDes8& aParameter,
            CAlwaysOnlineEComInterface& aPlugin );
        
        /**
        * Destructor.
        */
        virtual ~CAOServerCommandHandler();


    public: // Functions from base classes

        /**
        * From MsvOperation
        * @since 2.6
        * @return, progress of an operation
        */
        virtual const TDesC8& ProgressL();
        
        /**
        * From MsvOperation
        * @since 3.1
        * @return, final progress of an operation
        */
        virtual const TDesC8& FinalProgress();
        
    protected:  // Functions from base classes
        
        /**
        * From CActive
        */
		virtual void DoCancel();

        /**
        * From CActive
        */
		virtual void RunL();
		
        /**
		* Function to handle error during running the operation
		* @since Series60 2.6
		* @return KErrNone, when error has been handled
		*/
		virtual TInt RunError( TInt aError );

    private:

        /**
        * C++ default constructor.
        */
        CAOServerCommandHandler( CMsvSession& aMsvSession,
            TInt aPriority,
            TRequestStatus& aObserverRequestStatus,
            CAlwaysOnlineEComInterface& aPlugin,
            TManagerServerCommands aCommand );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( const TDes8& aParameter );

        /**
        * Starts the operation
        */
        void StartOperationL();


        /**
        * Sets final progress, which is received from operation.
        * @since S60 3.1
        * @param aResponse, response from operation
        */
        void SetFinalProgressFromResult( TAny* aResponse );


        /**
        * Handles response based on command.
        * @since S60 3.1
        * @param aResponse, response from operation
        */
        void HandleResponse( TAny* aResponse );
        
    protected:  // Data

        //dummy progress
        TBuf8<1> iProgress;
        //final progress to server
        TBuf8<KAOFinalProgressLength> iFinalProgress;
        //reference to plugin
        CAlwaysOnlineEComInterface& iPlugin;

    private:    // Data
        TManagerServerCommands iCommand;
        HBufC8*  iParameterBuf;
        
    };

#endif      // __HANDLESERVERCOMMANDOPERATION_H__
            
// End of File

/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Abstract base class for queries.
*
*/


#ifndef CLPDQUERYLAUNCHERBASE_H
#define CLPDQUERYLAUNCHERBASE_H

//  INCLUDES
#include "lpddlgobserver.h"

#include <e32base.h>
#include <badesca.h>
#include <coemain.h>

#include <epos_cposprivacynotifier.h>


// SYSTEM INCLUDE


// FORWARD DECLARATIONS
class CLpdGlobalPluginDialog;
class CLpdMessagePluginDialog;
class MLpdResultHandler;
class CLpdRequestorProcessor;

    
// CLASS DECLARATION

/**
*  Base class which handles the dialog and expiration timer.
*  Expiration timer can be used to ensure that if the user doesn't handle
*  a dialog in certain time the dialog is closed. NOTE: In current
*  implementation the timer is not started, i.e. dialogs never expire.
*  Implementations of this class should notify a result handler
*  when dialog result is available. This base class takes care of
*  notifying possible timeout with KErrTimedOut.
*
*  @lib locverifierdlg.lib
*  @since 2.1
*/
class CLpdQueryLauncherBase : public CBase, public MLpdDlgObserver
    {
    
protected:
        enum TRequestType
               {
               EVerification = 0,
               ENotification,
               EUnknown
               };
 
   
    public:  // Constructors and destructor
                
        /**
        * Destructor. Exits possibly running dialog without callback.
        */
        virtual ~CLpdQueryLauncherBase();
        
        
    public: // New functions, query setup and launching

       /**
        * Method for setting text array.
        * 
        */
        void SetRequestInfoL( CLpdRequestorProcessor* aRequestInfo );

       
        /**
        * Method for launching query, note that it must have been
        * correctly prepared first.
        */
        void StartQueryL();

              
        virtual void RequestType(TRequestType& aRequestType);

   
    public: // Functions from base classes

        /**
        * From MLpdDlgObserver, called when dialog is dismissed.
        * This method calls also pure virtual method
        * ExtendedHandleDlgDismissedL().
        * @param aResult code of the query
        */
        void HandleDlgDismissedL( TInt aResult);

    public: 
         void Cancel();
    protected: // Constructors and destructors

        /**
        * C++ constructor.
        * @param aHandler of the query result
        */
        CLpdQueryLauncherBase( MLpdResultHandler& aHandler );
        
        void ConstructL();
      
    protected: // New functions

        /**
        * This pure virtual method must be implemented
        * by the derived class to handle the result code
        * appropriately and to notify the observer of this
        * class.
        */
        virtual void ExtendedHandleDlgDismissedL() = 0;

   
    protected: // Data

        // Reference to the result handler
        MLpdResultHandler& iHandler;

        // Result code from the dialog
        TInt iResult;

        // Pointer to the list query dialog
        CLpdGlobalPluginDialog* iQueryDlg; // owns untils RunLD() called
            
        
    };

#endif      // CLPDQUERYLAUNCHERBASE_H   
            
// End of File

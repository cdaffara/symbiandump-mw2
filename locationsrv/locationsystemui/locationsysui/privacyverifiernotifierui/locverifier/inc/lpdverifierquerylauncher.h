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
* Description:  Class which handles the dialog and expiration timer.
*
*/


#ifndef CLPDVERIFIERQUERYLAUNCHER_H
#define CLPDVERIFIERQUERYLAUNCHER_H

//  INCLUDES
#include "lpdquerylauncherbase.h"
#include <e32base.h>
#include <lbs/epos_privacy.h>


// FORWARD DECLARATIONS
class CLpdGlobalPluginDialog;


// CLASS DECLARATION

/**
*  Implements functionality for executing verification queries.
*
*  @lib locverifierdlg.lib
*  @since 2.1
*/
class CLpdVerifierQueryLauncher :
public CLpdQueryLauncherBase
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        * @param aHandler of ther query result
        * @return created object
        */
        static CLpdVerifierQueryLauncher* NewL( 
            MLpdResultHandler& aHandler );
        
        /**
        * Destructor. Call Cancel() first if you have launched a query.
        */
        virtual ~CLpdVerifierQueryLauncher();

    public: // New functions
        
       
    protected: // Functions from base classes

        /**
        * From CLpdQueryLauncherBase, called when dialog is dismissed.
        */
        void ExtendedHandleDlgDismissedL();
        
        void RequestType(TRequestType& aRequestType);

    private: // Constructors and destructors

        /**
        * C++ constructor.
        * @param aHandler of the query result
        */
        CLpdVerifierQueryLauncher( MLpdResultHandler& aHandler );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
            
        
    };

#endif      // CLPDVERIFIERQUERYLAUNCHER_H   
            
// End of File

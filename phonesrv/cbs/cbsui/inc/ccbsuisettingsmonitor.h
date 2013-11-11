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
* Description:
*
*/


#ifndef     CCBSUISETTINGSMONITOR_H
#define     CCBSUISETTINGSMONITOR_H

//  INCLUDES
#include <E32BASE.H>
#include <cbscommon.h>
 
//  FORWARD DECLARATIONS
class MCbs;
class CCbsUiappAppUi;
class MCbsUiSettingsObserver;

//  CLASS DEFINITIONS 


/**
* Server notifies this class if settings are changed.
* 
*/
class CCbsUiSettingsMonitor : public CActive
    {
    public:     // New functions
        /**
        * Create an instance of the class. 
        *
        * @param aServer cbs server
        * @param aObserver settings observer
        */
        static CCbsUiSettingsMonitor* NewL(
            MCbs& aServer , MCbsUiSettingsObserver& aObserver );

        /**
        * Destructor.
        */
        ~CCbsUiSettingsMonitor();

    public: // From CActive
        
        /**
        * It is called when server notifies.
        */
        void RunL();

        /**
        * Cancels the request.
        */
        void DoCancel();

    private: // New functions

        /**
        * Issues the request..
        */
        void IssueRequest();

    private:

        /**
        * Constructor.
        *
        * @param aServer cbs server
        */
        CCbsUiSettingsMonitor( 
            MCbs& aServer, MCbsUiSettingsObserver& aObserver );

        /**
        * Finalizes the construction.
        */
        void ConstructL();

    private: // Data

        // ETrue if request is continued.
        TBool iContinue;
        
        // Server subsession for settings        
        MCbs& iServer;

        // Event which is registered to server
        TCbsSettingsEvent iEvent;

        // Observer who is interested about settings changes.       
        MCbsUiSettingsObserver& iObserver;
    };

#endif      //  CCBSUISETTINGSMONITOR_H
  
// End of File

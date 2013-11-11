/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  The Refresh handler to inform refresses.
*
*/



#ifndef CPSETREFRESHHANDLERIMPL_H
#define CPSETREFRESHHANDLERIMPL_H


//  INCLUDES
#include <psetrefreshhandler.h> 
#include <msatrefreshobserver.h>


// FORWARD DECLARATIONS
class CPsetCustomerServiceProfile;
class CSSSettingsRefreshContainer;
class MSSSettingsRefreshHandler;


// CLASS DECLARATION

/**
*  The Refresh handler to inform refresses.
*
*  @lib PhoneSettings.lib
*  @since 2.8
*/
class CPSetRefreshHandlerImpl :
    public CPSetRefreshHandler,
    public MSSSettingsRefreshObserver
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CPSetRefreshHandlerImpl* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CPSetRefreshHandlerImpl();


    public: // Functions from base classes

        /**
        * @see CPSetRefreshHandler::NotifyFileChangeL.
        */
        void NotifyFileChangeL(
            MSSSettingsRefreshObserver& aObserver,
            TSatElementaryFiles aObservedFiles,
            TSatRefreshType aObservedRefreshType );

        /**
        * @see CPSetRefreshHandler::CancelNotify.
        */
        void CancelNotify();

        /**
        * @see CPSetRefreshHandler::ChangedCspSettings.
        */
        TInt ChangedCspSettings( 
            TPSetChangedCspSetting& aChangedCspSettings,
            TPSetChangedCspSetting& aNewValues );


    public: // Functions from base classes

        /**
        * @see MSSSettingsRefreshObserver::AllowRefresh
        */
        TBool AllowRefresh(
            const TSatRefreshType aType,
            const TSatElementaryFiles aFiles );

        /**
        * @see MSSSettingsRefreshObserver::Refresh
        */
        void Refresh(
            const TSatRefreshType aType,
            const TSatElementaryFiles aFiles );


    private:

        /**
        * C++ default constructor.
        */
        CPSetRefreshHandlerImpl();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();


    private:    // Data
         
        // The Csp handler.
        CPsetCustomerServiceProfile*    iCsp;

        // The SSSettings refresh handler container.
        CSSSettingsRefreshContainer*    iSSSetRefContainer;

        // The SSSettings refresh handler.
        MSSSettingsRefreshHandler*      iSSSetRefHandler;

        // The observer for refresses
        MSSSettingsRefreshObserver*     iObserver;

    };

#endif      // CPSETREFRESHHANDLERIMPL_H 
            
// End of File

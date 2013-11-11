/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: header file for appui implementation
*
*/
 
#ifndef APPUI_H__
#define APPUI_H__

// INCLUDES
#include <aknappui.h> 

    class CEventReceiver;
        
    class CAppUi : public CAknAppUi
    {
    public:
        /**
         * Default Constructor
         */
        CAppUi();
        
        /**
         * Default Destructor
         */
        ~CAppUi();        
        
        /**
         * ConstructL.
         * 2nd phase constructor.
         */
        void ConstructL();
        
        /**
         * Sets the event receiver.
         *
         * @param aEventReceiver is set while creating AppUi.
         *        Appui will send wssession events( EEventScreenDeviceChanged,
         *        & EEventWindowVisibilityChanged) to CEventReceiver that 
         *        distributes it to its listeners.
         *         
         */
       void SetEventReceiver( CEventReceiver& aEventReceiver );

        
    protected:

       /**
        * Handles window server events.
        *
        * @param aEvent The window server event that occurred.
        * @param aDestination The control associated with the event.
        *
        */      
        void HandleWsEventL( const TWsEvent& aEvent, CCoeControl* aDestination );
        
        void HandleForegroundEventL(TBool aForeground);

    private:
        //handle to the event reciever.
        CEventReceiver* iEventReceiver;
    }; 

#endif  //APPUI_H__


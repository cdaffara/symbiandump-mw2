/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Header of EffectControlBase class.
*
*/


#ifndef C_CEFFECTCONTROLBASE_H
#define C_CEFFECTCONTROLBASE_H

#include <e32base.h>
#include <mmfcontrollerframework.h>

class MCustomCommand;

namespace multimedia
    {
    class MStreamControl;
    class MControlObserver;
    class MEffectControl;
    class CEffectControlBaseImpl;
    
    class CEffectControlBase
        {
    protected:
        // Called by subclass
        CEffectControlBase();
        // Has to be called by subclass
        void ConstructL( TUid aCIUid );
        
        // Returns ETrue if connected to Message Handler else EFalse
        TBool IsConnected();
        
        // Returns KErrNone if it is already connected to message handler
        // or connected to message handler successfully.
        TInt Connect();
              
        // Used by sub-classes for sending custom commands to Message Handler
        // Returns KErrNotReady if not connected
        TInt CustomCommandSync( TInt aFunction,
                                const TDesC8& aDataTo1,
                                const TDesC8& aDataTo2,
                                TDes8& aDataFrom);
        
        // Returns KErrNotReady if not connected
        TInt CustomCommandSync( TInt aFunction,
                                const TDesC8& aDataTo1,
                                const TDesC8& aDataTo2);
        
        // Client is signalled with KErrNotReady if not connected
        void CustomCommandAsync( TInt aFunction,
                                 const TDesC8& aDataTo1,
                                 const TDesC8& aDataTo2,
                                 TDes8& aDataFrom,
                                 TRequestStatus& aStatus);
                                
        // Client is signalled with KErrNotReady if not connected
        void CustomCommandAsync( TInt aFunction,
                                 const TDesC8& aDataTo1,
                                 const TDesC8& aDataTo2,
                                 TRequestStatus& aStatus);

        // Used by sub-classes for sending custom commands to known destination
        // This is intended for objects implementing custom commands who
        // knows destination address
        TInt CustomCommandSync( const TMMFMessageDestinationPckg& aDestination,
                                TInt aFunction,
                                const TDesC8& aDataTo1,
                                const TDesC8& aDataTo2,
                                TDes8& aDataFrom );
        TInt CustomCommandSync( const TMMFMessageDestinationPckg& aDestination,
                                TInt aFunction,
                                const TDesC8& aDataTo1,
                                const TDesC8& aDataTo2 );
        void CustomCommandAsync( const TMMFMessageDestinationPckg& aDestination,
                                 TInt aFunction,
                                 const TDesC8& aDataTo1,
                                 const TDesC8& aDataTo2,
                                 TDes8& aDataFrom,
                                 TRequestStatus& aStatus );
        void CustomCommandAsync( const TMMFMessageDestinationPckg& aDestination,
                                 TInt aFunction,
                                 const TDesC8& aDataTo1,
                                 const TDesC8& aDataTo2,
                                 TRequestStatus& aStatus );
		
		TInt GetMessageHandle(TMMFMessageDestinationPckg &aPckg);
		MCustomCommand* GetCustomCommand();
		
    public:
        virtual ~CEffectControlBase();
        // Called by Stream Control when Effect Control is added to it
        TInt GetControlObserver( MControlObserver*& aObserver );
        // Called by Stream Control when Effect Control is added to it
        TInt AddedToStreamControl( MStreamControl& aStreamControl,
                                   MCustomCommand& aCustomCommand,
                                   MEffectControl& aEffect );
        // Called by Stream Control when Effect Control is removed to it
        TInt RemovedFromStreamControl( MStreamControl& aStreamControl );
        // Called by Stream Control when it is deleted before removing Effect Control
        void StreamControlDeleted( MStreamControl& aStreamControl );
        // Returns KErrNone if it is not connected to message handler
        // or disconnected from Message Handler successfully.
        TInt Disconnect();
    	
    public:
        enum TEffectControlEvent
            {
            ECIBuilderCreated,
            EMessageHandlerDeleted
            };
    public:
        // Handles effect control event. The derived class must implement to
        // handle the event.
        virtual void Event( TEffectControlEvent aEvent );
        
    private:
        CEffectControlBaseImpl* iCEffectControlBaseImpl;
        };
    } // namespace multimedia

#endif // C_CEFFECTCONTROLBASE_H

// End of file

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
* Description:  Header of EffectControlBaseImpl class.
*
*/


#ifndef EFFECTCONTROLBASEIMPL_H
#define EFFECTCONTROLBASEIMPL_H

#include <e32base.h>
#include <ControlObserver.h>
#include <mmf/common/mmfcontrollerframework.h>

class MCustomCommand;

namespace multimedia
    {
    class MStreamControl;
    class MEffectControl;
    class CEffectControlBase;
    
    enum TEffectControlBaseEvent
        {
        EControllerLoaded,
        EControllerUnloaded
        };

    
    class CEffectControlBaseImpl : public CActive, public MControlObserver
        {
            
    public:
        static CEffectControlBaseImpl* NewL( CEffectControlBase& aParent, TUid aCIUid );
        virtual ~CEffectControlBaseImpl();
        TInt AddedToStreamControl( MStreamControl& aStreamControl,
                                   MCustomCommand& aCustomCommand,
                                   MEffectControl& aEffect );
        TInt RemovedFromStreamControl( MStreamControl& aStreamControl );
        void StreamControlDeleted( MStreamControl& aStreamControl );
        TBool IsConnected();        
        TInt Connect();        
        TInt Disconnect();
        TInt CustomCommandSync(TInt aFunction,
                               const TDesC8& aDataTo1,
                               const TDesC8& aDataTo2,
                               TDes8& aDataFrom);
        TInt CustomCommandSync(TInt aFunction,
                               const TDesC8& aDataTo1,
                               const TDesC8& aDataTo2);
        void CustomCommandAsync(TInt aFunction,
                                const TDesC8& aDataTo1,
                                const TDesC8& aDataTo2,
                                TDes8& aDataFrom,
                                TRequestStatus& aStatus);
        void CustomCommandAsync(TInt aFunction,
                                const TDesC8& aDataTo1,
                                const TDesC8& aDataTo2,
                                TRequestStatus& aStatus);
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

    protected:
        // From CActive begins
        void RunL();
        void DoCancel();
        TInt RunError( TInt aError );
        // From CActive ends

        // From MControlObserver begins
        void Event( MControl* aControl, TUint aEventType, TAny* aEventObject );
        // From MControlObserver ends
        
    private:
        CEffectControlBaseImpl( CEffectControlBase& aParent, TUid aCIUid );
        void ConstructL();

        enum CIFBuilderParserFunctions
            {
            ECIFCreateMessageHandler,
            ECIFRemoveMessageHandler
            };    
        // Messages sent to builder parser
        TInt BuilderParserMessage(CIFBuilderParserFunctions aCommand);

        
    private:
        CEffectControlBase& iParent;
        TUid iCIUid;
        TMMFMessageDestinationPckg iMsgHndlrHandlePckg;
        MStreamControl* iStreamControl;
        MCustomCommand* iCustomCommand;
        MEffectControl* iEffect;
        TMMFMessageDestinationPckg    iBuilderHandle;
        };
    } // namespace multimedia

#endif // EFFECTCONTROLBASEIMPL_H

// End of file

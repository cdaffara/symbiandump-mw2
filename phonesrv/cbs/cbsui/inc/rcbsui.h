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


#ifndef RCBSUI_H
#define RCBSUI_H

// INCLUDE FILES
#include <e32base.h>
#include <CbsCommon.h>
#include <RCbs.h>

#include "mcbs.h"


// CLASS DECLARATION

/**
* Delecates calls to RCbs.
*/
class RCbsUi : public MCbs
    {
    public: // Constructor and destructor

        /**
        * Constructor.
        */
        RCbsUi();
       
        /**
        * Destructor.
        */
        ~RCbsUi();
        
    public: // From MCbs

        TInt Connect();        
        void Close();
        TVersion Version() const;
        
        // === Settings-related methods        
        void GetReceptionStatus( TBool& aStatus );        
        TInt SetReceptionStatus( TBool aStatus );        
        void GetTopicDetectionStatus( TBool& aStatus );        
        TInt SetTopicDetectionStatus( TBool aStatus );        
        void GetLanguages( TCbsSettingsLanguages& aLanguages );
        TInt SetLanguages( const TCbsSettingsLanguages& aLanguages );
        void NotifySettingsChanged( TRequestStatus& aStatus, TCbsSettingsEvent& aEvent );
        void NotifySettingsChangedCancel();
        
        //  === Topic Collection-related methods        
        void StartCollectionBrowsing();        
        TBool HasNextCollectionTopic();                            
        TInt NextCollectionTopic( TCbsTopicInfo& aInfo );
        
        //  === Topic list-related methods        
        void GetTopicCount( TInt& aCount );
        TInt GetTopic( const TInt aIndex, TCbsTopic& aTopic );
        TInt FindTopicByNumber( TCbsTopicNumber aNumber, TCbsTopic& aTopic );
        TInt DeleteTopic( TCbsTopicNumber aNumber );      
        TInt DeleteAllTopics();
        TInt AddTopic( TCbsTopic& aTopic );
        TInt ChangeTopicNameAndNumber( 
                TCbsTopicNumber aOldNumber,
                TCbsTopicNumber aNewNumber, 
                const TCbsTopicName& aName );
        TInt ChangeTopicSubscriptionStatus( 
                TCbsTopicNumber aNumber, TBool aNewStatus );
        TInt ChangeTopicHotmarkStatus( TCbsTopicNumber aNumber,
                TBool aNewStatus );
        void NotifyOnTopicListEvent( 
                TRequestStatus& aStatus, 
                const TInt aRequested, 
                TCbsTopicListEvent& aEvent, 
                TCbsTopicNumber& aNumber );
        void NotifyOnTopicListEventCancel();
        TInt GetNewTopicsCount( TInt& aCount );
        TInt GetLatestTopicNumber( TCbsTopicNumber& aNumber );
        void GetUnreadMessageCount( TInt& aCount );
        void GetHotmarkedMessageHandle( TCbsMessageHandle& aMessage );
        TInt NumberOfUnreadHotmarkedMessages();
        TInt GetNextAndPrevTopicNumber( 
            const TCbsTopicNumber& aCurrentTopicNumber,
            TCbsTopicNumber& aPrevTopicNumber,
            TCbsTopicNumber& aNextTopicNumber,
            TInt& aPosition );
        
        // === Topic Messages-related methods       
        TInt GetMessageCount( TCbsTopicNumber aNumber, TInt& aCount );
        TInt GetMessage( TCbsTopicNumber aNumber, TInt aIndex,
                TCbsMessage& aMessage );
        TInt FindMessageByHandle( 
                const TCbsMessageHandle& aHandle, 
                TCbsMessage& aMessage );
        TInt GetMessageIndexByHandle( 
                const TCbsMessageHandle& aHandle, TInt& aIndex );
        TInt DeleteMessage( const TCbsMessageHandle& aHandle );
        TInt SaveMessage( const TCbsMessageHandle& aHandle );
        TInt LockMessage( const TCbsMessageHandle& aHandle );
        TInt ReadMessage( const TCbsMessageHandle& aHandle );
        TInt GetMessageContents( 
                const TCbsMessageHandle& aHandle, 
                TDes& aBuffer );
        TInt GetNextAndPrevMessageHandle(
                const TCbsMessageHandle& aCurrentMsgHandle,
                TCbsMessageHandle& aPrevMsgHandle,
                TCbsMessageHandle& aNextMsgHandle,
                TInt& aPosition );
        
        // Other methods
        TBool Connected() const;
        void Shutdown() const;

    private: // Data

        // Real cbs client
        RCbs iServer;
        
    };
    
#endif // RCBSUI_H

// End of file 

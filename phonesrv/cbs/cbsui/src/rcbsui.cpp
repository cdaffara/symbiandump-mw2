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
* Description: 
*     RCbsUi calls directly real cbs client, which handles
*     the actual calls to server.
*
*/


//  INCLUDES
#include <QObject>
#include "rcbsui.h"
#include <RCbs.h>

 
// ================= MEMBER FUNCTIONS =======================

RCbsUi::RCbsUi()
    {
    }

RCbsUi::~RCbsUi() 
    {
    }

TInt RCbsUi::Connect()
    {
    return iServer.Connect();
    }

void RCbsUi::Close()
    {
    iServer.Close();
    }

TVersion RCbsUi::Version() const
    {
    return iServer.Version();
    }

// Settings-related methods
void RCbsUi::GetReceptionStatus( 
    TBool&  aStatus )
    {
    iServer.GetReceptionStatus( aStatus );
    }

TInt RCbsUi::SetReceptionStatus( 
    TBool aStatus )
    {
    return iServer.SetReceptionStatus( aStatus );
    }

void RCbsUi::GetTopicDetectionStatus( 
    TBool&  aStatus )
    {
    iServer.GetTopicDetectionStatus( aStatus );
    }

TInt RCbsUi::SetTopicDetectionStatus( 
    TBool   aStatus )
    {
    return iServer.SetTopicDetectionStatus(aStatus);
    }

void RCbsUi::GetLanguages( 
    TCbsSettingsLanguages& aLanguages )
    {
    iServer.GetLanguages(aLanguages);
    }

TInt RCbsUi::SetLanguages( 
    const TCbsSettingsLanguages& aLanguages )
    {
    return iServer.SetLanguages(aLanguages);
    }

void RCbsUi::NotifySettingsChanged( 
    TRequestStatus&     aStatus, 
    TCbsSettingsEvent&  aEvent )
    {
    iServer.NotifySettingsChanged( aStatus, aEvent );
    }

void RCbsUi::NotifySettingsChangedCancel()
    {
    iServer.NotifySettingsChangedCancel();
    }

// Topic Collection -related methods
void RCbsUi::StartCollectionBrowsing()
    {
    iServer.StartCollectionBrowsing();
    }

TBool RCbsUi::HasNextCollectionTopic()
    {
    return iServer.HasNextCollectionTopic();
    }

TInt RCbsUi::NextCollectionTopic( 
    TCbsTopicInfo&      aInfo )
    {
    return iServer.NextCollectionTopic(aInfo);
    }

// Topic List-related methods
void RCbsUi::GetTopicCount( 
    TInt&   aCount )
    {
    iServer.GetTopicCount(aCount);
    }

TInt RCbsUi::GetTopic( 
    const TInt      aIndex, 
    TCbsTopic&      aTopic )
    {
    return iServer.GetTopic(aIndex, aTopic);
    }

TInt RCbsUi::FindTopicByNumber( 
    TCbsTopicNumber aNumber, 
    TCbsTopic& aTopic )
    {
    return iServer.FindTopicByNumber( aNumber, aTopic );
    }

TInt RCbsUi::DeleteTopic( 
    TCbsTopicNumber aNumber )
    {
    return iServer.DeleteTopic(aNumber);
    }

TInt RCbsUi::DeleteAllTopics()
    {
    return iServer.DeleteAllTopics();
    }

TInt RCbsUi::AddTopic( 
    TCbsTopic&      aTopic )
    {
    return iServer.AddTopic(aTopic);
    }

TInt RCbsUi::ChangeTopicNameAndNumber( 
    TCbsTopicNumber aOldNumber,
    TCbsTopicNumber aNewNumber, 
    const TCbsTopicName& aName )
    {
    return iServer.ChangeTopicNameAndNumber(aOldNumber, aNewNumber, aName);
    }

TInt RCbsUi::ChangeTopicSubscriptionStatus( 
    TCbsTopicNumber aNumber,
    TBool aNewStatus )
    {
    return iServer.ChangeTopicSubscriptionStatus( aNumber, aNewStatus );
    }

TInt RCbsUi::ChangeTopicHotmarkStatus( 
    TCbsTopicNumber aNumber,
    TBool aNewStatus )
    {
    return iServer.ChangeTopicHotmarkStatus( aNumber, aNewStatus );
    }

void RCbsUi::NotifyOnTopicListEvent( 
    TRequestStatus&         aStatus, 
    const TInt              aRequested, 
    TCbsTopicListEvent&     aEvent, 
    TCbsTopicNumber&        aNumber )
    {
    iServer.NotifyOnTopicListEvent(aStatus, aRequested, aEvent, aNumber);
    }

void RCbsUi::NotifyOnTopicListEventCancel()
    {
    iServer.NotifyOnTopicListEventCancel();
    }

TInt RCbsUi::GetNewTopicsCount( 
    TInt&       aCount )
    {
    return iServer.GetNewTopicsCount(aCount);
    }

TInt RCbsUi::GetLatestTopicNumber( TCbsTopicNumber& aNumber )
    {
    return iServer.GetLatestTopicNumber(aNumber);
    }

void RCbsUi::GetUnreadMessageCount( 
    TInt&       aCount )
    {
    iServer.GetUnreadMessageCount(aCount);
    }

void RCbsUi::GetHotmarkedMessageHandle( 
    TCbsMessageHandle&  aMessage )
    {
    iServer.GetHotmarkedMessageHandle(aMessage);
    }

TInt RCbsUi::NumberOfUnreadHotmarkedMessages() 
    {
    return iServer.NumberOfUnreadHotmarkedMessages();
    }

TInt RCbsUi::GetNextAndPrevTopicNumber( 
	const TCbsTopicNumber& aCurrentTopicNumber,
	TCbsTopicNumber& aPrevTopicNumber,
	TCbsTopicNumber& aNextTopicNumber,
	TInt& aPosition )
    {
    return iServer.GetNextAndPrevTopicNumber(
        aCurrentTopicNumber, aPrevTopicNumber, aNextTopicNumber, aPosition);
    }


// Topic Messages-related methods
TInt RCbsUi::GetMessageCount( 
    TCbsTopicNumber aNumber,
    TInt& aCount )
    {
    return iServer.GetMessageCount(aNumber, aCount);
    }
    
TInt RCbsUi::GetMessage( 
    TCbsTopicNumber aNumber, 
    TInt            aIndex, 
    TCbsMessage&    aMessage )
    {
    return iServer.GetMessage(aNumber, aIndex, aMessage);
    }

TInt RCbsUi::FindMessageByHandle( 
    const TCbsMessageHandle&    aHandle, 
    TCbsMessage&                aMessage )
    {
    return iServer.FindMessageByHandle(aHandle, aMessage);
    }

TInt RCbsUi::GetMessageIndexByHandle( 
    const TCbsMessageHandle& aHandle, TInt& aIndex )
    {
    return iServer.GetMessageIndexByHandle(aHandle, aIndex);
    }

TInt RCbsUi::DeleteMessage( 
    const TCbsMessageHandle&    aHandle )
    {    
    return iServer.DeleteMessage(aHandle);
    }

TInt RCbsUi::SaveMessage( 
    const TCbsMessageHandle&    aHandle )
    {    
    return iServer.SaveMessage(aHandle);
    }

TInt RCbsUi::LockMessage( 
    const TCbsMessageHandle&    aHandle )
    {    
    return iServer.LockMessage(aHandle);
    }

TInt RCbsUi::ReadMessage( 
    const TCbsMessageHandle&    aHandle )
    {
    return iServer.ReadMessage(aHandle);
    }

TInt RCbsUi::GetMessageContents( 
    const TCbsMessageHandle&    aHandle, 
    TDes&                       aBuffer )
    {
    return iServer.GetMessageContents(aHandle, aBuffer);
    }

TInt RCbsUi::GetNextAndPrevMessageHandle(
	const TCbsMessageHandle& aCurrentMsgHandle,
	TCbsMessageHandle& aPrevMsgHandle,
	TCbsMessageHandle& aNextMsgHandle,
	TInt& aPosition )
    {
    return iServer.GetNextAndPrevMessageHandle(
        aCurrentMsgHandle, aPrevMsgHandle, aNextMsgHandle, aPosition);
    }

TBool RCbsUi::Connected() const 
    {
    return iServer.Connected();   
    }

void RCbsUi::Shutdown() const
    {
    iServer.Shutdown();
    }

//  End of File

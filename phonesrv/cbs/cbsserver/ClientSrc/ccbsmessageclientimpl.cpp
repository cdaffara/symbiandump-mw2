/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of the class CCbsMessageClientImpl.
*
*/


// INCLUDE FILES
#include "ccbsmessageclientimpl.h"
#include "RCbs.h"

// ==================== LOCAL FUNCTIONS ====================

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsMessageClientImpl::CCbsMessageClientImpl
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsMessageClientImpl::CCbsMessageClientImpl()
    {    
    }

// -----------------------------------------------------------------------------
// CCbsMessageClientImpl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsMessageClientImpl::ConstructL()
    {    
    User::LeaveIfError( iCbsClient.Connect() );
    }

// -----------------------------------------------------------------------------
// CCbsMessageClientImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCbsMessageClientImpl* CCbsMessageClientImpl::NewL() 
    {
    CCbsMessageClientImpl* self = new ( ELeave ) CCbsMessageClientImpl();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

    
// Destructor
CCbsMessageClientImpl::~CCbsMessageClientImpl() 
    {
    iCbsClient.Close();
    }

// -----------------------------------------------------------------------------
// CCbsMessageClientImpl::FindMessageByHandle
//
// (Other items were commented in a header)
// -----------------------------------------------------------------------------
//
TInt CCbsMessageClientImpl::FindMessageByHandle( const TCbsMessageHandle& aHandle, 
    TCbsMessage& aMessage )
    {
    return iCbsClient.FindMessageByHandle( aHandle, aMessage );
    }

// -----------------------------------------------------------------------------
// CCbsMessageClientImpl::FindTopicByNumber
// 
// (Other items were commented in a header)
// -----------------------------------------------------------------------------
//    
TInt CCbsMessageClientImpl::FindTopicByNumber( const TCbsTopicNumber aNumber,
    TCbsTopic& aTopic )
    {
    return iCbsClient.FindTopicByNumber( aNumber, aTopic );
    }

// -----------------------------------------------------------------------------
// CCbsMessageClientImpl::TopicCount
// 
// (Other items were commented in a header)
// -----------------------------------------------------------------------------
//                
TInt CCbsMessageClientImpl::TopicCount()
    {
    TInt topicCount( 0 );
    iCbsClient.GetTopicCount( topicCount );
    return topicCount;
    }

// -----------------------------------------------------------------------------
// CCbsMessageClientImpl::GetTopic
// 
// (Other items were commented in a header)
// -----------------------------------------------------------------------------
//    
TInt CCbsMessageClientImpl::GetTopic( const TInt aIndex, TCbsTopic& aTopic )
    {
    return iCbsClient.GetTopic( aIndex, aTopic );
    }

// -----------------------------------------------------------------------------
// CCbsMessageClientImpl::GetUnreadMessageCount
// 
// (Other items were commented in a header)
// -----------------------------------------------------------------------------
//
TInt CCbsMessageClientImpl::UnreadMessageCount()
    {
    TInt msgCount( 0 );
    iCbsClient.GetUnreadMessageCount( msgCount );
    return msgCount;
    }

// -----------------------------------------------------------------------------
// CCbsMessageClientImpl::GetMessageCount
// 
// (Other items were commented in a header)
// -----------------------------------------------------------------------------
//
TInt CCbsMessageClientImpl::GetMessageCount( const TCbsTopicNumber aNumber,
    TInt& aCount )
    {
    return iCbsClient.GetMessageCount( aNumber, aCount );
    }

// -----------------------------------------------------------------------------
// CCbsMessageClientImpl::GetMessage
// 
// (Other items were commented in a header)
// -----------------------------------------------------------------------------
//
TInt CCbsMessageClientImpl::GetMessage( const TCbsTopicNumber aNumber, 
    const TInt aIndex,
    TCbsMessage& aMessage )
    {
    return iCbsClient.GetMessage( aNumber, aIndex, aMessage );
    }

// -----------------------------------------------------------------------------
// CCbsMessageClientImpl::GetMessageContents
// 
// (Other items were commented in a header)
// -----------------------------------------------------------------------------
//    
TInt CCbsMessageClientImpl::GetMessageContents( const TCbsMessageHandle& aHandle, 
    TDes& aBuffer )
    {
    return iCbsClient.GetMessageContents( aHandle, aBuffer );
    }

// -----------------------------------------------------------------------------
// CCbsMessageClientImpl::SetMessageAsRead
// 
// (Other items were commented in a header)
// -----------------------------------------------------------------------------
//
TInt CCbsMessageClientImpl::SetMessageAsRead( const TCbsMessageHandle& aHandle )
    {
    return iCbsClient.ReadMessage( aHandle );
    }

//  End of File  

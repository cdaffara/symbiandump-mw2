/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This class implements the server-side session.
*
*/



// INCLUDE FILES
#include 	<CustomInterfaceBuilder.h>
#include 	<CustomInterfaceCustomCommandParser.h>
#include    <mmf/common/mmfcustomcommandparsermanager.h>
#include    <mmf/common/mmfcontrollerframework.h>
#include    <mmf/common/mmfipcserver.h>

#include	"RadioServerSession.h"
#include	"RadioServer.h"
#include    "RadioDebug.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CRadioServerSession::CRadioServerSession
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CRadioServerSession::CRadioServerSession(
	CRadioServer& aServer )
	:	iServer(aServer),
		iCustomInterfaceBuilder(NULL),
		iObjectContainer(NULL),
		iCustomCommandParserManager(NULL)
    {
    }

// -----------------------------------------------------------------------------
// CRadioServerSession::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CRadioServerSession::ConstructL()
    {
	RADIO_RDEBUG(_L("[RADIO-SVR] CRadioServerSession::ConstructL()"));
	iServer.AddSession();

	iCustomCommandParserManager = CMMFCustomCommandParserManager::NewL();

	CCustomInterfaceCustomCommandParser* customInterfaceParser =
			CCustomInterfaceCustomCommandParser::NewL(*this);
	CleanupStack::PushL(customInterfaceParser);
	iCustomCommandParserManager->AddCustomCommandParserL(*customInterfaceParser);
    CleanupStack::Pop(customInterfaceParser);

	iCustomInterfaceBuilder = CCustomInterfaceBuilder::NewL(MMFObjectContainerL(),
															*(iServer.iDevSound));
	// This needs to be pushed eventhough it's member variable, since the ownership
	// is passed to the container. Otherwise, it will result in memory leak.
	CleanupStack::PushL(iCustomInterfaceBuilder);
	// Append custom interface builder to the array of MMFObjects
	User::LeaveIfError(MMFObjectContainerL().AddMMFObject(*iCustomInterfaceBuilder));
	CleanupStack::Pop(iCustomInterfaceBuilder);
    }

// -----------------------------------------------------------------------------
// CRadioServerSession::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CRadioServerSession* CRadioServerSession::NewL(
	CRadioServer& aServer )
    {
    CRadioServerSession* self = new( ELeave ) CRadioServerSession(aServer);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CRadioServerSession::~CRadioServerSession()
    {
	RADIO_RDEBUG(_L("[RADIO-SVR] CRadioServerSession::~CRadioServerSession"));
	iServer.DropSession();
	delete iCustomCommandParserManager;
	delete iObjectContainer;
    }

// -----------------------------------------------------------------------------
// CRadioServerSession::ServiceL
// -----------------------------------------------------------------------------
//
void CRadioServerSession::ServiceL(
    const RMessage2& aMessage )
    {
    // Get the destination info to see who the message is destined for
    TMMFMessageDestinationPckg destinationPckg;
	MmfMessageUtil::ReadL(aMessage, 0, destinationPckg);
	if ( (destinationPckg().DestinationHandle() == KRadioServerObjectHandle) &&
         (destinationPckg().InterfaceId() == KRadioServerInterfaceUid) )
	    {
		iServer.ServiceRequestL(aMessage);
		}
	else
		{
	    // Create a TMMFMessage and get the destination info from the client
        // into TMMFMessage.
        TMMFMessage message(aMessage);
    	message.FetchDestinationL();

	    // Check if Custom Command Parser manager can handle the message...
	    TBool handled = EFalse;
	    if ( iCustomCommandParserManager )
            {
		    handled = iCustomCommandParserManager->HandleRequest(message);
            }

        // If the CCCParser manager couldn't handle the message, check if the
        // Message is for one of the MMF Objects
	    if ( !handled )
            {
		    CMMFObject* object = NULL;
            TInt status(KErrNone);
            // Try to find MMFObject that handles this request
		    status = MMFObjectContainerL().FindMMFObject(message.Destination(), object);
            // If found, give message to the MMFObject
            if ( KErrNone == status )
                {
		        object->HandleRequest(message);
		        handled = ETrue;
                }
            }

        // No body has handled this message, the request is not supported
		if ( !handled )
			{
			message.Complete(KErrNotSupported);
			}
		}
	}

// -----------------------------------------------------------------------------
// CRadioServerSession::GetCustomInterfaceBuilderL
// -----------------------------------------------------------------------------
//
const TMMFMessageDestination& CRadioServerSession::GetCustomInterfaceBuilderL()
	{
	return iCustomInterfaceBuilder->Handle();
	}

// -----------------------------------------------------------------------------
// CRadioServerSession::MMFObjectContainerL()
// -----------------------------------------------------------------------------
//
CMMFObjectContainer& CRadioServerSession::MMFObjectContainerL()
	{
	// Construct iMMFObjectContainer if we haven't already.
	if ( !iObjectContainer )
	    {
		iObjectContainer = new(ELeave) CMMFObjectContainer;
	    }
	return *iObjectContainer;
	}

//  End of File

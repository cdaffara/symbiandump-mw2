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
* Description:  Representation of single event
*
*/

#include "mdeevent.h"
#include "mdcitem.h"
#include "mdcserializationbuffer.h"
#include "mdeeventdef.h"
#include "mdenamespacedef.h"

CMdEEvent* CMdEEvent::NewL(CMdEEventDef& aDef, TItemId aObjectId, TTime aTime,
			const TDesC* aSource, const TDesC* aParticipant)
	{
	CMdEEvent* self = CMdEEvent::NewLC( aDef, aObjectId, aTime, aSource, aParticipant );
	CleanupStack::Pop( self );
	return self;
	}

CMdEEvent* CMdEEvent::NewLC(CMdEEventDef& aDef, TItemId aObjectId, TTime aTime,
		const TDesC* aSource, const TDesC* aParticipant )
	{
	CMdEEvent* self = new (ELeave) CMdEEvent( NULL, KNoId, aDef, aObjectId, aTime );
	CleanupStack::PushL( self );
	self->ConstructL( aSource, aParticipant );
	return self;
	}


CMdEEvent::CMdEEvent(CMdESession* aSession, TItemId aId, CMdEEventDef& aDef, TItemId aObjectId, TTime aTime)
		: CMdEInstanceItem(aSession, aId), iDef(aDef), iObjectId(aObjectId), iTime(aTime)
	{
	}

void CMdEEvent::ConstructL(const TDesC* aSource, const TDesC* aParticipant)
	{
	InstanceItemBaseConstruct();

	if( aSource )
		{		
		iSource = aSource->AllocL();
		}
	else
		{
		iSource = NULL;
		}

	if( aParticipant )
		{		
		iParticipant = aParticipant->AllocL();
		}
	else
		{
		iParticipant = NULL;
		}
	}

CMdEEvent::~CMdEEvent()
	{
	delete iSource;
	iSource = NULL;
	delete iParticipant;
	iParticipant = NULL;
	}

CMdEEvent* CMdEEvent::NewL(CMdESession* aSession, CMdCSerializationBuffer& aBuffer, CMdENamespaceDef& aNamespaceDef )
	{
	CMdEEvent* ret = CMdEEvent::NewLC( aSession, aBuffer, aNamespaceDef );
	CleanupStack::Pop( ret );
	return ret;
	}

CMdEEvent* CMdEEvent::NewLC(CMdESession* aSession, CMdCSerializationBuffer& aBuffer, CMdENamespaceDef& aNamespaceDef )
	{
	const TMdCEvent& serializedEvent = TMdCEvent::GetFromBufferL( aBuffer );
	if (serializedEvent.iId == KNoId)
		{
		User::Leave( KErrNotFound );
		}
	if (serializedEvent.iDefId == KNoDefId)
		{
		User::Leave( KErrNotFound );
		}

	CMdEEventDef* eventDef = aNamespaceDef.GetEventDefL( serializedEvent.iDefId );
	if ( !eventDef )
		{
		User::Leave( KErrNotFound );
		}
	if ( serializedEvent.iObjectId == KNoId )
		{
		User::Leave( KErrNotFound );
		}

	CMdEEvent* event = new(ELeave) CMdEEvent( aSession, serializedEvent.iId, *eventDef,
			serializedEvent.iObjectId, serializedEvent.iTime );
	CleanupStack::PushL( event );

	TPtrC16 source; 
	TPtrC16 participant;

	TPtrC16* sourcePtr = NULL;
	TPtrC16* participantPtr = NULL;

	if( serializedEvent.iSourceText.iPtr.iCount > 0 )
		{
		aBuffer.PositionL( serializedEvent.iSourceText.iPtr.iOffset );
		source.Set( aBuffer.ReceivePtr16L() );
		sourcePtr = &source;
		}

	if( serializedEvent.iParticipantText.iPtr.iCount > 0 )
		{
		aBuffer.PositionL( serializedEvent.iParticipantText.iPtr.iOffset );
		participant.Set( aBuffer.ReceivePtr16L() );
		participantPtr = &participant;
		}

	event->ConstructL( sourcePtr, participantPtr );
	return event;
	}

EXPORT_C CMdEEventDef& CMdEEvent::Def() const
	{
	return iDef;
	}


EXPORT_C TItemId CMdEEvent::ObjectId() const
	{
	return iObjectId;
	}


EXPORT_C TTime CMdEEvent::Time() const
	{
	return iTime;
	}


EXPORT_C const TDesC* CMdEEvent::Source() const
	{
	return iSource;
	}


EXPORT_C const TDesC* CMdEEvent::Participant() const
	{
	return iParticipant;
	}

TUint32 CMdEEvent::RequiredBufferSize() const
	{
	TUint32 requiredSize = sizeof(TMdCEvent);

	if( iSource )
		{
		requiredSize += CMdCSerializationBuffer::RequiredSize(*iSource);
		}
		
	if( iParticipant )
		{
		requiredSize += CMdCSerializationBuffer::RequiredSize(*iParticipant);
		}

	return requiredSize;
	}

TMdCOffset CMdEEvent::SerializeL(CMdCSerializationBuffer& aBuffer, TMdCOffset aFreespaceOffset) const
	{
	const TMdCOffset eventPosition = aBuffer.Position();
	TMdCEvent event;
	event.iId = Id();
	event.iDefId = iDef.Id();
	event.iObjectId = iObjectId;
	event.iTime = iTime;

	if( iSource )
		{
		event.iSourceText.iPtr.iCount = iSource->Length();
		event.iSourceText.iPtr.iOffset = aFreespaceOffset;
		aBuffer.PositionL( aFreespaceOffset );
		aFreespaceOffset = aBuffer.InsertL( *iSource );
		}
	else
		{
		event.iSourceText.iPtr.iCount = 0;
		event.iSourceText.iPtr.iOffset = KNoOffset;
		}

	if( iParticipant )
		{
		event.iParticipantText.iPtr.iCount = iParticipant->Length();
		event.iParticipantText.iPtr.iOffset = aFreespaceOffset;
		aBuffer.PositionL( aFreespaceOffset );
		aFreespaceOffset = aBuffer.InsertL( *iParticipant );
		}
	else
		{
		event.iParticipantText.iPtr.iCount = 0;
		event.iParticipantText.iPtr.iOffset = KNoOffset;
		}

	aBuffer.PositionL( eventPosition );
	event.SerializeL( aBuffer );

	return aFreespaceOffset;
	}

TMdEInstanceType CMdEEvent::InstanceType() const
	{
	return EMdETypeEvent;
	}

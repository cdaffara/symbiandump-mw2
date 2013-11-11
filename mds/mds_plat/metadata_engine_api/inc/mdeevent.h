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


#ifndef __MDEEVENT_H__
#define __MDEEVENT_H__


#include <mdeinstanceitem.h>

class CMdEEventDef;
class CMdCSerializationBuffer;
class CMdENamespaceDef;

/**
 * Event.
 */
NONSHARABLE_CLASS(CMdEEvent) : public CMdEInstanceItem
	{
public:
	
	/**
	 * Create new event from serialized buffer
	 *
	 * @param aBuffer serialized relation
	 * @return new event
	 */
	static CMdEEvent* NewL(CMdESession* aSession, CMdCSerializationBuffer& aBuffer, CMdENamespaceDef& aNamespaceDef );

	/**
	 * Create new event from serialized buffer and leaves it on stack
	 *
	 * @param aBuffer serialized relation
	 * @return new event
	 */
	static CMdEEvent* NewLC(CMdESession* aSession, CMdCSerializationBuffer& aBuffer, CMdENamespaceDef& aNamespaceDef );

	/**
	 * Create new event from serialized buffer
	 *
	 * @param aDef       Definition of the new event.
	 * @param aObjectId  Identifier of the object that the new event is related to.
	 * @param aTime      Time when the new event occurred.
	 * @param aSource       Source of the new event. Ownership of the pointed object remains on caller.
	 * @param aParticipant  Participant of the new event. Ownership of the pointed object remains on caller.
	 * @return new event
	 */
	static CMdEEvent* NewL(CMdEEventDef& aDef, TItemId aObjectId, TTime aTime,
			const TDesC* aSource, const TDesC* aParticipant);

	/**
	 * Create new event from serialized buffer and leaves it on stack
	 *
	 * @param aDef       Definition of the new event.
	 * @param aObjectId  Identifier of the object that the new event is related to.
	 * @param aTime      Time when the new event occurred.
	 * @param aSource       Source of the new event. Ownership of the pointed object remains on caller.
	 * @param aParticipant  Participant of the new event. Ownership of the pointed object remains on caller.
	 * @return new event
	 */
	static CMdEEvent* NewLC(CMdEEventDef& aDef, TItemId aObjectId, TTime aTime,
			const TDesC* aSource, const TDesC* aParticipant );

	/**
	 * Destructor.
	 */
	virtual ~CMdEEvent();


	/* Methods. */

	/**
	 * Returns the definition of the event.
	 *
	 * @return  Event definition.
	 */
	IMPORT_C CMdEEventDef& Def() const;

	/**
	 * Returns the identifier of the object that the event is related to.
	 *
	 * @return  Object identifier.
	 */
	IMPORT_C TItemId ObjectId() const;

	/**
	 * Returns the time when the event occurred.
	 *
	 * @return  Time.
	 */
	IMPORT_C TTime Time() const;

	/**
	 * Returns the source of the event.
	 *
	 * @return  Source.
	 */
	IMPORT_C const TDesC* Source() const;

	/**
	 * Returns the participant of the event.
	 *
	 * @return  Participant.
	 */
	IMPORT_C const TDesC* Participant() const;

	/**
	 * Calculates required size for buffer to serialize event
	 *
	 * @return necessary buffer size
	 */
	TUint32 RequiredBufferSize() const;

	/**
	 * Serialize event
	 *
	 * @param aBuffer buffer where serialized event should go
	 */
	TMdCOffset SerializeL(CMdCSerializationBuffer& aBuffer, TMdCOffset aFreespaceOffset) const;

	TMdEInstanceType InstanceType() const;

private:
	/**
	 * Constructs a new event.
	 *
	 * @param aDef       Definition of the new event.
	 * @param aObjectId  Identifier of the object that the new event is related to.
	 * @param aTime      Time when the new event occurred.
	 */
	CMdEEvent(CMdESession* aSession, TItemId aId, CMdEEventDef& aDef, TItemId aObjectId, TTime aTime);

	/**
	 * Second-phase constructor.
	 *
	 * @param aSource       Source of the new event. Ownership of the pointed object remains on caller.
	 * @param aParticipant  Participant of the new event. Ownership of the pointed object remains on caller.
	 */
	void ConstructL(const TDesC* aSource, const TDesC* aParticipant);


private:

	/** Event definition. */
	CMdEEventDef& iDef;

	/** Object identifier. */
	TItemId iObjectId;

	/** Time when the event occurred. */
	TTime iTime;

	/** Source descriptor. */
	HBufC* iSource;

	/** Participant descriptor. */
	HBufC* iParticipant;
	};

#endif  // __MDEEVENT_H__

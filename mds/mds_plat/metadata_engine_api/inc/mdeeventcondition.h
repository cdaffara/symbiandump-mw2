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
* Description:  Representation of query event condition
*
*/

#ifndef __MDEEVENTCONDITION_H__
#define __MDEEVENTCONDITION_H__


#include <mdecondition.h>
#include <mderange.h>


/* Forward declarations. */
class CMdELogicCondition;
class CMdEEventDef;
class CMdEEventDef;


/**
 * Query condition for events. 
 */
NONSHARABLE_CLASS(CMdEEventCondition) : public CMdECondition
	{
public:
    
	/* Constructors and destructor. */

	/**
	 * Create new CMdEEventCondition from serialized buffer
	 *
	 * @return new CMdEEventCondition
	 */
	static CMdEEventCondition* NewL( TItemId aEventId = KNoId,
			const CMdEEventDef* aEventDef = NULL,
			const TMdETimeRange* aCreationTimeRange = NULL,
			TEventConditionCompareMethod aCompareMethod  = EEventConditionCompareNone,
            const TDesC* aString = NULL );
	
	/**
	 * Create new CMdEEventCondition from serialized buffer and leaves it on stack
	 *
	 * @return new CMdEEventCondition
	 */
	static CMdEEventCondition* NewLC( TItemId aEventId = KNoId,
			const CMdEEventDef* aEventDef = NULL,
			const TMdETimeRange* aCreationTimeRange = NULL,
			TEventConditionCompareMethod aCompareMethod  = EEventConditionCompareNone,
            const TDesC* aString = NULL );

	/**
	 * Destructor.
	 */
	virtual ~CMdEEventCondition();


	/* Methods. */

    /**
     * Returns the logic condition root node of the conditions defined for
     * the object which the event is attached to.
     *
     * @return  Logic condition.
     */
    IMPORT_C CMdELogicCondition& ObjectConditionsL();

    /**
     * Returns the logic condition root node of the condition defined for
     * the object whose URI matches the source URI of the event.
     */
    IMPORT_C CMdELogicCondition& SourceObjectConditionsL();

    /**
     * Returns the logic condition root node of the condition defined for
     * the object whose URI matches the source URI of the event.
     */
    IMPORT_C CMdELogicCondition& ParticipantObjectConditionsL();

    /**
     * Returns the event ID that the condition matches against.
     *
     * @return  Event ID.
     */
    IMPORT_C TItemId EventId() const;

    /**
     * Returns the event definition that the condition matches against.
     *
     * @return  Event definition. <code>NULL</code>, if not defined.
     */
    IMPORT_C const CMdEEventDef* EventDef() const;
    
    /** 
     * Returns the creation time range of the condition. 
     *
     * @return  Pointer to the time range. <code>NULL</code>, 
     *          if not defined.
     */
    IMPORT_C const TMdETimeRange* TimeRange() const;

    /** 
     * Returns the source URI the condition matches against.
     * 
     * @return  Source URI. <code>NULL</code>, if not defined. 
     */
    IMPORT_C const TDesC* SourceURI() const;

    /** 
     * Returns the participant URI the condition matches against.
     * 
     * @return  Participant URI. <code>NULL</code>, if not defined. 
     */
    IMPORT_C const TDesC* ParticipantURI() const;

    /**
     * Get possible query's internal optimization flags from condition.
     *
     * @param aFlaga query's internal optimization flags
     * @return last level internal optimization flags
     */
    TUint32 InternalQueryOptimizationFlags(TUint32& aFlags);

	/**
     * Get required size of serialized buffer when it is serialized.
     *
     * @return required serialized buffer size
     */
	TUint32 RequiredBufferSize() const;
	
	/**
     * Serialize own data to serialized buffer (correct position must be set 
     * before calling) and return new position of serialized buffer.
	 *
     * @param aBuffer  serialized buffer.
     */
    void SerializeL(CMdCSerializationBuffer& aBuffer,
    		TMdCOffset& aFreespaceOffset) const;

private:

    /* Private methods. */

  	/**
	 * Constructor for a condition that requires that the event ID a 
     * certain ID or event is of a certain type.
     *
     * @param aEventId  Event ID.
     * @param aEventDef  Event definition.
	 */
	CMdEEventCondition(TItemId aEventId, const CMdEEventDef* aEventDef);

    /**
	 * Second-phase constructor. Pass <code>NULL</code> in parameters that
	 * are not needed.
     *
     * @param aCreationTimeRange  Range for the creation time.
     * @param aCompareMethod      Comparison method for the string.
     * @param aString             Text string to match.
	 */
    void ConstructL(const TMdETimeRange* aCreationTimeRange,
                    TEventConditionCompareMethod aCompareMethod = EEventConditionCompareNone, 
                    const TDesC* aString = 0);

private:

	const TItemId iEventId;

    /** Event definition to match. <code>NULL</code>, if not defined. */
    const CMdEEventDef* iEventDef;

    /** Conditions for the object which the event is associated with. */
    CMdELogicCondition* iObjectConditions;

    /** Creation time range. <code>NULL</code>, if not defined. */
    TMdETimeRange* iCreationTimeRange;

    /** Source of participant URI to match. <code>NULL</code>, if not defined. */
    HBufC* iURI;

    /** Conditions for the object whose URI matches the source URI. */
    CMdELogicCondition* iSourceConditions;

    /** Conditions for the object whose URI matches the participant URI. */
    CMdELogicCondition* iParticipantConditions;

	/** Method of comparison. */
	TEventConditionCompareMethod iCompareMethod;
    };

#endif  // __MDEEVENTCONDITION_H__

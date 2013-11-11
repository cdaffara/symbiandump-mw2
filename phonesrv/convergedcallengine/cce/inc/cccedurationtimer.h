/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides call duration timer for PS calls
*
*/


#ifndef CCCEDURATIONTIMER_H
#define CCCEDURATIONTIMER_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class MCCECallObserver;
class MCCEConferenceCallObserver;

// CLASS DECLARATION

/**
*  CCCEDurationTimer call duration class.
*  Used for count call duration
*
*  @lib cce.lib
*  @since Series 60 3.2
*/
NONSHARABLE_CLASS(CCCEDurationTimer) : public CBase, public MBeating
    {
public:

    /**
    * Two-phased constructor.
    */
    static CCCEDurationTimer* NewL();

    /**
    * Destructor
    */
    virtual ~CCCEDurationTimer(); 

public:
    
    /**
     * Set observer
     *
     * @since S60 v3.2
     */
    void SetObserver( MCCECallObserver* aObserver );

    /**
     * Set conference call observer
     *
     * @since S60 v3.2
     */
    void SetConferenceObserver( MCCEConferenceCallObserver* aObserver );

    /**
     * Start timer.
     *
     * @since S60 v3.2
     */
    void Start();
    
    /**
     * Start timer
     *
     * @since S60 v3.2
     */
    void Start(TInt aBeatsOffset);
    
    /**
     * Restart air duration timer from the given value
     *
     * @since S60 v3.2
     * @param aBeatsOffset innitial timer value
     */
    void RestartAirTimeDuration(TInt aBeatsOffset);
    
    /**
     * Start timer
     *
     * @since S60 v3.2
     */
    void StartAirTimeDuration();
    
    /**
     * Call start time
     *
     * @since S60 v3.2
     */
    TDateTime StartTime();  

    /**
     * Stop timer
     *
     * @since S60 v3.2
     * @return True if heart beat runner was started
     */
    TBool Stop();

    /**
     * Returns number of beats from the previous Start of the timer.
     * @since S60 v3.2
     */
    TInt NumberOfBeats() const;
    
    /**
     * Set the number of beats to zero
     * @since S60 v3.2
     */
    void Reset();
    
// from base class MBeating

    /**
     * Called when we need to synchronize
     *
     * @since S60 v3.2
     */
    void Synchronize();

protected:

    /**
     * Default constructor
     */
    CCCEDurationTimer();

    /**
     * 2nd phase of the construction
     */
    void ConstructL();

// from base class MBeating

    /**
     * Called when beat works ok, once every second.
     *
     * @since S60 v3.2
     */
    void Beat();        

private: // data

    /**
     * Pointer to PS Data object. 
     * If this is NULL, timer is used to AirTime duration.
     */
    MCCECallObserver* iObserver;
    
    /**
     * Conference call duration observer.
     */
    MCCEConferenceCallObserver* iConferenceCallObserver;

    /**
     * Timer
     * Own
     */
    CHeartbeat*     iHeartbeat; 

    /**
     * True if timer is running
     */
    TBool           iActive; 

    /**
     * Current timer value
     */
    TInt            iNumberOfBeats;

    /**
     * Timer start time
     */
    TTime           iStartTime;

    /**
     * Is this timer instance used to airtimeduration
     */
    TBool           iAirTimeDuration;
    };

#endif      // CCCEDURATIONTIMER_H

// End of File

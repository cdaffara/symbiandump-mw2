/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef C_CPOSITIONINFO_H
#define C_CPOSITIONINFO_H

#include <lbs.h>
#include <lbssatellite.h>
#include "rlocationtrail.h"

/**
*  An observer interface, which is used for getting current position info.
*
*  @since S60 3.1
*/
class MPositionInfoObserver
    {
public:    
    /**
     * This method is used for setting the network cell id to the 
     * location trail.
     */
    virtual void Position( const TPositionInfo& aPositionInfo, 
                           const TInt aError ) = 0;
    };

/**
 *
 *  @since S60 3.1
 */
class CPositionInfo : public CActive
    {
public:  
    /**
     * 2-phased constructor.
     * @since S60 3.1
     */
    IMPORT_C static CPositionInfo* NewL( MPositionInfoObserver* aTrail );

    /**
     * C++ destructor.
     * @since S60 3.1
     */    
    IMPORT_C virtual ~CPositionInfo();

public:
    /**
     * Start collecing positions.
     * @since S60 3.1
     * @param None.
     * @return None.
     */    
    void StartL( RLocationTrail::TTrailCaptureSetting aCaptureSetting, TInt aUpdateInterval );
    
    /**
     * Get next position.
     * @since S60 3.1
     * @param None.
     * @return None.
     */  
    void NextPosition();   
    
    /**
     * Stop collecing positions.
     * @since S60 3.1
     * @param None.
     * @return None.
     */    
    void Stop();
    
    /**
     * Handle remap completed event
     * @since S60 9.2
     */    
    void HandleRemapComplete();    
protected:
    /**
     * Run error implementation in case of RunL leaving.
     * @since S60 3.1
     */
    TInt RunError( TInt aError );
    
private:
    /**
     * C++ constructor.
     */  
    CPositionInfo( MPositionInfoObserver* aTrail );
    
    /**
     * 2nd phase constructor.
     */
    void ConstructL();
    
private:
    /**
    * From CActive.
    */        
    void DoCancel();
    
    /**
    * From CActive.
    */        
    void RunL(); 

private:
    typedef enum
        {
        EPositionOptStateNone = 0x00,
        EPositionOptStateGetLastKnownPosition,
        EPositionOptStateNotifyUpdate
        }TPositionOptState;
    /**
     * An observer interface to set current position to the location trail.
     * Not own.
     */
    MPositionInfoObserver* iTrail;
    
    /**
     * Position server.
     * Own.
     */
    RPositionServer iPosServer;

    /**
     * Positioner.
     * Own.
     */
    RPositioner iPositioner;

    TPositionSatelliteInfo iPositionInfo; 
    TPositionUpdateOptions iUpdateOptions;
    RLocationTrail::TTrailCaptureSetting   iTrailCaptureSetting;
    
    TInt iUpdateInterval;
    
    TPositionOptState iState;
    TBool iConnectedPositionServer;
    };

#endif // C_CPOSITIONINFO_H

// End of file.

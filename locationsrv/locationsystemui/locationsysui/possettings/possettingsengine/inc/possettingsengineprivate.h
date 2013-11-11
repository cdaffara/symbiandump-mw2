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
* Description:  Positioning settings engine private class. 
*
*/

#ifndef POSITIONINGSETTINGSENGINE_PRIVATE_H
#define POSITIONINGSETTINGSENGINE_PRIVATE_H

#include "possettingscommon.h"

#include <EPos_MPosModulesObserver.h>
#include <lbtserver.h>
#include <lbtmanager.h>

// Forward declarations

class PosSettingsEngine;
class CPosModules;
class CSuplSettings;
struct TPosModulesEvent;

/**
 * The class defines the interface for receiving position
 * technology change event.The class interested in position
 * technology change should inherit from this class.
 */
class PositionTechnologyChangeObserver
    {
public:
    virtual void handlePositionTechnologyChange() = 0;
    };

/**
 * PositioningSettingsEnginePrivate is a class that provides implementation
 * for PositionSettingsEngine class.
 */
class PosSettingsEnginePrivate : public MPosModulesObserver
    {
public:
    /**
     * Default constructor
     */
    PosSettingsEnginePrivate( PositionTechnologyChangeObserver& 
                                      posTechChangeObserver );
    
    /**
     * Destructor 
     */
    ~PosSettingsEnginePrivate();
    
    /**
     * Returns whether specified position technology is available.
     * @param[in] posTechnology Position technology of interest.
     * @return true if specified position technology is available,
     *         false if it is not available.
     */
    bool isPositionTechnologyAvailable( PositionTechnology posTechnology );
    
    /**
     * Sets the state of position technology.
     * @param[in] posTechnologyState Resultant state of the position technology 
     * after this operation.
     * @param[in] posTechnology State of position technology to be changed.
     * @return KErrNone if successful,
     *         KErrNotFound if specified position technology is not available
     *         Otherwise, standard error code is returned,
     *         KErrServerBusy, etc.. 
     */
    int setPositionTechnologyState( State posTechnologyState,
                                    PositionTechnology posTechnology );
    
    /**
     * Returns whether specified position technology is enabled. It is recemonded 
     * to use this method after checking the availability of the position technology
     * using isPositionTechnologyAvailable method.
     * @param[in] posTechnology Position technology of interest.
     * @return true if specified position technology is enabled,
     *         false if it is disabled.
     */
    bool isPositionTechnologyEnabled( PositionTechnology posTechnology );

    
    /**
     * Sets the state of back ground positioning.
     * @param[in] backGroundPosState Resultant state of back ground positioning.
     * @return KErrNone if successful,
     *         KErrNotSupported if none of the position type are available,
     *         KErrArgument if none of the position type are enabled
     *         otherwise, standard error code is returned,
     *         KErrServerBusy, etc.. 
     */
    int setBackGroundPositioningState( State backGroundPosState );    
    
    /**
     * Returns whether back ground positioning is enabled. It is recemonded 
     * to use this method after checking the availability of the position type
     * using isPositionTypeAvailable method.
     * @return true if back ground positioning is enabled,
     *         false if it is disabled.
     */
    bool isBackGroundPositioningEnabled();
    
public: // from MPosModulesObserver
    virtual void HandleSettingsChangeL( TPosModulesEvent aEvent );
    
private:
	  /**
     * Sets the state of given positioning technology.
     * @param[in] posTechnologyState Resultant State of Positioning Technology 
     * StateEnable/StateDisable.
     * @param[in] posTechnology Positioning Technology Name 
     * PositionTechnologyGps/PositionTechnologyNetwork.
     */
    void setPositionTechnologyStateL( State posTechnologyState,
                                PositionTechnology posTechnology );
    /**
     * Sets the state of back ground positioning.
     * @param[in] backGroundPosState Resultant state of back ground positioning.
     */
    void setBackGroundPositioningStateL( State backGroundPosState );
    
    /**
     * Populates the PSY module information list.
     */
    void populatePsyModuleInfoListL();
    
    /**
     * Gets the state of back ground positioning.
     */
    void GetBackGroundPositioingStateL();

private: //Data members
    
    /**
     * Reference to position technology change observer.
     */
    PositionTechnologyChangeObserver& mPosTechChangeObserver;

    /**
     * Pointer to Supl settings object
     * Own
     */
    CSuplSettings* mSuplSettings;
    
    /**
     * Pointer to lbt server
     * Own
     */
    RLbtServer mLbtServer;
    
    /**
     * Pointer to lbt manager
     * Own
     */
    RLbtManager mLbtManager;
    
    /**
     * Pointer to Pos module;
     * Own
     */
    CPosModules* mPosModules;
    
    /**
     * List of PSY module info
     * Own
     */
    RArray<PsyModuleInfo> mPsyModuleInfoList;
    
    /**
     * State of back ground positioning
     */
    State mBackGroundPositioningState;
    
    friend class PosSettingsEngine;
    };


#endif /* POSITIONINGSETTINGSENGINE_PRIVATE_H */

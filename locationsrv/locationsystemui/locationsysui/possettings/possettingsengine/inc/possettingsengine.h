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
* Description:  Implementation of positioning settings engine class. 
*
*/

#ifndef POSITIONINGSETTINGS_ENGINE_H
#define POSITIONINGSETTINGS_ENGINE_H


#include "possettingscommon.h"
#include "possettingsengineprivate.h"
#include <QObject>
#include <qglobal.h>

/**
 * PositioningSettingsEngine class provides an interface that abstracts client
 * from different GPS and wireless positioning techniques. This exposes methods 
 * to perform operations on the positioning techniques.
 */

class PosSettingsEngine : public QObject,
                               public PositionTechnologyChangeObserver
    {
    Q_OBJECT
public:
    /**
     * Default constructor
     * This throws an error if construction fails.
     */
    PosSettingsEngine();
    
    /**
     * Destructor 
     */
    ~PosSettingsEngine();
    
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
    
    /**
     * It will set status of background positioning in CR key
     * It is required to retain old status when checkbox is inserted/removed
     * 
     * status [in] - The status of the background positioning in CR key
     */
    void setCRBackgroundPositioningStatus(State backGroundPosState);
  
   /**
    * It will return status of background positioning in CR key
    * It is required to retain old status when checkbox is inserted/removed.
    * 
    * returns true if enabled or false if disabled
    */
    bool cRBackgroundPositioningStatus();

public: // from PositionTechnologyChangeObserver
    void handlePositionTechnologyChange();

private:
	  signals:
    /**
     * This signal is emitted when there is change in any of position
     * technology wrt to its availabilty, state etc.. 
     */
    void positionTechnologyChange();
    
private:      
    Q_DECLARE_PRIVATE_D(d_ptr, PosSettingsEngine)
private: // data member
    /**
     * Pointer to private implementation.
     * Own
     */
    PosSettingsEnginePrivate* d_ptr; 
    };

#endif /* POSITIONINGSETTINGS_ENGINE_H */

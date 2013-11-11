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
 * Description: Declaration of the Positioning Methods private engine class
 *							used to get the list of available Psy & to manipulate 
 *							the state of the Psy by using the location framework apis.
 *							
 *
 */

#ifndef POSPSYSETTINGSENGINEPRIVATE_H
#define POSPSYSETTINGSENGINEPRIVATE_H

#include "pospsysettingscommon.h"

#include <qlist.h>
#include <qstring.h>
#include <EPos_MPosModulesObserver.h>

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


// Forward declarations
class CPosModules;
class PosPsySettingsEngine;

class PosPsySettingsEnginePrivate : public MPosModulesObserver
    {
public:
    /**
     * Constructor
     */
    PosPsySettingsEnginePrivate( PositionTechnologyChangeObserver& 
            posTechChangeObserver);

    /**
     * Destructor
     */

    ~PosPsySettingsEnginePrivate();

    /**
     * This method lists all the PSY that are available
     * @param[out] psyList - Contains the list of psys that are available
     * @return - KErrNone - if successfully finds any Psy
     *           KErrNotFound - if no Psy available.
     */
    int listPsys( QList<PosPsyInfo>& psyList );

    /**
     * This method is used to change the state of the Psy based on the user's
     * action of either enabling/disabling the Psy.
     * @param[in] psyModuleId - the uid of the Psy whose state is to be changed.
     * @param[in] psyState - The value of state to which the Psy has to be changed.
     */

    int changePsyState( TUid psyModuleId, PosPsyState psyState );
    
public: // from MPosModulesObserver
    virtual void HandleSettingsChangeL( TPosModulesEvent aEvent );

private:
    /**
     * This method uses the location framework apis to get the list of available psy
     */
    void listPsysL();
    /**
     * This method uses the location framework apis to manipulate the state of the Psy
     */
    void changePsyStateL( TUid psyModuleId, PosPsyState psyState );
private :
    /**
     * Pointer to CPosModules
     * Owns
     */
    CPosModules* mPosModules;
    
    /**
     * Contains list of PSY info
     * Own
     */
    RArray<PosPsyInfo> mPsyInfoList;
    /**
     * Reference to position technology change observer.
     */
    PositionTechnologyChangeObserver& mPosTechChangeObserver;
    /*
     * POinter to the name of the psy
     * ToDo: Should be removed
     */
    QList<QString*> mNameList;
    
    friend class PosPsySettingsEngine;
    };

#endif /* POSPSYSETTINGSENGINEPRIVATE_H */

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
* Description:  Positioning settings engine private class header.
*
*/

#ifndef POSSUPLSETTINGSENGINEPRIVATE_H
#define POSSUPLSETTINGSENGINEPRIVATE_H

#include "possuplsettingscommon.h"
#include <QList>
#include <epos_msuplsettingsobserver.h>

class CServerParams;
class CSuplSettings;

//Constants
const int KMaxHSLPAddrLen = 256;
const int KMaxIAPLen = 256;
/**
 * The class defines the interface for receiving SUPL settings
 *  change event.
 */
class PosSuplSettingsChangeObserver
    {
public:
     virtual void handleSuplSettingsChange( MSuplSettingsObserver::TSuplSettingsEventType aEvent,int aSlpId) = 0;
    };


/**
 * The class defines the interface for listing,enabling\disabling and deleting
 * SLP servers.
 */
class PosSuplSettingsEnginePrivate : public MSuplSettingsObserver
    {
public:
    /**
     * Default constructor
     */
	PosSuplSettingsEnginePrivate();

    /**
     * Destructor
     */
    ~PosSuplSettingsEnginePrivate();

    /**
     * Returns properties of all servers from SUPL settings storage..
     * @[OUT] paramValues Array of retrieved server entries
     *         - KErrNone if SUPL settings are retrieved successfully.
     *         - KErrNotFound if the settings does not have any server entry
     */
    int GetServerList( QList<PosServerParams>& aParamValues );

     /**
     * This method is used to change parameter which indicates whether server
     * can be used for SUPL session or not.
     *
     * @param [IN] slpId SLP ID for which the parameter needs to be changed
     * @param [IN] enableFlag Flag to indicate whether server can be enabled or not
     * @return one of the following error codes:
     *         - KErrNone if SUPL settings are changed successfully.
     *         - KErrArgument if the SLP ID passed is not valid
     *         - KErrGeneral if changing SUPL settings has failed
     */
    int  SetServerStatus( int aSlpId,
                          bool aEnableFlg
    					);


    /**
     * This method is deletes server entry from SUPL settings storage.
     *
     * @param [IN] slpId Server identity for which the entery needs to be deleted from the SUPL settings storage
     *
     * @return one of the following error codes:
     *         - KErrNone if entry is deleted successfully
     *         - KErrNotFound if the specified server identity does not exists in settings storage
     *         - KErrGeneral if changing SUPL settings has failed
     *
     */
    int RemoveServer( int aSlpId);

    /**
     * Returns list all servers from SUPL settings storage that can be deleted.Operator configured servers will be omitted from
     * this list.
     * @[OUT] paramValues Array of retrieved server entries
     *         - KErrNone if SUPL settings are retrieved successfully.
     *         - KErrNotFound if the list does not have any server entry
     */
    int GetRemovableServerList( QList<PosServerParams>& aParamValues );

       /* @param [IN] observer The observing object.
	    * @return one of the following values
	    * 	- KErrNone if setting observer is successful
	    * 	- KErrAlreadyExists if observer is already set
	    *
        */
    int SetObserver( PosSuplSettingsChangeObserver* aObserver );


public:
    void HandleSuplSettingsChangeL( TSuplSettingsEventType aEvent,TInt aSlpId );
    void HandleSuplTriggerStatusChangeL( CSuplSettings::TSuplTriggerStatus aSuplTriggerStatus );


private: //Data members

    /**
     * Reference to position technology change observer.
     * Does not own
     */
    PosSuplSettingsChangeObserver* mPosSuplSettingsObserver;

    /**
     * Pointer to Supl settings object
     * Owns
     */
    CSuplSettings* mSuplSettings;

    /**
     * Pointer to Supl Server List
     * Owns
     */
    RPointerArray<CServerParams> mServerList;
    };



#endif /* POSSUPLSETTINGSENGINEPRIVATE_H */

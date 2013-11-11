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
 * Description: This is the engine header for SUPL Positioning Settings plugin 
 *
 */

#ifndef POSSUPLSETTINGS_ENGINE_H
#define POSSUPLSETTINGS_ENGINE_H


//User Includes
#include "possuplsettingsengineprivate.h"
#include "possuplsettingscommon.h"

#include <e32base.h>
#include <QObject>
#include <qstringlist.h>
#include <epos_csuplsettingparams.h>

//Forward Declarations
class PosSuplSettingsEnginePrivate;
class MSuplSettingsObserver;

class PosSuplSettingsEngine : public QObject, public PosSuplSettingsChangeObserver
    {
Q_OBJECT

public:

    /**
     * Constructor 
     */
    PosSuplSettingsEngine();

    /**
     * Destructor
     */
    ~PosSuplSettingsEngine();

    /**
     * This method Lists all the Server that are present for Positioning.
     * @param [out] - aServerList. Contains the servers that are present.
     * @returns - KErrNone if successful
     *          - KErrNotfound is no servers are present
     */

    int suplServerList(QList<PosServerParams>& serverList);
    
    /**
     * This method Lists all the Server that are present for Positioning.
     * @param [out] - aServerList. Contains the servers that are present.
     * @returns - KErrNone if successful
     *          - KErrNotfound is no servers are present
     */

    int deletableServerList(QList<PosServerParams>& serverList);
    

    /**
     * This method enables/disables the server in use for postioning.
     * If the server is disabled, it will not be used for positioning unless enabled.
     * @param [in] - aServerIndex. The server index which has to be enabled/disabled.
     * @param [in] - aValue. true/false.
     * @returns - KErrNone if enabling/disabling is successful
     *          - KErrArgument if the SLP ID passed is not valid/out of bounds
     *          - KErrGeneral if changing SUPL settings has failed
     */
    int enableServer(int aServerIndex, bool aValue);

    /**
     * This method is used to delete the server.
     * @param [in] - aServerIndex. The server index which has to be deleted.
     * @returns - KErrNone if the deletion is successful
     *          - KErrNotFound if the server at the index in missing/out of bounds.
     *          - KErrPermissionDenied if the sever cannot be deleted
     *          - KErrGeneral is other cases
     */
    int deleteServer(int aServerIndex);


public:

    /** 
     * Inherited from PosSuplSettingsChangeObserver
     */
    void handleSuplSettingsChange( MSuplSettingsObserver::TSuplSettingsEventType event,int slpId);
signals:
    /**
     * This signal is emitted when the server is deleted/added.
     */
    void serverListUpdated();

private:
    /**
     * pointer to private implementation class
     */
     Q_DECLARE_PRIVATE_D(d_ptr, PosSuplSettingsEngine)
    PosSuplSettingsEnginePrivate * d_ptr;
    };
#endif // POSSUPLSETTINGS_ENGINE_H

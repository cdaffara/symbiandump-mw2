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
 * Description: This is for SUPL Positioning Settings plugin 
 *							for Advanced View
 *
 */

#ifndef POSSUPLSETTINGS_PLUGIN_H
#define POSSUPLSETTINGS_PLUGIN_H

//User Includes
#include "possuplsettingscommon.h"
#include "possettingssignalmapper.h"

#include <QObject>
#include <hbdataformmodelitem.h>
#include <possettingsadvinterface.h>
#include <qsignalmapper.h>
#include <hbdataformviewitem.h>

//Forward Declaration
class PosSettingsAdvOp;
class PosSuplSettingsEngine;
class QModelIndex;
class HbRadioButtonList;
class HbDialog;

class PosSuplSettingsPlugin : public QObject, public PosSettingsAdvInterface

    {
Q_OBJECT
    Q_INTERFACES( PosSettingsAdvInterface )
public:
    /**
     * Constructor
     */
    PosSuplSettingsPlugin();

    /**
     * Destructor
     */
    ~PosSuplSettingsPlugin();
public:
    /*
     * Derived from PosSettingsAdvInterface   
     */
    virtual int initialise(PosSettingsAdvOp* operation);

private:
    /**
     * This method is used to list all the server that are present for 
     * positioing. Depending on whether the server is enabled/disabled for positioning
     * the corresponding checkboxes are marked.
     */
    void listServers();
    
    /**
     * This method creates menu to delete the servers
     */
    void createMenu();

private slots:
    /**
     * This is used as a slot whenever the checkbox is marked/unmarked.
     * On changing the state of the checkbox the corresponding positioning
     * server is enabled/disabled
     * @param [in] - slpid for the server to be enababled/disabled
     */
    void changeServerUsage(int slpid);

    /**
     * This is slot, which is called whenever there is a new server added or 
     * a server in the list is deleted. This refreshes the view.
     */
    void updateList();

    /**
     * This is slot, which is called when the user selects the   
     * server to be deleted
     */
    void onDeleteServer();
    
    /**
     * This slot is used to enable the primary Action wheneven 
     * a server is selected from the list of servers to be deleted
     * @param [in] - index of the server to be deleted
     */
    void setPrimaryAction(int itemSelected);

    /**
     * This slot is used to delete the server whenever
     * a server is selected from the list of servers to be deleted
     */
    void deleteServer();
    
private:

    /**
     * an object if the supl setting engine class
     * Owns
     */
    PosSuplSettingsEngine* mSuplSettingsEngine;

    /**
     * an object of data form model item class
     * Owns
     */
    HbDataFormModelItem * mGroup;

    /**
     * Holds the params of the list of servers
     * Owns
     */
    QList<PosServerParams> mServerList;

    /**
     * hold the signal mapper objects of PosPsySettingsSignalMapper
     * Owns
     */
    QList<PosSettingsSignalMapper*> mSignalMapperList;

    /**
     * Handle to PosSettingsAdvOp object
     * Does not own
     */
    PosSettingsAdvOp * mOperation;

    /**
     * an object of QSignalMapper
     * Owns 
     */
    QSignalMapper * mSignalMapper;
    
    /**
     * an object on HbMenu
     * Owns
     */
    HbMenu* mMainMenu;
    
    /**
     * list of servers that can be deleted
     * Owns
     */    
    QList<PosServerParams> mDeleteableServerList;
    
    /**
     * an instance of HBAction, used for SK1 in 
     * the delete server Dialog box
     * Owns
     */
    HbAction* mPrimaryAction;
    
    /**
     * an instance of HbRadioButtonList, used to add server 
     * in the delete server selection Dialog box
     * Owns
     */
    HbRadioButtonList *mRadioButtonList;
    
    /**
     * holds the server delete query dialog
	 * Owns
     */
	HbDialog *mDeleteQuery;
    };
#endif // POSSUPLSETTINGS_PLUGIN_H

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
 * Description: This is for Location Based Servies Settings plugin 
 *							for Advanced View
 *
 */

#ifndef POSLOCATIONSERVICES_PLUGIN_H
#define POSLOCATIONSERVICES_PLUGIN_H

#include "poslocationservicesengine.h"

#include <QObject>
#include <hbdataformmodelitem.h>
#include <possettingsadvinterface.h>
#include <hbdataformviewitem.h>
#include <qsignalmapper.h>


//Forward Declaration
class PosSettingsAdvOp;
class PosSettingsSignalMapper;
class HbMessageBox;
class HbAction;

class PosLocationServices : public QObject, public PosSettingsAdvInterface
    {
Q_OBJECT
    Q_INTERFACES( PosSettingsAdvInterface)
public:
    /**
     * Constructor
     */
    PosLocationServices();

    /**
     * Destructor
     */
    ~PosLocationServices();
public:
    /*
     * Derived from PosSettingsAdvInterface   
     */
    virtual int initialise(PosSettingsAdvOp* operation);

private:
    /**
     * This method is used to list all the location services that are running
     */
    void listServices();
    
    /**
     * This method constructs the string for update interval to be displayed in
     * the second line of the widget.
     * @param [in]- updateInterval, update interval is seconds, gets from the engine
     * @param [out]- updateIntervalString, string to be displayed, depending on the update
     * interval is in day, hour, minute or second
     */
    void getUpdateIntervalString(int updateInterval, QString& updateIntervalString);

private slots:
    /**
     * This is slot, which is called when the user selects the   
     * service to be deleted
     */
    void stopService(int serviceId);

    /**
     * This is slot, which is called whenever there is a new location service added or 
     * a server in the list is stopped. This refreshes the view.
     */
    void updateList();
    
 	/**
     * This slot is used to stop sessions.
     * @param[in] action - Button which is pressed on messagebox dialog
     */
    void stopSession(HbAction* action);

private:

    /**
     * an object if the location services setting engine class
     */
    PosLocationServicesEngine* mLocationServiceEngine;

    /**
     * an object of data form model item class
     */
    HbDataFormModelItem * mGroup;

    /**
     * Holds the params of the list of location services
     */
    QList<PosLocationServiceInfo> mServiceList;

    /**
     * Handle to PosSettingsAdvOp object
     */
    PosSettingsAdvOp * mOperation;
    
    /**
     * an object of QSignalMapper 
     */
    QSignalMapper * mSignalMapper;
    
    /**
     * hold the signal mapper objects of PosPsySettingsSignalMapper
     */
    QList<PosSettingsSignalMapper*> mSignalMapperList;
    

    
    /**
     * holds the stop service query dialog
	 * Owns
     */
    HbMessageBox * mStopQuery;
    
    /**
     * holds service id to be stop
     */
    int mServiceId;
    };
#endif // POSLOCATIONSERVICES_PLUGIN_H

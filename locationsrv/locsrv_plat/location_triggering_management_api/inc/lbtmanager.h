/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  location triggering manager client library
*
*/



#ifndef LBTMANAGER_H
#define LBTMANAGER_H

#include <lbtcommon.h>
#include <lbtserver.h>
#include <lbt.h>
#include <lbttriggerentry.h>
#include <lbttriggeringsystemmanagementsettings.h>
#include <lbttriggeringsystemstatus.h>
#include <lbttriggerchangeevent.h>
#include <lbttriggermanageruiinfo.h>

class CLbtTriggerFilterBase;
class CLbtTriggerInfo;
class CLbtListTriggerOptions;
class CLbtClientRequester;
class CLbtSubSessnPtrHolder;

/**
 * This class provides interface to manage Location Triggering service.
 * 
 * RLbtManager is derived from RLbt. The Open method of RLbt is overwritten
 * in RLbtManager. RLbtManager::Open() will open a subsession to Location
 * Triggering Server. The subession can be used to manage triggering service.
 * Further function calls of RLbtManager through the management subsession 
 * may behave differently than those defined in the base class RLbt. 
 *
 * When the subsession is opened with RLbtManager::Open(), the methods
 * derived from RLbt apply to all triggers in the system. Client application
 * can list, modify and delete any trigger in the system even they are not
 * owned by the client application. @p ReadDeviceData capability is required 
 * to list triggers. @p WriteDeviceData capability is required to update or 
 * delete triggers requires.
 *
 * The following methods defined in RLbt operate on all triggers in 
 * the system when the subsession is opened with RLbtManager::Open().
 *
 * - DeleteTriggerL()
 * - DeleteTriggersL()
 * - DeleteTriggers()
 * - GetTriggerLC()
 * - UpdateTriggerL()
 * - SetTriggerStateL()
 * - SetTriggersStateL()
 * - SetTriggersState()
 * - ListTriggerIdsL()
 * - ListTriggerIds()
 * - GetTriggersL()
 * - GetTriggers()
 * - CreateGetTriggerIteratorL()
 * - CreateGetTriggerIterator()
 * - GetNextTriggerLC()
 * - NotifyTriggerChangeEvent()
 * - CancelNotifyTriggerChangeEvent()
 *
 * Client application of this API can only change trigger state attribute 
 * if the trigger is not owned by the client application.
 *
 * When the subsession is opened with RLbtManager::Open(), client application
 * can pass a TLbtTriggeringSystemManagementSettings type of variable as
 * output parameter to GetTriggeringSystemSettingL() to get triggering
 * management system settings. Function NotifyTriggeringSystemSettingChange() is
 * used to get change event of any setting in triggering system management 
 * settings is changed. 
 * 
 * Following code shows how to get triggering system management 
 * settings.
 *
 * @code
 *  //Connect to Location Triggering Server and open 
 *  //a subsession with RLbtManager::Open
 *  TLbtTriggeringSystemManagementSettings settings;
 *  lbtManager.GetTriggeringSystemSettingL(settings);
 * @endcode
 *
 * The following methods behave the same way as defined in RLbt even if the 
 * subsession is opened with RLbtManager::Open()
 *
 * - GetFiredStartupTriggersL().
 * - CreateTrigger() 
 * - CancelCreateTrigger() 
 * - NotifyTriggerFired().
 * - CancelNotifyTriggerFired()
 *
 * RLbtManager provides methods to set triggering
 * system management settings and to get and monitor triggering system status. 
 * 
 * Client application needs read or write device data capabilities to use services 
 * provided by this interface.
 * 
 * Before using any of these services, a connection to Location Triggering
 * Server must first be made.
 * 
 * @see RLbt
 *
 * @lib lbtmanager.lib
 * @since S60 5.1
 */
class RLbtManager : public RLbt
    {
public:
    /**
     * Opens a subsession with Location Triggering Server.
     *
     * This method overwrites the Open method in RLbt. It opens a subsession 
     * to manage location triggering service. A subsession must be opened before 
     * the management service can be used.
     * 
     * @panic LocTriggering ELbtServerBadHandle If a session to Location 
     * Triggering Server has not been connected.
     *
     * @param[in] aServer Reference to the Location Triggering Server 
     * session.
     *
     * @return KErrNone if successful. KErrNotSupported if Location Triggers
     * Server is not present in the system. Otherwise, Symbian standard 
     * error code( such as KErrNoMemory, KErrServerBusy, etc.) is returned.
     */
    IMPORT_C TInt Open( RLbtServer& aServer );

    /**
     * Sets triggering system management settings.
     * 
     * This method is used by client application to set triggering
     * system management settings. If the following value is used,
     * the corresponding setting will be reset to the system 
     * default value.
     * 
     * - minimum trigger area size is set to zero
     * - minimum location update interval is set to zero
     * - minimum location update interval when GPS fails is set to zero
     * - maximum user speed is set to zero
     * - the used positioning technology is set to KNullUid
     *
     * This function requires @p WriteDeviceData capability. 
     * 
     * @panic LocTriggering ELbtServerBadHandle If the subsession is not opened.
     *
     * @param[in] aSettings The triggering system management settings to be set in
     * Location Triggering Server.
     * @leave KErrPermissionDenied if the client application does not 
     * have enough capabilities.
     */
    IMPORT_C void SetTriggeringSystemSettingsL(
        const TLbtTriggeringSystemManagementSettings& aSettings,TLbtManagementSettingsMask aMask);

    /**
     * Sets triggering mechanism state on or off.
     *
     * When the triggering mechanism state is switched off, 
     * no trigger in the system will be supervised. Location
     * Triggering Server will release resources used for triggering
     * supervision and will not make new location request. 
     *
     * This function requires @p WriteDeviceData capability. 
     * 
     * @panic LocTriggering ELbtServerBadHandle If the subsession is not opened.
     *
     * @param[in] aState The new state of triggering mechanism state. 
     * @leave KErrPermissionDenied if the client application does not 
     * have enough capabilities.
     */
    IMPORT_C void SetTriggeringMechanismStateL(
        TLbtTriggeringMechanismState aState);

    /**
     * Gets triggering status.
     *
     * This function requires @p ReadDeviceData capability. 
     *
     * @panic LocTriggering ELbtServerBadHandle If the subsession is not opened.
     *
     * @param[out] aTriggeringSystemStatus On return, contains the system 
     * triggering status.
     * @leave KErrPermissionDenied if the client application does not 
     * have enough capabilities.
     */
    IMPORT_C void GetTriggeringSystemStatusL(
        TLbtTriggeringSystemStatus& aTriggeringSystemStatus);

    /**
     * Requests triggering status change event.
     *
     * This function is asynchronous and it will complete the request status
     * when system triggering status is changed. Client application can 
     * get new triggering status from method
     * GetTriggeringSystemStatusL(). Client application shall
     * call this function again to get further change event.
     *
     * Event listening can be cancelled by calling
     * CancelNotifyTriggeringSystemStatusChange().
     *
     * This function requires @p ReadDeviceData capability. 
     *
     * @panic LocTriggering ELbtServerBadHandle If the subsession is not opened.
     * @panic LocTriggering ELbtDuplicateRequest If the client application has
     * already an outstanding NotifyTriggeringSystemStatusChange() request.
     *
     * @param[out] aStatus Will be completed with @p KErrNone if an event occurs.
     *   aStatus will be completed with KErrPermissionDenied if client application
     *   does not have enough capability.
     */
    IMPORT_C void NotifyTriggeringSystemStatusChange(
        TRequestStatus& aStatus);

    /**
     * Cancels notification request of system triggering status change event.
     *
     * This function does not require any capabilities. 
     *
     * @see NotifyTriggeringSystemStatusChange
     */
    IMPORT_C void CancelNotifyTriggeringSystemStatusChange();
    
    /**
     * Listens for the change event of triggering system settings.
     *
     * This function is asynchronous and it will complete the 
     * request status when triggering system settings are changed.
     * Client applications can get detailed information of triggering 
     * system setting from method GetTriggeringSystemSettingL(). 
     * Client application shall call this function again to get 
     * further change event.
     *
     * Event listening can be cancelled by calling
     * CancelNotifyTriggeringSystemSettingChange().
     *
     * This function requires @p ReadUserData capability. 
     *
     * @panic LocTriggering ELbtServerBadHandle If the subsession is not opened.
     * @panic LocTriggering ELbtDuplicateRequest If the subsession has
     * already an outstanding NotifyTriggeringSystemSettingChange() 
     * request.
     *
     * @param[out] aManagementSettings On return contains the new triggering
     * system management settings.
     * @param[out] aStatus Will be completed with @p KErrNone if an 
     * event occurs and an error code( for example, KErrServerBusy, etc.) if 
     * some error was encountered. 
     * aStatus will be completed with KErrPermissionDenied if the client 
     * application does not have enough capability.
     */
    IMPORT_C void NotifyTriggeringSystemSettingChange( 
        TLbtTriggeringSystemManagementSettings& aManagementSettings,
        TRequestStatus& aStatus );
         
    /**
     * List all manager UIs that has triggers associated with.
     *
     * @param[out] aManagerUiList On returns, contains information of 
     * manager UIs that has triggers associated with. The information includes
     * the UID of manager UIs, number of enabled, disabled, and invalid triggers etc.
     */
    IMPORT_C void GetTriggerManagerUiInfoL(
        RArray < TLbtTriggerManagerUiInfo > &aManagerUiList );
    
    /**
     * Gets triggering system setting.
     *
     * This method is used by the management application to get triggering
     * system settings.
     *
     * This function requires @p ReadUserData capability. 
     *
     * @panic LocTriggering ELbtServerBadHandle If the subsession is not opened.
     *
     * @param[out] aSetting reference to TLbtTriggeringSystemManagementSettings object
     * which is derived from TLbtTriggeringSystemSettings 
     * and on return contains triggering system settings.
     *
     * @leave KErrPermissionDenied if the client application does not 
     * have enough capabilities to retrieve the settings.
     */    
    IMPORT_C void GetTriggeringSystemSettingsL( 
        TLbtTriggeringSystemSettings& aSetting );    
    
    /** 
     * Default constructor
     */
    IMPORT_C RLbtManager();
    
    /**
     * Destructor
     */ 
    IMPORT_C ~RLbtManager();    
    
private:
    /**
     * Subsession pointer holder
     * Own.
     */  	
    CLbtSubSessnPtrHolder* iPtrHolder;  
    
    /**
     * Management setting mask
     */
    TLbtManagementSettingsMask iMask;
    };


#endif // LBTMANAGER_H

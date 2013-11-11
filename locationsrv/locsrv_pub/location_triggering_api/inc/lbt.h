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
* Description:  location triggering server client interface
*
*/



#ifndef LBT_H
#define LBT_H

#include <e32std.h>
#include <lbs.h>
#include <lbtserver.h>
#include <lbttriggerentry.h>
#include <lbtcommon.h>
#include <lbttriggerchangeevent.h>
#include <lbttriggeringsystemsettings.h>
#include <lbtlisttriggeroptions.h>


class CLbtTriggerEntry;
class CLbtTriggerFilterBase;
class CLbtListTriggerOptions;
class CLbtTriggerInfo;
class CLbtClientRequester;
class CLbtSubSessnPtrHolder;
struct TLbtTriggerCreationInfo;
struct TLbtTriggerUpdationInfo;
struct TLbtTriggerStateInfo;


/**
 * A handle to Location Triggering Server subsession. This class provides 
 * methods to use location triggering service from Location Triggering 
 * Server.
 * 
 * RLbt is used to create subsession with Location Triggering Server for the 
 * purpose of using the location triggering service. This class provides 
 * mechanisms for creating, listing, modifying and deleting trigger entries in 
 * Location Triggering Server. Besides, there are also methods to get 
 * trigger change and system settings change events, and session trigger 
 * firing event. It also provides method for getting
 * location triggering related system settings. 
 * 
 * Before using any of these services, a connection to Location Triggering
 * Server must first be made.
 *
 * A client can have multiple sessions connected to the Location Triggering 
 * Server. There can be multiple subsessions opened from one session. 
 * Triggers created from one subsession can be accessed from other 
 * subsessions within the same process. Trigger change event, trigger
 * firing event and triggering system settings change event are 
 * send to all subsessions that have issued notification
 * requests to Location Triggering Server.
 *
 * Client must not issue a notification request while there is 
 * a same request still outstanding. An attempt to do so will generate a
 * panic with code ELbtDuplicateRequest in category "LocTriggering". This applies 
 * to the following functions.
 * 
 * - NotifyTriggerChangeEvent
 * - NotifyTriggerFired
 * - NotifyTriggeringSystemSettingChange
 *
 * Client may get error code KErrInUse if it tries to read, write or delete a 
 * trigger while the previous write or delete operation is not completed yet. 
 *
 * @see RLbtServer
 * 
 * @lib lbt.lib
 *
 * @since S60 5.1
 */
class RLbt : public RSubSessionBase
    {
    public:
       /**
        * Opens a subsession with Location Triggering Server.
        *
        * A subsession must be opened before any other service can be used.
        * 
        * @panic LocTriggering ELbtServerBadHandle If a session to Location 
        * Triggering Server has not been connected.
        *
        * @param[in] aServer Reference to the Location Triggering Server 
        * session.
        *
        * @return KErrNone if successful. Otherwise, Symbian standard 
        * error code is returned, such as KErrNoMemory, KErrServerBusy, etc.
        */
        IMPORT_C TInt Open( RLbtServer& aServer );

       /**
        * Connect and open a subsession with Location Triggering Server.
        *
        * Note, this function will connect and create a session to Location
        * Triggering Server. Client application shall avoid unnecesary
        * session connection to Location Triggering Server. Whenever
        * possible, client applicaiton shall reuse same session to
        * open a subsession. 
        *
        * @panic LocTriggering ELbtServerBadHandle If a session to Location 
        * Triggering Server has not been connected.
        *
        * @return KErrNone if successful. Otherwise, Symbian standard 
        * error code is returned, such as KErrNoMemory, KErrServerBusy, etc.
        */
        IMPORT_C TInt Open();
    
       /**
        * Closes the subsession with Location Triggering Server.
        *
        * Close() must be called when RLbt subsession is no longer required. 
        * 
        * Before a subsession is closed, the client application must ensure
        * that all outstanding notification requests have been cancelled. In
        * particular, the application must issue all the appropriate Cancel 
        * requests and then wait for a confirmation that the notification has 
        * been terminated. A failure to do so results in a panic.
        *
        * When the subsession is closed, all the session triggers owned by 
        * the client application are deleted by Location Triggering Server. 
        * Start-up triggers are not affected by this method. 
        * 
        * @panic LocTriggering ELbtRequestsNotCancelled If client application 
        * has requests outstanding with Location Triggering Server.
        */
        IMPORT_C void Close();
    
       /**
        * Creates a trigger in Location Triggering Server and returns the 
        * trigger Id.
        *
        * Client application may use this method to create a trigger in 
        * Location Triggering Server. When a trigger is created, the process 
        * of the client application becomes the owner process of the trigger.
        *
        * Trigger entry shall be a subclass of CLbtTriggerEntry.
        *
        * Start-up triggers are stored persistently. They can be deleted
        * by method RLbt::DeleteTriggerL(). Session triggers remain 
        * until DeleteTriggerL() is called or the client's subsession is 
        * closed. 
        *
        * While creating a trigger, the following attributes are mandatory 
        * for any type of trigger,
        * - Name
        * - Requestors
        * - Trigger condition
        *
        * In case of start-up trigger, the following attribute is 
        * also mandatory
        * - Process Identity
        *
        * Although manager UI is not a mandatory attribute, it's highly 
        * recommended that correct manager UI is specified. 
        *
        * Currently, the system only supports CLbtTriggerConditionArea
        * to be used as trigger condition. Following 
        * attributes must be specified,
        * - Trigger area 
        * - Direction
        *
        * Currently, only CLbtGeoCircle can be used as trigger area. The
        * center of the geographical circle must be specified. 
        * 
        * If the radius of the trigger area is not specified, minimum 
        * size of trigger area will be used in the created trigger entry. 
        * 
        * The trigger ID attribute is ignored while creating a trigger. If the 
        * trigger is successfully created, trigger ID is returned to the 
        * client application.  If the trigger is enabled, Location Triggering
        * Server will supervise the trigger and fires it when trigger 
        * conditions are met.
        *
        * Creating any type triggers requires @p Location capability. 
        * @p WriteUserData capability is required in addition to create start-up 
        * triggers. 
        *
        * @see CLbtTriggerEntry CLbtSessionTrigger CLbtStartupTrigger
        * @see CancelCreateTrigger
        *
        * @panic LocTriggering ELbtServerBadHandle If the subsession is 
        * not opened.
        *
        * @param[in] aTrigger The trigger to be created. Trigger Id attribute
        * is ignored by Location Triggering Server.
        * @param[out] aTriggerId Contains trigger ID of the created trigger
        * When the request is completed. Trigger is is unique among all triggers
        * currently exist in the system. If a trigger is removed from the system,
        * its Id may be reused by another trigger.
        * @param[in] aFireOnCreation The parameter specifies if the trigger
        * can be fired right after the creation. 
        * - If this parameter is ETrue. For entry type of trigger, 
        * if the trigger is created inside the trigger area, it is 
        * fired right after it is created. For exit type of trigger, if the
        * trigger is created outside of the trigger area, it is
        * fired right after it is created. 
        * - If this parameter is EFalse. For entry type of 
        * trigger, if the trigger is created inside the trigger area, it
        * will not be fired immediately. The trigger will be fired when 
        * the terminal moves outside of the trigger area and then enters 
        * the trigger area again. For exit type of trigger, if the trigger 
        * is created outside of trigger area it will be fired immediately. 
        * The trigger will be fired when the terminal moves into the trigger 
        * area and then  moves out again. 
        * @param[out] aStatus Contains the error code when the 
        * request is completed.
        * - KErrNone. If the trigger is created successfully.
        * - KErrArgument. If any of mandatory attributes are not specified, 
        * the manager UI is specified but it is not a valid UI application, 
        * or the length of the trigger name is zero or larger than 
        * @p KLbtMaxNameLength.
        * - KErrNotSupported. If the trigger condition is not 
        * an instance of @p CLbtTriggerConditionBasic, or if the trigger area is 
        * not an instance of CLbtGeoCircle. Also returned if the trigger direction
        * is EFireOnExit and the trigger being created is a cell based trigger.
        * - KErrAccessDenied. If the requestor attributes are missing, privacy 
        * checking by Location Server determines that any of the specified 
        * requestors do not have permission to retrieve location information, 
        * - KErrPermisionDenied. If the client application does not have 
        * enough capabilities to create this trigger.
        * - KErrTriggeringAreaTooSmall.  If the specified trigger area is 
        * smaller than minimum size of trigger area.
        * - KErrLbtMaxTriggerLimitExceeded. If creating startup trigger exceeds
        * the system defined limit.
        * - KErrDiskFull. Disk full when creating a start-up trigger.
        * - Other standard Symbian error code, such as KErrNoMemory, 
        * KErrServerBusy, KErrGeneral. If the operation fails. 
        */
        IMPORT_C void CreateTrigger( 
            const CLbtTriggerEntry& aTrigger,
            TLbtTriggerId& aTriggerId,
            TBool aFireOnCreation,
            TRequestStatus& aStatus );
            
       /**
        * Cancel trigger creation.
        *
        * This function does not require any capabilities. 
        *
        * @see CreateTriggerL
        */
        IMPORT_C void CancelCreateTrigger();
            
       /**
        * Deletes a specific trigger from Location Triggering Server.
        * 
        * Client applications can only delete triggers owned by it. 
        *
        * Deleting any type triggers requires @p Location capability. 
        * @p WriteUserData capability is required in addition to delete start-up 
        * triggers. 
        *
        * @panic LocTriggering ELbtServerBadHandle If the subsession is not opened.
        *
        * @param[in] aId The ID of the trigger to be deleted.
        *
        * @leave KErrNotFound If the specified trigger is not found or
        * it is not owned by the client application.
        * @leave KErrInUse If the previous write or delete operation on the
        * trigger is not completed yet.
        * @leave Other standard Symbian error code, such as KErrNoMemory,
        * KErrServerBusy, KErrGeneral, etc.
        */
        IMPORT_C void DeleteTriggerL( TLbtTriggerId aId );

       /**
        * Delete triggers that are owned by the client application and fulfill 
        * the specified criteria. 
        *
        * If none of the triggers that belong to the client application 
        * fulfill the specified criteria, the method leaves with KErrNotFound.
        *
        * If only a part of the triggers that fullfill the criteria belong to 
        * the client application, then only those triggers belonging to that 
        * client application would be deleted and the method would complete 
        * without any leave.
        * 
        * If no filter is specified, all triggers owned by the client 
        * application are deleted.
        *
        * Deleting any type triggers requires @p Location capability. 
        * @p WriteUserData capability is required in addition to delete 
        * start-up triggers. 
        * 
        * @panic LocTriggering ELbtServerBadHandle If the subsession is not 
        * opened.
        *
        * @param[in] aFilter Specify the filter for the delete operation. 
        * Trigger entries that fulfill the criteria will be deleted 
        * from Location Triggering Server. By default, no filter is used.
        * In this case, all triggers owned by the client applications 
        * will be deleted.
        * @leave KErrNotSupported If there is an area filter used and the area
        * is not a type of geographical circular or rectangular area.
        * @leave KErrNotFound If no trigger belonging to the client application
        * fullfills the criteria specified.
        * @leave Other standard Symbian error code, such as KErrNoMemory,
        * KErrServerBusy, KErrGeneral, etc.
        */
        IMPORT_C void DeleteTriggersL( 
            CLbtTriggerFilterBase* aFilter = NULL );

       /**
        * Delete triggers asynchronously. Triggers to be deleted must be owned
        * by the client application and fulfill the specified criteria.
        *
        * If no trigger that belong to the client application fulfills the 
        * specified criteria, the method completes the client request
        * with KErrNotFound.
        *
        * If only a part of the triggers that fullfill the criteria belong to 
        * the client application, then only those triggers belonging to that 
        * client application would be deleted and the method would complete 
        * without any error.
        *
        * If no filter is specified, all triggers owned by the client 
        * application are deleted.
        *
        * Deleting any type triggers requires @p Location capability. 
        * @p WriteUserData capability is required in addition to delete start-up 
        * triggers. 
        * 
        * @panic LocTriggering ELbtServerBadHandle If the subsession is not
        * opened.
        *
        * @param[out] aStatus Contains the error code when the 
        * request is completed.
        * - KErrNone If the operation was successful.
        * - KErrNotFound If no trigger belonging to the client application
        * fullfills the criteria specified.
        * - Other standard Symbian error code, such as KErrNoMemory,
        * KErrServerBusy, KErrGeneral, etc.
        * @param[in] aFilter Specify the filter for the delete operation. 
        * Trigger entries that fulfill the criteria will be deleted 
        * from Location Triggering Server. Default value is NULL in which case
        * all triggers owned by the client applications will be deleted.
        */
        IMPORT_C void DeleteTriggers( 
            TRequestStatus& aStatus, 
            CLbtTriggerFilterBase* aFilter = NULL );

       /**
        * Delete triggers based on a list of trigger Ids. The triggers to 
        * be deleted must be owned by the client application. 
        *
        * If none of the triggers to be deleted are owned by the client 
        * application then no triggers would be deleted and this method 
        * will leave with KErrNotFound.
        *
        * If the list is empty, no trigger will be deleted and this method 
        * completes without any leave. 
        * 
        * In the case where a list of trigger IDs are mentioned of which only 
        * a few of those belong to the client, then only all those triggers 
        * that belong to the client will be deleted and the rest ignored. The 
        * method will complete without any leave in this case.
        * 
        * Deleting any type of triggers requires @p Location capability. 
        * @p WriteUserData capability is required in addition to delete start-up 
        * triggers. 
        * 
        * @panic LocTriggering ELbtServerBadHandle If the subsession is not opened.
        *
        * @param[in] aTriggerIdList The list contains IDs of the triggers
        * that are to be deleted. 
        * @leave Other standard Symbian error code, such as KErrNoMemory,
        * KErrServerBusy, KErrGeneral, etc.
        */
        IMPORT_C void DeleteTriggersL( 
            const RArray<TLbtTriggerId> &aTriggerIdList );

       /**
        * Delete triggers asynchronously based on a list of trigger Ids. 
        * The triggers to be deleted must be owned by the client application.
        *
        * If none of the triggers to be deleted are owned by the client 
        * application then no triggers would be deleted and this method 
        * will complete the request with KErrNotFound.
        * 
        * If the list is empty, no trigger will be deleted and this method 
        * completes without any error code.
        * 
        * In the case where a list of trigger IDs are mentioned of which only 
        * a few of those belong to the client, then only all those triggers 
        * that belong to the client will be deleted and the rest ignored. The 
        * method will complete without any leave in this case.
        *
        * Deleting any type of triggers requires @p Location capability. 
        * @p WriteUserData capability is required in addition to delete 
        * start-up triggers. 
        * 
        * @panic LocTriggering ELbtServerBadHandle If the subsession is not 
        * opened.
        *
        * @param[in] aTriggerIdList The list contains IDs of the triggers
        * that are to be deleted. 
        * @param[out] aStatus Contains the error code when the 
        * request is completed.
        * - KErrNone If the operation was succeed.
        * - Other standard Symbian error code, such as KErrNoMemory,
        * KErrServerBusy, KErrGeneral, etc.
        */
        IMPORT_C void DeleteTriggers( 
            const RArray<TLbtTriggerId>& aTriggerIdList,
            TRequestStatus& aStatus );
        
       /**
        * Cancel delete triggers operation.
        *
        * This function does not require any capabilities. 
        *
        * @see DeleteTriggers
        */
        IMPORT_C void CancelDeleteTriggers();     

       /**
        * Gets the specified trigger from Location Triggering Server. 
        * 
        * Client application takes the ownership ofthe returned trigger object.
        * The returned trigger object is left in cleanup stack when the 
        * trigger entry is successfully retrieved.
        *
        * Each trigger entry object consumes about 100 - 200 bytes user heap,
        * if all attributes are filled. To save memory usage, 
        * client applications can retrieve trigger object with only partial 
        * attributes filled. 
        *
        * This method requires @p Location capability. 
        *
        * @panic LocTriggering ELbtServerBadHandle If the subsession is not 
        * opened.
        * @param[in] aId The ID of the trigger to be retrieved.
        * @param[in] aEntryFieldMask The trigger entry's attribute field mask.
        * It specifies what attributes shall be filled in the returned 
        * trigger object. The default value is KLbtTriggerAttributeFieldsAll, 
        * which means all attributes field will be filled. Wether the trigger ID 
        * attribute is specified or not in this mask, the returned 
        * trigger object always contains a valid trigger Id.
        * @param[in] aDynInfoFieldMask Specifies which dynamic information
        * field shall be filled in the returned object. The default value is
        * KLbtTriggerDynInfoFieldsAll, which means all dynamic information
        * fields will be filled.
        * @return The retrieved trigger object. Ownership of the object is
        * transferred to the client application.
        * @leave KErrNotFound If the specified trigger is not found or it's
        * not owned by the client application.
        * @leave Other standard Symbian error code, such as KErrNoMemory,
        * KErrServerBusy, KErrGeneral, etc.
        */
        IMPORT_C CLbtTriggerInfo* GetTriggerLC( 
            TLbtTriggerId aId,
            TLbtTriggerAttributeFieldsMask aEntryFieldMask = 
                KLbtTriggerAttributeFieldsAll,
            TLbtTriggerDynamicInfoFieldsMask  aDynInfoFieldMask = 
                KLbtTriggerDynInfoFieldsAll );

       /**
        * Changes the attributes of the specified trigger.
        *
        * Client applications can use this method to change attributes of a
        * specified trigger that is owned by it. Client applications can
        * only update triggers owned by itself.
        *
        * Some attributes are not modifiable after the trigger is created. Trying 
        * to change the following attributes will generate a leave with 
        * error code KErrAccessDenied.
        * 
        * For any type of the trigger, the following attributes can't be
        * modified after the trigger is created.
        * - ID
        * - Requestor
        * - Manager UI
        *
        * The following attribute can't be modified in addition for 
        * start-up triggers.
        * - Trigger handling process identity
        * - Trigger handling process SID
        *
        * If the specified trigger does not belong to the client application 
        * the method leaves with KErrNotFound.
        *
        * Updating any type triggers requires @p Location capability. 
        * @p WriteUserData capability is required in addition to update start-up 
        * triggers. 
        *
        * @panic LocTriggering ELbtServerBadHandle If the subsession is not opened.
        *
        * @param[in] aTrigger The trigger object to be updated in Location 
        * Triggering Server. The trigger ID identifies the trigger to be updated.
        * @param[in] aFieldMask Specifies the attribute fields that are valid in 
        * the aTrigger and shall be updated to the trigger. Trigger ID
        * field in aTrigger is always used regardless whether the trigger ID 
        * field is marked or not in the mask. The attribute value in aTrigger 
        * is ignored if the attribute field in aFieldMask is not marked.
        * @param[in] aFireOnUpdate The parameter specifies if the trigger
        * can be fired right after the update operation. 
        * - If this parameter is ETrue. For entry type of trigger, 
        * if the trigger is updated inside the trigger area, it is 
        * fired right after it is updated. For exit type of trigger, if the
        * trigger is updated outside of the trigger area, it is
        * fired right after it is updated. 
        * - If this parameter is EFalse. For entry type of 
        * trigger, if the trigger is updated inside the trigger area, it
        * will not be fired immediately. The trigger will be fired when 
        * the terminal moves outside of the trigger area and then enters 
        * the trigger area again. For exit type of trigger, if the trigger 
        * is updated outside of trigger area it will be fired immediately. 
        * The trigger will be fired when the terminal moves into the trigger 
        * area and then  moves out again. 
        * @leave KErrNotFound If the specified trigger is not found or it's
        * not owned by the client application.
        * @leave KErrAccessDenied If the client application tries to change 
        * the attributes which are not modifiable.
        * @leave KErrArgument If the length of trigger name is zero or 
        * larger than @p KLbtMaxNameLength. 
        * @leave Other standard Symbian error code, such as KErrNoMemory,
        * KErrServerBusy, KErrGeneral, etc.
        */
        IMPORT_C void UpdateTriggerL( 
            const CLbtTriggerEntry& aTrigger,
            TLbtTriggerAttributeFieldsMask aFieldMask,
            TLbtFireOnUpdate aFireOnUpdate );
            
            
       /**
        * Changes the attributes of the specified trigger asynchronously
        *
        * Client applications can use this method to change attributes of a
        * specified trigger that is owned by it. Client applications can
        * only update triggers owned by itself.
        *
        * Some attributes are not modifiable after the trigger is created. Trying 
        * to change the following attributes will generate a leave with 
        * error code KErrAccessDenied.
        * 
        * For any type of the trigger, the following attributes can't be
        * modified after the trigger is created.
        * - ID
        * - Requestor
        * - Manager UI
        *
        * The following attribute can't be modified in addition for 
        * start-up triggers.
        * - Trigger handling process identity
        * - Trigger handling process SID
        *
        * If the specified trigger does not belong to the client application 
        * the method leaves with KErrNotFound.
        *
        * Updating any type triggers requires @p Location capability. 
        * @p WriteUserData capability is required in addition to update start-up 
        * triggers. 
        *
        * @panic LocTriggering ELbtServerBadHandle If the subsession is not opened.
        *
        * @param[in] aTrigger The trigger object to be updated in Location 
        * Triggering Server. The trigger ID identifies the trigger to be updated.
        * @param[in] aFieldMask Specifies the attribute fields that are valid in 
        * the aTrigger and shall be updated to the trigger. Trigger ID
        * field in aTrigger is always used regardless whether the trigger ID 
        * field is marked or not in the mask. The attribute value in aTrigger 
        * is ignored if the attribute field in aFieldMask is not marked.
        * @param[in] aFireOnUpdate The parameter specifies if the trigger
        * can be fired right after the update operation. 
        * - If this parameter is ETrue. For entry type of trigger, 
        * if the trigger is updated inside the trigger area, it is 
        * fired right after it is updated. For exit type of trigger, if the
        * trigger is updated outside of the trigger area, it is
        * fired right after it is updated. 
        * - If this parameter is EFalse. For entry type of 
        * trigger, if the trigger is updated inside the trigger area, it
        * will not be fired immediately. The trigger will be fired when 
        * the terminal moves outside of the trigger area and then enters 
        * the trigger area again. For exit type of trigger, if the trigger 
        * is updated outside of trigger area it will be fired immediately. 
        * The trigger will be fired when the terminal moves into the trigger 
        * area and then  moves out again. 
        * @leave KErrNotFound If the specified trigger is not found or it's
        * not owned by the client application.
        * @leave KErrAccessDenied If the client application tries to change 
        * the attributes which are not modifiable.
        * @leave KErrArgument If the length of trigger name is zero or 
        * larger than @p KLbtMaxNameLength. 
        * @leave Other standard Symbian error code, such as KErrNoMemory,
        * KErrServerBusy, KErrGeneral, etc.
        */
        IMPORT_C void UpdateTrigger( 
            const CLbtTriggerEntry& aTrigger,
            TLbtTriggerAttributeFieldsMask aFieldMask,
            TLbtFireOnUpdate aFireOnUpdate,
            TRequestStatus& aStatus );            
    	
    	  /**
        * Cancel update trigger operation.
        *
        * This function does not require any capabilities. 
        *
        * @see UpdateTrigger
        */
        IMPORT_C void CancelUpdateTrigger();     
        
       /**
        * Sets the state of the specified trigger. Client application can 
        * change the state of only triggers owned by it.
        * 
        * To enable the trigger, set the trigger state to
        * @p ELbtTriggerEnabled. To disable the trigger, 
        * set the trigger state to @p ELbtTriggerDisabled.
        *
        * Changing state of any type triggers requires @p Location capability. 
        * @p WriteUserData capability is required in addition to change state of 
        * start-up triggers. 
        *
        * @panic LocTriggering ELbtServerBadHandle If the subsession is not opened.
        *
        * @param[in] aId The ID of the trigger whose state will be updated.
        * @param[in] aState New state of the specified trigger.
        * @param[in] aFireOnUpdate The parameter specifies if the trigger
        * can be fired right after the update operation. 
        * - If this parameter is ETrue. For entry type of trigger, 
        * if the trigger is updated inside the trigger area, it is 
        * fired right after it is updated. For exit type of trigger, if the
        * trigger is updated outside of the trigger area, it is
        * fired right after it is updated. 
        * - If this parameter is EFalse. For entry type of 
        * trigger, if the trigger is updated inside the trigger area, it
        * will not be fired immediately. The trigger will be fired when 
        * the terminal moves outside of the trigger area and then enters 
        * the trigger area again. For exit type of trigger, if the trigger 
        * is updated outside of trigger area it will be fired immediately. 
        * The trigger will be fired when the terminal moves into the trigger 
        * area and then  moves out again.        
        * @leave KErrNotFound If the specified trigger is not found or it's
        * not owned by the client application.
        * @leave Other standard Symbian error code, such as KErrNoMemory,
        * KErrServerBusy, KErrGeneral, etc.
        */
        IMPORT_C void SetTriggerStateL( 
            TLbtTriggerId aId, 
            CLbtTriggerEntry::TLbtTriggerState aState,
            TLbtFireOnUpdate aFireOnUpdate );

       /**
        * Sets state of multiple triggers. Client application can change state
        * of only triggers owned by it.
        *
        * If a filter is specified, all triggers that fulfill the criteria 
        * and owned by the requesting client application will be affected.
        * 
        * If no filter is specified, all triggers owned by the client 
        * application will be affected.
        *
        * If no trigger owned by the client application fulfills the specified 
        * criteria, no trigger will be modified and the method leaves with
        * KErrNotFound.
        *
        * Changing state of any type triggers requires @p Location capability. 
        * @p WriteUserData capability is required in addition to change state of 
        * start-up triggers. 
        *
        * @panic LocTriggering ELbtServerBadHandle If the subsession is not opened.
        *
        * @param[in] aState New state of the triggers.
        * @param[in] aFireOnUpdate The parameter specifies if the trigger
        * can be fired right after the update operation. 
        * - If this parameter is ETrue. For entry type of trigger, 
        * if the trigger is updated inside the trigger area, it is 
        * fired right after it is updated. For exit type of trigger, if the
        * trigger is updated outside of the trigger area, it is
        * fired right after it is updated. 
        * - If this parameter is EFalse. For entry type of 
        * trigger, if the trigger is updated inside the trigger area, it
        * will not be fired immediately. The trigger will be fired when 
        * the terminal moves outside of the trigger area and then enters 
        * the trigger area again. For exit type of trigger, if the trigger 
        * is updated outside of trigger area it will be fired immediately. 
        * The trigger will be fired when the terminal moves into the trigger 
        * area and then  moves out again.        
        * @param[in] aFilter The filter to be used. Triggers that fulfill
        * the criteria of the specified filter will be affected. 
        * Default value is NULL in which case all triggers owned by the client 
        * application will be updated.
        * @leave KErrNotSupported If there is an area filter used and the area
        * is not a type of geographical circular or rectangular area.
        * @leave Other standard Symbian error code, such as KErrNoMemory,
        * KErrServerBusy, KErrGeneral, etc.
        */
        IMPORT_C void SetTriggersStateL( 
            CLbtTriggerEntry::TLbtTriggerState aState,
            TLbtFireOnUpdate aFireOnUpdate,
            CLbtTriggerFilterBase* aFilter = NULL );

       /**
        * Sets state of multiple triggers asynchronously.
        *
        * If a filter is specified, all triggers owned by the client 
        * application that fulfill the  criteria will be affected.
        *
        * If no filter is specified, all triggers owned by the client 
        * application will be affected.
        *
        * If no trigger that are owned by the client application fulfills the
        * specified criteria, no trigger will be modified and this completes 
        * with KErrNotFound.
        *
        * Changing state of any type triggers requires @p Location capability. 
        * @p WriteUserData capability is required in addition to change state of 
        * start-up triggers. 
        *
        * @panic LocTriggering ELbtServerBadHandle If the subsession is not opened.
        *
        * @param[out] aStatus Contains the error code when the 
        * request is completed.
        * - KErrNotSupported If there is an area filter used and the area
        * is not a type of geographical circular or rectangular area.
        * - Other standard Symbian error code, such as KErrNoMemory,
        * KErrServerBusy, KErrGeneral, etc.
        * @param[in] aState New state of the triggers.
        * @param[in] aFireOnUpdate The parameter specifies if the trigger
        * can be fired right after the update operation. 
        * - If this parameter is ETrue. For entry type of trigger, 
        * if the trigger is updated inside the trigger area, it is 
        * fired right after it is updated. For exit type of trigger, if the
        * trigger is updated outside of the trigger area, it is
        * fired right after it is updated. 
        * - If this parameter is EFalse. For entry type of 
        * trigger, if the trigger is updated inside the trigger area, it
        * will not be fired immediately. The trigger will be fired when 
        * the terminal moves outside of the trigger area and then enters 
        * the trigger area again. For exit type of trigger, if the trigger 
        * is updated outside of trigger area it will be fired immediately. 
        * The trigger will be fired when the terminal moves into the trigger 
        * area and then  moves out again.        
        * @param[in] aFilter The filter to be used. Triggers that fulfill
        * the criteria of the specified filter will be affected. 
        * Default is value is NULL in which case all triggers owned by the 
        * client application will be updated.
        */
        IMPORT_C void SetTriggersState( 
            TRequestStatus& aStatus,
            CLbtTriggerEntry::TLbtTriggerState aState,
            TLbtFireOnUpdate aFireOnUpdate,
            CLbtTriggerFilterBase* aFilter = NULL );
        
       /**
        * Cancel set trigger state operation.
        *
        * This function does not require any capabilities. 
        *
        * @see SetTriggersState
        */
        IMPORT_C void CancelSetTriggersState();     

       /**
        * Lists IDs of triggers that are owned by the client application.
        * 
        * Client applications can specify options used in retrieving 
        * trigger IDs.
        * 
        * This method requires @p Location capability. 
        *
        * @see CLbtListTriggerOptions
        *
        * @panic LocTriggering ELbtServerBadHandle If the subsession is not opened.
        *
        * @param[out] aTriggerIdList On return, aTriggerIdList contains IDs of
        * retrieved triggers. The content of aTriggerIdList will be cleared 
        * even if this function fails.
        * @param[in] aListOptions Specified the options used for listing 
        * triggers. By default, the value is NULL. In this case, all triggers
        * owned by the client application will be retrieved. 
        * @leave KErrNotSupported If there is an area filter used and the area
        * is not a type of geographical circular or rectangular area.
        * @leave Other standard Symbian error code, such as KErrNoMemory,
        * KErrServerBusy, KErrGeneral, etc.
        */
        IMPORT_C void ListTriggerIdsL( 
            RArray < TLbtTriggerId >& aTriggerIdList,
            CLbtListTriggerOptions* aListOptions = NULL );

       /**
        * Lists asynchronously IDs of triggers that are owned by the 
        * client application.
        * 
        * Client applications can specify options used in retrieving 
        * trigger IDs.
        * 
        * This method requires @p Location capability. 
        *
        * @see CLbtListTriggerOptions
        *
        * @panic LocTriggering ELbtServerBadHandle If the subsession is not 
        * opened.
        *
        * @param[out] aStatus Contains the error code when the 
        * request is completed. KErrNotSupported is returned if there is an area 
        * filter used and the area is not a type of geographical circular or 
        * rectangular area.
        * - KErrNotSupported If there is an area filter used and the area
        * is not a type of geographical circular or rectangular area.
        * - Other standard Symbian error code, such as KErrNoMemory,
        * KErrServerBusy, KErrGeneral, etc.
        * @param[out] aTriggerIdList On return, aTriggerIdList contains IDs of
        * retrieved triggers. The content of aTriggerIdList will be cleared 
        * even if this function fails.
        * @param[in] aListOptions Specified the options used for listing 
        * triggers. Default value is NULL in which case all triggers owned by 
        * the client application will be retrieved. 
        */
        IMPORT_C void ListTriggerIds( 
            TRequestStatus& aStatus,
            RArray < TLbtTriggerId >& aTriggerIdList,
            CLbtListTriggerOptions* aListOptions = NULL );
            
       /**
        * Cancel list trigger ids operation.
        *
        * This function does not require any capabilities. 
        *
        * @see ListTriggerIds
        */
        IMPORT_C void CancelListTriggerIds();    

       /**
        * Gets triggers from Location Triggering Server. A client application
        * can only retrieve triggers owned by it.
        * 
        * Client applications can specify options used in retrieving triggers.
        * Ownership of the returned trigger objects is transferred to 
        * the client application.
        *
        * Note: This function may require large free heap memory from
        * the client application depending on the number of triggers to 
        * be retrieved and the attributes to be filled.
        *
        * This method requires @p Location capability. 
        *
        * @see CLbtListTriggerOptions
        *
        * @panic LocTriggering ELbtServerBadHandle If the subsession is not 
        * opened.
        *
        * @param[out] aTriggerList On return, contains trigger objects retrieved
        * from Location Triggering Server. The content of aTriggerList is
        * cleared even if this function fails. The ownership of the returned 
        * pointers is transfered to the client application. 
        * @param[in] aListOptions Specifies the options for listing triggers. 
        * By default, the value is NULL. In this case all triggers
        * owned by the client application will be retrieved. 
        * @leave KErrNotSupported If there is an area filter used and the area
        * is not a type of geographical circular or rectangular area.
        * @leave Other standard Symbian error code, such as KErrNoMemory,
        * KErrServerBusy, KErrGeneral, etc.
        */
        IMPORT_C void GetTriggersL( 
            RPointerArray < CLbtTriggerInfo >& aTriggerList,
            CLbtListTriggerOptions* aListOptions = NULL );
        
       /**
        * Gets triggers asynchronously from Location Triggering Server. A 
        * client application can only retrieve triggers owned by it.
        * 
        * Client applications can specify options used in retrieving triggers.
        * Ownership of the returned trigger objects is transferred to 
        * the client application.
        *
        * Note: This function may require large free heap memory from
        * the client application depending on the number of triggers to 
        * be retrieved and the attributes to be filled.
        *
        * This method requires @p Location capability. 
        *
        * @see CLbtListTriggerOptions
        *
        * @panic LocTriggering ELbtServerBadHandle If the subsession is not opened.
        *
        * @param[out] aStatus Contains the error code when the 
        * request is completed. 
        * - KErrNotSupported. If there is an area 
        * filter used and the area is not a type of geographical circular or 
        * rectangular area.
        * - Other standard Symbian error code, such as KErrNoMemory,
        * KErrServerBusy, KErrGeneral, etc.
        * @param[out] aTriggerList On return, contains trigger objects retrieved
        * from Location Triggering Server. The content of aTriggerList is
        * cleared even if this function fails. The ownership of the returned 
        * pointers is transfered to the client application. 
        * @param[in] aListOptions Specifies the options for listing triggers. 
        * By default, the value is NULL. In this case all triggers
        * owned by the client application will be retrieved. 
        */
        IMPORT_C void GetTriggers( 
            TRequestStatus& aStatus,
            RPointerArray < CLbtTriggerInfo >& aTriggerList,
            CLbtListTriggerOptions* aListOptions = NULL );
            
       /**
        * Cancel get triggers operation.
        *
        * This function does not require any capabilities. 
        *
        * @see GetTriggers
        */
        IMPORT_C void CancelGetTriggers();    

       /**
        * Creates an iterator in Location Triggering Server to retrieve
        * trigger objects incrementally. 
        * 
        * An iterator must be created before GetNextTriggerLC() can be called.
        * The iterator is constructed in the server side and it is subsession
        * specific. Calling this function again will reset the iterator.
        * After the iterator is constructed, the client application calls 
        * GetNextTriggerLC() repeatedly to retrieve all interested trigger 
        * objects. Note, client applications can only get triggers owned by
        * itself.
        *
        * If any trigger is changed during iteration, the client application
        * shall call this method again to reset the iterator and get the 
        * triggers again incrementally.
        *
        * This method requires @p Location capability. 
        *
        * @see CLbtListTriggerOptions
        *
        * @panic LocTriggering  ELbtServerBadHandle If the subsession is not opened.
        *
        * @param[in] aListOptions Specifies the options used for listing 
        * triggers. Default value is NULL, which will retrieve all triggers
        * owned by the client application. 
        * @leave KErrNotSupported If there is an area filter used and the area
        * is not a type of geographical circular or rectangular area.
        * @leave Other standard Symbian error code, such as KErrNoMemory,
        * KErrServerBusy, KErrGeneral, etc.
        */
        IMPORT_C void CreateGetTriggerIteratorL( 
            CLbtListTriggerOptions* aListOptions = NULL );

       /**
        * Creates an iterator asynchronously in Location Triggering Server 
        * to retrieve trigger objects incrementally. 
        * 
        * An iterator must be created before GetNextTriggerLC() can be called.
        * The iterator is constructed in the server side and it is subsession
        * specific. Calling this function again will reset the iterator.
        * After the iterator is constructed, the client application calls 
        * GetNextTriggerLC() repeatedly to retrieve all interested trigger 
        * objects. Note, client applications can only get triggers owned by
        * itself.
        *
        * If any trigger is changed during iteration, the client application
        * shall call this method again to reset the iterator and get the 
        * triggers again incrementally.
        *
        * This method requires @p Location capability. 
        *
        * @see CLbtListTriggerOptions
        *
        * @panic LocTriggering  ELbtServerBadHandle If the subsession is not opened.
        *
        * @param[out] aStatus Contains the error code when the 
        * request is completed. KErrNotSupported is returned if there is an area 
        * filter used and the area is not a type of geographical circular or 
        * rectangular area.
        * @param[in] aListOptions Specifies the options used for listing 
        * triggers. Default value is NULL, which will retrieve all triggers
        * owned by the client application. 
        */
        IMPORT_C void CreateGetTriggerIterator( 
            TRequestStatus& aStatus,
            CLbtListTriggerOptions* aListOptions = NULL );
        
       /**
        * Cancel create trigger iterator operation.
        *
        * This function does not require any capabilities. 
        *
        * @see CreateGetTriggerIterator
        */
        IMPORT_C void CancelCreateTriggerIterator();     

       /**
        * Gets trigger objects incrementally.
        * 
        * This method is used together with CreateGetTriggerIteratorL() to 
        * incrementally retrieve trigger objects owned by the client 
        * application. If the iterator is not created when this function is
        * called, client application gets a panic with code 
        * @p ELbtIteratorNotCreated.
        * 
        * This method returns NULL when all triggers are retrieved. Client
        * application shall call CreateGetTriggerIteratorL() again to
        * reset the iterator.
        * 
        * Client application takes ownership of the returned trigger object. 
        * The returned trigger object is left in cleanup stack when the trigger 
        * object is successfully retrieved.
        *
        * This method requires @p Location capability. 
        *
        * @panic LocTriggering ELbtServerBadHandle If the subsession is not opened.
        * @panic LocTriggering ELbtIteratorNotCreated If the iterator has not been created.
        *
        * @return The retrieved trigger object. Ownership of the returned 
        * object is transferred to the client application. 
        * Returns NULL if all triggers have been retrieved.
        * @leave Other standard Symbian error code, such as KErrNoMemory,
        * KErrServerBusy, KErrGeneral, etc.
        */
        IMPORT_C CLbtTriggerInfo* GetNextTriggerLC();

       /**
        * Listens for change events of the triggers owned by the client 
        * application.
        *
        * This method is used by the client application to get change events
        * when one or many of its triggers are changed.
        *
        * Triggers can be deleted and modified not only by the owner process and
        * trigger handling process, but also by other system application, 
        * e.g. system management UI application. 
        *
        * This function is asynchronous and it will complete the request status
        * when an event occurs. Client applications can get detailed information of
        * the change from the retrieved event object. Client application shall
        * call this function again to get further change event.
        *
        * Event listening can be cancelled by calling
        * CancelNotifyTriggerChangeEvent().
        *
        * This method requires @p Location capability. 
        *
        * @panic LocTriggering ELbtServerBadHandle If the subsession is not opened.
        * @panic LocTriggering ELbtDuplicateRequest If the subsession has
        * already an outstanding NotifyTriggerChangeEvent() request.
        *
        * @param[out] aEvent Will contain the event information when an event
        * occurs.
        * @param[out] aStatus Will be completed with @p KErrNone if an event occurs
        *   and an error code(for example, KErrServerBusy, etc.) if some error 
        *   was encountered.
        */
        IMPORT_C void NotifyTriggerChangeEvent( 
            TLbtTriggerChangeEvent& aEvent, 
            TRequestStatus& aStatus );

       /**
        * Cancels listening for trigger change event.
        * 
        * This function does not require any capabilities. 
        *
        * @see NotifyTriggerChangeEvent
        */
        IMPORT_C void CancelNotifyTriggerChangeEvent();

       /**
        * Listens for the event if any trigger is fired. 
        *
        * Client applications can use this method to get notified 
        * when a trigger (session triggers and start-up triggers) is 
        * fired. The firing information is 
        * returned to the client application. If more that one  
        * trigger is fired, Location Triggers Server will complete 
        * the request and  the first fired trigger is returned. 
        * Client application shall call this method again to get next 
        * trigger firing event. 
        *
        * When a start-up trigger is fired, Location Triggering
        * Server will first launch the specified trigger
        * handling process, and then notify the client application
        * about the firing event.
        *
        * A client application will get firing event of 
        * - triggers that are created by itself(Client application is
        * the owner process of the trigger).
        * - triggers that trigger handling process SID is set and
        * matches SID of the client application's process(Client 
        * application is the triggering handling process of the 
        * trigger, and it can access the trigger). 
        *
        * The request is canceled by CancelNotifyTriggerFired()
        *
        * This method requires @p Location capability. 
        *
        * @panic LocTriggering ELbtServerBadHandle If the subsession is not opened.
        * @panic LocTriggering ELbtDuplicateRequest If the subsession has
        * already an outstanding NotifyTriggerFired() request.
        *
        * @param[out] aFireInfo On return contains the fired  
        * trigger's firing information.
        * @param[out] aStatus Will be completed with @p KErrNone if an event
        * occurs, and an error code( for example KErrServerBusy, etc.) if some 
        * error encountered.
        */
        IMPORT_C void NotifyTriggerFired( 
            TLbtTriggerFireInfo& aFireInfo, 
            TRequestStatus& aStatus );

       /**
        * Cancels listening for the trigger fired event.
        *
        * This function does not require any capabilities. 
        *
        * @see NotifyTriggerFired
        */
        IMPORT_C void CancelNotifyTriggerFired();

       /**
        * Gets fired trigger's information. 
        * 
        * This method is used by the client application to get information 
        * of all the fired triggers( session triggers and start-up triggers). 
        * If the same trigger is 
        * fired more than once before the client application retrieves 
        * the firing information, only the most recent fired
        * information is returned. If no trigger has been fired, 
        * an empty list is returned.
        *
        * This method requires @p Location capability. 
        *
        * @panic LocTriggering ELbtServerBadHandle If the subsession is not opened.
        *
        * @param[out] aTriggerInfoList On return contains fired triggers'
        * information.
        * @leave Other standard Symbian error code, such as KErrNoMemory,
        * KErrServerBusy, KErrGeneral, etc.
        */
        IMPORT_C void GetFiredTriggersL( 
            RArray < TLbtTriggerFireInfo >& aTriggerInfoList );

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
        * @param[out] aSettings On return contains the new triggering
        * system settings.
        * @param[out] aStatus Will be completed with @p KErrNone if an 
        * event occurs and an error code( for example, KErrServerBusy, etc.) if 
        * some error was encountered. 
        * aStatus will be completed with KErrPermissionDenied if the client 
        * application does not have enough capability.
        */
        IMPORT_C void NotifyTriggeringSystemSettingChange( 
            TLbtTriggeringSystemSettings& aSettings,
            TRequestStatus& aStatus );
        
       /**
        * Cancels listening for triggering system setting change event.
        *
        * @see NotifyTriggeringSystemSettingChange
        */
        IMPORT_C void CancelNotifyTriggeringSystemSettingChange();
    
       /**
        * Gets triggering system setting.
        *
        * This method is used by the client application to get triggering
        * system settings. Client applications can use
        * NotifyTriggeringSystemSettingChange()
        * get the change event of the triggering system settings.
        *
        * This function requires @p ReadUserData capability. 
        *
        * @since S60 5.1
        *
        * @panic LocTriggering ELbtServerBadHandle If the subsession is not opened.
        *
        * @param[out] aSetting On return contains triggering system 
        * settings.
        * @leave KErrPermissionDenied if the client application does not 
        * have enough capabilities to retrieve the settings.
        */
        IMPORT_C void GetTriggeringSystemSettingsL( 
            TLbtTriggeringSystemSettings& aSetting );
        
       /**
        * Cancels all asynchronous operation that has been issued from 
        * this subsession.
        */    
        IMPORT_C void CancelAll();     
        
       /**
        * Default constructor.
        */
        IMPORT_C RLbt();
        
       /**
        * Destructor.
        */
        IMPORT_C ~RLbt();
        
       /** 
        * Handles list triggers operation
        */
        void HandleListTriggerIdsL();
        
       /**
        * Handles get triggers operation
        */
        
        void HandleGetTriggersL();
    
    private:
       /**
        * Helper method for create trigger operation.
        */
        void CreateTriggerL( 
            const CLbtTriggerEntry& aTrigger,
            TLbtTriggerId& aTriggerId,
            TBool aFireOnCreation,
            TRequestStatus& aStatus );
       /**
        * Helper method for delete triggers operation.
        */    
        void DeleteTriggersL( 
            CLbtTriggerFilterBase* aFilter,
            TRequestStatus& aStatus );
       /**
        * Helper method for delete triggers operation.
        */     
        void DeleteTriggersL( 
            const RArray<TLbtTriggerId>& aTriggerIdList,
            TRequestStatus& aStatus );
        
        /**
         * Helper method for update trigger operation.
         */ 
        void UpdateTriggerL( 
                    const CLbtTriggerEntry& aTrigger,
                    TLbtTriggerAttributeFieldsMask aFieldMask,
                    TLbtFireOnUpdate aFireOnUpdate,
                    TRequestStatus& aStatus ); 
       /**
        * Helper method for set triggers state operation.
        */    
        void SetTriggersStateL( 
            CLbtTriggerEntry::TLbtTriggerState aState,
            CLbtTriggerFilterBase* aFilter,
            TLbtFireOnUpdate aFireOnUpdate,
            TRequestStatus& aStatus );
       /**
        * Helper method for list trigger ids operation.
        */      
        void ListTriggerIdsL( 
            RArray < TLbtTriggerId >& aTriggerIdList,
            CLbtListTriggerOptions* aListOptions,
            TRequestStatus& aStatus );
        
       /**
        * Helper method for get triggers operation.
        */     
        void GetTriggersL( 
            RPointerArray < CLbtTriggerInfo >& aTriggerList,
            CLbtListTriggerOptions* aListOptions,
            TRequestStatus& aStatus );
        
       /**
        * Helper method for create trigger iterator operation.
        */     
        void CreateGetTriggerIteratorL( 
            CLbtListTriggerOptions* aListOptions,
            TRequestStatus& aStatus );                       
        
       /**
        * Helper method for get triggers operation.
        */ 
        void GetTriggersInServerL(CBufFlat* aBuf,CLbtListTriggerOptions* aListOptions,TInt& aBufLength );
        
        /**
         * Validates geo area information based on type e.g. Coordinate, Cell, WLan, Hybrid.
         * 
         * @panic ELbtErrArgument If invalid.
         * 
         * @param[in] aGeoArea the geographical area         
         * @leave Other standard Symbian error code, such as KErrNoMemory
         */
        void ValidateGeoAreaInformationL( CLbtGeoAreaBase* aGeoArea );
        
       /**
        * Symbian 2nd phase construction.
        */
        void ConstructL();
         
    private:// data
       /**
        * Subsession pointer holder
        * Own.
        */
        CLbtSubSessnPtrHolder* iPtrHolder;
        
       /**
        * Pointer to client requestor.  
        * Own.
        */
        CLbtClientRequester* iClientRequester; 
        
       /**
        * Trigger entry state.
        */
        CLbtTriggerEntry::TLbtTriggerState iState;
        
       /**
        * Pointer to TLbtTriggerCreationInfo object.
        * Own.
        */
        TLbtTriggerCreationInfo* iTriggerCreationInfo;
       
       
       /**
        * Pointer to TLbtTriggerCreationInfo object.
        * Own.
        */
        TLbtTriggerUpdationInfo* iTriggerUpdationInfo; 
       
        /**
        * Pointer to TLbtTriggerStateInfo object.
        * Own.
        */
        TLbtTriggerStateInfo* iTriggerStateInfo;  
        
       /**
        * CLbtTriggerInfo pointer array.
        */
        RPointerArray<CLbtTriggerInfo> iTriggerList;
		
	   /**
		* Iterator flag.
		*/
		TBool iCreateIteratorFlag;        
    };


#endif // LBT_H

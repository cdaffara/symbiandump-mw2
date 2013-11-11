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
* Description:  Abstract base class for location triggering entries
*
*/


#ifndef LBTTRIGGERENTRY_H
#define LBTTRIGGERENTRY_H

#include <lbtcommon.h>
#include <lbs.h>

class CLbtTriggerConditionBase;

/**
 *  Abstract base class for location triggering entries.
 *
 *  It provides methods to determine the trigger type and to define
 *  basic trigger attributes. The following attributes are defined
 *  in location triggering entries.
 * 
 *  -  <B>Id</B> Defines the identity of a trigger. The Id is allocated 
 *  by Location Triggering Server and it's unique among all triggers
 *  currently exist in the system. If a trigger is removed from the system,
 *  its Id may be reused by another trigger. 
 *  This attribute can not be modified after the trigger is created.
 *
 *  -  <B>Name</B> The name attribute defines a human-readable name for 
 *  the trigger. The purpose of the name attribute is mainly for the end 
 *  user to be able to identify the trigger in the UI. The maximum length 
 *  of the name is @p KLbtMaxNameLength characters. This attribute can 
 *  be modified after the trigger is created.
 *
 *  -  <B>State</B> This attribute specifies if the trigger is enabled 
 *  or disabled. A disabled trigger is not supervised by the Location 
 *  Triggering Server and thus will never be fired. This attribute can 
 *  be modified after the trigger is created. 
 *  
 *  -  <B>Requestors</B> The requestors attribute consists of a requestor stack
 *  with contacts and/or services using the location service. Requestor 
 *  information is used for privacy queries when trigger is created 
 *  and fired. Requestor information is not allowed to be changed
 *  once the trigger has been created. 
 *
 *  -  <B>Manager UI</B>. Manager UI is the UI application that can view, edit 
 * and delete the trigger. Manager UI application shall be provided by the user 
 * of this API. If it's possible, client application must specify the manager 
 * UI application when the trigger is created. Note, Manager UI may be different 
 * from the trigger's owner. Trigger's owner is the process that created the 
 * trigger. Triggers can be directly accessed by the Manager UI application. 
 * Although this attribute is not mandatory when creating a trigger. it's 
 * highly recommended that it is specified. It cannot be modified after the 
 * trigger is created.
 *
 *  -  <B>Trigger condition</B>. Trigger condition specifies in what situation
 *  a trigger shall be fired. Current system supports only basic trigger 
 *  condition. Basic trigger condition is defined by a trigger area and 
 *  direction of terminal's movement. Only circular area can be used as 
 *  trigger area in current system. This attribute can be modified after 
 *  the trigger is created.
 *
 *  @lib lbt.lib
 *  @since S60 5.1
 */
class CLbtTriggerEntry : public CBase
    {
public:
    /**
     * Trigger type definition. It defines the type of the trigger
     * entry.
     */
    enum TType
        {
        /**
         * Session trigger 
         */  
        ETypeSession = 1, 
        /**
         * Start-up trigger
         */
        ETypeStartup = 2
        };
        
    /**
     * Trigger attributes identifications. 
     *
     * These ids can be combined and used to form an attribute mask when
     * retrieving partial attributes from Location Triggering Server.
     */
    enum TAttribute
        {
        /**
         * Trigger Id
         */
        EAttributeId = 0x0001, 
        /**
         * Trigger name
         */
        EAttributeName = 0x0002, 
        /**
         * Trigger state
         */
        EAttributeState = 0x0004, 
        /**
         * Trigger requestor
         */
        EAttributeRequestor = 0x0008, 
        /**
         * Trigger manager UI
         */
        EAttributeManagerUi = 0x0010, 
        /**
         * Trigger rearm time
         */
        EAttributeRearmTime = 0x0020, 
        /**
         * Trigger condition
         */
        EAttributeCondition = 0x0080,
        /**
         * Process ID of start-up trigger. It includes
         * process executable name and process UID type.
         */
        EAttributeStartUpProcessId = 0x0100,
        /**
         * Command-line argument string for start-up trigger
         */
        EAttributeStartUpCommandLine = 0x0200,

        };


    /**
     * Specifies the state of a trigger.
     */
    enum TLbtTriggerState
        {
        /** 
         * The trigger is enabled
         */ 
        EStateEnabled = 1,    
        /**
         * The trigger is disabled
         */
        EStateDisabled = 2     
        };

    /**
     * Destructor
     */
    IMPORT_C virtual ~CLbtTriggerEntry();

    /**
     * Gets the type of the trigger.
     *
     * @return The type of trigger entry.
     */
    IMPORT_C virtual TType Type() const = 0;

    /**
     * Gets the trigger ID.
     *
     * The ID is allocated and set by the Location Triggering Server when a
     * trigger is successfully created. Trigger ID is unique in the system.
     * This function returns @p KLbtNullTriggerId if the ID is not 
     * set before.
     *
     * @return The ID of the trigger entry, or @p KLbtNullTriggerId if the
     *   trigger ID has not been set.
     */
    IMPORT_C const TLbtTriggerId& Id() const;
    
    /**
     * Sets trigger entry id. SetId is not used during trigger creation as it 
     * is auto generated by the LBT server. This information is only used during 
     * deletion / modification of triggers. This information when specified 
     * during trigger creation, will be ignored by LBT server. 
     *
     * @param[in] aId The ID of the trigger entry, or @p KLbtNullTriggerId. 
     */
    IMPORT_C void SetId( TLbtTriggerId aId );

    /**
     * Gets the name of the trigger entry.
     * 
     * If the name is not set, an empty string is returned. Maximum 
     * length of the name is @p KLbtMaxNameLength.
     *
     * @return The name of the trigger entry.
     */
    IMPORT_C const TDesC& Name() const;
    
    /**
     * Sets the name of the trigger entry. 
     *
     * @param[in] aName The name of the trigger entry.
     * @leave KErrArgument If the name of the trigger is longer than
     *   @p KLbtMaxNameLength.
     */
    IMPORT_C void SetNameL( const TDesC& aName );
    
    /**
     * Gets the rearm time interval.
     *
     * If no interval is specified for the trigger, this function
     * returns
     * @p KLbtDefaultTimeToRearm which is 600s or 10 minutes.
     *
     * @return Rearm time interval.
     */
    IMPORT_C TInt TimeToRearm() const;
    
    /**
     * Sets the time interval to reactivate the trigger after the
     * trigger is fired.
     *
     * @param[in] aSeconds The time interval after which the trigger
     * is set effective by the Location Triggering Server. Client
     * applications can set 0 to indicate no delay, in which case
     * the trigger will remain effective through out its life span.
     * Range is 0 (KLbtMinTimeToRearm) to +2147483647 (KLbtMaxTimeToRearm) 
     * which is +24855 days (approximately 68 years)
     * @panic KErrArgument If the time set is out of range.
     */
    IMPORT_C void SetTimeToRearm( TInt aSeconds );
    
    /**
     * Gets trigger entry state.
     *
     * If no state has been set for the trigger, this function returns
     * @p ELbtTriggerEnabled.
     *
     * @return The trigger entry state.
     */
    IMPORT_C TLbtTriggerState State() const;
    
    /**
     * Sets the trigger entry state.
     *
     * @param[in] aState The trigger entry state.
     */
    IMPORT_C void SetState( TLbtTriggerState aState );

    /**
     * Gets requestors of the trigger entry.
     * 
     * Requestor information is used by the Location Triggering Server to
     * verify that the user allows location information to be sent to 
     * the specified requestors when a trigger fires.
     *
     * Requestors attribute is a mandatory trigger attribute. If the requestors
     * attribute is not set when the trigger is created in the server, the
     * trigger creation will fail.
     *
     * If requestors have not been set before, this function returns
     * an empty requestor stack.
     *
     * Refer to Location Acquisition API for detailed information on 
     * requestors.
     *
     * @see RLbt::CreateTriggerL()
     * @see RRequestorStack
     *
     * @param[out] aRequestors The requestors of the trigger entry.
     */
    IMPORT_C void GetRequestorsL( RRequestorStack& aRequestors ) const;
    
    /**
     * Sets requestors of trigger entry.
     *
     * Requestor information is used by the Location Triggering Server to
     * verify that the user allows location information to be sent to 
     * the specified requestors when a trigger fires.
     *
     * Requestors attribute is a mandatory trigger attribute. If the requestors
     * attribute is not set when the trigger is created in the server, the
     * trigger creation will fail.
     *
     * Refer to Location Acquisition API for detailed information on 
     * requestors.
     *
     * @see RLbt::CreateTriggerL()
     *
     * @param[in] aRequestors The requestors of trigger entry.
     * @leave KErrArgument The stack contains no requestors.
     */
    IMPORT_C void SetRequestorsL( const RRequestorStack& aRequestors );
    
    /**
     * Sets requestor of trigger entry.
     *
     * Requestor information is used by the Location Triggering Server to
     * verify that the user allows information to be sent to the specified
     * requestors when a trigger fires.
     *
     * Requestors attribute is a mandatory trigger attribute. If the requestors
     * attribute is not set when the trigger is created in the server, the
     * trigger creation will fail.
     *
     * @see RLbt::CreateTriggerL()
     *
     * @param[in] aType identifies the type of requestor, a service or a contact.
     * @param[in] aFormat determines the type of data held in aData
     * @param[in] aData is requestor data. Can be a telephone number, a URL etc.
     */
    IMPORT_C void SetRequestorL( 
        CRequestor::TRequestorType aType,
        CRequestor::TRequestorFormat aFormat,
        const TDesC& aData );

    /**
     * Gets UID of the manager UI. The UID means 
     * the UID3 value, which identifies the particular application. 
     * If the UID of the manager UI is not set, this function 
     * returns KNullUid. 
     *
     * @return The UID of manager UI application. KNullUid if the UID
     * has not been set.
     */
    IMPORT_C TUid ManagerUi() const;

    /**
     * Sets UID of the manager UI.
     * 
     * @param[in] aUid The SID value if available. Else the UID3 value, which is 
     * the idenfifier of the particaular application. KNullUid can be used to 
     * remove previous setting.
     */
    IMPORT_C void SetManagerUi( TUid aUid );

    /**
     * Gets the trigger condition.
     *
     * The ownership of the returned trigger condition object is 
     * not transferred to the client. The pointer can be 
     * used to modify the trigger condition.
     *
     * @return A non-const pointer to the trigger's 
     * trigger condition. NULL if the trigger condition is not set. 
     */
    IMPORT_C CLbtTriggerConditionBase* GetCondition();
    
    /**
     * Gets the trigger condition.
     * 
     * The ownership of the returned trigger condition object is 
     * not transferred to the client. The pointer can be 
     * used to modify the trigger condition.
     *
     * @return A const pointer to the trigger's 
     * trigger condition. NULL if the trigger condition is not set. 
     */
    IMPORT_C const CLbtTriggerConditionBase* GetCondition() const;
    
    /**
     * Sets trigger condition.
     *
     * This object takes the ownership of the trigger condition object.
     * 
     * @param[in] aCondition Pointer to the new trigger condition object.
     * This object takes the ownership of aCondition. NULL can be used
     * to remove previous setting. 
     */
    IMPORT_C void SetCondition( CLbtTriggerConditionBase* aCondition );

    /**
     * Internalizes the trigger object's details and attributes 
     * from stream.
     *
     * The presence of this function means that the standard template 
     * operator>>() ( defined in s32strm.h ) is available to internalize objects 
     * of this class.
     *
     * @param[in] aStream Stream from which the object should be internalized.
     */
    IMPORT_C void InternalizeL( RReadStream& aStream );
    
    /**
     * Externalizes the trigger object's details and attributes
     * to stream.
     *
     * The presence of this function means that the standard template 
     * operator<<() ( defined in s32strm.h ) is available to externalize objects 
     * of this class.
     *
     * @param[in] aStream Stream to which the object should be externalized.
     */
    IMPORT_C void ExternalizeL( RWriteStream& aStream ) const;
   
protected:
    /**
     * Internalize method that subclass must implement.
     * @param[in] aStream Stream from which the object should be internalized.
     */
    virtual void DoInternalizeL( RReadStream& aStream ) = 0;
    
    /**
     * Externalize method that subclass must implement.
     * @param[in] aStream Stream to which the object should be externalized.
     */
    virtual void DoExternalizeL( RWriteStream& aStream ) const = 0;
    
    /**
     * Constructor.
     */
    CLbtTriggerEntry();

private:
    /**
     * By default, prohibit copy constructor
     */
    CLbtTriggerEntry( const CLbtTriggerEntry& );
    
    /**
     * Prohibit assigment operator
     */
    CLbtTriggerEntry& operator= ( const CLbtTriggerEntry& );

private: //data
    /**
     * Id
     */
    TLbtTriggerId iId;
    
    /**
     * Name
     */
    HBufC* iName;
    
    /**
     * State
     */
    TLbtTriggerState iState; 
    
    /**
     * Rearm time
     */
    TInt iRearmTime;
    
    /**
     * Requestors
     */
    RRequestorStack iRequestor;
    
    /**
     * manager UI
     */
    TUid iManagerUi;
    
    /**
     * Trigger condition
     */
    CLbtTriggerConditionBase* iTriggerCondition;
    
    /**
     * Reserved for schedule information
     */
    TAny* iSchedule; 
    
    /**
     * Reserved pointer for future extension
     */
    TAny* iReserved;
    };


#endif // LBTTRIGGERENTRY_H

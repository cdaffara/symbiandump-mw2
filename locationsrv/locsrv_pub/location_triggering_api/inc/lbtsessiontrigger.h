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
* Description:  Concrete class representing session trigger entries.
*
*/


#ifndef LBTSESSIONTRIGGER_H
#define LBTSESSIONTRIGGER_H

#include <lbttriggerentry.h>
#include <lbs.h>

/**
 * Concrete class representing session trigger entries.
 *
 * Session triggers are not stored in persistent
 * storage. They are deleted by Location Triggering Server if the 
 * client application's subsession to the server is closed 
 * or if the client calls RLbt::DeleteTriggerL(). 
 *
 * Client application shall make request to Location
 * Triggering Server to receive session trigger firing event. 
 *
 * @see RLbt
 *
 * @lib lbt.lib
 *
 * @since S60 5.1
 */
class CLbtSessionTrigger : public CLbtTriggerEntry
    {
public:
    /**
     * Allocates and constructs a new session trigger entry.
     *
     * In returned object, default values are set to the
     * attributes of the trigger. The default values are
     *  - Trigger Id is KLbtNullTriggerId.
     *
     *  - Trigger Name is an empty string.
     *
     *  - Trigger State is CLbtTriggerEntry::EStateEnabled.
     *  
     *  - Requestors are not set. 
     *
     *  - Manager UI is not set(KNullUid). 
     *
     *  - Trigger condition is not set.
     *
     *
     * @return Pointer to the new session trigger entry.
     */
    IMPORT_C static CLbtSessionTrigger* NewL();

    /**
     * Allocates and constructs a new session trigger entry. The 
     * constructed object is pushed onto cleanup stack.
     *
     * In returned object, default values are set to the
     * attributes of the trigger. The default values are
     *  - Trigger Id is KLbtNullTriggerId.
     *
     *  - Trigger Name is an empty string.
     *
     *  - Trigger State is CLbtTriggerEntry::EStateEnabled.
     *  
     *  - Requestors are not set. 
     *
     *  - Manager UI is not set(KNullUid). 
     *
     *  - Trigger condition is not set.
     *
     *
     * @return Pointer to the new session trigger entry.
     */
    IMPORT_C static CLbtSessionTrigger* NewLC();


    /**
     * Allocates and constructs a new session trigger entry
     * with specified attribute values.
     *
     * @since S60 5.1
     *
     * @param[in] aName The name of the trigger entry.
     * @param[in] aState The state of trigger entry.
     * @param[in] aRequestorType Identifies the type of requestor, 
     * a service or a contact.
     * @param[in] aRequestorFormat Determines the type of data held in 
     * aRequestorData
     * @param[in] aRequestorData Requestor data. Can be a telephone 
     * number, a URL etc.
     * @param[in] aManagerUi The UID of the manager UI application. 
     * @param[in] aCondition Pointer to the new trigger condition object.
     * This object takes the ownership of aCondition.
     * @return Pointer to the new session trigger entry.
     *
     * @leave KErrArgument If the name of the trigger is longer than
     *   @p KLbtMaxNameLength.
     * @leave Other standard symbian error code, such as KErrNoMemory,
     * KErrGeneral, etc.
     */
    IMPORT_C static CLbtSessionTrigger* NewL( 
        const TDesC& aName,
        TLbtTriggerState aState,
        CRequestor::TRequestorType aRequestorType,
        CRequestor::TRequestorFormat aRequestorFormat,
        const TDesC& aRequestorData,
        TUid aManagerUi,
        CLbtTriggerConditionBase* aCondition );
        
    /**
     * Allocates and constructs a new session trigger entry
     * with specified attribute values.
     *
     * @param[in] aName The name of the trigger entry.
     * @param[in] aState The state of trigger entry.
     * @param[in] aRequestors The requestor for the service. This
     * object does not take ownership of aRequestors.
     * @param[in] aManagerUi The UID of manager UI application.
     * @param[in] aCondition Pointer to the new trigger condition object.
     * This object takes the ownership of aCondition.
     * @return Pointer to the new session trigger entry.
     *
     * @leave KErrArgument If the name of the trigger is longer than
     *   @p KLbtMaxNameLength.
     * @leave Other standard symbian error code, such as KErrNoMemory,
     * KErrGeneral, etc.
     */
    IMPORT_C static CLbtSessionTrigger* NewL( 
        const TDesC& aName,
        TLbtTriggerState aState,
        const RRequestorStack& aRequestors,
        TUid aManagerUi,
        CLbtTriggerConditionBase* aCondition );

    /**
     * Destructor
     */
    IMPORT_C ~CLbtSessionTrigger();
    
    /**
     * Get the type of the trigger entry, CLbtTriggerEntry::ESession.
     *
     * @return CLbtTriggerEntry::ESession.
     */
    IMPORT_C TType Type() const;
    
	
    
protected:
    /**
     * Externalize method that subclass must implement.
     * @param[in] aStream Stream to which the object should be externalized.
     */
    virtual void DoExternalizeL(RWriteStream& aStream) const ;
    
    /**
     * Internalize method that subclass must implement.
     * @param[in] aStream Stream from which the object should be internalized.
     */
    virtual void DoInternalizeL(RReadStream& aStream)  ;
    

private:
    
    /**
     * Symbian 2nd phase constructor.
     *
     * @param[in] aName The name of the trigger entry.
     * @param[in] aState The state of trigger entry.
     * @param[in] aRequestorType Identifies the type of requestor, 
     * a service or a contact.
     * @param[in] aRequestorFormat Determines the type of data held in 
     * aRequestorData
     * @param[in] aRequestorData Requestor data. Can be a telephone 
     * number, a URL etc.
     * @param[in] aManagerUi The UID of the manager UI application. 
     * @param[in] aCondition Pointer to the new trigger condition object.
     */
    void ConstructL(const TDesC& aName,TLbtTriggerState aState,
        CRequestor::TRequestorType aRequestorType,
        CRequestor::TRequestorFormat aRequestorFormat,
        const TDesC& aRequestorData,
        TUid aManagerUi,
        CLbtTriggerConditionBase* aCondition);
    /**
     * Symbian 2nd phase constructor.
     *
     * @param[in] aName The name of the trigger entry.
     * @param[in] aState The state of trigger entry.
     * @param[in] aRequestor The requestor for the service. This
     * object does not take ownership of aRequestor.
     * @param[in] aManagerUi The UID of manager UI application.
     * @param[in] aCondition Pointer to the new trigger condition object.    
     */
    void ConstructL(const TDesC& aName,
        CLbtTriggerEntry::TLbtTriggerState aState,
        const RRequestorStack& aRequestors,
        TUid aManagerUi,
        CLbtTriggerConditionBase* aCondition);
   
   /**
    * Symbian 2nd phase constructor.
    */     
   void ConstructL();         
   
   /**
    * Default constructor
    */ 
   CLbtSessionTrigger();
   
   /**
    * By default, prohibit copy constructor
    */
   CLbtSessionTrigger(  CLbtSessionTrigger& aEntry);
   
   /**
    * Prohibit assigment operator
    */ 
   CLbtSessionTrigger& operator= ( const CLbtSessionTrigger& );

    };


#endif // LBTSESSIONTRIGGER_H

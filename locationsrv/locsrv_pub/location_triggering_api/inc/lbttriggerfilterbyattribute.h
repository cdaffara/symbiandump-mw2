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
* Description:  Class for filter based on the attributes value of a trigger. 
*
*/


#ifndef LBTTRIGGERFILTERBYATTRIBUTE_H
#define LBTTRIGGERFILTERBYATTRIBUTE_H

#include <lbttriggerfilterbase.h>
#include <lbttriggerentry.h>
#include <lbttriggerdynamicinfo.h>

/**
 * Class representing filter based on the attribute values of a trigger. 
 *
 * Following attributes of triggers can be used in the filter when
 * listing triggers from Location Triggering Server.
 *
 * -  <B>Trigger Type</B>. It specifies what type of triggers shall be 
 * retrieved. By default, the trigger type is not used in filtering. 
 * If one or more trigger types are added 
 * to the filter, only triggers matching the specified 
 * type will be retrieved.
 *
 * -  <B>Trigger State</B>. It specifies the state of the interested 
 * trigger entries. By default, trigger state is not used in
 * filtering. If one or more states are added in the filter, only
 * trigger entries with the specified states will be retrieved.
 *
 * -  <B>Trigger Validity</B>. It specifies the validity status
 * that the retrieved triggers must have. By default, trigger validity 
 * status is not used in filtering.  If one or more trigger validity status
 * is added to the filter, only trigger entries with the specified 
 * validity status will be retrieved.
 *
 * - <B>Manager UI</B>. It specifies the manager UI of the interested 
 * trigger entries. But default, manager UI attribute is not used in
 * filtering. If one or more manager UI UIDs are added in the filter,
 * only trigger entries that their manager UI UIDs are defined in the 
 * filter will be retrieved. KNullUid can also be used to fetch
 * those triggers that have no manager UI set. 
 * 
 * - <B>Id</B>. It specified the Id of the interested trigger entries. 
 * By default, trigger ID attribute is not used in filtering. If one or
 * more Ids are added in the filter, only trigger entries that their IDs 
 * are defined in the filter will be retrieved. 
 * 
 * @lib lbt.lib
 * @since S60 5.1
 */
class CLbtTriggerFilterByAttribute : public CLbtTriggerFilterBase
    {
public:
    /**
     * Returns CLbtTriggerFilterBase::EFilterByAttribute.
     *
     * @return CLbtTriggerFilterBase::EFilterByAttribute
     */
    IMPORT_C virtual TFilterType Type() const;

    /**
     * Constructs a new instance of CLbtTriggerFilterByAttribute. 
     * In the returned object, trigger type, trigger state 
     * trigger validity status,  manager UI and trigger Id are not 
     * used in filtering. 
     *
     * @return New instance of CLbtTriggerFilterByAttribute.
     */
    IMPORT_C static CLbtTriggerFilterByAttribute* NewL();

    /**
     * Constructs a new instance of CLbtTriggerFilterByAttribute
     * and pushes it onto cleanup stack. 
     * In the returned object, trigger type, trigger state 
     * trigger validity status,  manager UI and trigger Id are not 
     * used in filtering. 
     *
     * @return New instance of CLbtTriggerFilterByAttribute.
     */
    IMPORT_C static CLbtTriggerFilterByAttribute* NewLC();

    /**
     * Destructor.
     */
    IMPORT_C virtual ~CLbtTriggerFilterByAttribute();
    
    /**
     * Resets the filter. The trigger type, trigger state,
     * trigger validity, manager UI and trigger Id are not used
     * in filtering.
     */
    IMPORT_C void ResetFilter();
    
    /**
     * Adds a trigger type to the filter.
     *
     * @param[in] aType The type of the trigger to be retrieved.
     */
    IMPORT_C void AddTriggerTypeL( 
        CLbtTriggerEntry::TType aType ); 
        
    /**
     * Tests if the specified trigger type is used in the filter.
     * 
     * @param[in] aType The trigger type used in the filter.
     * @return ETrue if the the trigger type is used in filter.
     * Otherwise, EFalse is returned.
     */
    IMPORT_C TBool IsTriggerTypeInFilter( 
        CLbtTriggerEntry::TType aType ) const;
        
        
    /**
     * Adds a trigger state to the filter.
     *
     * @param[in] aState The trigger state used in the filter.
     */
    IMPORT_C void AddTriggerStateL( 
        CLbtTriggerEntry::TLbtTriggerState aState );
        
    
    /**
     * Tests if the specified trigger state is in the filter.
     *
     * @param[in] aState The trigger state to be tested.
     * @return ETrue if the the trigger state is used in filter.
     * Otherwise, EFalse is returned.
     */
    IMPORT_C TBool IsTriggerStateInFilter( 
        CLbtTriggerEntry::TLbtTriggerState aState ) const;
        
    
    /**
     * Adds a trigger validity status to the filter.
     *
     * @param[in] aValidity The validity status value used in the filter.
     */
    IMPORT_C void AddTriggerValidityL( 
        TLbtTriggerDynamicInfo::TLbtTriggerValidity aValidity );
        
    /**
     * Tests if the trigger validity status value is in the filter.
     *
     * @param[in] aValidity The validity status value to be tested.
     *
     * @return ETrue if the the trigger validity status is used in filter.
     * Otherwise, EFalse is returned.
     */
    IMPORT_C TBool IsTriggerValidityStatusInFilter( 
        TLbtTriggerDynamicInfo::TLbtTriggerValidity aValidity ) const;

    /**
     * Adds a manager UI UID to the filter.
     *
     * @param[in] aUid The UID of the manager UI to be used in the filter.
     */
    IMPORT_C void AddTriggerManagerUiL(
        TUid aUid );

    /**
     * Tests if the specified UID of a manager UI is used in the filter.
     * 
     * @param[in] aUid The UID of a manager UI to be tested.
     * 
     * @return ETrue if the specified UID is used in the filter. Otherwise, 
     * EFalse is returned.
     */
    IMPORT_C TBool IsTriggerManagerUiInFilter(
        TUid aUid ) const;

    /**
     * Adds a trigger Id to the filter.
     *
     * @param[in] aId The trigger Id to be used in the filter.
     */
    IMPORT_C void AddTriggerIdL(
        TLbtTriggerId aId );

    /**
     * Tests if the specified trigger Id is used in the filter.
     *
     * @param[in] aId the trigger Id to be tested.
     *
     * @return ETrue if the specified trigger Id is used in the filter.
     * Otherwise, EFalse is returned.
     */
    IMPORT_C TBool IsTriggerIdInFilter(
        TLbtTriggerId aId ) const;
    
    /**
     * Gets trigger states that have been used in this filter
     *
     * @param[out]  aTriggerStateArray  On return contains trigger states that
     * have been used in this filter.
     */ 
    IMPORT_C void GetStateArrayL( RArray <CLbtTriggerEntry::TLbtTriggerState>& aTriggerStateArray) const;
    
    /**
     * Gets trigger entry types that have been used in this filter. 
     *
     * @param[out] aTriggerTypeArray  On return contains trigger types that have
     * been used in this filter.  
     */
    IMPORT_C void GetTypeArrayL( RArray <CLbtTriggerEntry::TType>& aTriggerTypeArray) const;
    
    /**
     * Gets trigger validity status that has been used in this filter. 
     *
     * @param[out]  aTriggerValidityArray  On return, contains trigger validity status 
     * that have been used in this filter.  
     */
    IMPORT_C void GetValidityArrayL( RArray < TLbtTriggerDynamicInfo::TLbtTriggerValidity >& aTriggerValidityArray) const;
    
    /**
     * Gets manager UI UIDs that have been used in this filter. 
     *
     * @param[out] aManagerUiArray  On return, contains manager UI UIDs that
     * have been used in this filter.  
     */
    IMPORT_C void GetManagerUiArrayL( RArray < TUid >& aManagerUiArray) const;
    
    /**
     * Gets trigger IDs that have been used in this filter. 
     *
     * @param[out] aIdArray  On return contains trigger IDs that have been used in this filter.  
     */
    IMPORT_C void GetTriggerIdArrayL( RArray < TLbtTriggerId >& aIdArray) const;
    

protected:    
    
    /**
     * Internalize method that subclass must implement.
     * @param[in] aStream Stream from which the object should be internalized.
     */
    virtual void DoInternalizeL( RReadStream& aStream ) ;
    
    /**
     * Externalize method that subclass must implement.
     * @param[in] aStream Stream to which the object should be externalized.
     */
    virtual void DoExternalizeL( RWriteStream& aStream ) const ;

private:
    /**
     * Symbian 2nd phase constructor.
     */
    void ConstructL();    
    
    /**
     * Default constructor.
     */
    CLbtTriggerFilterByAttribute();
    
    /**        
     * By default, prohibit copy constructor
     */
    CLbtTriggerFilterByAttribute( const CLbtTriggerFilterByAttribute& );
    
    /**
     * Prohibit assigment operator
     */
    CLbtTriggerFilterByAttribute& operator= ( const CLbtTriggerFilterByAttribute& );

private:
    /**
     * The type of the trigger to be listed
     */
    RArray < CLbtTriggerEntry::TType > iTriggerTypeArray;

    /**
     * Trigger state
     */
    RArray < CLbtTriggerEntry::TLbtTriggerState > iTriggerStateArray;

    /**
     * Trigger validity
     */
    RArray < TLbtTriggerDynamicInfo::TLbtTriggerValidity > iTriggerValidityArray;

    /**
     * Manager UI array
     */
    RArray < TUid > iManagerUiArray;

    /**
     * Id array
     */
    RArray < TLbtTriggerId > iIdArray;

    /**
     * Reserved
     */
    TUint iReserved[8];

    };

#endif // LBTTRIGGERFILTERBYATTRIBUTE_H

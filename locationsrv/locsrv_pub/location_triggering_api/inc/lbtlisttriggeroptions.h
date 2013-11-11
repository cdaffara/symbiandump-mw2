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
* Description:  Class of options used in listing triggers
*
*/


#ifndef LBTLISTTRIGGEROPTIONS_H
#define LBTLISTTRIGGEROPTIONS_H

#include <lbtcommon.h>
#include <lbtgeoareabase.h>

class CLbtTriggerFilterBase;

/**
 * Helper class for options used when listing trigger entries 
 * from Location Triggering Server.
 *
 * The following options can be set.
 *
 * -  <B>Retrieved fields</B>. It specifies what trigger 
 * entry attributes field and trigger dynamic information field 
 * shall be filled in the returned trigger objects. 
 * Default value for trigger entry attribute field is 
 * KLbtTriggerAttributeFieldsAll, which means 
 * all attributes field shall be filled. Whether the Trigger 
 * ID field is specified or not, the trigger ID attribute is always valid
 * in returned trigger objects. Default value for trigger dynamic 
 * information field is KLbtTriggerDynInfoFieldsAll, which means all 
 * dynamic information fields will be filled.
 *
 * -  <B>Trigger filter</B>. It specifies the filter used in retrieving 
 * trigger entries. Retrieved triggers shall fulfill the criteria defined 
 * in the filter. By default, the option does not contain a filter
 * and all triggers will be retrieved. A composite filter can also be
 * used.
 *
 * -  <B>Sorting option</B>. It specifies in which order the retrieved
 * trigger shall be sorted. The default value is ELbtNoSorting. 
 * 
 * @lib lbt.lib
 *
 * @since S60 5.1
 */
class CLbtListTriggerOptions : public CBase
    {
public:
    /**
     * Sorting order when retrieving triggers
     * from Location Triggering Server.
     */
    enum TLbtListTriggerSorting
        {
        /**
         * Result is not sorted.
         */
        ELbtNoSorting = 1,

        /**
         * Result is in ascending order according to the trigger name.
         */
        ELbtTriggerNameAscending = 2,

        /**
         * Result is in descending order according to the trigger name.
         */
        ELbtTriggerNameDescending = 3,

        /**
         * Result is in ascending order according to the distance from 
         * the trigger area to the latest acquired location by Location
         * Triggering Server.
         */
        ELbtDistanceToLatestLocationAscending = 4
        };


public:

    /**
     * Constructs a new instance of trigger listing option object.
     * In the returned object, retrieved fields is set 
     * to KLbtTriggerAttributeFieldsAll and KLbtTriggerDynInfoFieldsAll, 
     * no trigger filters is set and sorting option is set to 
     * ELbtNoSorting.
     *
     * @return Pointer to the new instance of the trigger 
     * retrieving option object.
     */
    IMPORT_C static CLbtListTriggerOptions* NewL();

    /**
     * Constructs a new instance of trigger listing option object and 
     * pushes it onto cleanup stack.
     * In the returned object, retrieved fields is set 
     * to KLbtTriggerAttributeFieldsAll and KLbtTriggerDynInfoFieldsAll, 
     * no trigger filters is set and sorting option is set to 
     * ELbtNoSorting.
     *
     * @return Pointer to the new instance of the trigger 
     * retrieving option object.
     */
    IMPORT_C static CLbtListTriggerOptions* NewLC();
    
    /**
     * Destructor
     */
    IMPORT_C ~CLbtListTriggerOptions();

    /**
     * Gets the fields that Location Triggering Server will fill when
     * list triggers 
     *
     * If the entry fields mask and dynamic information field mask 
     * is not set before, KLbtTriggerAttributeFieldsAll and 
     * KLbtTriggerDynInfoFieldsAll
     * are returned.
     *
     * @param[out] aEntryMask On return contains the field mask defines
     * which trigger entry fields shall be filled.
     * @param[out] aDynInfoMask On return contains the field mask defines
     * which trigger dynamic information field shall be filled.
     */
    IMPORT_C void GetRetrievedFields( 
        TLbtTriggerAttributeFieldsMask &aEntryMask,
        TLbtTriggerDynamicInfoFieldsMask &aDynInfoMask ) const;

    /**
     * Sets which fields shall be filled by Location
     * Triggering Server when listing triggers.
     *
     * @param[in] aEntryMask Mask defines which trigger entry fields shall 
     * be filled.
     * @param[in] aDynInfoMask The mask defines which dynamic information
     * field shall be filled.
     */
    IMPORT_C void SetRetrievedFields( 
        TLbtTriggerAttributeFieldsMask aEntryMask,
        TLbtTriggerDynamicInfoFieldsMask aDynInfoMask = KLbtTriggerDynInfoFieldsAll );

    /**
     * Gets the sorting option when listing triggers. This
     * function returns ELbtNoSorting if the sorting option has
     * not been set.
     *
     * @return The sorting option when listing triggers.
     */
    IMPORT_C TLbtListTriggerSorting SortingOption() const;

    /**
     * Sets the sorting option when listing triggers. 
     *
     * @param[in] aOption The sorting option when retrieving triggers.
     */
    IMPORT_C void SetSortingOption( TLbtListTriggerSorting aOption );

    /**
     * Sets the filter used for listing triggers.
     *
     * @param[in] aFilter Pointer to the filter used for
     * listing triggers. If the pointer is NULL, previous set 
     * filter is deleted. Ownership of aFilter is transferred to
     * the client application.
     */
    IMPORT_C void SetFilter( 
        CLbtTriggerFilterBase* aFilter );

    /**
     * Gets pointer to the filter used for listing triggers.
     * The function returns NULL if the filter is not previously set.
     *
     * @return The filter used for listing triggers. Ownership
     * of the returned object is not transferred to the client
     * application.
     */
    IMPORT_C CLbtTriggerFilterBase* Filter();
    
    /**
     * Internalizes the trigger condition object's details and attributes 
     * from stream.
     *
     * The presence of this function means that the standard templated 
     * operator>>() ( defined in s32strm.h ) is available to internalize objects 
     * of this class.
     *
     * @param[in] aStream Stream from which the object should be internalized.
     */
    IMPORT_C void InternalizeL( RReadStream& aStream );
    
    /**
     * Externalizes the trigger condition object's details and attributes
     * to stream.
     *
     * The presence of this function means that the standard templated 
     * operator<<() ( defined in s32strm.h ) is available to externalize objects 
     * of this class.
     *
     * @param[in] aStream Stream to which the object should be externalized.
     */
    IMPORT_C void ExternalizeL( RWriteStream& aStream ) const;
    
private:
    /**
     * Default Constructor.
     */
    CLbtListTriggerOptions();
    
    /**
     * By default, prohibit copy constructor
     */
    CLbtListTriggerOptions( const CLbtListTriggerOptions& );
    
    /**
     * Prohibit assigment operator
     */
    CLbtListTriggerOptions& operator= ( const CLbtListTriggerOptions& );
    
    /**
     * Symbian 2nd phase constructor
     */
    void ConstructL();
  
private:
    /**
     * Trigger entry fields mask
     */
    TLbtTriggerAttributeFieldsMask iTriigerAttributeFieldsMask;

    /**
     * Trigger dynamic information field mask
     */
    TLbtTriggerDynamicInfoFieldsMask  iDynInfoFieldsMask;

    /**
     * Filter
     */
    CLbtTriggerFilterBase* iFilter;

    /**
     * Sorting
     */
    TLbtListTriggerSorting iSorting;
    
    /**
     * Reserved
     */
    TUint iReserved[8];
    };

#endif // LBTLISTTRIGGEROPTIONS_H

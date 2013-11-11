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


#ifndef LBTCONTAINERTRIGGERFILTERBYATTRIBUTE_H
#define LBTCONTAINERTRIGGERFILTERBYATTRIBUTE_H

#include <lbttriggerfilterbyattribute.h>
#include <lbttriggerentry.h>
#include <lbttriggerdynamicinfo.h>
#include <lbttriggerfilterbase.h>

#include "lbtcontainertriggerentry.h"
#include "lbtcontainerextendedtriggerinfo.h"

/**
 * Class representing filter based on the additiona attributes  of a trigger. 
 *
 * Following trigger entry's attribute can be used in the filter when
 * listing triggers from Location Triggering Server.
 *
 * -  Hysteresis area. It specifies the hysteresis radius of the trigger.
 *
 * -  Rectangular Trigger Area. It specifies the trigger area in a rectangular form.
 * -  Trigger Fired Information. It specifies whether the trigger has fired or not.
 *
 * @lib lbt.lib
 * @since S60 4.0
 */


NONSHARABLE_CLASS( CLbtContainerFilterByAttribute ) : public CLbtTriggerFilterByAttribute
    {
public:
    /**
     * Constructs a new instance of CLbtContainerFilterByAttribute. 
     * In the returned object, trigger type, trigger state and
     * trigger validity status is set to unused. 
     *
     * @return New instance of CLbtContainerFilterByAttribute.
     */
    IMPORT_C static CLbtContainerFilterByAttribute* NewL();

    /**
     * Destructor.
     */
    IMPORT_C virtual ~CLbtContainerFilterByAttribute();
        
    /**
     * Resets the filter. The trigger type, trigger state
     * and trigger validity are set to unused.
     */
    IMPORT_C void ResetFilter();
        
    /**
     * Adds a hysteresis radius to the filter.
     *
     * @param[in] aRadius The type of the trigger to be retrieved.
     */
    IMPORT_C void AddHysteresisRadiusInFilter( 
        TReal aRadius ); 
            
    /**
     * Tests if the specified hysteresis radius is used in the filter.
     * 
     * @param[in] aRadius The trigger type used in the filter.
     * @return ETrue if the the trigger type is used in filter.
     * Otherwise, EFalse is returned.
     */
    IMPORT_C TBool IsHysteresisRadiusInFilter( 
        TReal aRadius ) const;
            
            
    /**
     * Adds a trigger area to the filter.
     *
     * @param[in] aState The trigger state used in the filter.
     */
    IMPORT_C void AddRectTrigAreaInFilterL( 
        CLbtExtendedTriggerInfo::TLbtTriggerRectArea aArea );
            
        
    /**
     * Tests if the specified rectangular area is in the filter.
     *
     * @param[in] aArea The trigger rectangular area.
     * @return ETrue if the the trigger state is used in filter.
     * Otherwise, EFalse is returned.
     */
    IMPORT_C TBool IsRectTriggerAreaInFilter( 
        CLbtExtendedTriggerInfo::TLbtTriggerRectArea aArea ) const;
            
        
    /**
     * Adds a trigger  firing information to the filter
     *
     * @param[in] aValidity The validity status value used in the filter.
     */
    IMPORT_C void AddFiredInfoInFilterL( 
        TBool aFired );
            
    /**
     * Tests if the trigger  firing information is in the filter.
     *
     * @param[in] aFired The validity status value used in the filter.
     *
     * @return ETrue if the the trigger firing information is used in filter.
     * Otherwise, EFalse is returned.
     */
    IMPORT_C TBool IsTriggerFiredInfoInFilter( 
        TBool aFired ) const;
            
    /**
     * Adds a secure Id information to the filter
     *
     * @param[in] aSid The Secure Id value that needs to be used in the filter.
     */
    IMPORT_C void AddOwnerSidInFilterL( 
        TSecureId aSid );
            
    /**
     * Tests if a secure Id information is present in the filter.
     *
     * @param[in] aSid The Secure Id value that needs to be chcked in the filter.
     *
     * @return ETrue if the specified Secure Id is used in filter.
     * Otherwise, EFalse is returned.
     */
    IMPORT_C TBool IsOwnerSidInFilter( 
        TSecureId aSid ) const;
        
private:
        
    /**
     * Constructor
     */
    CLbtContainerFilterByAttribute();
                
    /**
     * Symbian 2nd Phase Constructor
     */
    void ConstructL();

private:
    /**
     * The type of the trigger to be listed
     */
    RArray < TReal > iHystRadiusArray;

    /**
     * Trigger state
     */
    RArray < CLbtExtendedTriggerInfo::TLbtTriggerRectArea > iTriggerRectAreaArray;

    /**
     * Trigger validity
     */
    RArray < TBool > iTriggerFiredArray;

    /**
     * Trigger SID value
     */
    RArray< TSecureId > iSidArray;
         
    //Reserved
    TUint iReserved[8];

    };

#endif // LBTTRIGGERFILTERBYATTRIBUTE_H

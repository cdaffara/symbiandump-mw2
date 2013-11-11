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


#ifndef C_LBT_CONTAINER_UPDATE_FILTER_H
#define C_LBT_CONTAINER_UPDATE_FILTER_H

#include <lbtcommon.h>
#include <lbttriggerfilterbase.h>
#include <lbttriggerentry.h>
#include <lbtlisttriggeroptions.h>

typedef TUint64 TLbtTriggerDataMask;

// forward declaration
class CLbtContainerFilter;

class CLbtExtendedTriggerInfo;
   

       	
/* This is the main Trigger Information class used to set and access trigger information in the container.
* It is derived from the CLbtTriggerInfo class from the Location Triggering API.
*/
       	
NONSHARABLE_CLASS( CLbtContainerUpdateFilter ) :public CBase
    {
public:
    
        
    /**
     * Constructs a new instance of container trigger entry object.
     *
     * @return The new instance of the container trigger entry object.
     */
    IMPORT_C static CLbtContainerUpdateFilter* NewL();

    /**
     * Constructs a new instance of container trigger entry object with the 
     * given parameters.
     *
     * @param[in] aEntry The pointer to the trigger entry. 
     * Ownership is transferred.
     * @param[in] aDynInfo The pointer to the trigger dynamic 
     * information object. Ownership is transferred.
     * @param[in] aExtendedTriggerInfo The pointer to the trigger derived dynamic 
     * information object. Ownership is transferred.
     *
     * @return The new instance of trigger information object.
     */
    IMPORT_C static CLbtContainerUpdateFilter* NewL(
                        CLbtTriggerFilterBase* aTrigInfoFilter,
                        CLbtContainerFilter* aContFilter );                
       
    /**
     * Destructor
     */
    IMPORT_C virtual ~CLbtContainerUpdateFilter();

    /**
     * Gets trigger entry object.
     *
     * @return Pointer to the trigger entry object. Ownership is
     * not transferred.
     */
    IMPORT_C CLbtTriggerFilterBase* TrigInfoFilter();

    /**
     * Sets trigger entry.
     *
     * @param[in] aEntry The trigger entry object. Ownership is transferred.
     */
    IMPORT_C void SetTrigInfoFilter( CLbtTriggerFilterBase* aTrigInfoFilter );

    
    /**
     * Gets extended trigger information object.
     *
     * @return Pointer to the extended trigger information object.
     * Ownership is not transferred.
     */
    IMPORT_C CLbtContainerFilter* ContainerFilter();
		
    /**
     * Sets extended trigger information object.
     *
     * @param[in] aDynInfo Pointer to the extended trigger
     * information object. Ownership is transferred.
     */
    IMPORT_C void SetContainerFilter( 
                        CLbtContainerFilter* aContFilter );
                        
                        
     IMPORT_C TLbtTriggerDataMask DataMask();
     
     IMPORT_C void SetDataMask( TLbtTriggerDataMask aMask);
                        
	
private:
    /**
     * Constructor.
     */
 	  CLbtContainerUpdateFilter();
        
     
    /**
     * Symbian 2nd phase of Construction.
     */
    void ConstructL();

    /**
     * Symbian 2nd phase of Construction.
     */
    void ConstructL(
                    CLbtTriggerFilterBase* aTrigInfoFilter,
                    CLbtContainerFilter* aContFilter ); 
                    
     void CloneL(CLbtContainerUpdateFilter& aEntry);              
       
private:
    /**
     * Filter from the Client
     */
    CLbtTriggerFilterBase* iTrigInfoFilter;

    TLbtTriggerDataMask iMask; 
    
    /**
     * Derived Trigger dynamic info
     */
    CLbtContainerFilter *iContFilter;
    
    // Reserved pointer for future extension
    TAny* iReserved;
    };


#endif // C_LBT_CONTAINER_UPDATE_FILTER_H

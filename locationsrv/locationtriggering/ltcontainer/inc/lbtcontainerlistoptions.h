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


#ifndef C_LBT_CONTAINER_LIST_OPTIONS_H
#define C_LBT_CONTAINER_LIST_OPTIONS_H

#include <lbtcommon.h>
#include <lbttriggerinfo.h>
#include <lbttriggerentry.h>
#include <lbtlisttriggeroptions.h>

typedef TUint64 TLbtTriggerDataMask;

// forward declaration
class CLbtContainerFilter;

class CLbtExtendedTriggerInfo;
   

       	
/* This is the main Trigger Information class used to set and access trigger information in the container.
* It is derived from the CLbtTriggerInfo class from the Location Triggering API.
*/
       	
NONSHARABLE_CLASS( CLbtContainerListOptions ) :public CBase
    {
public:
    
        
    /**
     * Constructs a new instance of container trigger entry object.
     *
     * @return The new instance of the container trigger entry object.
     */
    IMPORT_C static CLbtContainerListOptions* NewL();

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
    IMPORT_C static CLbtContainerListOptions* NewL(
                        CLbtListTriggerOptions* aListOptions,
                        CLbtContainerFilter* aFilter );                
       
    /**
     * Destructor
     */
    IMPORT_C virtual ~CLbtContainerListOptions();

    /**
     * Gets trigger entry object.
     *
     * @return Pointer to the trigger entry object. Ownership is
     * not transferred.
     */
    IMPORT_C CLbtListTriggerOptions* ListOptions();

    /**
     * Sets trigger entry.
     *
     * @param[in] aEntry The trigger entry object. Ownership is transferred.
     */
    IMPORT_C void SetListOptions( CLbtListTriggerOptions* aListOptions );

    
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
                        CLbtContainerFilter* aFilter );
                        
                        
     IMPORT_C TLbtTriggerDataMask DataMask();
     
     IMPORT_C void SetDataMask( TLbtTriggerDataMask aMask);
	
private:
    /**
     * Constructor.
     */
 	  CLbtContainerListOptions();
        
     // CLbtContainerListOptions( CLbtContainerListOptions& aEntry);   
    /**
     * Symbian 2nd phase of Construction.
     */
    void ConstructL();

    /**
     * Symbian 2nd phase of Construction.
     */
    void ConstructL(
                    CLbtListTriggerOptions* aListOptions,
                    CLbtContainerFilter* aFilter ); 
                    
     void CloneL(CLbtContainerListOptions& aEntry);              
       
private:
    /**
     * Trigger entry
     */
    CLbtListTriggerOptions* iListOptions;

    TLbtTriggerDataMask iMask; 
    
    /**
     * Derived Trigger dynamic info
     */
    CLbtContainerFilter *iFilter;
    
    // Reserved pointer for future extension
    TAny* iReserved;
    };


#endif // C_LBT_CONTAINER_LIST_OPTIONS_H

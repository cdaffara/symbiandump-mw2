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


#ifndef C_LBT_CONTAINER_TRIGGER_ENTRY_H
#define C_LBT_CONTAINER_TRIGGER_ENTRY_H

#include <lbtcommon.h>
#include <lbttriggerinfo.h>
#include <lbttriggerentry.h>

// CONSTANTS
const static TInt KVisitedCellSize(2); 

typedef TUint64 TLbtTriggerDataMask;

// forward declaration
class CLbtExtendedTriggerInfo;

// Activity status for strategy
enum TLbtTriggerActivityStatus
    {
    /**
     * Active
     */
    EActive = 1,        
    /**
     * Inactive
     */
    EInactive = 2       
    };
// Hybrid status of the trigger
enum THybridState
    {
    EHybridNone,
    
    EHybridActive,
    
    EHybridInActive 
    };

/**
 * Structure used by startegy for storing supervision
 * information
 */ 
struct TStrategyData
	{
	// Trigger hybrid status
	THybridState iHybridState;
	// The time at which the trigger fired
	TInt64 iFireTime;
	
	// The activity status of the trigger
	TLbtTriggerActivityStatus iActivityStatus;
	
	// Boundary condition for cell supervision
	TInt iBoundaryCondition;
	
	// Visited cells 
	TInt iVisitedCell[KVisitedCellSize];
	};
   

       	
/* This is the main Trigger Information class used to set and access trigger information in the container.
* It is derived from the CLbtTriggerInfo class from the Location Triggering API.
*/
       	
NONSHARABLE_CLASS( CLbtContainerTriggerEntry ) : public CLbtTriggerInfo
    {
public:
    enum TLbtContainerTriggerAttribute
        {
        EContainerAttributeRectTriggerArea=0x00010000,
        EContainerAttributeHysteresisRadius=0x00020000,
        EContainerAttributeIsFired=0x00040000,
        EContainerAttributeStrategyData=0x0080000,
        EContainerAttributeOwnerSid=0x00100000,
        EContainerDynInfoAttributeValidity=0x00200000,
        EContainerDynInfoAttributeActivity=0x00400000,
        EContainerDynInfoAttributeDistance=0x00800000,
        EContainerAttributeIsTriggerFireOnCreation=0x001000000,
        EContainerAttributeFireInfo=0X002000000,
        EContainerAttributeAll = 0xFFFFFFFF
        };
        
    /**
     * Constructs a new instance of container trigger entry object.
     *
     * @return The new instance of the container trigger entry object.
     */
    IMPORT_C static CLbtContainerTriggerEntry* NewL();

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
    IMPORT_C static CLbtContainerTriggerEntry* NewL(
                        CLbtTriggerEntry* aEntry,
                        TLbtTriggerDynamicInfo* aDynInfo,
                        CLbtExtendedTriggerInfo* aExtendedTriggerInfo );                
       
    /**
     * Destructor
     */
    IMPORT_C virtual ~CLbtContainerTriggerEntry();

  
    /**
     * Gets extended trigger information object.
     *
     * @return Pointer to the extended trigger information object.
     * Ownership is not transferred.
     */
    IMPORT_C CLbtExtendedTriggerInfo* ExtendedTriggerInfo();
		
    /**
     * Sets extended trigger information object.
     *
     * @param[in] aDynInfo Pointer to the extended trigger
     * information object. Ownership is transferred.
     */
    IMPORT_C void SetExtendedTriggerInfo( 
                        CLbtExtendedTriggerInfo* aExtendedTriggerInfo );
                        
    /**
     * 
     */
    IMPORT_C void SetStrategyData( TStrategyData& aStartegyData );    
    
    /**
     * 
     */
    IMPORT_C TStrategyData StrategyData();                  
                        
    /**
     * Copy constructor
     */                    
    CLbtContainerTriggerEntry( CLbtContainerTriggerEntry& aEntry);   	

private:
    /**
     * Constructor.
     */
 	CLbtContainerTriggerEntry();
        
     // CLbtContainerTriggerEntry( CLbtContainerTriggerEntry& aEntry);   
    /**
     * Symbian 2nd phase of Construction.
     */
    void ConstructL();

    /**
     * Symbian 2nd phase of Construction.
     */
    void ConstructL(
                    CLbtTriggerEntry* aEntry,
                    TLbtTriggerDynamicInfo* aDynInfo,
                    CLbtExtendedTriggerInfo* aExtendedTriggerInfo ); 


	void InitializeStrategyData();
                    
    void CloneL(CLbtContainerTriggerEntry& aEntry);              
       
private:
    
    /**
     * Derived Trigger dynamic info
     */
    CLbtExtendedTriggerInfo *iExtendedTriggerInfo;
    
    // Strategy Data
    TStrategyData iStrategyData;
    };


#endif // C_LBT_CONTAINER_TRIGGER_ENTRY_H

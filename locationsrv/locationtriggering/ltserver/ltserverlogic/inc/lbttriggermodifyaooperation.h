/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Active Object operation for serving server logic requests
*				 for modification or deletion of triggers.
*
*/


#ifndef C_LBTTRIGGERMODIFYOOPERATION_H
#define C_LBTTRIGGERMODIFYOOPERATION_H

// INCLUDE FILES
#include <e32base.h>
#include <lbtcommon.h>
#include "lbtaooperationbase.h"

// FORWARD DECLARATIONS
class MLbtAOObserver;
class CLbtContainer;
class CLbtContainerTriggerEntry;
class CLbtSettingsManager;

/**
 * Class declaration of active object operation
 * to service client requests. This object services
 * requests which require listing of triggers.
 * The IPC messages are provided to
 * this class to service the request asynchronously
 *
 * @since S60 v4.0
 */
class CLbtTriggerModifyAOOperation : public CLbtAOOperationBase
	{
	public:
	
		/**
	     * Instantiates a new object of 
	     * CLbtTriggerModifyAOOperation
	     *
	     * @param[in] aObserver reference to the active
	     * object observer
	     * @param[in] aMessage the IPC request which the active 
	     * object has to service. The active object takes responsibility
	     * of completing this IPC request
	     * @param aContainer reference to the container which holds
	     * the reference to the logic processor
	     */
	    static CLbtTriggerModifyAOOperation* NewL( MLbtAOObserver& aObserver,
				    							   const RMessage2& aMessage,
				    							   CLbtContainer& aContainer,
				    							   TInt aClientType,
				    							   CLbtSettingsManager& aSettingsManager );
	    
	    /**
	     * Destructor
	     *
	     */
	    ~CLbtTriggerModifyAOOperation();
	    
	private:
	
		/**
	     * 2nd phase constuctor for instantiating member variables
	     *
	     */
		void ConstructL();
		
		/**
	     * Default C++ Constructor
	     *
	     * @param[in] aObserver reference to the active object observer
	     * @param[in] aMessage the IPC request which the active 
	     * object has to service. The active object takes responsibility
	     * of completing this IPC request
	     * @param aContainer reference to the container which holds
	     * the reference to the logic processor
	     */
	    CLbtTriggerModifyAOOperation( MLbtAOObserver& aObserver, 
				    				  const RMessage2& aMessage,
				    				  CLbtContainer& aContainer,
				    				  TInt aClientType ,
				    				  CLbtSettingsManager& aSettingsManager );
	
	public: // From CLbtAOOperationBase
	
	    /**
	     * Starts the AO operation of serving the request	     
	     *
	     */ 
		void StartOperationL();
		
		/**
	     * Cancels Modify Operation.	     
	     *
	     */
	     void CancelModifyOperation();
				
		/**
		 * Returns the trigger id of the trigger which is being modified
		 *
		 * @return the trigger id of the trigger being modified. If the 
		 * operation object modifies multiple triggers then KLbtNullTriggerId
		 * is returned
		 */
		TLbtTriggerId GetTriggerId();

    public:
        RPointerArray<CLbtContainerTriggerEntry>& GetUpdatedTriggerList();

    private:
    
        void ListTriggersL();
        void UpdateTriggersL();
        CLbtTriggerEntry* CreateTriggerEntryLC( TInt aType );
        CLbtTriggerFilterBase* CreateTriggerFilterLC( TInt aType );
        CLbtTriggerFilterBase* InternalizeFilterFromIpcLC( 
            const RMessage2& aMessage, 
            const TInt aIpcSlot );
        CLbtTriggerEntry* InternalizeTriggerFromIpcLC( 
            const RMessage2& aMessage, 
            const TInt aIpcSlot );
        void HandleUpdateTriggerStateOpL();
        void HandleModifyTriggerStateOpL();
        void HandleDeleteTriggerOpL();
        TBool CheckEnabledTriggerCap();

    protected: // From CActive
    	void RunL();
    	void DoCancel();

    private: // Data
        enum TLbtModifyReqProcessingState
            {
            ENone = 0,
            EListTriggers,
            EUpdateTriggers
            };
	private: // Data
		
		// The id of the trigger being modified (valid only if one trigger is being modified)
		TLbtTriggerId iTriggerId;
				
		// Container trigger entry object
		CLbtContainerTriggerEntry* iContainerEntry;
		
		// The state of the request processing
		TLbtModifyReqProcessingState iState;
		
		// The pointer array which will be populated by the container
		// based on the search
		RPointerArray<CLbtContainerTriggerEntry> iUpdatedTriggerList;
		
		// Settings manager
		CLbtSettingsManager& iSettingsManager;
		
		// Fire on updation flag
		TLbtFireOnUpdate iFireOnUpdate;
	};

#endif // C_LBTTRIGGERMODIFYOOPERATION_H
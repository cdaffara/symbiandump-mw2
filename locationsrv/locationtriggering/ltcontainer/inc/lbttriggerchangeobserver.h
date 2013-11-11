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
* Description: Interface to notify clients about trigger change 
* events
*
*/


#ifndef M_TRIGGERCHANGEOBSERVER_H
#define M_TRIGGERCHANGEOBSERVER_H


#include <e32cmn.h>

#include "lbtcontainer.h"

/**
 *  MLbtContainerChangeEventObserver is a an observer  
 *  
 *  @since S60 v4.0
 */
class MLbtContainerChangeEventObserver
    {

public:  // Interface methods

	/**
	 * 
	 */
	 enum TLbtContainerChangedAreaType
	 	{
	 	// None
	 	ETypeUnKnown = 0x0000,
	 	// Only Cicular triggers have changed
	 	ETypeCircularTriggerChange = 0x0001,
	 	// Only rectangular triggers have changed
	 	ETypeRectangularTriggerChange = 0x0002,
	 	// Only cellular triggers have changed
	 	ETypeCellularTriggerChange = 0x0004,
	 	// Hybrid triggers have changed
	 	ETypeHybridTriggerChange = 0x0010,
	 	// All the different type of triggers have changed
	 	ETypeAll = 0xFFFF
	 	};
	 
    /**
     * The method that has to be implemented to obtain events when the trigger
     * store is modified.
     *
     * @param[in] aTriggerIds aList of triggers that were changed. The ownership
     * is NOT transferred to the client. The client will have to delete this after 
     * processing the event.
     * @param[in] aEventMask The type of changes on the triggers. This is a 
     * bitmask of CLbtContainer::TLbtTriggerChangeEvent.
     *
     * @note To obtain the events, the client will have to set itself as an
     * observer using CLbtContainer::SetChangeObserver()
     * 
     * @note The client should not block this method for a long time.
     */
    virtual void TriggerStoreChanged( RArray<TLbtTriggerId>& aTriggerIds,
                                      TLbtTriggerEventMask aEventMask,
                                      TLbtContainerChangedAreaType aAreaType,
                                      RArray<TUid>& aManagerUids ) = 0;
    };

#endif      // M_TRIGGERCHANGEOBSERVER_H

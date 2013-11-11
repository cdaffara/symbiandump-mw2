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
* Description:  Utility class for location triggering container
*
*/


#ifndef LBT_CONTAINER_UTILITIES_H
#define LBT_CONTAINER_UTILITIES_H

// INCLUDE FILES
#include <e32cmn.h>

// FORWARD DECLARATIONS
class CLbtContainerFilterBase;
class CLbtContainerListOptions;
class CLbtContainerUpdateFilter;
class CLbtContainerTriggerEntry;
class CLbtGeoAreaBase;
class CLbtTriggerFilterBase;
class TLbtSecurityPolicy;

class LbtContainerUtilities
	{
	public:
		/**
		 * From the provided Container list options this method extracts the container filter and makes a clone of it.
		 * The owner ship of the copy is transferred to the client which invokes this method. The filter could be 
		 * any of the following filters derived from CLbtContainerFilterBase :-
		 *
		 *			1) CLbtContainerAttrFilter
		 *			2) CLbtContainerAreaFilter
		 *			3) CLbtContainerCompFilter
		 *
		 * For any other type of filter this function will leave with KErrNotSupported. Support should be added for any 
		 * further filters that are derived from CLbtContainerFilterBase.
		 * 
		 * @param aFilter the filter that needs to be cloned
		 *
		 * @return pointer to the cloned filter. The client which invokes this method will have to take ownership
		 *         of the returned filter. The returned filter is pushed onto the cleanup stack and left in it.
		 *		   The client will have to remove the pointer from the cleanup stack.
		 */
		static CLbtContainerFilterBase* GetContainerFilterFromListOptionsLC( CLbtContainerListOptions* aListOptions );
		
		/**
		 * Makes a copy of the provided trigger filter and returns the pointer to the copy. The filter is based on the 
		 * type of filter supplied. The owner ship of the copy is transferred to the client which invokes this method. 
		 * The filter could be any of the following filters derived from CLbtTriggerFilterBase :-
		 *
		 *		1) CLbtTriggerFilterByAttribute
		 *		2) CLbtTriggerFilterByArea
		 *		3) CLbtTriggerFilterComposite
		 *
		 * For any other type of filter this function will leave with KErrNotSupported. Support should be added for any 
		 * further filters that are derived from CLbtTriggerFilterBase.
		 *
		 * @param aFilter the filter that needs to be cloned
		 *
		 * @return pointer to the cloned filter. The client which invokes this method will have to take ownership
		 *         of the returned filter
		 */
		static CLbtTriggerFilterBase* CloneFilterL(CLbtTriggerFilterBase* aFilter);
		
		/**
		 * From the provided Container update filter this method extracts the container filter and makes a clone of it.
		 * The owner ship of the copy is transferred to the client which invokes this method. The filter could be 
		 * any of the following filters derived from CLbtContainerFilterBase :-
		 *
		 *			1) CLbtContainerAttrFilter
		 *			2) CLbtContainerAreaFilter
		 *			3) CLbtContainerCompFilter
		 *
		 * For any other type of filter this function will leave with KErrNotSupported. Support should be added for any 
		 * further filters that are derived from CLbtContainerFilterBase.
		 * 
		 * @param aFilter the filter that needs to be cloned
		 *
		 * @return pointer to the cloned filter. The client which invokes this method will have to take ownership
		 *         of the returned filter. The returned filter is pushed onto the cleanup stack and left in it.
		 *		   The client will have to remove the pointer from the cleanup stack.
		 */
		static CLbtContainerFilterBase* GetContainerFilterFromUpdateFilterLC( CLbtContainerUpdateFilter* aListOptions );
		
		/**
		 * Copies the provided geo area type and returns a copy of the provided object.
		 * 
		 * @param aGeoAreaBase the area object that has to be copied
		 * @return the copies area object 
		 */
		static CLbtGeoAreaBase* CopyGeoAreaL( CLbtGeoAreaBase* aGeoAreaBase ); 
		
		/**
		 * Applies the security policy on the supplied trigger and checks if the trigger suffices
		 * the security policy 
		 * 
		 * @return ETrue if the trigger suffices the security polity, EFalse otherwise
		 */
		static TBool RunSecurityPolicy(CLbtContainerTriggerEntry* aTriggerEntry, TLbtSecurityPolicy& aSecurityPolicy);
		
		
		/**
		 * Applies the security policy on the supplied UID values and checks if it suffices
		 * the security policy 
		 * 
		 * @return ETrue if the UIDs suffices the security polity, EFalse otherwise
		 */
		static TBool RunSecurityPolicy( TUid& aOwnerUid,
															   TUid& aManagerUid,
															   TSecureId& aStartupProcessUid,
															   TLbtSecurityPolicy& aSecurityPolicy );
	};

#endif // LBT_CONTAINER_UTILITIES_H

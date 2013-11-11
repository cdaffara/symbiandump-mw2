/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Remap location data to location objects*
*/


#ifndef LOCATIONREMAPPINGAO_H_
#define LOCATIONREMAPPINGAO_H_

#include <e32base.h>
#include <etel3rdparty.h>
#include <lbsposition.h> 
#include <centralrepository.h>

#include "locationdatatype.h"
#include "mdesession.h"
#include "rlocationtrail.h"
#include "locationmanagerdebug.h"
#include "locationtraildefs.h"
#ifdef LOC_REVERSEGEOCODE
#include "ctagcreator.h"
#endif

class TRemapItem
	{
public: 
	TRemapItem();
public:
	TItemId iObjectId;
	TItemId iLocationId;
	TItemId iRelationId;
	TTime iTime;
	};

/**
 * Class is handling remapping operations
 */ 
class CLocationRemappingAO : public CActive
{

public:

	/**
	 * Remapping Active object states
	 */
	enum TMappingState
     {
     EIdle = 0,
     //EGatherObjects,
     ERemapObjects,
     ECommitObjects
     };

	/**
	* Creates and constructs a new instance of CCameraTrailMonitorAO.
	*
	* @return A pointer to the new instance of CCameraTrailMonitorAO
	*/
	static CLocationRemappingAO* NewL();
	
	/**
	* Destructor
	*/
	virtual ~CLocationRemappingAO();
	
	/**
	* Stops remapping
	*
	* @return ETrue if success, EFalse if not
	*/
	void StopRemapping();
	
	void Append( TRemapItem aItem );
	
	void ResetQueue();
	
	TBool ItemsInQueue();

	/**
	 * Check items in remapping queue
	 * 
	 * @return ETrue if new location object must be created for remapping
	 */ 
	TBool CheckQueue();
	
	/**
	 * Remap location objects between time limits.
	 */ 
	void StartRemappingObjects( const TLocationData& aLocationData );

	/**
	* Inherited from CActive. This method will be called on file server notifying.
	*/
	void RunL();
	
	/**
	 * Initialise parameters
	 * 
	 * @param aMdEClient, Instance of CMdESession
	 */ 
	void InitialiseL(CMdESession* aMdEClient);
	
	/**
	* Inherited from CActive. Handles a leave occurring in the request completion event handler RunL().
	*
	* @param aError  An error code.
	* @return An error code.
	*/
	TInt RunError( TInt aError );
	
	/**
	* Inherited from CActive.
	*/
	void DoCancel();
	
	/**
	 * Update relations to new location object
	 * 
	 * @param aLocationId, locationId for relations 
	 */ 
	void UpdateRelationsL( TItemId aLocationId );

#ifdef LOC_REVERSEGEOCODE
	/**
	* AttachGeoTagsL
	* Attaches the country and city tags to all objects
	* @param  aTagCreator An instance of the tag creator
	* @param  aCountryTagId country tagId
	* @param  aCityTagId	city tag Id
	*/
    void AttachGeoTagsL( CTagCreator *aTagCreator,
                             const TItemId aCountryTagId, 
                             const TItemId aCityTagId );
#endif    
private:

	/**
	* C++ constructor - not exported;
	* implicitly called from NewL()
	*/
	CLocationRemappingAO();
	
	/**
	* 2nd phase construction, called by NewL()
	*/
	void ConstructL();
	
	/**
	 * Set AO next state
	 */ 
	void NextState(TMappingState aState);


	/**
	 * Remap objects data
	 */ 
	void RemapObjectsL();
	
	/**
	 * Commit remapped objects to database
	 */ 
	void CommitObjectsL();
	
	/**
	 * Read time limit value from Central Repository
	 */ 
	void ReadTimeFromCenRepL();

private:

	/**
	 * Active object state
	 */ 
	TMappingState iState;
	
	/**
	 * Is there come a new remapping request when 
	 * AO have been executing earlier request.
	 */ 
	TBool iContinue;
	
	/**
	 * Is start time earlier stamped.
	 */ 
	TBool iTimed;
	
	/**
	 * Is location data requested from Location Mananger
	 */ 
	TBool iRequested;
	
	/**
	 * Instance of MdeClient
	 */ 
	CMdESession* iMdEClient;

	/**
	 * Database definitions.
	 */
	CMdENamespaceDef* iNamespaceDef;
    CMdEObjectDef* iObjImageDef;
    CMdEObjectDef* iObjVideoDef;
    CMdEPropertyDef* iPropDateDef;
    CMdEPropertyDef* iPropModifiedDef;
	CMdEObjectDef* iObjLocationDef;
	CMdEPropertyDef* iPropLatDef;
    CMdEPropertyDef* iPropLongDef;
    CMdEPropertyDef* iPropAltDef;
    CMdERelationDef* iContainsLocationRelDef;
    
    /**
     * Object id's which location data will be remapped.
     */ 
    RArray<TItemId> iObjectIds; 
    
    /**
     * Location objects which location data will remapped
     * and it will committed back to Mde.
     */ 
    RPointerArray<CMdEObject> iObjects; 
    
    /**
     * Array of Items to be remapped
     */
    RArray<TRemapItem> iRemapItems;
    
	/**
	 * When location trail have started.
	 * Time is compared to time when GPS fix is received.
	 */ 
	TTime iStartTime;
	
	/**
	 * When GPS fix is received.
	 */ 
	TTime iEndTime;
	
	/**
	 * Time limit for object data mapping (Default is 10min -> 600s)
	 */ 
	TTimeIntervalSeconds iTimeLimit;
	
	/**
	 * Location data
	 */
	TLocationData iLocationData;
	
};


#endif /*LOCATIONREMAPPINGAO_H_*/

// End of file

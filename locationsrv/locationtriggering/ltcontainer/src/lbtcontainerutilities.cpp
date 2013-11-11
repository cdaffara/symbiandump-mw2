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


// INCLUDE FILES
#include <e32base.h>
#include <lbttriggerfilterbyattribute.h>
#include <s32mem.h>
#include <lbttriggerfilterbyarea.h>
#include <lbttriggerfiltercomposite.h>
#include <lbtstartuptrigger.h>
#include <lbtgeocircle.h>
#include <lbtgeohybrid.h>
#include <lbtgeocell.h>
#include "lbtcontainerutilities.h"
#include "lbttriggerfilterbyattribute.h"
#include "lbtlisttriggeroptions.h"
#include "lbtcontainerlistoptions.h"
#include "lbtcontainertriggerfilter.h"
#include "lbtcontainerareafilter.h"
#include "lbtcontainerattrfilter.h"
#include "lbtcontainercompfilter.h"
#include "lbtcontainerupdatefilter.h"
#include "lbtsecuritypolicy.h"
#include "lbtcontainertriggerentry.h"
#include "lbtlogger.h"

//----------------------------------------------------------------------------
// LbtContainerUtilities::GetContainerFilterBaseType
//----------------------------------------------------------------------------
//
CLbtContainerFilterBase* LbtContainerUtilities::GetContainerFilterFromListOptionsLC( CLbtContainerListOptions* aListOptions )
	{
	FUNC_ENTER("LbtContainerUtilities::GetContainerFilterFromListOptions");
	if(aListOptions == NULL)
		{
		User::Leave(KErrArgument);
		}

	CLbtListTriggerOptions* listOptions = aListOptions->ListOptions();
	CLbtTriggerFilterBase *filter = NULL;
	
	if( listOptions != NULL )
        {
        filter = CloneFilterL(listOptions->Filter());
        }
    
    if(filter == NULL)
    	{
    	filter = CLbtTriggerFilterByAttribute::NewL();    	
        CLbtTriggerFilterByAttribute* attrFilter = static_cast<CLbtTriggerFilterByAttribute*> (filter);
        attrFilter->AddTriggerStateL(CLbtTriggerEntry::EStateEnabled);
        attrFilter->AddTriggerStateL(CLbtTriggerEntry::EStateDisabled);
    	}
    CleanupStack::PushL(filter);
    
    CLbtContainerFilter* contFilter = NULL;
    TBool containerFilterOnStack = EFalse;    
    if(aListOptions->ContainerFilter() != NULL)
    	{
    	containerFilterOnStack = ETrue;
    	contFilter = CLbtContainerFilter::NewL(aListOptions->ContainerFilter());
    	CleanupStack::PushL(contFilter);
    	}

    CLbtContainerFilterBase* retContainerFilter = NULL;
	switch(filter->Type())
		{
		case CLbtTriggerFilterBase::EFilterByArea:
			{
			CLbtContainerAreaFilter* areaFilter = CLbtContainerAreaFilter::NewL(filter ,contFilter);
		    retContainerFilter = areaFilter;
		    break;
			}
		case CLbtTriggerFilterBase::EFilterByAttribute:
			{
			CLbtContainerAttrFilter* attrFilter = CLbtContainerAttrFilter::NewL(filter, contFilter);
			retContainerFilter = attrFilter;
			break;
			}
		case CLbtTriggerFilterBase::EFilterComposite:
			{
			CLbtContainerCompFilter* compFilter = CLbtContainerCompFilter::NewL(filter, contFilter);
			retContainerFilter = compFilter;
			break;
			}
		default:
			{
			// This is some filter which is not supported
			User::Leave(KErrNotSupported);
			break;
			}
		}	
	if(containerFilterOnStack)
		{
		CleanupStack::Pop(contFilter); // Ownership taken by container filter
		}	
	CleanupStack::Pop(filter); // Ownership taken by container filter
	CleanupStack::PushL(retContainerFilter);
	return retContainerFilter;
	}

//----------------------------------------------------------------------------
// LbtContainerUtilities::GetContainerFilterFromUpdateFilterLC
//----------------------------------------------------------------------------
//
CLbtContainerFilterBase* LbtContainerUtilities::GetContainerFilterFromUpdateFilterLC( CLbtContainerUpdateFilter* aListOptions )
	{
	FUNC_ENTER("LbtContainerUtilities::GetContainerFilterFromUpdateFilterLC");
	
	if(aListOptions == NULL)
		{
		User::Leave(KErrArgument);
		}
	
	CLbtTriggerFilterBase* filterBase = CloneFilterL(aListOptions->TrigInfoFilter());
	CleanupStack::PushL(filterBase);
	
	CLbtContainerFilter* contFilter = NULL;
	TBool containerFilterOnStack = EFalse;
	if(aListOptions->ContainerFilter() != NULL)
		{
		containerFilterOnStack = ETrue;
		contFilter = CLbtContainerFilter::NewL(aListOptions->ContainerFilter());
		CleanupStack::PushL(contFilter);
		}
    CLbtContainerFilterBase* filter = NULL;
    
    switch(filterBase->Type())
    	{
		case CLbtTriggerFilterBase::EFilterByAttribute: 	
			{
			CLbtContainerAttrFilter* attrFilter = CLbtContainerAttrFilter::NewL(filterBase, contFilter);
			filter = attrFilter;
			break;
			}
		case CLbtTriggerFilterBase::EFilterByArea:
			{
			CLbtContainerAreaFilter* areaFilter = CLbtContainerAreaFilter::NewL(filterBase, contFilter);
			filter = areaFilter;
			break;
			}									
		case CLbtTriggerFilterBase::EFilterComposite:
			{
			CLbtContainerCompFilter* compFilter = CLbtContainerCompFilter::NewL(filterBase, contFilter);
			filter = compFilter;
			break;
			}
		default:
			{
			User::Leave(KErrNotSupported);
			break;
			}
    	}
    if(containerFilterOnStack)
		{
		CleanupStack::Pop(contFilter); // Ownership taken by container filter
		}
    CleanupStack::Pop(); //filterBase, 
    CleanupStack::PushL(filter);
    return filter;
	}

//----------------------------------------------------------------------------
// LbtContainerUtilities::CopyGeoAreaL
//----------------------------------------------------------------------------
//
CLbtGeoAreaBase* LbtContainerUtilities::CopyGeoAreaL( CLbtGeoAreaBase* aGeoAreaBase )
    {
    CBufFlat* buffer = CBufFlat::NewL(512);
    CleanupStack::PushL(buffer);
                        
    RBufWriteStream writeStream;
    writeStream.Open( *buffer );
    CleanupClosePushL(writeStream);
    
    aGeoAreaBase->ExternalizeL( writeStream );
    writeStream.CommitL();
    CleanupStack::PopAndDestroy(&writeStream);
    
    RBufReadStream readStream;
    readStream.Open( *buffer );
    CleanupClosePushL(readStream);
    CLbtGeoAreaBase* trigArea = NULL;
    
    switch( aGeoAreaBase->Type() )
        {
        case CLbtGeoAreaBase::ECircle:
            {
            trigArea = CLbtGeoCircle::NewLC();
            break;
            }
        case CLbtGeoAreaBase::ECellular:
            {
            trigArea = CLbtGeoCell::NewLC();
            break;
            }
        case CLbtGeoAreaBase::EHybrid:
            {
            trigArea = CLbtGeoHybrid::NewLC();
            break;
            }
        default:
            {
            User::Leave( KErrArgument );
            }
        }
    
    trigArea->InternalizeL( readStream );
    CleanupStack::Pop(3); //trigArea, readStream and buffer
    readStream.Close();
    delete buffer;
    return trigArea; 
    }

//----------------------------------------------------------------------------
// LbtContainerUtilities::GetContainerFilterBaseType
//----------------------------------------------------------------------------
//
CLbtTriggerFilterBase* LbtContainerUtilities::CloneFilterL(CLbtTriggerFilterBase* aFilter)
	{
	FUNC_ENTER("CLbtDbTriggersManager::CloneFilterL");

	if(aFilter == NULL)
		{
		return NULL;
		}

	CBufFlat* buffer = CBufFlat::NewL(512);
	CleanupStack::PushL(buffer);
	
	RBufWriteStream writeStream(*buffer);
	CleanupClosePushL(writeStream);
	
	aFilter->ExternalizeL(writeStream);
	CleanupStack::PopAndDestroy(); //writeStream

	CLbtTriggerFilterBase* retFilter = NULL;
	RBufReadStream stream(*buffer);
	CleanupClosePushL(stream);

	switch(aFilter->Type())
		{
		case CLbtTriggerFilterBase::EFilterByAttribute:
			{
			CLbtTriggerFilterByAttribute* attributeFilter = 
								CLbtTriggerFilterByAttribute::NewLC();
			attributeFilter->InternalizeL(stream);			
			CleanupStack::Pop(attributeFilter);
			retFilter = attributeFilter;
			break;
			}
		case CLbtTriggerFilterBase::EFilterByArea:
			{
			CLbtTriggerFilterByArea* areaFilter = 
								CLbtTriggerFilterByArea::NewLC();			
			areaFilter->InternalizeL(stream);			
			CleanupStack::Pop(areaFilter);
			retFilter = areaFilter;
			break;
			}
		case CLbtTriggerFilterBase::EFilterComposite:
			{
			CLbtTriggerFilterComposite* compositeFilter = 
								CLbtTriggerFilterComposite::NewLC();
			compositeFilter->InternalizeL(stream);			
			CleanupStack::Pop(compositeFilter);
			retFilter = compositeFilter;
			break;
			}
		default:
			{
			User::Leave(KErrNotSupported);
			break;
			}
		}
	CleanupStack::PopAndDestroy(2); //stream, buffer
	return retFilter;
	}

//----------------------------------------------------------------------------
// LbtContainerUtilities::RunSecurityPolicy
//----------------------------------------------------------------------------
//
TBool LbtContainerUtilities::RunSecurityPolicy( CLbtContainerTriggerEntry* aTriggerEntry, 
												TLbtSecurityPolicy& aSecurityPolicy )
	{
	FUNC_ENTER("LbtContainerUtilities::RunSecurityPolicy");
	// Check if the security policy is NULL
	if(aSecurityPolicy.IsNull())
		{
		// No security policy required
		return ETrue;
		}
	
	// Check if the trigger's owner SID matches the secure id of the client
	TSecureId ownerSecureId = aSecurityPolicy.GetOwnerSecureId();
	
	if(ownerSecureId.iId != KNullUidValue)
		{
		if(aTriggerEntry->ExtendedTriggerInfo()->OwnerSid() == ownerSecureId)
			{
			// The Secure ID of the policy matches the owner SID.
			return ETrue;
			}
		
		// Check if the secure id specified in the security policy matches the manager ui
		if( aTriggerEntry->TriggerEntry()->ManagerUi() == ownerSecureId )
			{
			return ETrue;
			}
		}
	
	// Check if the security policies owner secure id and the secure id of the 
	// trigger handling process are the same. If yes then the security check passes
	CLbtTriggerEntry* triggerEntry = aTriggerEntry->TriggerEntry();	
	if( triggerEntry->Type() == CLbtTriggerEntry::ETypeStartup)
		{
		CLbtStartupTrigger* startupTrigger = static_cast<CLbtStartupTrigger*>(triggerEntry);
		TSecureId secureId;
		TBuf16<KMaxFileName> fileName;
		startupTrigger->GetProcessId(fileName, secureId);
		if( secureId == aSecurityPolicy.GetOwnerSecureId() )
			{
			return ETrue;
			}
		}		
	return EFalse;
	}

//----------------------------------------------------------------------------
// LbtContainerUtilities::RunSecurityPolicy
//----------------------------------------------------------------------------
//
TBool LbtContainerUtilities::RunSecurityPolicy( TUid& aOwnerUid,
												TUid& aManagerUid,
												TSecureId& aStartupProcessUid,
												TLbtSecurityPolicy& aSecurityPolicy )
	{
	FUNC_ENTER("LbtContainerUtilities::RunSecurityPolicy-UID");
	
	// Check if the security policy is NULL
	if(aSecurityPolicy.IsNull())
		{
		// No security policy required
		return ETrue;
		}
	
	// Check if the trigger's owner SID matches the secure id of the client
	TSecureId ownerSecureId = aSecurityPolicy.GetOwnerSecureId();
	
	if(ownerSecureId.iId != KNullUidValue)
		{
		if( aOwnerUid == ownerSecureId )
			{
			// The Secure ID of the policy matches the owner SID.
			return ETrue;
			}
		
		// Check if the secure id specified in the security policy matches the manager ui
		if( aManagerUid == ownerSecureId )
			{
			return ETrue;
			}
		
		if( aStartupProcessUid == ownerSecureId )
			{
			return ETrue;
			}	
		}		
	return EFalse;
	}

//end of file

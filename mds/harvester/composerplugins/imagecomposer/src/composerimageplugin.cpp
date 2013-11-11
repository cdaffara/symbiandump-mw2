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
* Description:  Composer Image Plug-in
*
*/


#include <e32base.h>

#include "mdelogiccondition.h"
#include "mdenamespacedef.h"
#include "composerimageplugin.h"
#include "harvesterlog.h"
#include "mdeconstants.h"

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CComposerImagePlugin* CComposerImagePlugin::NewL()
    {
    WRITELOG( "CComposerImagePlugin::NewL()" );

    CComposerImagePlugin* self = new (ELeave) CComposerImagePlugin();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// ~CComposerImagePlugin
// ---------------------------------------------------------------------------
//
CComposerImagePlugin::~CComposerImagePlugin() // destruct
    {   
    WRITELOG( "CComposerImagePlugin::~CComposerImagePlugin()" );

    delete iImageComposerAO;
    iImageComposerAO = NULL;
    delete iLocationRelationObserver;
    iLocationRelationObserver = NULL;
    delete iImagePresentObserver;
    iImagePresentObserver = NULL;
    delete iMdEHarvesterSession;
    iMdEHarvesterSession = NULL;
    }

// ---------------------------------------------------------------------------
// CComposerImagePlugin
// ---------------------------------------------------------------------------
//
CComposerImagePlugin::CComposerImagePlugin() : // first-phase C++ constructor
    iImageComposerAO( NULL ), iLocationRelationObserver( NULL ),
    iImagePresentObserver( NULL ), iMdEHarvesterSession( NULL )
    {
    WRITELOG( "CComposerImagePlugin::CComposerImagePlugin()" );
    }

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CComposerImagePlugin::ConstructL() // second-phase constructor
    {
    WRITELOG( "CComposerImagePlugin::ConstructL()" );
    iImageComposerAO = CImageComposerAO::NewL();
    iLocationRelationObserver = NULL;

    }

// ---------------------------------------------------------------------------
// SetObservers
// ---------------------------------------------------------------------------
//
void CComposerImagePlugin::SetObservers()
    {
    WRITELOG( "CComposerImagePlugin::SetObservers()" );

#ifdef _DEBUG
    TRAPD( err, SetObserversL() );
    if ( err != KErrNone )
        {
        WRITELOG1( "CComposerImagePlugin::SetObservers Error: %d", err );
        }
#else
    TRAP_IGNORE( SetObserversL() );    
#endif
    }

// ---------------------------------------------------------------------------
// SetObserversL
// ---------------------------------------------------------------------------
//
void CComposerImagePlugin::SetObserversL()
    {
    if ( iSession )
        {
        iImageComposerAO->SetSession( iSession );

        CMdELogicCondition* condition = CMdELogicCondition::NewL( ELogicConditionOperatorAnd );
        CleanupStack::PushL( condition );

        CMdENamespaceDef& defaultNamespace = iSession->GetDefaultNamespaceDefL();
        CMdEObjectDef& imageDef = 
        	defaultNamespace.GetObjectDefL( MdeConstants::Image::KImageObject );
        condition->AddObjectConditionL( imageDef );

        CleanupStack::Pop( condition );
        iSession->AddObjectObserverL( *this, condition, ENotifyModify, &defaultNamespace );

        if ( iLocationRelationObserver )
            {
            delete iLocationRelationObserver;
            iLocationRelationObserver = NULL;
            }

        iLocationRelationObserver = CLocationRelationObserver::NewL( iSession, this );
        
        if ( iMdEHarvesterSession )
        	{
        	delete iMdEHarvesterSession;
        	iMdEHarvesterSession = NULL;
        	}
        iMdEHarvesterSession = CMdEHarvesterSession::NewL ( *iSession );

        if ( iImagePresentObserver )
        	{
        	delete iImagePresentObserver;
        	iImagePresentObserver = NULL;
        	}
        
        iImagePresentObserver = CImagePresentObserver::NewL( iSession, this, iMdEHarvesterSession );
        }
    }

// ---------------------------------------------------------------------------
// RemoveObservers
// ---------------------------------------------------------------------------
//
void CComposerImagePlugin::RemoveObservers()
    {
    if ( iSession )
        {
        TRAP_IGNORE( iSession->RemoveObjectObserverL( *this ) );

        iImageComposerAO->RemoveSession();
        
        delete iImagePresentObserver;
        iImagePresentObserver = NULL;
        
        delete iLocationRelationObserver;
        iLocationRelationObserver = NULL;
        }
    }

// ---------------------------------------------------------------------------
// IsComposingComplete
// ---------------------------------------------------------------------------
//
TBool CComposerImagePlugin::IsComposingComplete()
    {
    WRITELOG( "CComposerImagePlugin::IsComposingComplete()" );
    
    return iImageComposerAO->IsComposingComplete();
    }

/** 
 * Handle object modifications
 */
void CComposerImagePlugin::HandleObjectNotification(CMdESession& /*aSession*/, 
		TObserverNotificationType aType,
		const RArray<TItemId>& aObjectIdArray)
	{
    if ( aType == ENotifyModify )
    	{
#ifdef _DEBUG
        WRITELOG1( "CComposerImagePlugin::HandleObjectNotification() objects: %d", aObjectIdArray.Count() );
#endif
        TRAP_IGNORE( iMdEHarvesterSession->SetPendingL( aObjectIdArray ) );
    	iImageComposerAO->AddToQueue( aObjectIdArray, EFalse );
    	}
	}

void CComposerImagePlugin::WriteGPSTagsL( TItemId aObjectId, TItemId locationId )
	{
	RArray<TItemId> objectId;
	CleanupClosePushL( objectId );
	objectId.Append( aObjectId );
	iMdEHarvesterSession->SetPendingL( objectId );
	
	CMdENamespaceDef& defaultNamespace = iSession->GetDefaultNamespaceDefL();
    CMdEObjectDef& imageObjDef = defaultNamespace.GetObjectDefL( MdeConstants::Image::KImageObject );
    CMdEObjectDef& locationObjDef = defaultNamespace.GetObjectDefL( MdeConstants::Location::KLocationObject );
	CMdEObject* object = iSession->GetObjectL( aObjectId, imageObjDef );
	CleanupStack::PushL( object );
	CMdEObject* location = iSession->GetObjectL( locationId, locationObjDef );
	CleanupStack::PushL( location );
	
	if( object && location )
		{
		TRAPD( err, iImageComposerAO->DoWriteExifL( object, location ) );
	
		if ( err == KErrNone )
			{
			iMdEHarvesterSession->ResetPendingL( objectId );
			}
		}
	CleanupStack::PopAndDestroy( 3, &objectId );
	}

void CComposerImagePlugin::HandlePendingObjects( const RArray<TItemId>& aObjectIdArray )
	{
	iImageComposerAO->AddToQueue( aObjectIdArray, ETrue);
	}

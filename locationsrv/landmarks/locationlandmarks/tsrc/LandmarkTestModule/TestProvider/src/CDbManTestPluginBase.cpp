/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*   ?description_line
*
*/



// INCLUDE FILES
//#include <EPos_PosLmDbInfoExtension.h>
#include "CDbManTestPluginBase.h"
#include "FT_RPosLandmarkServer.h"


// CONSTANTS
_LIT(KPluginProtocol, "test");

// MEMBER FUNCTIONS

// Destructor
CDbManTestPluginBase::~CDbManTestPluginBase()
    {
    iDatabases.ResetAndDestroy();
    }

CDbManTestPluginBase::CDbManTestPluginBase()
    {
    }

// -----------------------------------------------------------------------------
// CDbManTestPluginBase::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CDbManTestPluginBase* CDbManTestPluginBase::NewL(TAny* aConstructionParams)
    {
    CDbManTestPluginBase* self = new( ELeave ) CDbManTestPluginBase;
    CleanupStack::PushL(self);
    self->BaseConstructL(aConstructionParams);
    CleanupStack::Pop(self);
    return self;

    }
 
// ---------------------------------------------------------
// CDbManTestPluginBase::ListDatabasesLC
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CDesCArray* CDbManTestPluginBase::ListDatabasesLC() 
    {
    TInt count = iDatabases.Count();
    CDesCArrayFlat* result = 
       new(ELeave) CDesCArrayFlat(count == 0 ? 1 : count);
    CleanupStack::PushL(result);

    for (TInt i = 0; i < count; i++)
        {
        result->AppendL(iDatabases[i]->DatabaseUri());
        }
    return result;
    }

// ---------------------------------------------------------
// CDbManTestPluginBase::ListDatabasesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CDbManTestPluginBase::ListDatabasesL(
               RPointerArray<HPosLmDatabaseInfo>& aDatabaseInfoArray
        ) 
    {
    TInt count = iDatabases.Count();

    for (TInt i = 0; i < count; i++)
        {
        HPosLmDatabaseInfo* tmpDbInfo = 
            HPosLmDatabaseInfo::NewLC(*(iDatabases[i]));
        aDatabaseInfoArray.AppendL(tmpDbInfo);
        CleanupStack::Pop(tmpDbInfo);
        }
    }

// ---------------------------------------------------------
// CDbManTestPluginBase::RegisterDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CDbManTestPluginBase::RegisterDatabaseL(
               HPosLmDatabaseInfo& aDatabaseInfo
        ) 
    {
    if (DatabaseExistsL(aDatabaseInfo.DatabaseUri()))
        {
        User::Leave(KErrAlreadyExists);
        }
    HPosLmDatabaseInfo* tmpDbInfo = 
        HPosLmDatabaseInfo::NewLC(aDatabaseInfo);
    iDatabases.AppendL(tmpDbInfo);
    CleanupStack::Pop(tmpDbInfo);
    
    TPosLmDatabaseEvent event;
    event.iEventType = EPosLmDbDatabaseRegistered;
    Session().ReportEvent(event, aDatabaseInfo.DatabaseUri());    
    }

// ---------------------------------------------------------
// CDbManTestPluginBase::UnregisterDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CDbManTestPluginBase::UnregisterDatabaseL(
               const TDesC& aDatabaseUri
        ) 
    {
    TInt index = FindUri(aDatabaseUri);
    User::LeaveIfError(index);
    delete iDatabases[index];
    iDatabases.Remove(index);
    
    TPosLmDatabaseEvent event;
    event.iEventType = EPosLmDbDatabaseUnregistered;
    Session().ReportEvent(event, aDatabaseUri);
    }

// ---------------------------------------------------------
// CDbManTestPluginBase::UnregisterAllDatabasesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CDbManTestPluginBase::UnregisterAllDatabasesL() 
    {
    iDatabases.ResetAndDestroy();
    }

// ---------------------------------------------------------
// CDbManTestPluginBase::ModifyDatabaseSettingsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CDbManTestPluginBase::ModifyDatabaseSettingsL(
               const TDesC& aDatabaseUri,
               const TPosLmDatabaseSettings& aDatabaseSettings
        ) 
    {
    TInt index = FindUri(aDatabaseUri);
    User::LeaveIfError(index);
    iDatabases[index]->Settings() = aDatabaseSettings;
    
    TPosLmDatabaseEvent event;
    event.iEventType = EPosLmDbSettingsModified;
    Session().ReportEvent(event, aDatabaseUri);
    }

// ---------------------------------------------------------
// CDbManTestPluginBase::GetDatabaseInfoL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CDbManTestPluginBase::GetDatabaseInfoL(
            HPosLmDatabaseInfo& aDatabaseInfo
        ) 
    {
    TInt index = FindUri(aDatabaseInfo.DatabaseUri());
    User::LeaveIfError(index);
    HPosLmDatabaseInfo* dbInfo = iDatabases[index];
    aDatabaseInfo.Settings() = dbInfo->Settings();
    }

// ---------------------------------------------------------
// CDbManTestPluginBase::DatabaseExistsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CDbManTestPluginBase::DatabaseExistsL( 
               const TDesC&  aDatabaseUri
        ) 
    {
    return FindUri(aDatabaseUri) >= 0 ? ETrue : EFalse;
    }

// ---------------------------------------------------------
// CDbManTestPluginBase::CreateDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CDbManTestPluginBase::CreateDatabaseL( 
               HPosLmDatabaseInfo& aDatabaseInfo
        ) 
    {
    // Do nothing
    //RegisterDatabaseL(aDatabaseInfo);
    //
    }

// ---------------------------------------------------------
// CDbManTestPluginBase::DeleteDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CDbManTestPluginBase::DeleteDatabaseL(
               const TDesC&  aDatabaseUri
        ) 
    {
    UnregisterDatabaseL(aDatabaseUri);
    }

// ---------------------------------------------------------
// CDbManTestPluginBase::CopyDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CDbManTestPluginBase::CopyDatabaseL(
               const TDesC&  aSourceUri,
               const TDesC&  aTargetUri
        ) 
    {
    TInt srcIndex = FindUri(aSourceUri);
    User::LeaveIfError(srcIndex);
    TInt trgIndex = FindUri(aTargetUri);
    if (trgIndex >= 0)
        {
        User::Leave(KErrAlreadyExists);
        }
    HPosLmDatabaseInfo* trgInfo = 
        HPosLmDatabaseInfo::NewLC(aTargetUri);
    iDatabases.AppendL(trgInfo);
    CleanupStack::Pop(trgInfo);
    }
    
// ---------------------------------------------------------
// CDbManTestPluginBase::DatabaseMedia
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TMediaType CDbManTestPluginBase::DatabaseMedia(
               const TDesC& /* aDatabaseUri */
        ) 
    {
    return EMediaRemote;
    }

// ---------------------------------------------------------
// CDbManTestPluginBase::DatabaseDrive
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TChar CDbManTestPluginBase::DatabaseDrive(
               const TDesC& /* aDatabaseUri */
        ) 
    {
    return 'U';
    }
    
// ---------------------------------------------------------
// CDbManTestPluginBase::Protocol
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TPtrC CDbManTestPluginBase::Protocol() 
    {
    return TPtrC(KPluginProtocol);
    }

// ---------------------------------------------------------
// CDbManTestPluginBase::FindUri
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CDbManTestPluginBase::FindUri(const TDesC& aUri) 
    {
    TInt index;
    TInt count = iDatabases.Count();
    TBool found = EFalse;
    
    for (index = 0; index < count && !found; index++)
        {
        if (aUri.Compare(iDatabases[index]->DatabaseUri()) == 0)
            {
            found = ETrue;
            }
        }
    index--;

    if (!found)
        {
        index = KErrNotFound;
        }
    return index;
    }




//  End of File

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
* Description: This class is used to manage landmark databases.
*
*
*/


// INCLUDE FILES
#include <ecom/ecom.h>
#include <epos_cposlmsettingshandler.h>
#include <EPos_CPosLmDatabaseManagerPluginBase.h>
#include <EPos_PosLmDbInfoExtension.h>
#include <epos_poslmfilefinder.h>
#include <epos_cposlmresourcereader.h>
#include <eposlmdefaultdbname.rsg>
#include "EPos_CPosLmDbManPluginStore.h"
#include "EPos_CPosLmDatabaseManagerImpl.h"
#include "epos_poslmdatabaseutility.h"


_LIT(KProtocolSeparator, "://");
_LIT(KLocalProtocol, "file");
_LIT(KDefaultDbNameResourceFile, "\\resource\\eposlmdefaultdbname.rsc");

// ================= LOCAL FUNCTIONS ========================

void CleanupPointerArray(TAny* aArray)
    {
    (static_cast<RPointerArray<HPosLmDatabaseInfo>*>(
        aArray))->ResetAndDestroy();
    }


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPosLmDatabaseManagerImpl::CPosLmDatabaseManagerImpl
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmDatabaseManagerImpl::CPosLmDatabaseManagerImpl()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmLocalImportedLmIterator::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmDatabaseManagerImpl::ConstructL()
    {
    iSettingsHandler = CPosLmSettingsHandler::NewL();
    User::LeaveIfError(iSession.Connect());
    User::LeaveIfError(iEventListener.Open(iSession));
    iPlugins = CPosLmDbManPluginStore::NewL(&iSession);
    iDefaultDatabaseDisplayName = DefaultDatabaseDisplayNameL();
    }

// -----------------------------------------------------------------------------
// CPosLmDatabaseManagerImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmDatabaseManagerImpl* CPosLmDatabaseManagerImpl::NewL()
    {
    CPosLmDatabaseManagerImpl* self = new( ELeave ) CPosLmDatabaseManagerImpl;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosLmDatabaseManagerImpl::~CPosLmDatabaseManagerImpl()
    {
    delete iSettingsHandler;
    delete iPlugins;
    iEventListener.Close();
    iSession.Close();
    delete iDefaultDatabaseDisplayName;
    }

// ---------------------------------------------------------
// CPosLmDatabaseManagerImpl::DefaultDatabaseUriLC
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
HBufC* CPosLmDatabaseManagerImpl::DefaultDatabaseUriLC()
    {
    HBufC* uri = iSettingsHandler->GetDefaultDatabaseUriL();
    CleanupStack::PushL(uri);
    return uri;
    }

// ---------------------------------------------------------
// CPosLmDatabaseManagerImpl::SetDefaultDatabaseUriL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLmDatabaseManagerImpl::SetDefaultDatabaseUriL(
    const TDesC& aDatabaseUri)
    {
    if (!IsLocalDatabase(aDatabaseUri))
        {
        User::Leave(KErrNotSupported);
        }
    if (!DatabaseExistsL(aDatabaseUri))
        {
        User::Leave(KErrNotFound);
        }
    iSettingsHandler->SetDefaultDatabaseUriL(aDatabaseUri);
    }

// ---------------------------------------------------------
// CPosLmDatabaseManagerImpl::NotifyDatabaseEvent
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLmDatabaseManagerImpl::NotifyDatabaseEvent(
    TPosLmDatabaseEvent&  aEvent,
    TRequestStatus&  aStatus)
    {
    iEventListener.NotifyDatabaseEvent(aEvent, aStatus);
    }

// ---------------------------------------------------------
// CPosLmDatabaseManagerImpl::CancelNotifyDatabaseEvent
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CPosLmDatabaseManagerImpl::CancelNotifyDatabaseEvent()
    {
    return iEventListener.CancelNotifyDatabaseEvent();
    }

// ---------------------------------------------------------
// CPosLmDatabaseManagerImpl::DatabaseUriFromLastEventLC
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
HBufC* CPosLmDatabaseManagerImpl::DatabaseUriFromLastEventLC()
    {
    return iEventListener.DatabaseUriFromLastEventLC();
    }

// ---------------------------------------------------------
// CPosLmDatabaseManagerImpl::ListDatabasesLC
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CDesCArray* CPosLmDatabaseManagerImpl::ListDatabasesLC(
    const TDesC& aProtocol)
    {
    CDesCArray* resultList = NULL;

    if (aProtocol == KNullDesC)
        {
        TInt pluginCount = iPlugins->NumberOfInstances();
        for (TInt i = 0; i < pluginCount; i++)
            {
            if (i == 0)
                {
                resultList = iPlugins->GetInstanceAt(i)->ListDatabasesLC();
                }
            else
                {
                CDesCArray* dbList =
                    iPlugins->GetInstanceAt(i)->ListDatabasesLC();
                TInt dbCount = dbList->Count();
                for (TInt j = 0; j < dbCount; j++)
                    {
                    resultList->AppendL((*dbList)[j]);
                    }
                CleanupStack::PopAndDestroy(dbList);
                }
            }
        }
    else
        {
        CPosLmDatabaseManagerPluginBase* plugin =
            iPlugins->GetInstanceL(aProtocol);
        resultList = plugin->ListDatabasesLC();
        }
    return resultList;
    }

// ---------------------------------------------------------
// CPosLmDatabaseManagerImpl::ListDatabasesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLmDatabaseManagerImpl::ListDatabasesL(
    RPointerArray<HPosLmDatabaseInfo>& aDatabaseInfoArray,
    const TDesC& aProtocol)
    {
    if (aProtocol == KNullDesC)
        {
        TInt pluginCount = iPlugins->NumberOfInstances();
        for (TInt i = 0; i < pluginCount; i++)
            {
            if (i == 0)
                {
                CPosLmDatabaseManagerPluginBase* plugin =
                    iPlugins->GetInstanceAt(i);
                plugin->ListDatabasesL(aDatabaseInfoArray);
                FillInDbInfoListL(aDatabaseInfoArray, plugin);
                }
            else
                {
                RPointerArray<HPosLmDatabaseInfo> dbList;
                CleanupStack::PushL(TCleanupItem(CleanupPointerArray, &dbList));
                CPosLmDatabaseManagerPluginBase* plugin =
                    iPlugins->GetInstanceAt(i);
                plugin->ListDatabasesL(dbList);
                FillInDbInfoListL(dbList, plugin);
                TInt dbCount = dbList.Count();
                for (TInt j = 0; j < dbCount; j++)
                    {
                    // transfer ownership instead of copy object
                    User::LeaveIfError(aDatabaseInfoArray.Append(dbList[0]));
                    dbList.Remove(0);
                    }
                CleanupStack::PopAndDestroy(&dbList);
                }
            }
        }
    else
        {
        CPosLmDatabaseManagerPluginBase* plugin =
            iPlugins->GetInstanceL(aProtocol);
        plugin->ListDatabasesL(aDatabaseInfoArray);
        FillInDbInfoListL(aDatabaseInfoArray, plugin);
        }
    }

// ---------------------------------------------------------
// CPosLmDatabaseManagerImpl::RegisterDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLmDatabaseManagerImpl::RegisterDatabaseL(
    HPosLmDatabaseInfo& aDatabaseInfo)
    {
    TPtrC uri = aDatabaseInfo.DatabaseUri();
    CPosLmDatabaseManagerPluginBase* plugin =
        iPlugins->GetInstanceL(GetProtocolFromUri(uri));

    PosLmDbInfoExtension::SetDatabaseDrive(aDatabaseInfo,
                                           plugin->DatabaseDrive(uri));
    PosLmDbInfoExtension::SetMediaType(aDatabaseInfo,
                                           plugin->DatabaseMedia(uri));
    plugin->RegisterDatabaseL(aDatabaseInfo);
    }

// ---------------------------------------------------------
// CPosLmDatabaseManagerImpl::UnregisterDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLmDatabaseManagerImpl::UnregisterDatabaseL(
    const TDesC& aDatabaseUri)
    {
    CPosLmDatabaseManagerPluginBase* plugin =
        iPlugins->GetInstanceL(GetProtocolFromUri(aDatabaseUri));
    plugin->UnregisterDatabaseL(aDatabaseUri);
    }

// ---------------------------------------------------------
// CPosLmDatabaseManagerImpl::UnregisterAllDatabasesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLmDatabaseManagerImpl::UnregisterAllDatabasesL(
    const TDesC& aProtocol)
    {
    if (aProtocol == KNullDesC)
        {
        User::Leave(KErrArgument);
        }
    CPosLmDatabaseManagerPluginBase* plugin =
        iPlugins->GetInstanceL(aProtocol);
    plugin->UnregisterAllDatabasesL();
    }

// ---------------------------------------------------------
// CPosLmDatabaseManagerImpl::ModifyDatabaseSettingsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLmDatabaseManagerImpl::ModifyDatabaseSettingsL(
    const TDesC& aDatabaseUri,
    const TPosLmDatabaseSettings& aDatabaseSettings)
    {
    CPosLmDatabaseManagerPluginBase* plugin =
        iPlugins->GetInstanceL(GetProtocolFromUri(aDatabaseUri));
    plugin->ModifyDatabaseSettingsL(aDatabaseUri, aDatabaseSettings);
    }

// ---------------------------------------------------------
// CPosLmDatabaseManagerImpl::GetDatabaseInfoL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLmDatabaseManagerImpl::GetDatabaseInfoL(
    HPosLmDatabaseInfo& aDatabaseInfo)
    {
    TPtrC uri = aDatabaseInfo.DatabaseUri();
    CPosLmDatabaseManagerPluginBase* plugin =
        iPlugins->GetInstanceL(GetProtocolFromUri(uri));
    HBufC* defaultDbUri = iSettingsHandler->GetDefaultDatabaseUriL();
    CleanupStack::PushL(defaultDbUri);

    // must fill in drive before sending to provider
    PosLmDbInfoExtension::SetDatabaseDrive(aDatabaseInfo,
                                           plugin->DatabaseDrive(uri));
    PosLmDbInfoExtension::SetMediaType(aDatabaseInfo,
                                           plugin->DatabaseMedia(uri));

    // fills in settings
    plugin->GetDatabaseInfoL(aDatabaseInfo);

    // fills in the rest of the parameters
    FillInDbInfoL(aDatabaseInfo, plugin, *defaultDbUri);

    CleanupStack::PopAndDestroy(defaultDbUri);
    }

// ---------------------------------------------------------
// CPosLmDatabaseManagerImpl::DatabaseExistsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CPosLmDatabaseManagerImpl::DatabaseExistsL(
    const TDesC& aDatabaseUri)
    {
    CPosLmDatabaseManagerPluginBase* plugin =
        iPlugins->GetInstanceL(GetProtocolFromUri(aDatabaseUri));
    return plugin->DatabaseExistsL(aDatabaseUri);
    }

// ---------------------------------------------------------
// CPosLmDatabaseManagerImpl::CreateDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLmDatabaseManagerImpl::CreateDatabaseL(
    HPosLmDatabaseInfo& aDatabaseInfo)
    {
    TPtrC uri = aDatabaseInfo.DatabaseUri();
    CPosLmDatabaseManagerPluginBase* plugin =
        iPlugins->GetInstanceL(GetProtocolFromUri(uri));

    TChar drive = plugin->DatabaseDrive(uri);

    __ASSERT_ALWAYS((drive >= 'a' && drive <= 'z') ||
                    (drive >= 'A' && drive <= 'Z') ||
                    drive == 0, User::Leave(KErrArgument)
                    );

    PosLmDbInfoExtension::SetDatabaseDrive(aDatabaseInfo, drive);

    PosLmDbInfoExtension::SetMediaType(aDatabaseInfo,
                                           plugin->DatabaseMedia(uri));
    plugin->CreateDatabaseL(aDatabaseInfo);
    }

// ---------------------------------------------------------
// CPosLmDatabaseManagerImpl::DeleteDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLmDatabaseManagerImpl::DeleteDatabaseL(
    const TDesC& aDatabaseUri)
    {
    CPosLmDatabaseManagerPluginBase* plugin =
        iPlugins->GetInstanceL(GetProtocolFromUri(aDatabaseUri));
    plugin->DeleteDatabaseL(aDatabaseUri);
    }

// ---------------------------------------------------------
// CPosLmDatabaseManagerImpl::CopyDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLmDatabaseManagerImpl::CopyDatabaseL(
    const TDesC&  aSourceUri,
    const TDesC&  aTargetUri)
    {
    TPtrC sourceProtocol = GetProtocolFromUri(aSourceUri);
    TPtrC targetProtocol = GetProtocolFromUri(aTargetUri);

    if (sourceProtocol != targetProtocol)
        {
        User::Leave(KErrArgument);
        }
    CPosLmDatabaseManagerPluginBase* plugin =
        iPlugins->GetInstanceL(sourceProtocol);
    plugin->CopyDatabaseL(aSourceUri, aTargetUri);
    }

// ---------------------------------------------------------
// CPosLmDatabaseManagerImpl::GetProtocolFromUri
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
const TPtrC CPosLmDatabaseManagerImpl::GetProtocolFromUri(
    const TDesC& aUri)
    {
    TInt offset = aUri.Find(KProtocolSeparator);
    if (offset == KErrNotFound)
        {
        return KLocalProtocol();
        }
    else
        {
        return aUri.Left(offset);
        }
    }

// ---------------------------------------------------------
// CPosLmDatabaseManagerImpl::IsLocalDatabase
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CPosLmDatabaseManagerImpl::IsLocalDatabase(
    const TDesC& aUri)
    {
    return GetProtocolFromUri(aUri) == KLocalProtocol;
    }

// -----------------------------------------------------------------------------
// CPosLmDatabaseManagerImpl::DefaultDatabaseDisplayNameL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
HBufC* CPosLmDatabaseManagerImpl::DefaultDatabaseDisplayNameL()
    {
    TFileName* filename = new (ELeave) TFileName();
    CleanupStack::PushL(filename);

    RFs fileSession;
    CleanupClosePushL(fileSession);
    User::LeaveIfError(fileSession.Connect());

    User::LeaveIfError(PosLmFileFinder::ResourceFileL(filename, fileSession,
        KDefaultDbNameResourceFile));

    CPosLmResourceReader* resource =
        CPosLmResourceReader::NewLC(*filename);

    HBufC* name = resource->ReadHBufCL(R_POS_LM_DEFAULT_DB_DISPLAY_NAME);

    CleanupStack::PopAndDestroy(3, filename);
    return name;
    }

// ---------------------------------------------------------
// CPosLmDatabaseManagerImpl::FillInDbInfoListL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLmDatabaseManagerImpl::FillInDbInfoListL(
    RPointerArray<HPosLmDatabaseInfo>& aDatabaseInfoArray,
    CPosLmDatabaseManagerPluginBase* aPlugin)
    {
    // set default, database drive and media type for each database in list
    HBufC* defaultDbUri = iSettingsHandler->GetDefaultDatabaseUriL();
    CleanupStack::PushL(defaultDbUri);

    TInt count = aDatabaseInfoArray.Count();
    for (TInt i = 0; i < count; i++)
        {
        HPosLmDatabaseInfo* dbInfo = aDatabaseInfoArray[i];
        FillInDbInfoL(*dbInfo, aPlugin, *defaultDbUri);
        dbInfo = NULL;
        }
    CleanupStack::PopAndDestroy(defaultDbUri);
    }

// ---------------------------------------------------------
// CPosLmDatabaseManagerImpl::FillInDbInfoL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLmDatabaseManagerImpl::FillInDbInfoL(
    HPosLmDatabaseInfo& aDatabaseInfo,
    CPosLmDatabaseManagerPluginBase* aPlugin,
    const TDesC& aDefaultDbUri)
    {
    TPtrC uri = aDatabaseInfo.DatabaseUri();
    PosLmDbInfoExtension::SetDatabaseDrive(aDatabaseInfo,
                                           aPlugin->DatabaseDrive(uri));
    PosLmDbInfoExtension::SetMediaType(aDatabaseInfo,
                                       aPlugin->DatabaseMedia(uri));

    if (aPlugin->Protocol() == KNullDesC ||
        aPlugin->Protocol() == KLocalProtocol)
        {
        // Create correct URIs (they may be missing protocol) so that
        // compareC will be correct
        HBufC* tmpUri = PosLmDatabaseUtility::CreateDatabaseUriL(uri);
        CleanupStack::PushL(tmpUri);
        HBufC* tmpDefaultUri = PosLmDatabaseUtility::CreateDatabaseUriL(aDefaultDbUri);
        CleanupStack::PushL(tmpDefaultUri);

        // check if default database
        if (tmpUri->CompareC(*tmpDefaultUri) == KErrNone)
            {
            PosLmDbInfoExtension::SetDefault(aDatabaseInfo, ETrue);
            // set default dispay name if no display name exists
            if (aDatabaseInfo.Settings().DatabaseName() == KNullDesC)
                {
                aDatabaseInfo.Settings().SetDatabaseName(
                    *iDefaultDatabaseDisplayName);
                }
            }
        CleanupStack::PopAndDestroy(tmpDefaultUri);
        CleanupStack::PopAndDestroy(tmpUri);
        }
    }

// End of File

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
* Description:  See class description below.
*
*/


// INCLUDE FILES
#include <d32dbms.h>
#include <badesca.h>
#include <EPos_Landmarks.h>
#include <epos_poslmdatabaseutility.h>
#include <epos_cposlmdiskutilities.h>
#include <EPos_PosLmDbInfoExtension.h>
#include "EPos_CPosLmLocalDatabaseManager.h"

// CONSTANTS
const TInt KPosDbListGranularity = 2;
const TInt KDriveAndDelimiterLength = 2;

_LIT(KFileProtocol, "file");
_LIT(KProtocolDelimiter, "://");
_LIT(KFileExtension, ".ldb");

// ================= LOCAL FUNCTIONS ========================

void CleanupRPointerArray(TAny* aArray)

    {
    (static_cast<RPointerArray<HPosLmDatabaseInfo>*>(
        aArray))->ResetAndDestroy();
    }

TBool MediaInfoIsEqual(const TMediaInfo& m1, const TMediaInfo& m2)
    {
    return m1.iDrive == m2.iDrive;
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmLocalDatabaseManager::CPosLmLocalDatabaseManager
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmLocalDatabaseManager::CPosLmLocalDatabaseManager()
    : CPosLmDatabaseManagerPluginBase()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmLocalDatabaseManager::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmLocalDatabaseManager::ConstructL()
    {
    User::LeaveIfError(iSubsession.Open(Session()));
    SynchronizeRegistryL();
    CreateDriveInfoListL();
    }

// -----------------------------------------------------------------------------
// CPosLmLocalDatabaseManager::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmLocalDatabaseManager* CPosLmLocalDatabaseManager::NewL(
    TAny* aConstructionParams)
    {
    CPosLmLocalDatabaseManager* self =
        new (ELeave) CPosLmLocalDatabaseManager();
    CleanupStack::PushL(self);
    self->BaseConstructL(aConstructionParams);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosLmLocalDatabaseManager::~CPosLmLocalDatabaseManager()
    {
    iSubsession.Close();
    iMediaInfoList.Close();
    }

// ---------------------------------------------------------
// CPosLmLocalDatabaseManager::DatabaseMedia
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TMediaType CPosLmLocalDatabaseManager::DatabaseMedia(
    const TDesC&  aDatabaseUri)
    {
    TMediaType result = EMediaUnknown;
    TMediaInfo searchItem;
    searchItem.iDrive = DatabaseDrive(aDatabaseUri);

    TInt index = iMediaInfoList.Find(
        searchItem,
        TIdentityRelation<TMediaInfo>(MediaInfoIsEqual));
    if (index != KErrNotFound)
        {
        result = iMediaInfoList[index].iMediaType;
        }

    return result;
    }

// ---------------------------------------------------------
// CPosLmLocalDatabaseManager::DatabaseDrive
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TChar CPosLmLocalDatabaseManager::DatabaseDrive(
    const TDesC&  aDatabaseUri)
    {
    TChar sysDriveChar = RFs::GetSystemDriveChar();
    
    // if protocol delimiter present,
    // verify that URI starts with "file://"
    TInt offset = aDatabaseUri.Find(KProtocolDelimiter);
    if (offset != KErrNotFound && aDatabaseUri.Left(offset) != KFileProtocol)
        {
        // wrong protocol
        return 0;
        }

    // adjust offset depending on whether or not protocol is present
    offset = offset == KErrNotFound ?
             0 :
             KFileProtocol().Length() + KProtocolDelimiter().Length();

    if (aDatabaseUri.Length() < offset + KDriveAndDelimiterLength)
        {
        return 0;
        }

    // if second character is ":", return first character
    // otherwise return default drive
    if (aDatabaseUri[offset + 1] == KDriveDelimiter)
        {
        return TChar(aDatabaseUri[offset]).GetUpperCase();
        }
    return sysDriveChar;
    }

// ---------------------------------------------------------
// CPosLmLocalDatabaseManager::Protocol
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TPtrC CPosLmLocalDatabaseManager::Protocol()
    {
    return TPtrC(KFileProtocol);
    }


// -----------------------------------------------------------------------------
// CPosLmLocalDatabaseManager::ListDatabasesLC
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CDesCArray* CPosLmLocalDatabaseManager::ListDatabasesLC()
    {
    CDesCArray* dbArray = new (ELeave) CDesCArrayFlat(KPosDbListGranularity);
    CleanupStack::PushL(dbArray);

    RFs fs;
    CleanupClosePushL(fs);
    User::LeaveIfError(fs.Connect());

    TDriveList driveList;
    User::LeaveIfError(fs.DriveList(driveList));

    RDbs dbSession;
    CleanupClosePushL(dbSession);
    User::LeaveIfError(dbSession.Connect());

    for (TInt drive = EDriveA; drive <= EDriveZ; drive++)
        {
        if ( driveList[drive] && !( driveList[drive] & KDriveAttRemote ) ) // avoid remote drives
            {
            TDriveInfo drvInfo;
            TInt err = fs.Drive( drvInfo, drive );

            if ( !err && drvInfo.iType != EMediaNotPresent )
                {
                CDbDatabaseNames* dbNames = NULL;

                TRAPD(err, dbNames = dbSession.DatabaseNamesL(
                    static_cast<TDriveNumber>(drive), KPosLmDbSecureUid));

                // Ignore all errors except KErrNoMemory (and KErrNone)
                if (err == KErrNone)
                    {
                    CleanupStack::PushL(dbNames);
                    for (TInt i = 0; i < dbNames->Count(); i++)
                        {
                        TPtrC ptr = (*dbNames)[i];

                        if (ptr.Right(KFileExtension().Length()).
                            CompareF(KFileExtension) == 0)
                            {
                            HBufC* fullName = HBufC::NewLC(
                                KFileProtocol().Length() +
                                KProtocolDelimiter().Length() +
                                KDriveAndDelimiterLength +
                                (*dbNames)[i].Length());

                            TChar chr;
                            fs.DriveToChar(drive, chr);

                            TPtr ptr2 = fullName->Des();

                            ptr2.Append(KFileProtocol);
                            ptr2.Append(KProtocolDelimiter);
                            ptr2.Append(chr);
                            ptr2.Append(KDriveDelimiter);
                            ptr2.Append((*dbNames)[i]);

                            dbArray->AppendL(*fullName);
                            CleanupStack::PopAndDestroy(fullName);
                            }
                        }
                    CleanupStack::PopAndDestroy(dbNames);
                    }
                else if (err == KErrNoMemory)
                    {
                    User::Leave(err);
                    }
                }
            }
        }
    CleanupStack::PopAndDestroy(2, &fs); //dbSession
    return dbArray;
    }

// ---------------------------------------------------------
// CPosLmLocalDatabaseManager::ListDatabasesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLmLocalDatabaseManager::ListDatabasesL(
    RPointerArray<HPosLmDatabaseInfo>& aDatabaseInfoArray)
    {
    // list URIs
    CDesCArray* dbUriArray = ListDatabasesLC();
    // list registered databases
    Session().ListDatabasesL(aDatabaseInfoArray, KFileProtocol);
    // delete from URI list those databases that are registered
    for (TInt i = aDatabaseInfoArray.Count() - 1; i >= 0; i--)
        {
        TInt index;
        if (dbUriArray->Find(aDatabaseInfoArray[i]->DatabaseUri(), index) == 0)
            {
            dbUriArray->Delete(index);
            }
        }

    // for each remaining URI in the list, fill in database info
    // and add to list
    TInt count = dbUriArray->Count();
    for (TInt i = 0; i < count; i++)
        {
        HPosLmDatabaseInfo* info = HPosLmDatabaseInfo::NewLC((*dbUriArray)[i]);
        aDatabaseInfoArray.AppendL(info);
        CleanupStack::Pop(info); // transfer of ownership to array
        }

    CleanupStack::PopAndDestroy(dbUriArray);
    // database drive, database media and default flag
    // will be filled in by database management provider.
    }

// ---------------------------------------------------------
// CPosLmLocalDatabaseManager::RegisterDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLmLocalDatabaseManager::RegisterDatabaseL(
               HPosLmDatabaseInfo& /* aDatabaseInfo */)
    {
    User::Leave(KErrNotSupported);
    }

// ---------------------------------------------------------
// CPosLmLocalDatabaseManager::UnregisterDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLmLocalDatabaseManager::UnregisterDatabaseL(
    const TDesC& /* aDatabaseUri */)
    {
    User::Leave(KErrNotSupported);
    }

// ---------------------------------------------------------
// CPosLmLocalDatabaseManager::UnregisterAllDatabasesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLmLocalDatabaseManager::UnregisterAllDatabasesL()
    {
    User::Leave(KErrNotSupported);
    }

// ---------------------------------------------------------
// CPosLmLocalDatabaseManager::ModifyDatabaseSettingsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLmLocalDatabaseManager::ModifyDatabaseSettingsL(
    const TDesC& aDatabaseUri,
    const TPosLmDatabaseSettings& aDatabaseSettings
        )
    {
    // validate URI
    HBufC* uri = PosLmDatabaseUtility::CreateDatabaseUriL(aDatabaseUri);
    CleanupStack::PushL(uri);

    if(!DatabaseExistsL(*uri))
        {
        User::Leave(KErrNotFound);
        }

    TChar drive = DatabaseDrive(*uri);
    TInt err = Session().ModifyDatabaseSettings(*uri,
                                                aDatabaseSettings,
                                                drive);
    if (err == KErrNotFound)
        {
        // database was not registered
        HPosLmDatabaseInfo* info =
            HPosLmDatabaseInfo::NewLC(*uri);
        info->Settings() = aDatabaseSettings;
        PosLmDbInfoExtension::SetDatabaseDrive(*info, drive);
        User::LeaveIfError(Session().RegisterDatabase(*info));
        CleanupStack::PopAndDestroy(info);
        }
    else if (err != KErrNone)
        {
        User::LeaveIfError(err);
        }

    // report event

    // To remove a warning, the event must be default constructed.
    TPosLmDatabaseEvent event =
        {
        EPosLmDbSettingsModified,
        {0,0,0,0,0,0,0,0}
        };

    User::LeaveIfError(Session().ReportEvent(event, *uri));

    CleanupStack::PopAndDestroy(uri);
    }

// ---------------------------------------------------------
// CPosLmLocalDatabaseManager::GetDatabaseInfoL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLmLocalDatabaseManager::GetDatabaseInfoL(
    HPosLmDatabaseInfo& aDatabaseInfo
        )
    {
    // database drive, database media and default flag
    // will be filled in by database management provider.

    // must validate URI, copy to new object
    HBufC* uri = PosLmDatabaseUtility::CreateDatabaseUriL(
        aDatabaseInfo.DatabaseUri());
    CleanupStack::PushL(uri);

    if(!DatabaseExistsL(*uri))
        {
        User::Leave(KErrNotFound);
        }

    HPosLmDatabaseInfo* info = HPosLmDatabaseInfo::NewLC(*uri);
    PosLmDbInfoExtension::SetDatabaseDrive(
        *info,
        aDatabaseInfo.DatabaseDrive());

    TInt err = Session().GetDatabaseInfo(*info);

    if (err == KErrNone)
        {
        // database is registered, transfer settings to original object
        aDatabaseInfo.Settings() = info->Settings();
        }
    else if (err != KErrNotFound)
        {
        User::Leave(err);
        }

    // database is not registered - do nothing.

    CleanupStack::PopAndDestroy(2, uri); // info
    }

// -----------------------------------------------------------------------------
// CPosLmLocalDatabaseManager::DatabaseExistsL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalDatabaseManager::DatabaseExistsL(const TDesC& aDatabaseUri)
    {
    // validate URI
    HBufC* uri = PosLmDatabaseUtility::CreateDatabaseUriL(aDatabaseUri);
    CleanupStack::PushL(uri);

    TPtrC ptr(uri->Des());
    PosLmDatabaseUtility::RemoveProtocolFromUriL(ptr);

    TParse parse;
    User::LeaveIfError(parse.Set(ptr, NULL, NULL));
    TFileName searchUri = parse.NameAndExt();
    searchUri.UpperCase();

    TInt drive;
    User::LeaveIfError(RFs::CharToDrive(parse.Drive()[0], drive));

    RDbs dbSession;
    CleanupClosePushL(dbSession);
    User::LeaveIfError(dbSession.Connect());

    CDbDatabaseNames* names = dbSession.DatabaseNamesL(
            static_cast<TDriveNumber>(drive), KPosLmDbSecureUid);
    CleanupStack::PushL(names);

    TBool exists(EFalse);
    for (TInt i = 0; i < names->Count() && !exists; i++)
        {
        HBufC* databaseName = (*names)[i].AllocLC();
        databaseName->Des().UpperCase();
        if (searchUri == *databaseName)
            {
            exists = ETrue;
            }
        CleanupStack::PopAndDestroy(databaseName);
        }

    CleanupStack::PopAndDestroy(3, uri);  //dbSession
    return exists;
    }

// ---------------------------------------------------------
// CPosLmLocalDatabaseManager::CreateDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLmLocalDatabaseManager::CreateDatabaseL(
    HPosLmDatabaseInfo& aDatabaseInfo)
    {
    // database drive and database media has been filled in by
    // database management provider.

    User::LeaveIfError(iSubsession.CreateDatabase(aDatabaseInfo));
    }

// ---------------------------------------------------------
// CPosLmLocalDatabaseManager::DeleteDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLmLocalDatabaseManager::DeleteDatabaseL(
    const TDesC&  aDatabaseUri)
    {
    HPosLmDatabaseInfo* info = HPosLmDatabaseInfo::NewLC(aDatabaseUri);
    PosLmDbInfoExtension::SetDatabaseDrive(*info,
                                           DatabaseDrive(aDatabaseUri));

    TInt err = iSubsession.DeleteDatabase(*info);
    if (err != KErrNone && err != KErrNotFound)
        {
        User::Leave(err);
        }

    CleanupStack::PopAndDestroy(info);
    }

// ---------------------------------------------------------
// CPosLmLocalDatabaseManager::CopyDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLmLocalDatabaseManager::CopyDatabaseL(
    const TDesC&  aSourceUri,
    const TDesC&  aTargetUri)
    {
    User::LeaveIfError(iSubsession.CopyDatabase(aSourceUri, aTargetUri));
    }

// ---------------------------------------------------------
// CPosLmLocalDatabaseManager::SynchronizeRegistryL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLmLocalDatabaseManager::SynchronizeRegistryL()
    {
    RPointerArray<HPosLmDatabaseInfo> databaseInfoArray(KPosDbListGranularity);
    CleanupStack::PushL(TCleanupItem(CleanupRPointerArray, &databaseInfoArray));

    // list URIs
    CDesCArray* dbUriArray = ListDatabasesLC();

    // list registered databases
    Session().ListDatabasesL(databaseInfoArray, KFileProtocol);

    // unregister database which cannot be found on disk
    for (TInt i = databaseInfoArray.Count() - 1; i >= 0; i--)
        {
        TInt index;
        TPtrC registeredUri = databaseInfoArray[i]->DatabaseUri();
        if (dbUriArray->Find(registeredUri, index) != 0)
            {
            // unregister database
            User::LeaveIfError(Session().UnregisterDatabase(registeredUri,
                DatabaseDrive(registeredUri)));
            }
        }
    CleanupStack::PopAndDestroy(2, &databaseInfoArray);  // dbUriArray
    }

// ---------------------------------------------------------
// CPosLmLocalDatabaseManager::CreateDriveInfoListL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosLmLocalDatabaseManager::CreateDriveInfoListL()
    {
    RFs fs;
    CleanupClosePushL(fs);
    User::LeaveIfError(fs.Connect());

    TDriveList driveList;
    User::LeaveIfError(fs.DriveList(driveList));

    for (TInt drive = EDriveA; drive <= EDriveZ; drive++)
        {
        if ( driveList[drive] && !( driveList[drive] & KDriveAttRemote ) ) // avoid remote drives
            {
            TDriveInfo drvInfo;
            TInt err = fs.Drive( drvInfo, drive );

            if ( !err && drvInfo.iType != EMediaNotPresent )
                {
                TMediaInfo mediaInfo;
                mediaInfo.iMediaType = drvInfo.iType;

                User::LeaveIfError(fs.DriveToChar(drive, mediaInfo.iDrive));

                User::LeaveIfError(iMediaInfoList.Append(mediaInfo));
                }
            }
        }
    CleanupStack::PopAndDestroy(&fs);
    }

//  End of File

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
* Description: Provides functions for reading and changing
* settings in the Landmarks settings.
*
*
*/


// INCLUDE FILES
#include <centralrepository.h>
#include "EPos_CPosLmSettingsHandler.h"
#include "EPos_PosLmDatabaseUtility.h"
#include "LandmarksPrivateCRKeys.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmSettingsHandler::CPosLmSettingsHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmSettingsHandler::CPosLmSettingsHandler()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmSettingsHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmSettingsHandler::ConstructL()
    {
    iRepository = CRepository::NewL(KCRUidLandmarks);
    }

// -----------------------------------------------------------------------------
// CPosLmSettingsHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmSettingsHandler* CPosLmSettingsHandler::NewL()
    {
    CPosLmSettingsHandler* self = new (ELeave) CPosLmSettingsHandler;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosLmSettingsHandler::~CPosLmSettingsHandler()
    {
    delete iRepository;
    }

// -----------------------------------------------------------------------------
// CPosLmSettingsHandler::GetDefaultDatabaseUriL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC* CPosLmSettingsHandler::GetDefaultDatabaseUriL()
    {
    TBuf16<NCentralRepositoryConstants::KMaxUnicodeStringLength> buffer;
    User::LeaveIfError(iRepository->Get(KLandmarksDefaultUri, buffer));
    return buffer.AllocL();
    }

// -----------------------------------------------------------------------------
// CPosLmSettingsHandler::SetDefaultDatabaseUriL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmSettingsHandler::SetDefaultDatabaseUriL(
    const TDesC16& aDatabaseUri)
    {
    HBufC* uri = PosLmDatabaseUtility::CreateDatabaseUriL(aDatabaseUri);
    CleanupStack::PushL(uri);

    User::LeaveIfError(iRepository->Set(KLandmarksDefaultUri, *uri));
    CleanupStack::PopAndDestroy(uri);
    }

// -----------------------------------------------------------------------------
// CPosLmSettingsHandler::NotifySettingsChanged
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLmSettingsHandler::NotifySettingsChanged(
    TRequestStatus& aStatus)
    {
    return iRepository->NotifyRequest(KLandmarksDefaultUri, aStatus);
    }

// -----------------------------------------------------------------------------
// CPosLmSettingsHandler::CancelNotifySettingsChanged
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLmSettingsHandler::CancelNotifySettingsChanged()
    {
    return iRepository->NotifyCancel(KLandmarksDefaultUri);
    }

//  End of File

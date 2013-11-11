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
* Description:  This file contains the base implementation of Source.
*
*/


// INCLUDES
#include "SourceBase.h"

using namespace multimedia;

CSourceBase::~CSourceBase()
    {
    // No impl
    }

void CSourceBase::ServerSourceCreated(MCustomCommand& /*aCustomCommand*/,
                                      TMMFMessageDestination& /*aSourceHandle*/ )
    {
    // No impl
    }

void CSourceBase::ServerSourceDeleted()
    {
    // No impl
    }

TBool CSourceBase::IsEncrypted()
    {
    // No impl
    return false;
    }

TUid CSourceBase::GetSourceUid()
    {
    // No impl
    return KNullUid;
    }
TInt CSourceBase::GetHeaderData(TPtr& /*aPtr*/)
    {
    return KErrNone;
    }

//  End of File

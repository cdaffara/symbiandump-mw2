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
* Description:  Implementation of MultimediaDataSource inline.
*
*/


inline TInt CMultimediaDataSource::SetPrioritySettings(
                                                       const TMMFPrioritySettings& /*aPrioritySettings*/ )
    {
    return KErrNotSupported;
    }

inline TInt CMultimediaDataSource::GetDRMProtection( TBool& aIsProtected )
    {
    aIsProtected = EFalse;
    return KErrNone;
    }

inline TInt CMultimediaDataSource::EvaluateIntent( 
                                                  ContentAccess::TIntent /*aIntent*/ )
    {
    return KErrNotSupported;
    };

inline TInt CMultimediaDataSource::ExecuteIntent( 
                                                 ContentAccess::TIntent /*aIntent*/ )
    {
    return KErrNotSupported;
    };

inline TInt CMultimediaDataSource::SetAgentProperty( 
                                                    ContentAccess::TAgentProperty /*aProperty*/,
                                                    TInt /*aValue*/)
    {
    return KErrNotSupported;
    };

inline void CMultimediaDataSource::GetState( TState& aState )
    {
    aState = iState;
    };

inline TInt CMultimediaDataSource::GetSeekingSupport( TBool& aSeekSupport )
    {
    aSeekSupport = EFalse;
    return KErrNone;
    };

inline TInt CMultimediaDataSource::GetRandomSeekingSupport( 
                                                           TBool& aSeekSupport )
    {
    aSeekSupport = EFalse;
    return KErrNone;
    };

inline TInt CMultimediaDataSource::Seek(TUint /*aPosInBytes*/)
    {
    return KErrNotSupported;
    };

inline CMultimediaDataSource::~CMultimediaDataSource()
    {
    // No impl
    };

inline TInt CMultimediaDataSource::GetInterface(
                                        TUid /*aInterfaceId*/,
                                        TVersion& /*aVer*/,
                                        TAny*& aInterfaceImpl )
    {
    aInterfaceImpl = NULL;
    return KErrNotFound;
    }

// End of file

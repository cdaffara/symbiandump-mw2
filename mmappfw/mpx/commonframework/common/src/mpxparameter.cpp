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
* Description:  MPX parameter for commandline/message
*
*/


// INCLUDE FILES
#include <s32strm.h>
#include <mpxcollectionpath.h>

#include "mpxparameter.h"
#include "mpxlog.h"

// CONSTANTS

const TInt KMPXParameterVesion = 2;

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// C++ constructor can NOT contain any code that might leave.
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXParameter::CMPXParameter():
    iCollectionPath( NULL ),
    iType( KNullUid ),
    iCmdForward( EMPXCmdFwdStandAlone )
    {
    }

// Destructor
EXPORT_C CMPXParameter::~CMPXParameter()
    {
    delete iCollectionPath;
    }

// ---------------------------------------------------------------------------
// Externalize the object
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXParameter::ExternalizeL( RWriteStream& aStream ) const
    {
    MPX_FUNC( "CMPXParameter::ExternalizeL" );

    MPX_DEBUG2( "CMPXParameter::ExternalizeL parameter version = %d", KMPXParameterVesion );
    aStream.WriteInt32L( KMPXParameterVesion );

    MPX_DEBUG2( "CMPXParameter::ExternalizeL iCmdForward = %d", iCmdForward );
    aStream.WriteInt32L( iCmdForward );

    // externalize playlist
    if ( iCollectionPath )
        {
        // collection path exist
        MPX_DEBUG1( "CMPXParameter::ExternalizeL collection path exist" );
        MPX_DEBUG_PATH( *iCollectionPath );
        aStream.WriteInt32L( 1 );
        aStream << *iCollectionPath;
        }
    else
        {
        // no collection path
        MPX_DEBUG1( "CMPXParameter::ExternalizeL no collection path" );
        aStream.WriteInt32L( 0 );
        }

    MPX_DEBUG2( "CMPXParameter::ExternalizeL collection path type = %d", iPathType );
    aStream.WriteInt32L( iPathType );

    if ( iType.iUid > 0 )
        {
        // view uid exist
        MPX_DEBUG2( "CMPXParameter::ExternalizeL uid = 0x%x", iType.iUid );
        aStream.WriteInt32L( 1 );
        aStream.WriteInt32L( iType.iUid );
        }
    else
        {
        // no view uid
        MPX_DEBUG1( "CMPXParameter::ExternalizeL no uid" );
        aStream.WriteInt32L( 0 );
        }
    }

// ---------------------------------------------------------------------------
// Internalize the object
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXParameter::InternalizeL( RReadStream& aStream )
    {
    MPX_FUNC( "CMPXParameter::InternalizeL" );
    delete iCollectionPath;
    iCollectionPath = NULL;
    iPathType = EMPXTypeCollectionPath;
    iType = KNullUid;

    iVersion = aStream.ReadInt32L();
    MPX_DEBUG2( "CMPXParameter::InternalizeL iVersion = %d", iVersion );

    switch ( iVersion )
        {
        // this 0/1 is not the actual version number
        // In the first version of parameter,
        // 0 means no collection path, 1 means collection path exist
        case 1:
            {
            // collection path exist
            MPX_DEBUG1( "CMPXParameter::InternalizeL collection path" );
            iCollectionPath = CMPXCollectionPath::NewL();
            aStream >> *iCollectionPath;
            MPX_DEBUG_PATH( *iCollectionPath );
            } // fall through on purpose
        case 0:
            {
            iVersion = 1; // if it's 0, set the version to 1
            iPathType = static_cast<TMPXPathType>( aStream.ReadInt32L() );
            MPX_DEBUG2( "CMPXParameter::InternalizeL iPathType = %d", iPathType );

            TInt paramExist( aStream.ReadInt32L() );
            if ( paramExist == 1 )
                {
                // view uid exist
                iType.iUid = aStream.ReadInt32L();
                MPX_DEBUG2( "CMPXParameter::InternalizeL uid = 0x%x", iType.iUid );
                }
#ifdef _DEBUG
            else
                {
                MPX_DEBUG1( "CMPXParameter::InternalizeL no uid" );
                }
#endif
            break;
            }
        case 2:
            {
            iCmdForward = aStream.ReadInt32L();
            MPX_DEBUG2( "CMPXParameter::InternalizeL iCmdForward = %d", iCmdForward );

            TInt paramExist = aStream.ReadInt32L();
            if ( paramExist == 1 )
                {
                // collection path exist
                iCollectionPath = CMPXCollectionPath::NewL();
                aStream >> *iCollectionPath;
                MPX_DEBUG_PATH( *iCollectionPath );
                }
#ifdef _DEBUG
            else
                {
                MPX_DEBUG1( "CMPXParameter::InternalizeL no collection path" );
                }
#endif

            iPathType = static_cast<TMPXPathType>( aStream.ReadInt32L() );
            MPX_DEBUG2( "CMPXParameter::InternalizeL iPathType = %d", iPathType );

            paramExist = aStream.ReadInt32L();
            if ( paramExist == 1 )
                {
                // view uid exist
                iType.iUid = aStream.ReadInt32L();
                MPX_DEBUG2( "CMPXParameter::InternalizeL uid = 0x%x", iType.iUid );
                }
#ifdef _DEBUG
            else
                {
                MPX_DEBUG1( "CMPXParameter::InternalizeL no uid" );
                }
#endif
            break;
            }
        default:
            {
            User::Leave( KErrNotSupported );
            break;
            }
        }
    }


// End of File

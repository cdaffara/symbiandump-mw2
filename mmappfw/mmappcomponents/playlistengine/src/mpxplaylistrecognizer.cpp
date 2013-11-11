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
* Description:  Recognizer for playlist files.
*
*/



// INCLUDES
#include    <f32file.h>
#include    <barsread.h>
#include    <ecom/implementationproxy.h>
#include    <mpxplaylistenginedefs.h>
#include    <mpxplaylistenginedefs.hrh>
#include    <mpxlog.h>
#include    <mpxpluginhandlerbase.h>
#include    <mpxpluginhandlerobserver.h>
#include    "mpxplaylistrecognizer.h"


// LOCAL CONSTANTS AND MACROS
const TInt KExtLength = 4;

// MIME type of the playlist.
_LIT8(KPlaylistMimeType, "playlist/mpegurl");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMPXPlaylistRecognizer::CMPXPlaylistRecognizer()
// C++ default constructor can NOT contain any code, that
// might leave.
// Call base constructor with the recognizer's UID and confidence level
// -----------------------------------------------------------------------------
CMPXPlaylistRecognizer::CMPXPlaylistRecognizer()
    : CApaDataRecognizerType(TUid::Uid(KMPXPlaylistRecognizerDllUid),
                             CApaDataRecognizerType::EHigh)
    {
    }

// -----------------------------------------------------------------------------
// CMPXPlaylistRecognizer::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
void CMPXPlaylistRecognizer::ConstructL()
    {
    // Set the number of data (MIME) types supported by this recognizer.
    iCountDataTypes = 1;

    // instantiate a plugin hanlder that manages playlist plugins.
    iPluginHandler = new(ELeave)CMPXPluginHandlerBase(
                            TUid::Uid(KMPXPlaylistInterfaceUid),
                            CMPXPluginHandlerBase::ESelectionType, EMPXPlaylistTypeM3U,
                            *this);
    }

// -----------------------------------------------------------------------------
// CMPXPlaylistRecognizer::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
CMPXPlaylistRecognizer* CMPXPlaylistRecognizer::NewL()
    {
    MPX_DEBUG1("CMPXPlaylistRecognizer::NewL");
    CMPXPlaylistRecognizer* self = new (ELeave) CMPXPlaylistRecognizer();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
CMPXPlaylistRecognizer::~CMPXPlaylistRecognizer()
    {
    delete iPluginHandler;
    }

// -----------------------------------------------------------------------------
// CMPXPlaylistRecognizer::SupportedDataTypeL
// Return the MimeType of the DataType depending on
// mime type of the file
// -----------------------------------------------------------------------------
TDataType CMPXPlaylistRecognizer::SupportedDataTypeL(TInt aIndex ) const
    {
    MPX_DEBUG2("CMPXPlaylistRecognizer::SupportedDataTypeL [%d]", aIndex);

    TDataType dataType;
    switch ( aIndex )
        {
        case 0:
            dataType = TDataType(KPlaylistMimeType);
            break;
        default:
            User::Leave(KErrArgument);
            break;
        }
        return dataType;
    }

// -----------------------------------------------------------------------------
// CMPXPlaylistRecognizer::PreferredBufSize()
// Return the supposed minimum buffer size we need to
// successfully recognize the data
// -----------------------------------------------------------------------------
TUint CMPXPlaylistRecognizer::PreferredBufSize()
    {
    return 0;
    }

// -----------------------------------------------------------------------------
// CMPXPlaylistRecognizer::DoRecognizeL
// Attempt to recognize the data
// this recognizer only attempts to match the data and/or file extension
//
// NB if the file is not recognized, this function should NOT leave :
// it should instead set iConfidence = ENotRecognized and return
// the function should only leave if there is an out-of-memory condition
// -----------------------------------------------------------------------------
void CMPXPlaylistRecognizer::DoRecognizeL(
        const TDesC& aName,
        const TDesC8& /*aBuffer*/ )
    {
    MPX_DEBUG2("CMPXPlaylistRecognizer::DoRecognizeL: aName = %S", &aName);

    // assume match will fail
    iConfidence = CApaDataRecognizerType::ENotRecognized;
    if ( aName.Length() < KExtLength )
        {
        MPX_DEBUG1("CMPXPlaylistRecognizer::DoRecognizeL - Ext not present");
        return;
        }

    // get a list of supported extension from playlist plugin handler
    iPluginHandler->CreatePluginListL();
    CDesCArray* supportedExtensions =
        iPluginHandler->SupportedExtensionsL();

    //
    // determine if the given filename contains an extension that's listed
    // as supported
    //
    TInt count = supportedExtensions->Count();
    for ( TInt i = 0; i < count; i++ )
        {
        TPtrC extension = (*supportedExtensions)[i];

        if ( aName.Right( extension.Length() ).CompareF(extension) == 0 )
            {
            MPX_DEBUG1("CMPXPlaylistRecognizer::DoRecognizeL - Possible match");
            iDataType = TDataType(KPlaylistMimeType);
            iConfidence = ECertain;
            break;
            }
        }

    delete supportedExtensions;
    }

// -----------------------------------------------------------------------------
// CMPXPlaylistRecognizer::HandlePluginHandlerEvent
// -----------------------------------------------------------------------------
//
void CMPXPlaylistRecognizer::HandlePluginHandlerEvent(
    TPluginHandlerEvents /* aEvent */,
    const TUid& /* aPluginUid */,
    TBool /* aLoaded */,
    TInt /* aData */)
    {
    // Do nothing
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================
// -----------------------------------------------------------------------------
// provides an array that maps a UID for each implementation to its respective
// creation function
// -----------------------------------------------------------------------------
const TImplementationProxy ImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY(KMPXPlaylistRecognizerImplUid, CMPXPlaylistRecognizer::NewL)
    };

// -----------------------------------------------------------------------------
// returns the proxy of implementations
// -----------------------------------------------------------------------------
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }

// End of file



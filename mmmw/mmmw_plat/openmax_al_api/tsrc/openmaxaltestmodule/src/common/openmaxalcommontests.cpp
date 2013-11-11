/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: This file contains testclass implementation.
*
*/

// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <StifTestInterface.h>
#include "openmaxaltestmodule.h"
#include <coedef.h>

// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def

// LOCAL CONSTANTS AND MACROS
//const ?type ?constant_var = ?constant;
//#define ?macro_name ?macro_def

// MODULE DATA STRUCTURES
//enum ?declaration
//typedef ?declaration

// LOCAL FUNCTION PROTOTYPES
//?type ?function_name( ?arg_type, ?arg_type );

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ?function_name ?description.
// ?description
// Returns: ?value_1: ?description
//          ?value_n: ?description_line1
//                    ?description_line2
// -----------------------------------------------------------------------------
//
/*
?type ?function_name(
    ?arg_type arg,  // ?description
    ?arg_type arg)  // ?description
    {

    ?code  // ?comment

    // ?comment
    ?code
    }
*/

TInt COpenMAXALTestModule::al_SetDataSink( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt type;
    TInt formattype(0);

    TInt sinktype(0);
    XADataSink commonSink = {NULL,NULL};

    status = aItem.GetNextInt(type);
    if(!status)
        {
        switch(type)
            {
            case XA_DATALOCATOR_URI:
                {
                commonSink.pLocator = &m_Uri;
                }
                break;
            case XA_DATALOCATOR_ADDRESS:
            case XA_DATALOCATOR_IODEVICE:
                {
                m_SinkIODevice.locatorType = XA_DATALOCATOR_IODEVICE;
                commonSink.pLocator = &m_SinkIODevice;
                }
                break;
            case XA_DATALOCATOR_NATIVEDISPLAY:
                {
                m_NativeDisplay.locatorType = XA_DATALOCATOR_NATIVEDISPLAY;
                m_NativeDisplay.hWindow = (void*)&iRWindow;
                m_NativeDisplay.hDisplay = (void*)&iRwSession;
                commonSink.pLocator = &m_NativeDisplay;
                }
                break;
            case XA_DATALOCATOR_OUTPUTMIX:
            default:
                status = KErrGeneral;
                break;
            }
        }

    if(!status)
        {
        status = aItem.GetNextInt(formattype);
        if (type == XA_DATALOCATOR_IODEVICE)
            { /* Sink IO device can only be audio output*/
            if (formattype == 6)
                {
                m_SinkIODevice.deviceType = formattype;
                }
            else
                {
                status = KErrGeneral;
                }
            }
        else
            {
            switch(formattype)
                {
                case XA_DATAFORMAT_MIME:
                    {
                    commonSink.pFormat = &m_Mime;
                    }
                    break;
                case XA_DATAFORMAT_PCM:
                case XA_DATAFORMAT_RAWIMAGE:
                    break;
                default:
                    break;
                }
            }
        }

    if(!status)
         {
         status = aItem.GetNextInt(sinktype);
         if(!status)
             {
             switch(sinktype)
                 {
                 case 1:
                     m_AudioSink = commonSink;
                     break;
                 case 2:
                     m_VideoSink = commonSink;
                     break;
                 case 3:
                     m_LEDSink = commonSink;
                     break;
                 case 4:
                     m_VibraSink = commonSink;
                     break;
                 case 5:
                     m_DataSink = commonSink;
                     break;
                 default:
                     status = KErrGeneral;
                     break;
                 }
             }
         }
    return status;
    }

TInt COpenMAXALTestModule::al_SetDataSource( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt type;
    TInt sourcetype(0);
    TInt formattype(0);

    XADataSource commonSource = {NULL,NULL};
    status = aItem.GetNextInt(type);
    if(!status)
        {
        switch(type)
            {
            case XA_DATALOCATOR_URI:
                {
                commonSource.pLocator = &m_Uri;
                }
                break;
            case XA_DATALOCATOR_ADDRESS:
            case XA_DATALOCATOR_IODEVICE:
                {
                commonSource.pLocator = &m_SrcIODevice;
                }
            case XA_DATALOCATOR_OUTPUTMIX:
            case XA_DATALOCATOR_NATIVEDISPLAY:
                break;
            default:
                status = KErrGeneral;
                break;
            }
        }

    if(!status)
        {
        status = aItem.GetNextInt(formattype);
        if(!status)
            {
            switch(formattype)
                {
                case XA_DATAFORMAT_MIME:
                    {
                    commonSource.pFormat = (void*) &m_Mime;
                    }
                    break;
                case XA_DATAFORMAT_PCM:
                case XA_DATAFORMAT_RAWIMAGE:
                    break;
                default:
                    break;
                }
            }
        }


    if(!status)
        {
        status = aItem.GetNextInt(sourcetype);
        if(!status)
            {
            switch(sourcetype)
                {
                case 1:
                    m_AudioSource = commonSource;
                    break;
                case 2:
                    m_VideoSource = commonSource;
                    break;
                case 3:
                    m_MOMetadataExtractorSource = commonSource;
                    break;
                case 4:
                    m_DynamicSource = commonSource;
                    break;

                default:
                    status = KErrGeneral;
                    break;
                }
            }
        }

    return status;
    }

TInt COpenMAXALTestModule::al_SetDataLocator( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt type;

    status = aItem.GetNextInt(type);

    switch(type)
        {
        case XA_DATALOCATOR_URI:
            {
            TPtrC uri;
            if(m_URIName)
                {
                delete m_URIName;
                m_URIName = NULL;
                }
            //status = aItem.SetParsingType(CStifItemParser::EQuoteStyleParsing);
            status = aItem.GetNextString(uri);
            if(!status)
                {
                m_URIName = HBufC8::NewL(uri.Length()+1);
                TPtr8 desc = m_URIName->Des();
                desc.Copy(uri);
                m_Uri.locatorType = XA_DATALOCATOR_URI;
                m_Uri.URI = (XAchar*) desc.PtrZ();
                }
            else
                {
                status = KErrGeneral;
                }
            }
            break;
        case XA_DATALOCATOR_IODEVICE:
            {
            TInt devicetype;
            TUint deviceId;

            status = aItem.GetNextInt(devicetype);
            if(!status)
                {
                status = aItem.GetNextInt(deviceId);
                if(!status)
                    {
                    TInt srcsinktype(0);
                    status = aItem.GetNextInt(srcsinktype);
                    if(!status)
                        {
                        if(srcsinktype == 1)
                            {
                            m_SrcIODevice.deviceID = deviceId;
                            m_SrcIODevice.deviceType = devicetype;
                            m_SrcIODevice.locatorType = XA_DATALOCATOR_IODEVICE;
                            }
                        else
                            {
                            m_SinkIODevice.deviceID = deviceId;
                            m_SinkIODevice.deviceType = devicetype;
                            m_SinkIODevice.locatorType = XA_DATALOCATOR_IODEVICE;
                            }
                        }
                    else
                        {
                        status = KErrGeneral;
                        }
                    }
                else
                    {
                    status = KErrGeneral;
                    }
                }
            else
                {
                status = KErrGeneral;
                }
            }
            break;
        case XA_DATALOCATOR_OUTPUTMIX:
        case XA_DATALOCATOR_NATIVEDISPLAY:
        case XA_DATALOCATOR_ADDRESS:
            break;
        default:
            status = KErrGeneral;
            break;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_SetDataFormat( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt type;
    TInt contType(0);
    TPtrC mimetype;
    status = aItem.GetNextInt(type);

    switch(type)
        {
        case XA_DATAFORMAT_MIME:
            {
            status = aItem.GetNextString(mimetype);
            if(!status)
                {
                status = aItem.GetNextInt(contType);
                if(!status)
                    {
                    if(m_MimeType)
                        {
                        delete m_MimeType;
                        m_MimeType = NULL;
                        }
                    m_MimeType = HBufC8::NewL(mimetype.Length()+1);
                    TPtr8 desc = m_MimeType->Des();
                    desc.Copy(mimetype);
                    m_Mime.formatType = XA_DATAFORMAT_MIME;
                    m_Mime.mimeType = (XAchar*) desc.PtrZ();
                    m_Mime.containerType = contType;
                    }
                else
                    {
                    status = KErrGeneral;
                    }
                }
            else
                {
                status = KErrGeneral;
                }
            }
            break;
        case XA_DATAFORMAT_PCM:
        case XA_DATAFORMAT_RAWIMAGE:
            break;
        default:
            status = KErrGeneral;
            break;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_CreateWindow( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    if (iRwSession.Handle())
        User::Leave(KErrAlreadyExists);

    TInt topX;
    TInt topY ;
    TInt bottomX;
    TInt bottomY;
    TInt posX;
    TInt posY;

    status = aItem.GetNextInt(posX);
    RET_ERR_IF_ERR(status);
    status = aItem.GetNextInt(posY);
    RET_ERR_IF_ERR(status);

    status = aItem.GetNextInt(topX);
    RET_ERR_IF_ERR(status);
    status = aItem.GetNextInt(topY);
    RET_ERR_IF_ERR(status);
    status = aItem.GetNextInt(bottomX);
    RET_ERR_IF_ERR(status);
    status = aItem.GetNextInt(bottomY);
    RET_ERR_IF_ERR(status);

    /* Create resources needed for video play back*/
    status = iRwSession.Connect();
    RET_ERR_IF_ERR(status);

    TInt groupId = iRwSession.GetFocusWindowGroup();
    iRwGroup = RWindowGroup(iRwSession);
    status = iRwGroup.Construct(groupId, ETrue);
    RET_ERR_IF_ERR(status);
    _LIT(KXASTIFWindowGroupName,"XASTIF");
    iRwGroup.SetName(KXASTIFWindowGroupName);
    iRwGroup.SetOrdinalPosition(0, ECoeWinPriorityAlwaysAtFront);

    iRWindow = RWindow(iRwSession);
    status = iRWindow.Construct(iRwGroup, (TUint32) this + 1);
    RET_ERR_IF_ERR(status);

    TPoint point(posX, posY);
    iRWindow.SetPosition(point);
    TRect rect(topX, topY, bottomX, bottomY);
    TSize size(rect.Size());
    iRWindow.SetSize(size);
    iRWindow.Activate();
    iRWindow.SetVisible(ETrue);
    iRwSession.Flush();
    iDevice = NULL;
    iDevice = new CWsScreenDevice(iRwSession);
    if (!iDevice)
        {
        status = KErrNoMemory;
        }
    RET_ERR_IF_ERR(status);

    status = iDevice->Construct(0);
    return status;
    }

TInt COpenMAXALTestModule::al_DeleteWindow( CStifItemParser& /*aItem*/ )
    {
    delete iDevice;
    iDevice = NULL;
    iRWindow.SetVisible(EFalse);
    iRWindow.Close();
    iRwGroup.Close();
    iRwSession.Close();
    return KErrNone;
    }

TInt COpenMAXALTestModule::CheckForNull(CStifItemParser& aItem, void*& paramPtr)
    {
    TInt status(KErrNone);
    TPtrC itfc;
    _LIT(ifc,"null");
    _LIT(invalidItfc, "invalid");
    status = aItem.GetNextString(itfc);
    if(!status)
        {
        if(!itfc.Compare(ifc))
            {
            paramPtr = NULL;
            }
        else if (itfc.Compare(invalidItfc) == 0)
            {
            paramPtr = (void *)&aItem;
            }
        }
    return status;
    }

TInt COpenMAXALTestModule::CheckForNullParam(CStifItemParser& aItem, void*& paramPtr)
    {
    TInt status(KErrNone);
    TInt itfc;
    status = aItem.GetNextInt(itfc);
    if(!status)
        {
        if(itfc != KErrNotFound)
            {
            paramPtr = (void*)&itfc;
            }
        else
            {
            paramPtr = NULL;
            }
        }
    return status;
    }

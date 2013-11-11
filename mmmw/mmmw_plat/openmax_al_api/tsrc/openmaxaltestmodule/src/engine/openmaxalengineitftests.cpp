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
#include "profileutilmacro.h"

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
TInt COpenMAXALTestModule::al_engitf_CreateCameraDevice( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt objectId;
    TInt numInterfaces(0);    
    XAresult res;
    
    
    /* Initialize arrays required[] and iidArray[] */
    for (TInt i = 0; i < MAX_NUMBER_INTERFACES; i++)
        {
        required[i] = XA_BOOLEAN_FALSE;
        iidArray[i] = XA_IID_NULL;
        }

    status = aItem.GetNextInt(numInterfaces);
    if(!status)
        {
        for(TInt j = 0; j < numInterfaces; j++)
            {
            TInt interface(0);
            status = aItem.GetNextInt(interface);
            if(!status)
                {
                required[j] = XA_BOOLEAN_TRUE;
                iidArray[j] = MapInterface(interface);
                numInterfaces++;
                }
            else
                {
                break;
                }
            }
        }

    
    status = aItem.GetNextInt(objectId);
    if(!status)
        {
        if(m_EngineItf)
            {
            res = (*m_EngineItf)->CreateCameraDevice(
                    m_EngineItf, &m_MOCameraObject, objectId,
                    numInterfaces, iidArray, required);
            status = res;
            }
        else
            {
            status = KErrNotFound;
            }    
        }
    return status;
    }


TInt COpenMAXALTestModule::al_engitf_CreateRadioDevice( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt numInterfaces(0);    
    XAresult res;
    
    
    /* Initialize arrays required[] and iidArray[] */
    for (TInt i = 0; i < MAX_NUMBER_INTERFACES; i++)
        {
        required[i] = XA_BOOLEAN_FALSE;
        iidArray[i] = XA_IID_NULL;
        }

    status = aItem.GetNextInt(numInterfaces);
    if(!status)
        {
        for(TInt j = 0; j < numInterfaces; j++)
            {
            TInt interface(0);
            status = aItem.GetNextInt(interface);
            if(!status)
                {
                required[j] = XA_BOOLEAN_TRUE;
                iidArray[j] = MapInterface(interface);
                numInterfaces++;
                }
            else
                {
                break;
                }
            }
        }
    
    
    if(m_EngineItf)
        {
        res = (*m_EngineItf)->CreateRadioDevice(
                m_EngineItf, &m_MORadioObject,
                numInterfaces, iidArray, required);
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }    
    return status;
    }

TInt COpenMAXALTestModule::al_engitf_CreateLEDDevice( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt objectId;
    TInt numInterfaces(0);    
    XAresult res;
    
    
    /* Initialize arrays required[] and iidArray[] */
    for (TInt i = 0; i < MAX_NUMBER_INTERFACES; i++)
        {
        required[i] = XA_BOOLEAN_FALSE;
        iidArray[i] = XA_IID_NULL;
        }

    status = aItem.GetNextInt(numInterfaces);
    if(!status)
        {
        for(TInt j = 0; j < numInterfaces; j++)
            {
            TInt interface(0);
            status = aItem.GetNextInt(interface);
            if(!status)
                {
                required[j] = XA_BOOLEAN_TRUE;
                iidArray[j] = MapInterface(interface);
                numInterfaces++;
                }
            else
                {
                break;
                }
            }
        }
    
    
    status = aItem.GetNextInt(objectId);
    if(!status)
        {
        if(m_EngineItf)
            {
            res = (*m_EngineItf)->CreateLEDDevice(
                    m_EngineItf, &m_MOVibraObject, objectId,
                    numInterfaces, iidArray, required);
            status = res;
            }
        else
            {
            status = KErrNotFound;
            }    
        }
    return status;
    }

TInt COpenMAXALTestModule::al_engitf_CreateVibraDevice( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt objectId;
    TInt numInterfaces(0);    
    XAresult res;
    
    
    /* Initialize arrays required[] and iidArray[] */
    for (TInt i = 0; i < MAX_NUMBER_INTERFACES; i++)
        {
        required[i] = XA_BOOLEAN_FALSE;
        iidArray[i] = XA_IID_NULL;
        }

    status = aItem.GetNextInt(numInterfaces);
    if(!status)
        {
        for(TInt j = 0; j < numInterfaces; j++)
            {
            TInt interface(0);
            status = aItem.GetNextInt(interface);
            if(!status)
                {
                required[j] = XA_BOOLEAN_TRUE;
                iidArray[j] = MapInterface(interface);
                numInterfaces++;
                }
            else
                {
                break;
                }
            }
        }
    
    
    status = aItem.GetNextInt(objectId);
    if(!status)
        {
        if(m_EngineItf)
            {
            res = (*m_EngineItf)->CreateVibraDevice(
                    m_EngineItf, &m_MOVibraObject, objectId,
                    numInterfaces, iidArray, required);
            status = res;
            }
        else
            {
            status = KErrNotFound;
            }    
        }
    return status;
    }

TInt COpenMAXALTestModule::al_engitf_CreateMediaPlayer( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XADataSource* dataSource(NULL);
    XADataSink* audioSink(NULL);
    XADataSink* videoSink(NULL);
    XADataSink* ledSink(NULL);
    XADataSink* vibraSink(NULL);
    TInt numInterfaces(0);    
    XAresult res;
    
    
    /* Initialize arrays required[] and iidArray[] */
    for (TInt i = 0; i < MAX_NUMBER_INTERFACES; i++)
        {
        required[i] = XA_BOOLEAN_FALSE;
        iidArray[i] = XA_IID_NULL;
        }

    status = aItem.GetNextInt(numInterfaces);
    if(!status)
        {
        for(TInt j = 0; j < numInterfaces; j++)
            {
            TInt interface(0);
            status = aItem.GetNextInt(interface);
            if(!status)
                {
                required[j] = XA_BOOLEAN_TRUE;
                iidArray[j] = MapInterface(interface);
                numInterfaces++;
                }
            else
                {
                break;
                }
            }
        }
    
    /*pFormat is not mandatory for RadioSrc*/
    if(/*m_AudioSource.pFormat &&*/ m_AudioSource.pLocator)
        {
        dataSource = &m_AudioSource;
        }
    else if(m_VideoSource.pFormat && m_VideoSource.pLocator)
        {
        dataSource = &m_VideoSource;
        }
    
    if(m_VibraSink.pFormat && m_VibraSink.pLocator)
        {
        vibraSink = &m_VibraSink;
        }
    
    if(m_LEDSink.pFormat && m_LEDSink.pLocator)
        {
        ledSink = &m_LEDSink;
        }
    /*pFormat is not mandatory for IODevice*/
    if(/*m_AudioSink.pFormat && */m_AudioSink.pLocator)
        {
        audioSink = &m_AudioSink;
        }
    /*pFormat is not mandatory for NativeDisplay*/
    if(/*m_VideoSink.pFormat && */m_VideoSink.pLocator)
        {
        videoSink = &m_VideoSink;
        }    
    
    if(m_EngineItf)
        {
        TAG_TIME_PROFILING_BEGIN;
        res = (*m_EngineItf)->CreateMediaPlayer(
                m_EngineItf, &m_MOPlayer, dataSource, NULL,
                audioSink,videoSink, ledSink, vibraSink,
                numInterfaces, iidArray, required);
        TAG_TIME_PROFILING_END;
        PRINT_TO_CONSOLE_TIME_DIFF;
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_engitf_CreateMediaRecorder( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    TInt numInterfaces(0);
    XADataSource* audioSource(NULL);
    XADataSource* videoSource(NULL);
    XADataSink* dataSink(NULL);

    audioSource = &m_AudioSource;
    videoSource = &m_VideoSource;
    dataSink = &m_DataSink;
    
    void* itfPtr = (void*) m_EngineItf;
    
    status = CheckForNull(aItem, itfPtr);
    RET_ERR_IF_ERR(status);
    
    void* param0 = (void*)(&m_MORecorder);
    void* param1 = audioSource;
    void* param2 = videoSource;
    void* param3 = dataSink;
    
    status = CheckForNull(aItem, param0);
    RET_ERR_IF_ERR(status);
    status = CheckForNull(aItem, param1);
    RET_ERR_IF_ERR(status);    
    status = CheckForNull(aItem, param2);
    RET_ERR_IF_ERR(status);    
    status = CheckForNull(aItem, param3);
    RET_ERR_IF_ERR(status);    
    /* Initialize arrays required[] and iidArray[] */
    for (TInt i = 0; i < MAX_NUMBER_INTERFACES; i++)
        {
        required[i] = XA_BOOLEAN_FALSE;
        iidArray[i] = XA_IID_NULL;
        }

    status = aItem.GetNextInt(numInterfaces);
    if(!status)
        {
        for(TInt j = 0; j < numInterfaces; j++)
            {
            TInt interface(0);
            status = aItem.GetNextInt(interface);
            if(!status)
                {
                required[j] = XA_BOOLEAN_TRUE;
                iidArray[j] = MapInterface(interface);
                numInterfaces++;
                }
            else
                {
                break;
                }
            }
        }
    
    if(m_EngineItf)
        {
        TAG_TIME_PROFILING_BEGIN;
        res = (*m_EngineItf)->CreateMediaRecorder(
                XAEngineItf(itfPtr), /*&(*((XAObjectItf*)param0))*/&m_MORecorder, (XADataSource*)(param1),
                (XADataSource*)(param2), (XADataSink*)(param3),
                numInterfaces, iidArray, required);
        TAG_TIME_PROFILING_END;
        PRINT_TO_CONSOLE_TIME_DIFF;
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_engitf_CreateOutputMix( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    TInt numInterfaces(0);

    /* Initialize arrays required[] and iidArray[] */
    for (TInt i = 0; i < MAX_NUMBER_INTERFACES; i++)
        {
        required[i] = XA_BOOLEAN_FALSE;
        iidArray[i] = XA_IID_NULL;
        }

    status = aItem.GetNextInt(numInterfaces);
    if(!status)
        {
        for(TInt j = 0; j < numInterfaces; j++)
            {
            TInt interface(0);
            status = aItem.GetNextInt(interface);
            if(!status)
                {
                required[j] = XA_BOOLEAN_TRUE;
                iidArray[j] = MapInterface(interface);
                numInterfaces++;
                }
            else
                {
                break;
                }
            }
        }    
    
    if(m_EngineItf)
        {
        res = (*m_EngineItf)->CreateOutputMix(
                m_EngineItf, &m_MOOutputMix, 
                numInterfaces, iidArray, required);
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_engitf_CreateMetadataExtractor( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    TInt numInterfaces(0);
    XADataSource* metadataSource(NULL);
    /* Initialize arrays required[] and iidArray[] */
    for (TInt i = 0; i < MAX_NUMBER_INTERFACES; i++)
        {
        required[i] = XA_BOOLEAN_FALSE;
        iidArray[i] = XA_IID_NULL;
        }

    status = aItem.GetNextInt(numInterfaces);
    if(!status)
        {
        for(TInt j = 0; j < numInterfaces; j++)
            {
            TInt interface(0);
            status = aItem.GetNextInt(interface);
            if(!status)
                {
                required[j] = XA_BOOLEAN_TRUE;
                iidArray[j] = MapInterface(interface);
                numInterfaces++;
                }
            else
                {
                break;
                }
            }
        }
    
    if(m_MOMetadataExtractorSource.pFormat && m_MOMetadataExtractorSource.pLocator)
        {
        metadataSource = &m_MOMetadataExtractorSource;
        }
    
    if(m_EngineItf)
        {
        res = (*m_EngineItf)->CreateMetadataExtractor(
                m_EngineItf, &m_MOMetadataExtractor, metadataSource,
                numInterfaces, iidArray, required);
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_engitf_CreateExtensionObject( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    TInt numInterfaces(0);
    TInt objectId(0);
    
    /* Initialize arrays required[] and iidArray[] */
    for (TInt i = 0; i < MAX_NUMBER_INTERFACES; i++)
        {
        required[i] = XA_BOOLEAN_FALSE;
        iidArray[i] = XA_IID_NULL;
        }

    status = aItem.GetNextInt(numInterfaces);
    if(!status)
        {
        for(TInt j = 0; j < numInterfaces; j++)
            {
            TInt interface(0);
            status = aItem.GetNextInt(interface);
            if(!status)
                {
                required[j] = XA_BOOLEAN_TRUE;
                iidArray[j] = MapInterface(interface);
                numInterfaces++;
                }
            else
                {
                break;
                }
            }
        }    
    
    
    status = aItem.GetNextInt(objectId);
    if(!status)
        {
        if(m_EngineItf)
            {
            res = (*m_EngineItf)->CreateExtensionObject(
                    m_EngineItf, &m_MOExtnObject, NULL, objectId,
                    numInterfaces, iidArray, required);
            status = res;
            }
        else
            {
            status = KErrNotFound;
            }    
        }
    return status;
    }

TInt COpenMAXALTestModule::al_engitf_GetImplementationInfo( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAresult res;
    XAuint32 major;
    XAuint32 minor;
    XAuint32 step;
    const XAchar* implText(NULL); 
    
    if(m_EngineItf)
        {
        res = (*m_EngineItf)->GetImplementationInfo(
                m_EngineItf, &major, &minor, &step, implText);
        status = res;
        iLog->Log(_L("GetImplementationInfo Major[%d] Minor[%d] Step[%d]"),major,minor,step);
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_engitf_QuerySupportedProfiles( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAresult res;
    XAint16 profiles;
    
    if(m_EngineItf)
        {
        res = (*m_EngineItf)->QuerySupportedProfiles(
                m_EngineItf, &profiles);
        status = res;
        iLog->Log(_L("QuerySupportedProfiles Profiles[%d] "),profiles);
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_engitf_QueryNumSupportedInterfaces( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TUint object(0);
    XAuint32 numInterfaces(0);
    XAresult res;
    void* itfPtr = (void*) m_EngineItf;
    void* param = (void*)&numInterfaces;
    
    status = CheckForNull(aItem, itfPtr);
    RET_ERR_IF_ERR(status);
    
    status = aItem.GetNextInt(object);
    RET_ERR_IF_ERR(status);
    
    status = CheckForNull(aItem, param);
    RET_ERR_IF_ERR(status);

    if(m_EngineItf)
        {
        res = (*m_EngineItf)->QueryNumSupportedInterfaces(
                XAEngineItf(itfPtr), (XAuint32)object, (XAuint32*)param);
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_engitf_QuerySupportedInterfaces( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TUint object(0);
    TInt index(0);
    XAresult res;
    XAInterfaceID interface;
    
    void* itfPtr = (void*) m_EngineItf;
    void* param = (void*)&interface;
    
    status = CheckForNull(aItem, itfPtr);
    RET_ERR_IF_ERR(status);
    
    status = aItem.GetNextInt(object);
    RET_ERR_IF_ERR(status);
    

    status = aItem.GetNextInt(index);
    RET_ERR_IF_ERR(status);

    status = CheckForNull(aItem, param);
    RET_ERR_IF_ERR(status);
    
    if(m_EngineItf)
        {
        res = (*m_EngineItf)->QuerySupportedInterfaces(
                XAEngineItf(itfPtr), (XAuint32)object, index, (XAInterfaceID*)param);
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }

    return status;
    }

TInt COpenMAXALTestModule::al_engitf_QueryNumSupportedExtension( CStifItemParser& /*aItem*/ )
    {
    TInt status(KErrNone);
    XAresult res;
    XAuint32 extensions;
    
    if(m_EngineItf)
        {
        res = (*m_EngineItf)->QueryNumSupportedExtensions(
                m_EngineItf, &extensions);
        status = res;
        iLog->Log(_L("QueryNumSupportedExtensions Extensions[%d] "),extensions);
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_engitf_QuerySupportedExtension( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    TInt length(0);
    TInt index(0);
    XAresult res;
    XAint16 namelen;
    status = aItem.GetNextInt(index);
    XAchar extensionName[20];
    if(!status)
        {
        status = aItem.GetNextInt(length);
        if(!status)
            {
            if(m_EngineItf)
                {
                res = (*m_EngineItf)->QuerySupportedExtension(
                        m_EngineItf, index, extensionName, &namelen);
                status = res;
                }
            else
                {
                status = KErrNotFound;
                }
            }
        else
            {
            if(m_EngineItf)
                {
                res = (*m_EngineItf)->QuerySupportedExtension(
                        m_EngineItf,  index, NULL, &namelen);
                status = res;
                }
            else
                {
                status = KErrNotFound;
                }
            }
        }
    else
        {
        status = KErrGeneral;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_engitf_IsExtensionSupported( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAboolean supported(0);
    TPtrC extensionName;
    XAresult res;
    HBufC8* extnName;
    
    status = aItem.GetNextString(extensionName);
    if(!status)
        {
        extnName = HBufC8::NewL(extensionName.Length()+1);
        CleanupStack::PushL(extnName);
        TPtr8 des = extnName->Des();
        des.Copy(extensionName);
        if(m_EngineItf)
            {
            res = (*m_EngineItf)->IsExtensionSupported(
                    m_EngineItf,des.PtrZ(), &supported );
            status = res;
            }
        else
            {
            status = KErrNotFound;
            }
        CleanupStack::PopAndDestroy(extnName);
        }
    
    return status;
    }

TInt COpenMAXALTestModule::al_engitf_QueryLEDCapabilities( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAuint32 deviceId(0);
    TInt index(0);
    XAuint32 ledindex(0);
    XAresult res;
    XALEDDescriptor desc;

    status = aItem.GetNextInt(index);    
    if(!status)
        {
        ledindex = index;
        if(m_EngineItf)
            {
            res = (*m_EngineItf)->QueryLEDCapabilities(
                    m_EngineItf, &ledindex, &deviceId, &desc);
            status = res;
            }
        else
            {
            status = KErrNotFound;
            }
        }
    else
        {
        if(m_EngineItf)
            {
            res = (*m_EngineItf)->QueryLEDCapabilities(
                    m_EngineItf,  &ledindex, NULL, NULL);
            status = res;
            }
        else
            {
            status = KErrNotFound;
            }
        }
    return status;
    }

TInt COpenMAXALTestModule::al_engitf_QueryVibraCapabilities( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAuint32 deviceId(0);
    TInt index(0);
    XAuint32 ledindex(0);
    XAresult res;
    XAVibraDescriptor desc;

    status = aItem.GetNextInt(index);    
    if(!status)
        {
        ledindex = index;
        if(m_EngineItf)
            {
            res = (*m_EngineItf)->QueryVibraCapabilities(
                    m_EngineItf, &ledindex, &deviceId, &desc);
            status = res;
            }
        else
            {
            status = KErrNotFound;
            }
        }
    else
        {
        if(m_EngineItf)
            {
            res = (*m_EngineItf)->QueryVibraCapabilities(
                    m_EngineItf,  &ledindex, NULL, NULL);
            status = res;
            }
        else
            {
            status = KErrNotFound;
            }
        }
    return status;
    }


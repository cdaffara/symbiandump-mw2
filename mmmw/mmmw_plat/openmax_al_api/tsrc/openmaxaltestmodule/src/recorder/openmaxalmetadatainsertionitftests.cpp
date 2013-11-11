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
#include <string.h>
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
void MetadataInsertionCallback (
    XAMetadataInsertionItf caller,
    void * pContext,
    XAMetadataInfo * pKey,
    XAMetadataInfo * pValue,
    XAint32 nodeID,
    XAboolean result
);
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

TInt COpenMAXALTestModule::al_metadatainsertionitf_CreateChildNode( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    TInt parentId(0);
    TInt type(0);
    TPtrC mimetype;
    XAint32 childnode;
    _LIT8(paramnull, "null");
    
    void* param1 = (void*)&childnode;
    void* itfPtr = (void*) m_MetadataInsertionItf;
    void* param2 = NULL;
    status = CheckForNull(aItem, itfPtr);
    RET_ERR_IF_ERR(status);
    
    status = aItem.GetNextInt(parentId);
    RET_ERR_IF_ERR(status);
    
    status = aItem.GetNextInt(type);
    RET_ERR_IF_ERR(status);
    
    status = aItem.GetNextString(mimetype);
    RET_ERR_IF_ERR(status);
        
    HBufC8* buf = HBufC8::NewL(mimetype.Length()+1);
    CleanupStack::PushL(buf);
    TPtr8 des = buf->Des();
    des.Copy(mimetype);
    if(des.Compare(paramnull))
        {
        param2 = (void*)des.PtrZ();
        }
    
    status = CheckForNullParam(aItem, param1);
    RET_ERR_IF_ERR(status);
    
    if(m_MetadataInsertionItf)
        {
        res = (*m_MetadataInsertionItf)->CreateChildNode(
                XAMetadataInsertionItf(itfPtr), parentId , type, (XAchar*)param2, (XAint32*)param1);              
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }
    CleanupStack::PopAndDestroy(buf);
    return status;
    }

TInt COpenMAXALTestModule::al_metadatainsertionitf_GetSupportedKeysCount( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    TInt parentId(0);
    XAboolean freeKeys;
    XAuint32 keycount(0);
    XAuint32 encodingcount(0);
    void* param1 = (void*)&freeKeys;
    void* itfPtr = (void*) m_MetadataInsertionItf;
    void* param2 = (void*)&keycount;
    void* param3 = (void*)&encodingcount;
    
    status = CheckForNull(aItem, itfPtr);
    RET_ERR_IF_ERR(status);
    
    status = aItem.GetNextInt(parentId);
    RET_ERR_IF_ERR(status);
    
    status = CheckForNullParam(aItem, param1);
    RET_ERR_IF_ERR(status);
    
    status = CheckForNullParam(aItem, param2);
    RET_ERR_IF_ERR(status);
    
    status = CheckForNullParam(aItem, param3);
    RET_ERR_IF_ERR(status);
    
    if(m_MetadataInsertionItf)
        {
        res = (*m_MetadataInsertionItf)->GetSupportedKeysCount(
                XAMetadataInsertionItf(itfPtr), parentId , (XAuint32*)param1,
                                        (XAuint32*)param2, (XAuint32*)param3);              
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_metadatainsertionitf_GetKeySize( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    TInt nodeId(0);
    TUint index(0);
    XAuint32 keysize(0);
    void* param1 = (void*)&keysize;
    void* itfPtr = (void*) m_MetadataInsertionItf;
    
    status = CheckForNull(aItem, itfPtr);
    RET_ERR_IF_ERR(status);
    
    status = aItem.GetNextInt(nodeId);
    RET_ERR_IF_ERR(status);
    
    status = aItem.GetNextInt(index);
    RET_ERR_IF_ERR(status);

    status = CheckForNullParam(aItem, param1);
    RET_ERR_IF_ERR(status);  
    
    if(m_MetadataInsertionItf)
        {
        res = (*m_MetadataInsertionItf)->GetKeySize(
                XAMetadataInsertionItf(itfPtr), nodeId , index, (XAuint32*)param1);              
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_metadatainsertionitf_GetKey( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    TInt nodeId(0);
    TUint index(0);
    TUint keysize(0);
    XAMetadataInfo info = {1,1,"en-us",'a'};
    void* param1 = (void*)&info;
    void* itfPtr = (void*) m_MetadataInsertionItf;
    
    status = CheckForNull(aItem, itfPtr);
    RET_ERR_IF_ERR(status);
    
    status = aItem.GetNextInt(nodeId);
    RET_ERR_IF_ERR(status);
    
    status = aItem.GetNextInt(index);
    RET_ERR_IF_ERR(status);
    
    status = aItem.GetNextInt(keysize);
    RET_ERR_IF_ERR(status);
    
    status = CheckForNullParam(aItem, param1);
    RET_ERR_IF_ERR(status);
    
    if(m_MetadataInsertionItf)
        {
        res = (*m_MetadataInsertionItf)->GetKey(
                XAMetadataInsertionItf(itfPtr), nodeId , index, keysize, (XAMetadataInfo*)param1);              
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_metadatainsertionitf_GetFreeKeysEncoding( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    TInt nodeId(0);
    TUint index(0);
    XAuint32 encoding(0);
    void* param1 = (void*)&encoding;
    void* itfPtr = (void*) m_MetadataInsertionItf;
    
    status = CheckForNull(aItem, itfPtr);
    RET_ERR_IF_ERR(status);
    
    status = aItem.GetNextInt(nodeId);
    RET_ERR_IF_ERR(status);
    
    status = aItem.GetNextInt(index);
    RET_ERR_IF_ERR(status);
    
    status = CheckForNullParam(aItem, param1);
    RET_ERR_IF_ERR(status);
    
    if(m_MetadataInsertionItf)
        {
        res = (*m_MetadataInsertionItf)->GetFreeKeysEncoding(
                XAMetadataInsertionItf(itfPtr), nodeId , index, (XAuint32*)param1);              
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_metadatainsertionitf_InsertMetadataItem( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    TInt nodeId(0);

    TUint encoding(0);
    XAchar keylangCountry[16];
    XAchar valuelangCountry[16];    
    XAMetadataInfo keyinput = {1,1,"abc",'a'};
    XAMetadataInfo valueinput = {1,1,"efg",'a'};
    void* param1 = (void*)&keyinput;
    void* param2 = (void*)&valueinput;
    void* itfPtr = (void*) m_MetadataInsertionItf;
    
    status = CheckForNull(aItem, itfPtr);
    RET_ERR_IF_ERR(status);

    TUint keyinputsize;
    TPtrC language;
    TPtr8 keylangC(keylangCountry,16,16);
    TPtr8 valuelangC(valuelangCountry,16,16);
    TInt overwrite;
    //langC.Set(&keyangCountry,16,16);
    
    status = aItem.GetNextInt(nodeId);
    RET_ERR_IF_ERR(status);
    
    status = aItem.GetNextInt(keyinputsize);
    RET_ERR_IF_ERR(status);
    keyinput.size = keyinputsize;
    status = aItem.GetNextInt(encoding);
    RET_ERR_IF_ERR(status);    
    keyinput.encoding = encoding;

    status = aItem.GetNextString(language);
    RET_ERR_IF_ERR(status);
    keylangC.Copy(language);
    XAchar* destkey = const_cast<XAchar*>(keyinput.langCountry); 
    strcpy((char*)destkey, (char*)keylangC.Ptr());    
    //keyinput.langCountry = keylangCountry;
    
    status = aItem.GetNextInt(keyinputsize);
    RET_ERR_IF_ERR(status);
    valueinput.size = keyinputsize;
    status = aItem.GetNextInt(encoding);
    RET_ERR_IF_ERR(status);    
    valueinput.encoding = encoding;

    status = aItem.GetNextString(language);
    RET_ERR_IF_ERR(status);
    valuelangC.Copy(language);
    XAchar* destvalue = const_cast<XAchar*>(valueinput.langCountry); 
    strcpy((char*)destvalue, (char*)valuelangC.Ptr());
    
    status = aItem.GetNextInt(overwrite);
    RET_ERR_IF_ERR(status);
    
    status = CheckForNull(aItem, param1);
    RET_ERR_IF_ERR(status);
    
    status = CheckForNull(aItem, param2);
    RET_ERR_IF_ERR(status);
    
    if(m_MetadataInsertionItf)
        {
        res = (*m_MetadataInsertionItf)->InsertMetadataItem(
                XAMetadataInsertionItf(itfPtr), nodeId , (XAMetadataInfo*)param1,
                (XAMetadataInfo*)param2, overwrite);              
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }

TInt COpenMAXALTestModule::al_metadatainsertionitf_RegisterCallback( CStifItemParser& aItem )
    {
    TInt status(KErrNone);
    XAresult res;
    void* itfPtr = (void*) m_MetadataInsertionItf;
    
    status = CheckForNull(aItem, itfPtr);
    RET_ERR_IF_ERR(status);
    
    if(m_MetadataInsertionItf)
        {
        res = (*m_MetadataInsertionItf)->RegisterCallback(
                XAMetadataInsertionItf(itfPtr), MetadataInsertionCallback , (void*)this);              
        status = res;
        }
    else
        {
        status = KErrNotFound;
        }
    return status;
    }



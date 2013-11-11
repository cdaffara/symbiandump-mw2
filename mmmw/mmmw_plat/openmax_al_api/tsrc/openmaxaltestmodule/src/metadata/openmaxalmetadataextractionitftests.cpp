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

#include <stdlib.h>
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

TInt COpenMAXALTestModule::al_metadataextractionitf_GetItemCount( CStifItemParser& aItem )
{
    iLog->Log( _L("al_metadataextractionitf_GetItemCount >>") );
    
    TInt status(KErrNotFound);
    XAresult res;
    XAuint32 numItems = 0;
    if(m_MetadataExtractionItf)
    {
        res = (*m_MetadataExtractionItf)->GetItemCount(
                 m_MetadataExtractionItf, &numItems);              
        status = MapErr(res);
            
        TInt expectedNum = 0;
        if(aItem.GetNextInt(expectedNum) == KErrNone)
        {				
			if((status==KErrNone) && (expectedNum != numItems))
			{
                status = KErrGeneral;
			}
		}
			
		iLog->Log( _L("The number of Metadata Items:%d Expected: %d"), numItems, expectedNum );
  }
	
  iLog->Log( _L("al_metadataextractionitf_GetItemCount (%d) <<"), status );
  return status;
}

TInt COpenMAXALTestModule::al_metadataextractionitf_GetKeySize( CStifItemParser& aItem )
{
    iLog->Log( _L("al_metadataextractionitf_GetKeySize >>") );
    
    TInt status(KErrNotFound);
    XAresult res;
    XAuint32 keySize = 0;
    if(m_MetadataExtractionItf)
    {
        TInt index;
        if(aItem.GetNextInt(index) == KErrNone)
        {
            res     = (*m_MetadataExtractionItf)->GetKeySize(
                        m_MetadataExtractionItf, index, &keySize);              
            status  = MapErr(res);
  			
            TInt expectedSize = 0;
            if(aItem.GetNextInt(expectedSize) == KErrNone)
			{	      
                if((status==KErrNone) && (expectedSize != keySize))
                {
                    status = KErrNotFound;
                }
			}
			iLog->Log( _L("KeySize[%d]: %d Expected Size:%d"), index, keySize, expectedSize);				
		}
	}
	
	iLog->Log( _L("al_metadataextractionitf_GetKeySize (%d) <<"), status );
  return status;
}

TInt COpenMAXALTestModule::al_metadataextractionitf_GetKey( CStifItemParser& aItem )
{
    iLog->Log( _L("al_metadataextractionitf_GetKey >>") );
    
    TInt status(KErrNotFound);
    XAresult res;
    XAMetadataInfo *key = NULL;
    
    if(m_MetadataExtractionItf)
    {
        TPtrC keyToFind;
        if(aItem.GetNextString(keyToFind) == KErrNone)
        {
            HBufC *pB = HBufC16::NewL(keyToFind.Size());
            TPtr tP = pB->Des();
            tP.Copy(keyToFind);
            iLog->Log( _L("Expected Key: %S"), pB);
            delete pB;
  		
            XAuint32 numItems = 0;
            res = (*m_MetadataExtractionItf)->GetItemCount(m_MetadataExtractionItf, &numItems);              

            for(int i = 0; (i < numItems) && (MapErr(res) == KErrNone) && (status == KErrNotFound); i++)
            {
                XAuint32 keySize = 0;
                res = (*m_MetadataExtractionItf)->GetKeySize(
                       m_MetadataExtractionItf, i, &keySize);  
      
                if(MapErr(res) == KErrNone)
                {
                    key = (XAMetadataInfo *)calloc(keySize,1);
                    if(key)
                    {
                        res = (*m_MetadataExtractionItf)->GetKey(
                                        m_MetadataExtractionItf, i , keySize, key);              
                        status = MapErr(res);
                        if(status==KErrNone)
                        {
                            HBufC*   temp  = NULL;
                            if(key->encoding == XA_CHARACTERENCODING_ASCII)
                            {
                                TPtrC8 des((TUint8*)(key->data), key->size-1 );
                                temp = HBufC16::NewL(key->size-1);
                                TPtr tempPtr = temp->Des();
                                tempPtr.Copy(des);                        
                                iLog->Log( _L("Key[%d]: %S"), i, temp);
							}
							else if(key->encoding == XA_CHARACTERENCODING_UTF16LE)
							{
                                TPtrC des((const TUint16*)key->data, key->size/2 - 1);
                                temp = HBufC16::NewL(key->size/2 - 1);
                                TPtr tempPtr = temp->Des();
                                tempPtr.Copy(des);                        
                                iLog->Log( _L("Key[%d]: %S"), i, temp);
							}
									
							if(temp && !keyToFind.Compare(temp->Des()))
							{
								status = KErrNone;
							}
							else
							{
								status = KErrNotFound;
							}
							
							if(temp)
							{
								delete temp;
							}
						}//end if(status==KErrNone)
						
						free(key);
                    }//end if(key)
                }//end if(MapErr(res) == KErrNone)
            }//end for
        }//end if(aItem.GetNextString(index) == KErrNone)
    }//end if(m_MetadataExtractionItf)
        
    iLog->Log( _L("al_metadataextractionitf_GetKey (%d)<<"), status );
    return status;
}


TInt COpenMAXALTestModule::al_metadataextractionitf_GetValueSize( CStifItemParser& aItem )
{
    iLog->Log( _L("al_metadataextractionitf_GetValueSize >>") );
    
    TInt status(KErrNotFound);
    XAresult res;
    XAuint32 valueSize = 0;
    if(m_MetadataExtractionItf)
    {
        TInt index;
        if(aItem.GetNextInt(index) == KErrNone)
        {
            res     = (*m_MetadataExtractionItf)->GetValueSize(
                            m_MetadataExtractionItf, index, &valueSize);              
            status  = MapErr(res);
			iLog->Log( _L("ValueSize[%d]: %d"), index, valueSize);
        }
    }
   
   iLog->Log( _L("al_metadataextractionitf_GetValueSize (%d) <<"), status );
   return status;
}



TInt COpenMAXALTestModule::al_metadataextractionitf_GetValue( CStifItemParser& aItem )
{
    iLog->Log( _L("al_metadataextractionitf_GetValue >>") );
    
    TInt status(KErrNotFound);
    XAresult res;
    
    XAMetadataInfo *key = NULL;
    
    if(m_MetadataExtractionItf)
    {
        TInt index;
        
        //Find Key Index
        TPtrC keyToFind;
        if(aItem.GetNextString(keyToFind) == KErrNone)
        {
            HBufC *pB = HBufC16::NewL(keyToFind.Size());
            TPtr tP = pB->Des();
            tP.Copy(keyToFind);
            iLog->Log( _L("Expected Key: %S"), pB);
            delete pB;
            
            XAuint32 numItems = 0;
            res = (*m_MetadataExtractionItf)->GetItemCount(m_MetadataExtractionItf, &numItems);              
    
            for(index = 0; (index < numItems) && (MapErr(res) == KErrNone) && (status == KErrNotFound); index++)
            {
                XAuint32 keySize = 0;
                res = (*m_MetadataExtractionItf)->GetKeySize(
                       m_MetadataExtractionItf, index, &keySize);  
          
                if(MapErr(res) == KErrNone)
                {
                    key = (XAMetadataInfo *)calloc(keySize,1);
                    if(key)
                    {
                        res = (*m_MetadataExtractionItf)->GetKey(
                                        m_MetadataExtractionItf, index , keySize, key);              
                        status = MapErr(res);
                        if(status==KErrNone)
                        {
                            HBufC*   temp  = NULL;
                            TDesC16* pdesc = NULL;
                                    
                            if(key->encoding == XA_CHARACTERENCODING_ASCII)
                            {
                                TPtrC8 des((TUint8*)(key->data), key->size - 1 );
                                temp = HBufC16::NewL(key->size - 1);
                                TPtr tempPtr = temp->Des();
                                tempPtr.Copy(des);                        
                                iLog->Log( _L("Key[%d]: %S"), index, temp);
                                pdesc = &tempPtr;
                            }
                            else if(key->encoding == XA_CHARACTERENCODING_UTF16LE)
                            {
                                TPtrC des((const TUint16*)key->data, key->size/2 - 1);
                                temp = HBufC16::NewL(key->size/2 - 1);
                                TPtr tempPtr = temp->Des();
                                tempPtr.Copy(des);                        
                                iLog->Log( _L("Key[%d]: %S"), index, temp);
                                pdesc = &tempPtr;
                            }
                                        
                            if(!keyToFind.Compare(*pdesc))
                            {
                                status = KErrNone;
                            }
                            else
                            {
                                status = KErrNotFound;
                            }
                            
                            if(temp)
                            {
                                delete temp;
                            }
                        }//end if(status==KErrNone)
                            
                        free(key);
                    }//end if(key)
                }//end if(MapErr(res) == KErrNone)
            }//end for
        }//end if(aItem.GetNextString(index) == KErrNone)
            
        if((status == KErrNone) && (--index >= 0))
        {
            XAMetadataInfo *value = NULL;
            XAuint32 valueSize;
        
            res = (*m_MetadataExtractionItf)->GetValueSize(
                                   m_MetadataExtractionItf, index, &valueSize);  
                
            if(MapErr(res) == KErrNone)
            {
                value = (XAMetadataInfo *)calloc(valueSize,1);
                if(value)
                {
                    res = (*m_MetadataExtractionItf)->GetValue(
                            m_MetadataExtractionItf, index, valueSize, value);              
                    status = MapErr(res);

                    HBufC*   temp  = NULL;
                    if(status == KErrNone)
                    {
                        if(value->encoding == XA_CHARACTERENCODING_ASCII)
                        {
                            TPtrC8 des((TUint8*)(value->data), value->size - 1 );
                            temp = HBufC16::NewL(value->size - 1);
                            TPtr tempPtr = temp->Des();
                            tempPtr.Copy(des);
                            iLog->Log( _L("Value[%d]: %S"), index, temp);
                        }
                        else if(value->encoding == XA_CHARACTERENCODING_UTF16LE)
                        {
                            TPtrC des((const TUint16*)value->data, value->size/2 - 1);
                            temp = HBufC16::NewL(value->size/2 - 1);
                            TPtr tempPtr = temp->Des();
                            tempPtr.Copy(des);
                            iLog->Log( _L("Value[%d]: %S"), index, temp);
                       }
                        else
                        {
                            iLog->Log( _L("Value[%d]: <Binary Value>"), index);
                        }					

                        if(value->encoding != XA_CHARACTERENCODING_BINARY)
                        {            
                            TPtrC expectedValue;
                            if(aItem.GetNextString(expectedValue) == KErrNone)
                            {            
                                HBufC *pB = HBufC16::NewL(expectedValue.Size());
                                TPtr tP = pB->Des();
                                tP.Copy(expectedValue);
                                iLog->Log( _L("Expected Value: %S"), pB);
								iLog->Log(_L("Size: Expected(%d), Actual(%d)"),expectedValue.Size(), temp->Des().Size());
                                delete pB;
                                if(temp && !expectedValue.Compare(temp->Des()))
                                {
                                    status = KErrNone;
                                }
                                else
                                {
                                    status = KErrNotFound;
                                }
                            }
                        }
                               
                        if(temp)
                        {
                            delete temp;
                        }
                    }//end if(status == KErrNone)
                    delete value;
                }//end if(value)
            }//end if(MapErr(res) == KErrNone)
        }//end if(status == KErrNone)   
    }//end   if(m_MetadataExtractionItf)
	
    iLog->Log( _L("al_metadataextractionitf_GetValue (%d)<<"), status );
    return status;
}

TInt COpenMAXALTestModule::al_metadataextractionitf_AddKeyFilter( CStifItemParser& /*aItem */)
{
    iLog->Log( _L("al_metadataextractionitf_AddKeyFilter >>") );
    TInt status(KErrNotFound);
    if(m_MetadataExtractionItf)
    {
		XAresult res = (*m_MetadataExtractionItf)->AddKeyFilter(m_MetadataExtractionItf,
			        		                                       0, NULL, 0, NULL, 0, 0);
		status = res;
    }

	iLog->Log( _L("al_metadataextractionitf_AddKeyFilter (%d)<<"), status );	

	return status;
}

TInt COpenMAXALTestModule::al_metadataextractionitf_ClearKeyFilter( CStifItemParser& /*aItem*/ )
{
    iLog->Log( _L("al_metadataextractionitf_ClearKeyFilter >>") );
    TInt status(KErrNotFound);
    if(m_MetadataExtractionItf)
    {
	    XAresult res    = (*m_MetadataExtractionItf)->ClearKeyFilter(m_MetadataExtractionItf);

		status = res;
    }

	iLog->Log( _L("al_metadataextractionitf_ClearKeyFilter (%d)<<"), status );

	return status;
}

TInt COpenMAXALTestModule::al_metadataextractionitf_GetCoverArt( CStifItemParser& /*aItem*/ )
{
    iLog->Log( _L("al_metadataextractionitf_GetCoverArt >>") );

    TInt status(KErrNotFound);
    XAresult res;
    
    if(m_MetadataExtractionItf)
    {
        XAuint32 numItems = 0;
        res = (*m_MetadataExtractionItf)->GetItemCount(
                    m_MetadataExtractionItf, &numItems);              

        for(int i = 0; (i < numItems) && (MapErr(res) == KErrNone) ; i++)
        {
            XAuint32 keySize = 0;
            res = (*m_MetadataExtractionItf)->GetKeySize(
                        m_MetadataExtractionItf, i, &keySize);  
                     
            if(MapErr(res) == KErrNone)
            {
                XAMetadataInfo* key = (XAMetadataInfo *)calloc(keySize,1);
                if(key)
                {
                    res = (*m_MetadataExtractionItf)->GetKey(
                             m_MetadataExtractionItf, i, keySize, key);              

                    if(MapErr(res) == KErrNone)
                    {
                        if( key->encoding == XA_CHARACTERENCODING_ASCII)
                        {
                            if(!strcmp((const char*)key->data, "attachedpicture"))
                            {
                                XAuint32 valueSize;
                                res = (*m_MetadataExtractionItf)->GetValueSize(
                                              m_MetadataExtractionItf, i, &valueSize);  
                                if(MapErr(res) == KErrNone)
                                {
                                    XAMetadataInfo* value = (XAMetadataInfo *)calloc(valueSize,1);
                                    if(value)
                                    {
                                        res = (*m_MetadataExtractionItf)->GetValue(
                                                m_MetadataExtractionItf, i, valueSize, value);              
                                        if((MapErr(res) == KErrNone) && 
                                           (value->encoding == XA_CHARACTERENCODING_BINARY))
                                        {
                                            iLog->Log( _L("CoverArt[%d] Size: %d"), i, value->size );
                                        }
                                        free(value);
                                    }
                                
                                    break;
                                }
                            }            
                        }
                        free(key);
                    }
                }
             }
        }// end for
        
        status = MapErr(res);
    }//end if(m_MetadataExtractionItf)
    
    return status;
}

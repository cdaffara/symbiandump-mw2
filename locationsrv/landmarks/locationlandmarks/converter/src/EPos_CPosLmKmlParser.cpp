/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Parser implementation for KML format.
*
*
*/


#include <utf.h>
#include <gulutil.h>
#include <bautils.h>
#include <xml/xmlparsererrors.h>
#include <lbs.h>
#include <EPos_PosLmImplExtension.h>
#include <EPos_LandmarksErrors.h>
#include "EPos_CPosLmXmlParser.h"
#include "EPos_CPosLmXmlParserOperation.h"
#include "EPos_CPosLmCollectionData.h"
#include "EPos_CPosLmParsedLmBuffer.h"
#include "EPos_PosLmConverterUtils.h"
#include "EPos_LmConverter.h"
#include "EPos_CPosLmKmlParser.h"
#include "EPos_PosLmUnzipUtil.h"

// Maximum and Minimum values for Lat, Long
//as defined in KML reference Document
const TReal KPosLmParserMinLatitude = -90.0;
const TReal KPosLmParserMaxLatitude = 90.0;
const TReal KPosLmParserMinLongitude = -180.0;
const TReal KPosLmParserMaxLongitude = 180.0;

//kmz mime type - defiend in KML reference doc
_LIT8(KPosKmzMimetype,       "application/vnd.google-earth.kmz");
const TUint KPosXmlCommaSeparator = ',';
const TInt KMaxDirName = 256;
const TInt KMaxPostalCode = 10;

_LIT(KExtractZipPath, "extracts\\");
_LIT(KFindKmlExtension, "*.kml");


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmKmlParser::CPosLmKmlParser(TAny* aType)
    {
    	//is it KMZ file? check the mime type
       	HBufC8* aMimeType =(HBufC8*)aType;
	    TPtrC8 aPointer = aMimeType->Des();
	    if(aPointer.CompareF(KPosKmzMimetype) == 0)
	    {
	    	iIsKmzFile = ETrue;
	    }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmKmlParser* CPosLmKmlParser::NewL(TAny* aType)
    {
    	
	    CPosLmKmlParser* self = new (ELeave) CPosLmKmlParser(aType);
	    
	    
	    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmKmlParser::~CPosLmKmlParser()
    {
     iCategoryArray.ResetAndDestroy();
     iCategoryAdded.Reset();
     iCategoryAdded.Close();
     if(iKmlFileList)
     	delete iKmlFileList;
     ClearParserData();
     
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmKmlParser::OnStartDocumentL(
    const Xml::RDocumentParameters& aDocParam, 
    TInt aErrorCode)
    {
    CPosLmXmlParser::OnStartDocumentL( aDocParam, aErrorCode );
    
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmKmlParser::GetNextAllowedTagsL( RArray<TPosXmlTagType>& aChildTags )
    {
    switch ( CurrentTree() )
        {
        case EPosXmlNothing:
            {
            aChildTags.AppendL( EPosXmlRoot );
            break;
            }
        case EPosXmlRoot:
            {
           
            const TPosXmlTagType rootChildren[] = { 
                
                EPosXmlLandmark,EPosXmlDocument,EPosXmlLandmark,EPosXmlFolder,
                EPosXmlDocument,EPosXmlFolder, EPosXmlLandmark,EPosXmlLandmark,
                EPosXmlDocument,EPosXmlFolder //multiple documents, Folders & landmark possible 
                };
            const TInt rootChildrenSize = sizeof( rootChildren ) / sizeof( TPosXmlTagType );
    
            
            GenValidSequenceL( rootChildren, rootChildrenSize, aChildTags );
             
            break;
            }
            
        case EPosXmlDocument:
            {
            
            const TPosXmlTagType documentChildren[] = { 
                
                EPosXmlLmCollName,EPosXmlDocument,EPosXmlFolder, 
                EPosXmlLandmark,EPosXmlLandmark, // landmark can be repeated
                 EPosXmlFolder,EPosXmlDocument//Folder& Document can be repeated
                };
            const TInt documentChildrenSize = sizeof( documentChildren ) / sizeof( TPosXmlTagType );
    
            
            GenValidSequenceL( documentChildren, documentChildrenSize, aChildTags );
             
            break;
            }    
         case EPosXmlFolder:
            {
           
            const TPosXmlTagType folderChildren[] = { 
               
                EPosXmlLmCollName,EPosXmlDocument,EPosXmlFolder,
                EPosXmlLandmark,EPosXmlLandmark, // landmark can be repeated
                EPosXmlFolder,EPosXmlDocument//Folder& Document can be repeated
                };
            const TInt folderChildrenSize = sizeof( folderChildren ) / sizeof( TPosXmlTagType );
    
            
            GenValidSequenceL( folderChildren, folderChildrenSize, aChildTags );
             
            break;
            }       
        case EPosXmlLandmark:
            {
           
            const TPosXmlTagType landmarkChildren[] = { 
                EPosXmlLmName,EPosXmlAddress,EPosXmlPhoneNumber,EPosXmlLmDescription,
                 EPosXmlpoint
                };
            const TInt landmarkChildrenSize = sizeof( landmarkChildren ) / sizeof( TPosXmlTagType );

           
                GenValidSequenceL( landmarkChildren, landmarkChildrenSize, aChildTags );
             
            break;
            }
          case EPosXmlpoint:
            {
          
            const TPosXmlTagType pointChildren[] = { 
                EPosXmlpointcoordinates
                };
            const TInt pointChildrenSize = sizeof( pointChildren ) / sizeof( TPosXmlTagType );

           
                GenValidSequenceL( pointChildren, pointChildrenSize, aChildTags );
             
            break;
            }  
       
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmKmlParser::CheckMandatoryTagsL( TPosXmlTagType /*aTreeType */ )
    {
    
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmKmlParser::SetContentL( TPosXmlTagType aTag, const TDesC& aContent )
    {
    switch (aTag)
        {
        
        case EPosXmlLmCollName: // Folder/Document names will be taken as category
        	iCategoryArray.AppendL(aContent.Left( KPosLmMaxCategoryNameLength ).AllocL());
        	iCategoryAdded[iFolderCount-1] = ETrue;
            break;
        
        case EPosXmlLandmark: // landmark Tags
        {
        
            HBufC* buf= HBufC::NewLC(KPosLmMaxTextFieldLength);
            TPtrC name = buf->Des();
            iWorkingLandmark->GetLandmarkName(name);
            //Landmark name is mendatory field in landmark
            PosLmConverterUtils::AssertL( !(name == KNullDesC));
            
            if ( !Math::IsNaN( iParsedLatitude ) && !Math::IsNaN( iParsedLongitude ) )
	                {
	                TLocality loc(
	                    TCoordinate(
	                        iParsedLatitude, 
	                        iParsedLongitude,
	                        iParsedAltitude), 
	                    0, //horizontal accuracy 0
	                    0); //vertiacal accuracy 0
	                
	                iWorkingLandmark->SetPositionL(loc);
	                }
	                else
	                {
	                	if(!Math::IsNaN( iParsedLatitude ) || !Math::IsNaN( iParsedLongitude ))
	                	{
	                		User::Leave(KErrPosLmUnknownFormat); // Long and lat both are mendatory. If only one is available - leave
	                	}
	                }
				
				//Set category for current landmark if any.
	           if(iCategoryArray.Count()>0)
	           {
	            if (iCategoryArray[iCategoryArray.Count()-1]->Length() > 0 )
	            {
	            	CPosLandmarkCategory* category = CPosLandmarkCategory::NewLC();
	            	PosLmImplExtension::SetCategoryIdL( *category, iWorkingItemId );
	            	category->SetCategoryNameL( iCategoryArray[iCategoryArray.Count()-1]->Left( KPosLmMaxCategoryNameLength ) );

	            	iWorkingLandmark->AddCategoryL( iWorkingItemId++ );
	            	User::LeaveIfError( iCategories.Append( category ) );
	            	CleanupStack::Pop(category);//buf
	            	category = NULL;
	            }
	           
	           }
            HandleLandmarkParsedL();
            
            CleanupStack::PopAndDestroy(buf);//buf
            
        break;
        }
        case EPosXmlLmName:
            iWorkingLandmark->SetLandmarkNameL( aContent.Left( KPosLmMaxTextFieldLength ) );
            break;
        case EPosXmlLmDescription:
            iWorkingLandmark->SetLandmarkDescriptionL( aContent.Left( KPosLmMaxDescriptionLength ) );
            break;
        case EPosXmlAddress: //extracting address field
        {
	       HBufC* Buf= HBufC::NewLC(KPosLmMaxTextFieldLength);
	       TPtrC Pointer = Buf->Des();
	       TPtrC contentBuf;
				TInt aLength=aContent.Length();
				TInt column = 0;
				TInt addressField[] = {EPositionFieldStreet, EPositionFieldCity, EPositionFieldPostalCode, EPositionFieldState, EPositionFieldCountry };
				
				//extracting street information
				//if '<' found in address string then till the first comma after the '<'
				//will be considered as Stree name.
				TInt anglePosition1 = aContent.Locate('<');
				TInt aCount =0;
				if(anglePosition1 > 0)
				{
					//First comma position after '<'
					TInt commaPosition = aContent.Mid(anglePosition1 + 1).Locate(KPosXmlCommaSeparator);
					commaPosition +=anglePosition1 + 1;
					//Set street name.
					iWorkingLandmark->SetPositionFieldL(addressField[0],aContent.Left(commaPosition));
					column++;
					contentBuf.Set(aContent.Right(aLength-(commaPosition+1)));
				}
				else
				{
					contentBuf.Set(aContent);
				}
					
				while (TextUtils::ColumnText(Pointer,aCount,&contentBuf,KPosXmlCommaSeparator) != KErrNotFound )
				{
					
					if(addressField[column] == EPositionFieldPostalCode)
					{
						if(!(IsNumber(Pointer.Left( KMaxPostalCode ))))
						 column++; // if it is not number then set it to state
					}
					if(column>=5)	
						break;	

					if( addressField[column] == EPositionFieldCountry)
				 	{
				 		if(IsNumber(Pointer.Left( KMaxPostalCode )))
						   column = 2; //if it is number then set it to postal code
				 	}
					iWorkingLandmark->SetPositionFieldL(addressField[column],Pointer.Left( KPosLmMaxTextFieldLength ));
					if(column == 2)
						column+=2; //If it is postal code then next should be country so add 2
					
					else
						column++;
					aCount++;
					if(aCount >= 4 || column>=5)	
						break;	
				}
				
				CleanupStack::PopAndDestroy();
        }
        break;
        case EPosXmlPhoneNumber:   
        	 iWorkingLandmark->SetPositionFieldL(EPositionFieldBuildingTelephone,aContent.Left( KPosLmMaxTextFieldLength ) );
             break;   
        
        case EPosXmlpointcoordinates:
        {
        	HBufC* aBuf= HBufC::NewLC(KPosLmMaxTextFieldLength);
          TPtrC aPoints = aBuf->Des();
					TInt count = 0;
					while (TextUtils::ColumnText(aPoints,count,&aContent,KPosXmlCommaSeparator) != KErrNotFound )
						{
							switch(count)
							{
								case 0:
									PosLmConverterUtils::DesToRealL(aPoints, iParsedLongitude);
									// Ensure longtitude is in valid range
									PosLmConverterUtils::AssertL((iParsedLongitude <= KPosLmParserMaxLongitude &&
					                iParsedLongitude >= KPosLmParserMinLongitude) ||
					                Math::IsNaN(iParsedLongitude));
								break;
								case 1:
									PosLmConverterUtils::DesToRealL(aPoints,iParsedLatitude );
									// Ensure latitude is in valid range
									PosLmConverterUtils::AssertL(
					                (iParsedLatitude <= KPosLmParserMaxLatitude &&
					                iParsedLatitude >= KPosLmParserMinLatitude) ||
					                Math::IsNaN(iParsedLatitude));
				                break;
								case 2:
									PosLmConverterUtils::DesToRealL(aPoints,iParsedAltitude );
								default:
								break;
								
							}
							count++;		
						}
					CleanupStack::PopAndDestroy();	
		}
     break;
    default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmKmlParser::IsLandmarkStarted( TPosXmlTagType aTag )
    {
    return ( aTag == EPosXmlLandmark );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmKmlParser::IsCategoryStarted( TPosXmlTagType aTag )
    {
    return ( aTag == EPosXmlCategoryName );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmKmlParser::IsTreeTag( TPosXmlTagType aTag )
    {
    __ASSERT_DEBUG( aTag != EPosXmlNothing, Panic( KPosLmConverterPanic, KErrGeneral ) );
    __ASSERT_DEBUG( aTag != EPosXmlUnknown, Panic( KPosLmConverterPanic, KErrGeneral ) );
    
    return ( aTag >= EPosXmlLmLastTree ) ? ETrue : EFalse;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TPosXmlTagType CPosLmKmlParser::AttributeTypeL( TPosXmlTagType /*aParent*/, const Xml::RTagInfo& /*aElement*/ )
    {
    TPosXmlTagType tagType = EPosXmlUnknown;
    
  
    return tagType;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TPosXmlTagType CPosLmKmlParser::TagTypeL( const Xml::RTagInfo& aElement )
    {
    TPosXmlTagType tagType = EPosXmlUnknown;
    TPosXmlTagType treeType = CurrentTree();
    
    TPtrC8 tagName = aElement.LocalName().DesC();
    
    if ( treeType == EPosXmlNothing )
        {
        if ( EqualsTagL( tagName, KPosXmlKmlRoot ) )
            {
            tagType = EPosXmlRoot;
            }
        }
    else if ( MatchesRootNamespace( aElement ) )
        {
        switch ( treeType )
            {
            case EPosXmlRoot:
                {
                if ( EqualsTagL( tagName, KPosXmlKmlMetadata ) )
                    {
                    tagType = EPosXmlMetadata;
                    }
                else if ( EqualsTagL( tagName, KPosXmlKmlDocument ) )
                    {
                    tagType = EPosXmlDocument;
                    
                    }  
                else if ( EqualsTagL( tagName, KPosXmlKmlFolder ) )
                    {
                    tagType = EPosXmlFolder;
                    }        
                else if ( EqualsTagL( tagName, KPosXmlKmlPlacemark ) )
                    {
                    tagType = EPosXmlLandmark;
                    }
                else if ( EqualsTagL( tagName, KPosXmlKmlCollName ) ) 
                    {
                    tagType = EPosXmlLmCollName;
                    }
             
                else if ( EqualsTagL( tagName, KPosXmlKmlRoot ) )
                    {
                    User::Leave( EXmlRecursiveEntity );
                    }
                }
                break;
                
            case EPosXmlDocument:
            	 if ( EqualsTagL( tagName, KPosXmlKmlPlacemark ) )
                    {
                    tagType = EPosXmlLandmark;
                    }
                 else if ( EqualsTagL( tagName, KPosXmlKmlFolder ) )
                    {
                    tagType = EPosXmlFolder;
                    
                    }   
                 else if ( EqualsTagL( tagName, KPosXmlKmlDocument ) )
                    {
                    //User::Leave( EXmlRecursiveEntity );
                    tagType = EPosXmlDocument;
                    }
                 else if ( EqualsTagL( tagName, KPosXmlKmlCollName ) ) 
                    {
                    tagType = EPosXmlLmCollName;
                    }     
                
                break;
                
             case EPosXmlFolder:
            	 if ( EqualsTagL( tagName, KPosXmlKmlPlacemark ) )
                    {
                    tagType = EPosXmlLandmark;
                    }
                 else if ( EqualsTagL( tagName, KPosXmlKmlDocument ) )
                    {
                    tagType = EPosXmlDocument;
                    
                    }     
                 else if ( EqualsTagL( tagName, KPosXmlKmlFolder ) )
                    {
                  //  User::Leave( EXmlRecursiveEntity );
                  	tagType = EPosXmlFolder;
                  	
                    }
                else if ( EqualsTagL( tagName, KPosXmlKmlCollName ) ) 
                {
                tagType = EPosXmlLmCollName;
                
                }      
                
                break;   
            case EPosXmlMetadata:
                {
                if ( EqualsTagL( tagName, KPosXmlKmlCollName ) ) 
                    {
                    tagType = EPosXmlLmCollName;
                    }
               
                else if ( EqualsTagL( tagName, KPosXmlKmlMetadata ) )
                    {
                    User::Leave( EXmlRecursiveEntity );
                    }
                }
                break;
                
            case EPosXmlLandmark:
                {
                if ( EqualsTagL( tagName, KPosXmlKmlLmName ) ) 
                    {
                    tagType = EPosXmlLmName;
                    }
                else if ( EqualsTagL( tagName, KPosXmlKmlLmDescription ) )
                    {
                    tagType = EPosXmlLmDescription;
                    }
                else if ( EqualsTagL( tagName, KPosXmlKmlLmAddress ) ) 
                    {
                    tagType = EPosXmlAddress;
                    }
                 else if ( EqualsTagL( tagName, KPosXmlKmlLmPhonenumber ) ) 
                    {
                    tagType = EPosXmlPhoneNumber;
                    }    
                else if ( EqualsTagL( tagName, KPosXmlKmlLmlocation ) ) 
                    {
                    tagType = EPosXmlpoint;
                    }
              
                else if ( EqualsTagL( tagName, KPosXmlKmlPlacemark ) )
                    {
                    User::Leave( EXmlRecursiveEntity );
                    }
                }
                break;
         
                
            case EPosXmlpoint:
                {
                if ( EqualsTagL( tagName, KPosXmlKmlLmCoordinates ) ) 
                    {
                    tagType = EPosXmlpointcoordinates;
                    } 
                }
            default:
                break; // EPosXmlUnknown
            }
        }
    else
        {
        }
    
    return tagType;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmKmlParser::OnAttributesL( 
    TPosXmlTagType /*aParent*/,
    const Xml::RAttributeArray& /*aAttributes*/ )
    {
    
    }
    
//-----------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//
void CPosLmKmlParser::OnStartElementL( 
    const Xml::RTagInfo& aElement,
    const Xml::RAttributeArray& aAttributes, 
    TInt aErrorCode)
    {
	    CPosLmXmlParser::OnStartElementL(aElement, aAttributes, aErrorCode);
	    TPosXmlTagType startTag = TagTypeL( aElement );
	    if(startTag == EPosXmlFolder || startTag == EPosXmlDocument)
	    {
	    	iFolderCount++;
	    	iCategoryAdded.AppendL(EFalse);
	    }
    
    }

//-----------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//   
void CPosLmKmlParser::OnEndElementL( const Xml::RTagInfo& aElement, TInt aErrorCode)
{
	CPosLmXmlParser::OnEndElementL(aElement, aErrorCode);
	TPosXmlTagType startTag = TagTypeL( aElement );
	if(startTag == EPosXmlFolder || startTag == EPosXmlDocument)
	{
		
		if(iCategoryAdded[iFolderCount-1] && iCategoryArray.Count() > 0)
		{
			//RPointerArray::Remove does not take care of deleting the pointer.
			//store the pointer in temp n delete it.
			HBufC* temp = iCategoryArray[iCategoryArray.Count() - 1];
			iCategoryArray.Remove(iCategoryArray.Count() - 1);
			delete temp;
		}
    	iCategoryAdded.Remove(iCategoryAdded.Count() -1 );
      	iFolderCount--;
    }
    
}

//-----------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//
TBool CPosLmKmlParser::EqualsTagL( const TDesC8& aTag, const TDesC& aDefinedTag )
    {
    HBufC* tag = HBufC::NewLC( aTag.Length() );
    TPtr16 tagptr = tag->Des();

    PosLmConverterUtils::Utf8ToUnicodeL( aTag, tagptr );

    TBool answer = ( tag->CompareC( aDefinedTag ) == 0 );
    CleanupStack::PopAndDestroy( tag );

    return answer;
    }


//-----------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//
TBool CPosLmKmlParser::IsNumber( const TDesC& aStr)
{
	TBuf<KMaxPostalCode> tempBuf(aStr);//store in temp buffer and trim.
	tempBuf.Trim();
	TLex lexer(tempBuf);
	TInt val =0;
    TInt aStatus = lexer.Val(val);
    
    if (aStatus == KErrNone)
        {
        if (lexer.Remainder() != KNullDesC)
            {
            	return EFalse;
            }
        }
    else 
        {
        return EFalse;
        }
    
    return ETrue;
}

//-----------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//
TInt CPosLmKmlParser::GetKmlFileCountL(const TDesC& aFileName)
{
	
	PosLmUnzipUtil::DoUnzipL(aFileName);
		
	RFs fs; 
	User::LeaveIfError(fs.Connect()); //Connect to file session
	TBuf<KMaxDirName> targetPath;
	fs.PrivatePath(targetPath);
	targetPath.Append(KExtractZipPath);
	targetPath.Append(KFindKmlExtension);
	//Get list of extracted KML files
	User::LeaveIfError(fs.GetDir(targetPath, KEntryAttNormal, ESortByName, iKmlFileList)); 
	fs.Close();
	if(iKmlFileList)
		return iKmlFileList->Count();
	else
		return 0;
	
}

//-----------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//
TInt CPosLmKmlParser::ParseNextPartL( TReal32& aProgress )
{
 if(iIsKmzFile && iKmlFileList->Count() > 0)
 {
 	
 	//Set the file handle to next KML file in Kmz 
	if(iPreviousFileCompleted)
	{
		RFs fs; 
		User::LeaveIfError(fs.Connect()); //Connect to file session
		TBuf<KMaxDirName> targetPath;
		fs.PrivatePath(targetPath);
		targetPath.Append(KExtractZipPath);
		targetPath.Append((*iKmlFileList)[iCurrentProssedFile].iName);
		//storing the parsed landmark number in temp 
		//later reset the iNumOfParsedLandmarks with temp values
		TInt temp = iNumOfParsedLandmarks;
		CPosLmXmlParser::SetInputFileL( targetPath );
		ResetContentL(); //this function will reset the iNumOfParsedLandmarks to 0
		iNumOfParsedLandmarks = temp;
		iPreviousFileCompleted = EFalse;
		iCurrentProssedFile++;
		fs.Close();
	}
	TInt status = CPosLmXmlParser::ParseNextPartL( aProgress );
	if( status == KErrNone && iCurrentProssedFile != iKmlFileList->Count())
	{
		//completed current file parsing, next file need to be set
		iPreviousFileCompleted = ETrue;
		return KPosLmOperationNotComplete;
    	
	}
	else
	if(iCurrentProssedFile == iKmlFileList->Count()) 
	{
		//last file parsing
		if(status == KErrNone)
		{
			iPreviousFileCompleted = ETrue;
			iCurrentProssedFile = 0;
			
		}
			//ResetParsing();
		return status;
	}
	else
		return KPosLmOperationNotComplete; 
 }
 else
 {
 	return CPosLmXmlParser::ParseNextPartL( aProgress );
 }

}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmKmlParser::SetInputFileL( const TDesC& aFile )
{
	if(iIsKmzFile)
	{
		CloseCurrentFileHandle();
		TInt kmlFileCount = GetKmlFileCountL(aFile);
        if(kmlFileCount > 0)
	    {
        	RFs fs; 
			User::LeaveIfError(fs.Connect()); //Connect to file session
        	TBuf<KMaxDirName> targetPath;
			fs.PrivatePath(targetPath);
			targetPath.Append(KExtractZipPath);
			//Set first KML file handle for parsing
			targetPath.Append((*iKmlFileList)[0].iName);
			iCurrentProssedFile++;
			CPosLmXmlParser::SetInputFileL( targetPath );
			fs.Close();
        }
        else
        {
        	//zero KML files in provided KMZ file
        	User::Leave(KErrPosLmUnknownFormat);
        }
	}
	else
		CPosLmXmlParser::SetInputFileL( aFile );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmKmlParser::SetInputFileHandleL( RFile& aFileHandle )
{
	if(iIsKmzFile)
	{
		CloseCurrentFileHandle();
		TBuf16<KMaxDirName> fileName;
		aFileHandle.FullName(fileName);
		TInt err = aFileHandle.ChangeMode(EFileShareReadersOnly);
		if(err != KErrNone)
			User::Leave(KErrAccessDenied); //leave if file is not opened in read mode.
		//aFileHandle.Close();
		RFs fs; 
		User::LeaveIfError(fs.Connect()); //Connect to file session
		TInt kmlFileCount = GetKmlFileCountL(fileName);
	
        if(kmlFileCount > 0)
        {
        	
        	TBuf<KMaxDirName> targetPath;
			fs.PrivatePath(targetPath);
			targetPath.Append(KExtractZipPath);
			//Set first KML file handle for parsing
			targetPath.Append((*iKmlFileList)[0].iName);
			iCurrentProssedFile++;
			CPosLmXmlParser::SetInputFileL( targetPath );
        	
        }
        else
        {
        	//zero KML files in provided KMZ file
        	User::Leave(KErrPosLmUnknownFormat);
        }	
	     fs.Close();
	    	
	}
	else
		CPosLmXmlParser::SetInputFileHandleL(aFileHandle );
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//
void CPosLmKmlParser::ClearParserData()
{
	if(iIsKmzFile)
	{
		//delete all the extracted files from private folder.
		CloseCurrentFileHandle();
		
		RFs fs; 
		TInt err = fs.Connect(); //Connect to file session
		if(err == KErrNone)
		{
			
		TBuf<KMaxDirName> targetPath;
		fs.PrivatePath(targetPath);
		targetPath.Append(KExtractZipPath);
		CDir* dir;
		fs.GetDir(targetPath, KEntryAttNormal | KEntryAttDir, ESortByName, dir); //Get list of files to unzip
		
		for (TInt y = 0; y < dir->Count(); y++) 
		{
			TBuf<KMaxDirName> fileName;
			fileName.Append(targetPath);
			fileName.Append((*dir)[y].iName);
			TInt status = fs.Delete(fileName);
		
		}
		fs.Close();
		delete dir;
		}
	}
}


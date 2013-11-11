/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Metadata schema container within server
*
*/

#include <e32cmn.h>
#include <charconv.h>
#include <convgeneratedcpp.h>
#include <badesca.h>
#include <bautils.h>
#include <e32math.h>
#include <pathinfo.h>

#include <mdeconstants.h>

#include "mdsimportexport.h"

#include "mdcresult.h"
#include "mdcitem.h"
#include "mdcdef.h"
#include "mdsnamespacedef.h"
#include "mdsobjectdef.h"
#include "mdccommon.h"
#include "mdspropertydef.h"
#include "mdsschema.h"
#include "mdssqliteconnection.h"
#include "mdsdbconnectionpool.h"
#include "mdcserializationbuffer.h"
#include "mdssqlobjectmanipulate.h"
#include "mdsrelationdef.h"
#include "mdseventdef.h"
#include "mdsfindsqlclausedef.h"
#include "mdsclausebuffer.h"
#include "mdsmaintenanceengine.h"
#include "mdssqldbmaintenance.h"
#include "mdspreferences.h"
#include "mdscommoninternal.h"
#include "mdsdatabaseupdater.h"

const TInt KMdsMaxUriLenght = KMaxFileName;
const TChar KMdsLineFeed = '\n';

#ifdef _DEBUG
const TInt KMdsMaxLogLineLenght( 2056 );

_LIT( KMdsErrorLogDirectory, "Metadata" );
_LIT( KMdsErrorLogFilename, "importerror.log" );
#endif

// keyword definitions in import file
_LIT8( KMdsKeywordComment,     "//" );
_LIT8( KMdsKeywordNamespace,   "namespace" );
_LIT8( KMdsKeywordObjectDef,   "object" );
_LIT8( KMdsKeywordPropertyDef, "property" );
_LIT8( KMdsKeywordRelationDef, "relationdef" );
_LIT8( KMdsKeywordEventDef,    "eventdef" );

_LIT8( KMdsKeywordVersion, "version" );

_LIT8( KMdsKeywordObject,   "object" );
_LIT8( KMdsKeywordProperty, "property" );
_LIT8( KMdsKeywordFreeText, "freetext" );
_LIT8( KMdsKeywordRelation, "relation" );
_LIT8( KMdsKeywordEvent,    "event" );

_LIT8( KExportMetadataNewLine, "\r\n" );
_LIT8( KExportMetadataSpace, " " );
_LIT8( KExportMetadataQuotationMark, "\"" );
_LIT8( KExportMetadataTimeFormat, "%04d%02d%02d%02d%02d%02d" );


_LIT( KMdsKeywordMinValue, "min" );
_LIT( KMdsKeywordMaxValue, "max" );
_LIT( KMdsKeywordDefValue, "def" );

/**
 * NewLC
 */
CMdsImportExport* CMdsImportExport::NewLC()
	{
	CMdsImportExport* ret = new(ELeave) CMdsImportExport();
	CleanupStack::PushL( ret );
	ret->ConstructL();
	return ret;
	}
	
/**
 * NewL
 */
CMdsImportExport* CMdsImportExport::NewL()
	{
	CMdsImportExport* ret = CMdsImportExport::NewLC();
	CleanupStack::Pop( ret );
	return ret;
	}
	
/**
 * Constructor
 */
CMdsImportExport::CMdsImportExport() : iLastDriveNumber ( -1 )
	{
	}
	
/**
 * Default destructor
 */
CMdsImportExport::~CMdsImportExport()
	{
   	delete iConverter;
   	iConverter = NULL;

#ifdef _DEBUG
   	if( iLogEnabled )
   	    {
        iLog.CloseLog(); 
        iLog.Close();
   	    }
#endif
    iFs.Close();

   	delete iBuffer;
   	iBuffer = NULL;

   	delete iSchema;
   	iSchema = NULL;
	}

/**
 * ConstructL
 */
void CMdsImportExport::ConstructL()
	{
	iSchema = CMdsSchema::NewL();
    iConverter = CCnvCharacterSetConverter::NewL();

    User::LeaveIfError( iFs.Connect() );
    iConverter->PrepareToConvertToOrFromL( KCharacterSetIdentifierUtf8, iFs );

#ifdef _DEBUG
    const TInt logError( iLog.Connect( ) );
    if( logError == KErrNone )
        {
        iLogEnabled = ETrue;
        iLog.CreateLog( KMdsErrorLogDirectory, KMdsErrorLogFilename, EFileLoggingModeOverwrite );
        iLog.SetDateAndTime(EFalse, ETrue);
        }
#endif

	iBuffer = NULL;
    iLastObjectDef = NULL;
	}

void CMdsImportExport::ImportSchemaFromFileL( const TDesC& aFileName,
							CMdsSchema& aSchema, TUint32 aVendorId )
    {
    iDefaultSchema = &aSchema;
    iVendorId = aVendorId;

    if ( !iDefaultSchema->iBaseObject )
    	{
    	// If the default schema doesn't already have a base object, add one to iSchema together
    	// with default namespace.
    	iSchema->AddDefaultObjectL();
		iDefaultSchema->iBaseObject = iSchema->iBaseObject;
		iSchema->AddDefaultNamespaceL();
    	}
    else
    	{
    	iSchema->iBaseObject = iDefaultSchema->iBaseObject;
    	}

	iReadStream.PushL();
	// Read the schema file.
	TInt fileOpenStatus = iReadStream.Open( iFs, aFileName, EFileStreamText );
	if (fileOpenStatus != KErrNone)
		{
	    iSchema->iBaseObject = NULL;
	    iDefaultSchema = NULL;
	    _LIT( KError, "Failed to open schema file" );
    	LogError( KError );
		User::Leave( fileOpenStatus );
		}
    iFileName.Copy( aFileName );

    iLineNumber = 0;
    iLastObjectDef = NULL;
    if ( aFileName == KSchemaImportFile || aFileName == KSchemaRomImportFile )
    	{
    	iVersionFlags = EVersionNone;
    	}
    else
    	{
    	iVersionFlags = EVersionAlreadyRead;
    	}

    // start reading lines from the schema file
    while( ETrue )
        {
        TRAPD( e, iReadStream.ReadL( iLine, KMdsLineFeed ) );
        if ( e == KErrEof ) break; // succesful exit
        else if ( e != KErrNone )
            {
		    iSchema->iBaseObject = NULL;
		    iDefaultSchema = NULL;
		    _LIT( KError, "Failed to read line from schema file" );
 	   		LogError( KError );
			User::Leave( e );
            }

        ++iLineNumber;
        if ( iLine.Length() > 0 )
            {
            // parse the line
            TLex8 parser( iLine );
            TRAP( e, ImportSchemaLineL( parser ) );
            if ( e != KErrNone )
            	{
			    iSchema->iBaseObject = NULL;
			    iDefaultSchema = NULL;
			    _LIT( KError, "Schema corrupted" );
            	LogError( KError );
            	User::Leave( e );
            	}
            }
        }
    
    CleanupStack::PopAndDestroy( &iReadStream ); // Closes stream.

    TRAPD( err, aSchema.MergeNamespacesL( *iSchema ) );

    iSchema->iBaseObject = NULL;
    iDefaultSchema = NULL;
    iVendorId = 0;

    if ( err != KErrNone )
    	{
    	User::Leave( err );
    	}
    }

/**
 * ImportL imports a single line of data
 */
void CMdsImportExport::ImportSchemaLineL( TLex8& aParser )
    {
    TPtrC8 token = aParser.NextToken();

    // valid keywords in beginning of line
    if ( token.Length() == 0 || token.Left(2) == KMdsKeywordComment )
        {
        // ignore
        return;
        }
    else if ( token == KMdsKeywordVersion )
        {
        ImportSchemaVersionL( aParser );
        }
    else if ( !(iVersionFlags & EVersionAlreadyRead ) )
        {
        User::Leave( KErrCorrupt );
        }
    else if ( token == KMdsKeywordNamespace )
        {
        ImportSchemaNamespaceL( aParser );
        return;
        }
    else if ( token == KMdsKeywordObjectDef )
        {        
        ImportSchemaObjectDefL( aParser );
        }
    else if(token == KMdsKeywordPropertyDef )
    	{
    	ImportSchemaPropertyDefL( aParser );
    	}
    else if ( token == KMdsKeywordRelationDef )
        {
        ImportSchemaRelationDefL( aParser );
        }
    else if ( token == KMdsKeywordEventDef )
        {
        ImportSchemaEventDefL( aParser );
        }
    else
        {
        _LIT( KError, "Keyword not recognized" );
        LogError( KError );
        User::Leave( KErrCorrupt );
        }
    TPtrC8 tokenLast = aParser.NextToken();
    if ( tokenLast.Length() != 0 && tokenLast.Left(2) != KMdsKeywordComment )
    	{
    	_LIT( KMdsUnknownToken, "Undefined schema item" );
    	LogError( KMdsUnknownToken );
    	User::Leave( KErrCorrupt );
    	}
    }

/**
 * Schema import checklist:
 *	1. Are all the namespaces new? If not, discard any duplicates
 *	2. Do all the (object/event/relation) defs about to be imported actually 
 *	   belong to a proper namespace (which is also not read-only)? If not, discard
 */
// ------------------------------------------------
// ImportSchemaNamespaceL
// ------------------------------------------------
//
void CMdsImportExport::ImportSchemaNamespaceL( TLex8& aParser )
    {
    // namespace <name> <readonly>
    TBuf16<KMdsMaxUriLenght> name;
    User::LeaveIfError( ImportText( name, aParser ) );
    
    TInt ro = 0;
    TBool readOnly = EFalse;
    User::LeaveIfError( ImportNum( ro, aParser ) );
    readOnly = ro ? ETrue : EFalse;

    // Check out if the namespace already exists
	if ( iSchema->GetNamespace( name ) )
		{
		return;
		}
	
	// in case there is namespace in old schema
	CMdsNamespaceDef* actualNamespace = iDefaultSchema->GetNamespace( name );
	if ( actualNamespace )
		{
		iSchema->NamespaceAddL( actualNamespace->GetName(),
                actualNamespace->GetReadOnly(), actualNamespace->GetVendorId(),
                actualNamespace->GetId() );
		return;
		}

	// Ok, the namespace is new
	iSchema->NamespaceAddL( name, readOnly, iVendorId );
    }

// ------------------------------------------------
// ImportSchemaObjectDefL
// ------------------------------------------------
//
void CMdsImportExport::ImportSchemaObjectDefL( TLex8& aParser )
    {
    // First read the name of the namespace.
    TBuf16<KMdsMaxUriLenght> name;
    User::LeaveIfError( ImportText( name, aParser ) );

	CMdsNamespaceDef* actualNamespace = iSchema->GetNamespace( name );
	if ( !actualNamespace )
		{
        actualNamespace = iDefaultSchema->GetNamespace( name );
		if ( !actualNamespace )
			{
			_LIT( KMdsNamespaceNotFound, "Namespace not found !!!" );
			LogError( KMdsNamespaceNotFound );
	    	User::Leave( KErrAccessDenied );
			}
		CMdsNamespaceDef* nmsp = iSchema->NamespaceAddL(
                actualNamespace->GetName(), actualNamespace->GetReadOnly(),
				actualNamespace->GetVendorId() );
		if ( !actualNamespace->GetFirstRead() )
			{
			nmsp->UnsetFirstRead();
			}
		actualNamespace = nmsp;
		}

    if ( actualNamespace->GetReadOnly() && !actualNamespace->GetFirstRead() )
    	{
    	iLastObjectDef = NULL;
    	_LIT( KError, "Namespace not allowed" );
    	LogError( KError );
    	User::Leave( KErrAccessDenied );
    	}

    // Next read the name of the object.
    TBuf16<KMdsMaxUriLenght> nameObject;
    User::LeaveIfError( ImportText( nameObject, aParser ) );
    if ( nameObject == MdeConstants::Object::KBaseObject )
    	{
    	_LIT( KError, "Cannot redefine Object" );
		LogError( KError );
    	User::Leave( KErrArgument );
    	}

    // Next read the name of the parent object.
    User::LeaveIfError( ImportText( name, aParser ) );

    // Add new object definition to the DB.
    TRAPD( err, iLastObjectDef = actualNamespace->AddObjectDefL( nameObject, name, iDefaultSchema ) );

    if ( err != KErrNone && err != KErrAlreadyExists  )
   		{
   		User::Leave( err );
    	}

    // Finally read object flags (currently only two values: 0/1).
    TInt flags;
    CMdsObjectDef::TObjectDefFlags objFlags = CMdsObjectDef::EObjectDefFlagsNone;
    User::LeaveIfError( ImportNum(flags, aParser) );
    switch( flags )
    	{
    	case CMdsObjectDef::EObjectDefFlagsNone:
    		objFlags = CMdsObjectDef::EObjectDefFlagsNone;
    		break;

    	case CMdsObjectDef::EObjectDefFlagsContext:
    		objFlags = CMdsObjectDef::EObjectDefFlagsContext;
    		break;

    	default:
    		_LIT( KUnsupportedFlag, "Unsupported flag" );
    		LogError( KUnsupportedFlag );
    		User::Leave( KErrCorrupt );
    	}
    iLastObjectDef->SetFlags( objFlags );
    }

// ------------------------------------------------
// ImportSchemaPropertyDefL
// ------------------------------------------------
//
void CMdsImportExport::ImportSchemaPropertyDefL( TLex8& aParser )
	{
	// property  <name>  <readonly> <mandatory>  <type> <minv> <maxv>
    _LIT( KMinMaxWrongValue, "Min value is bigger then max" );
    _LIT( KWrongValue,       "Min or max value is incorrect" );
    _LIT( KTextWrongValue,   "Min or max text value is incorrect" );

    // Property definitions always come immediately after object definitions in a schema file.
    // If there is no object definition before property definitions, it's an error.
	if ( !iLastObjectDef )
		{
		_LIT( KNoLastObject, "Try to add to no existing object" );
		LogError( KNoLastObject );
		User::Leave( KErrNotFound );
		}

	// Read property name.
    TBuf16<KMdsMaxUriLenght> name;
    User::LeaveIfError( ImportText( name, aParser ) );
    if ( iLastObjectDef->GetProperty( name ) )
    	{
    	_LIT( KError, "Property already exists" );
		LogError( KError );
		User::Leave( KErrArgument );
    	}

    // Read read-only and mandatory flags.
    TBool readOnly, mandatory;
	User::LeaveIfError( ImportNum( readOnly, aParser ) );
	User::LeaveIfError( ImportNum( mandatory, aParser ) );

	// Read property type.
    TPropertyType type;
    TInt32 readType;
    User::LeaveIfError( ImportNum( readType, aParser ) );
    type = static_cast<TPropertyType>(readType);
 
    // Depending on the property type read the next two parameters with the correct data type.
    TInt error = KErrNone;
    switch( type )	
    	{
    	case EPropertyBool:
    		{
    		TInt32  minVal32,
    		        maxVal32;
    		error = SetMinMaxDefValueL<TInt32>( CheckForConstant(aParser), minVal32, 0, 1, 0 );
       		if ( error < KErrNone )
    			{
    			User::LeaveIfError( ImportNum( minVal32, aParser ) );
    			}
    		error = SetMinMaxDefValueL<TInt32>( CheckForConstant(aParser), maxVal32, 0, 1, 1 );
       		if ( error < KErrNone )
    			{
    			User::LeaveIfError( ImportNum( maxVal32, aParser ) );
    			}
       		
       		CheckNoMoreNumericParametersL( aParser );
       		
    		// boolean cannot have different values
    		minVal32 = 0;
    		maxVal32 = 1;
    		// Add this property to the previously defined object.
    		iLastObjectDef->AddPropertyL( name, type, minVal32, maxVal32, readOnly, mandatory, EFalse );
    		break;
    		}
    	case EPropertyInt8:
    	case EPropertyUint8:
    	case EPropertyInt16:
    	case EPropertyUint16:
    	case EPropertyInt32:
    		{
    		TInt32  minVal32,
    		        maxVal32;
    		const TInt32 KMinVal32 = type == EPropertyInt8 ? KMinTInt8 :
    		                          type == EPropertyInt16 ? KMinTInt16 :
    		                           type == EPropertyInt32 ? KMinTInt32 : 0;
    		const TInt32 KMaxVal32 = type == EPropertyInt8 ? KMaxTInt8 :
    		                          type == EPropertyUint8 ? KMaxTUint8 :
    		                           type == EPropertyInt16 ? KMaxTInt16 :
    		                            type == EPropertyUint16 ? KMaxTUint16 : KMaxTInt32;
   			error = SetMinMaxDefValueL<TInt32>( CheckForConstant(aParser),
   					minVal32, KMinVal32, KMaxVal32, 0 );
       		if ( error < KErrNone )
    			{
    		    User::LeaveIfError( ImportNum( minVal32, aParser ) );
    			}
   			error = SetMinMaxDefValueL<TInt32>( CheckForConstant(aParser),
   					maxVal32, KMinVal32, KMaxVal32, KMaxVal32 );
       		if ( error < KErrNone )
    			{
        		User::LeaveIfError( ImportNum( maxVal32, aParser ) );
    			}
		    if ( minVal32 > maxVal32 )
		    	{
		    	LogError( KMinMaxWrongValue );
		    	User::Leave( KErrCorrupt );
		    	}
		    if ( ( type == EPropertyInt8 && ( minVal32 < KMinTInt8 || maxVal32 > KMaxTInt8 ) ) ||
		         ( type == EPropertyUint8 && ( minVal32 < 0 || maxVal32 > KMaxTUint8 ) ) ||
		         ( type == EPropertyInt16 && ( minVal32 < KMinTInt16 || maxVal32 > KMaxTInt16 ) ) ||
		         ( type == EPropertyUint16 && ( minVal32 < 0 || maxVal32 > KMaxTUint16 ) ) )
		    	{
		    	LogError( KWrongValue );
		    	User::Leave( KErrCorrupt );
		    	}
		    
		    CheckNoMoreNumericParametersL( aParser );
    		// Add this property to the previously defined object.
    		iLastObjectDef->AddPropertyL( name, type, minVal32, maxVal32, readOnly, mandatory, EFalse );
    		break;
    		}
    	case EPropertyText:
    		{
    		TInt32  minVal32,
    		        maxVal32;
   			error = SetMinMaxDefValueL<TInt32>( CheckForConstant(aParser),
   					minVal32, 1, KSerializedDesMaxLength, 1 );
       		if ( error < KErrNone )
    			{
    			User::LeaveIfError( ImportNum( minVal32, aParser ) );
    			}
   			error = SetMinMaxDefValueL<TInt32>( CheckForConstant(aParser),
   					maxVal32, 1, KSerializedDesMaxLength, 256 );
       		if ( error < KErrNone )
    			{
    			User::LeaveIfError( ImportNum( maxVal32, aParser ) );
    			}
		    if ( minVal32 > maxVal32 )
		    	{
		    	LogError( KMinMaxWrongValue );
		    	User::Leave( KErrCorrupt );
		    	}
		    if ( minVal32 < 1 || minVal32 > KSerializedDesMaxLength ||
		         maxVal32 < 1 || maxVal32 > KSerializedDesMaxLength )
		    	{
		    	LogError( KTextWrongValue );
		    	User::Leave( KErrCorrupt );
		    	}
		    
		    // Text properties may have an extra flag: index flag.
		    TBool indexed( EFalse );
		    // Ignore error code as the flag might not exist and indexing is not mandatory
		    ImportNum( indexed, aParser );
    		// Add this property to the previously defined object.
    		iLastObjectDef->AddPropertyL( name, type, minVal32, maxVal32, readOnly, mandatory, indexed );
    		break;
    		}
    	case EPropertyUint32:
    		{
    		TUint32  minVal32,
    		         maxVal32;
   			error = SetMinMaxDefValueL<TUint32>( CheckForConstant(aParser),
   					minVal32, 0, KMaxTUint32, 0 );
       		if ( error < KErrNone )
    			{
    			User::LeaveIfError( ImportUInt32( minVal32, aParser ) );
    			}
   			error = SetMinMaxDefValueL<TUint32>( CheckForConstant(aParser),
   					maxVal32, 0, KMaxTUint32, KMaxTUint32 );
       		if ( error < KErrNone )
    			{
    			User::LeaveIfError( ImportUInt32( maxVal32, aParser ) );
    			}
		    if ( minVal32 > maxVal32 )
		    	{
		    	LogError( KMinMaxWrongValue );
		    	User::Leave( KErrCorrupt );
		    	}
		    
		    CheckNoMoreNumericParametersL( aParser );
    		// Add this property to the previously defined object.
    		iLastObjectDef->AddPropertyL( name, type, minVal32, maxVal32, readOnly, mandatory, EFalse );
    		break;
    		}
    	case EPropertyInt64:
    		{
    		TInt64 minVal64, maxVal64;
   			error = SetMinMaxDefValueL<TInt64>( CheckForConstant(aParser),
   					minVal64, KMinTInt64, KMaxTInt64, 0 );
       		if ( error < KErrNone )
       			{
       			User::LeaveIfError( ImportInt64( minVal64, aParser ) );
       			}
   			error = SetMinMaxDefValueL<TInt64>( CheckForConstant(aParser),
   					maxVal64, KMinTInt64, KMaxTInt64, KMaxTInt64 );
       		if ( error < KErrNone )
       			{
       			User::LeaveIfError( ImportInt64( maxVal64, aParser ) );
       			}
		    if ( minVal64 > maxVal64 )
		    	{
		    	LogError( KMinMaxWrongValue );
		    	User::Leave( KErrCorrupt );
		    	}
		    
		    CheckNoMoreNumericParametersL( aParser );
    		// Add this property to the previously defined object.
    		iLastObjectDef->AddPropertyL( name, type, minVal64, maxVal64, readOnly, mandatory, EFalse );
    		break;
    		}
    	case EPropertyTime:
    		{
    		TTime minTime, maxTime;
   			error = SetMinMaxDefValueL<TTime>( CheckForConstant(aParser),
   					minTime, 0, KMaxTInt64, 0 );
       		if ( error < KErrNone )
       			{
       			User::LeaveIfError( ImportTime( minTime, aParser ) );
       			}
   			error = SetMinMaxDefValueL<TTime>( CheckForConstant(aParser),
   					maxTime, 0, KMaxTInt64, KMaxTInt64 );
       		if ( error < KErrNone )
       			{
       			User::LeaveIfError( ImportTime( maxTime, aParser ) );
       			}
		    if ( minTime > maxTime )
		    	{
		    	LogError( KMinMaxWrongValue );
		    	User::Leave( KErrCorrupt );
		    	}
		    
		    CheckNoMoreNumericParametersL( aParser );
    		// Add this property to the previously defined object.
    		iLastObjectDef->AddPropertyL( name, type, minTime.Int64(), maxTime.Int64(), readOnly, 
    				mandatory, EFalse );
    		break;
    		}
    	case EPropertyReal32:
    		{
    		TReal32 minReal, maxReal;
   			error = SetMinMaxDefValueL<TReal32>( CheckForConstant(aParser),
   					minReal, -KMaxTReal32, KMaxTReal32, 0 );
       		if ( error < KErrNone )
       			{
       			User::LeaveIfError( ImportNum( minReal, aParser ) );
       			}
   			error = SetMinMaxDefValueL<TReal32>( CheckForConstant(aParser),
   					maxReal, -KMaxTReal32, KMaxTReal32, KMaxTReal32 );
       		if ( error < KErrNone )
       			{
       			User::LeaveIfError( ImportNum( maxReal, aParser ) );
       			}
		    if ( minReal > maxReal )
		    	{
		    	LogError( KMinMaxWrongValue );
		    	User::Leave( KErrCorrupt );
		    	}
		    
		    CheckNoMoreNumericParametersL( aParser );
    		// Add this property to the previously defined object.
    		iLastObjectDef->AddPropertyL( name, type, minReal, maxReal, readOnly, mandatory, EFalse );
    		break;
    		}
    	case EPropertyReal64:
    		{
    		const TReal64 KMDSMaxTReal64( 1.79769313486200E+308 );
    		TReal64 minReal, maxReal;
   			error = SetMinMaxDefValueL<TReal64>( CheckForConstant(aParser),
   					minReal, -KMDSMaxTReal64, KMDSMaxTReal64, 0 );
       		if ( error < KErrNone )
       			{
       			User::LeaveIfError( ImportNum( minReal, aParser ) );
       			}
   			error = SetMinMaxDefValueL<TReal64>( CheckForConstant(aParser),
   					maxReal, -KMDSMaxTReal64, KMDSMaxTReal64, KMDSMaxTReal64 );
       		if ( error < KErrNone )
       			{
       			User::LeaveIfError( ImportNum( maxReal, aParser ) );
       			}
		    if ( minReal > maxReal )
		    	{
		    	LogError( KMinMaxWrongValue );
		    	User::Leave( KErrCorrupt );
		    	}
		    
		    CheckNoMoreNumericParametersL( aParser );
    		// Add this property to the previously defined object.
    		iLastObjectDef->AddPropertyL( name, type, minReal, maxReal, readOnly, mandatory, EFalse );
    		break;
    		}
    	default:
    		User::Leave( KErrNotFound );
    	}
	}

// ------------------------------------------------
// CheckNoIndexFlagL
// ------------------------------------------------
//
void CMdsImportExport::CheckNoMoreNumericParametersL( TLex8& aParser )
	{
	// This function is used to check that indexing flag is not set for other than
	// text properties.
	TBool parameterFound( EFalse );
	TInt err = ImportNum( parameterFound, aParser );
	if ( err == KErrNone )
		{
		// Index boolean not supported for other property types than text.
		User::Leave( KErrNotSupported );
		}
	}

// ------------------------------------------------
// ImportSchemaRelationDefL
// ------------------------------------------------
//
void CMdsImportExport::ImportSchemaRelationDefL( TLex8& aParser )
    {
    // relationdef <ns> <name>
    TBuf16<KMdsMaxUriLenght> name;
    // read namespace
    User::LeaveIfError( ImportText( name, aParser ) );

	CMdsNamespaceDef* actualNamespace = iSchema->GetNamespace( name );
	if ( !actualNamespace )
		{
		_LIT( KMdsNamespaceNotFound, "Namespace not found !!!" );
		LogError( KMdsNamespaceNotFound );
    	User::Leave( KErrAccessDenied );
		}

    if( actualNamespace->GetReadOnly() && !actualNamespace->GetFirstRead() )
    	{
    	_LIT( KError, "Namespace not allowed" );
    	LogError( KError );
    	User::Leave( KErrAccessDenied );
    	}

    // read name
    User::LeaveIfError( ImportText( name, aParser ) );

	actualNamespace->AddRelationDefL( name );
    }

// ------------------------------------------------
// ImportSchemaEventDefL
// ------------------------------------------------
//
void CMdsImportExport::ImportSchemaEventDefL( TLex8& aParser )
    {
    // eventdef <ns> <name> <priority>
    TBuf16<KMdsMaxUriLenght> name;
    User::LeaveIfError( ImportText( name, aParser ) );

	CMdsNamespaceDef* actualNamespace = iSchema->GetNamespace( name );
	if ( !actualNamespace )
		{
		_LIT( KMdsNamespaceNotFound, "Namespace not found !!!" );
		LogError( KMdsNamespaceNotFound );
    	User::Leave( KErrAccessDenied );
		}

    if ( actualNamespace->GetReadOnly() && !actualNamespace->GetFirstRead() )
    	{
    	_LIT( KError, "Namespace not allowed" );
    	LogError( KError );
    	User::Leave( KErrAccessDenied );
    	}

    User::LeaveIfError( ImportText( name, aParser ) );
    
    TInt32 priority=0;
    User::LeaveIfError( ImportNum( priority, aParser ) );

	actualNamespace->AddEventDefL( name, priority );
    }

void CMdsImportExport::ImportSchemaVersionL( TLex8& aParser )
	{
	if ( iVersionFlags & EVersionAlreadyRead )
		{
		_LIT( KError, "Schema version redefined" );
		LogError( KError );
		User::Leave( KErrCorrupt );
		}
    TBuf16<KMdsMaxUriLenght> version;
    User::LeaveIfError( ImportText( version, aParser ) );

	// convert version to two numbers
	TInt32 majorVersion, minorVersion;
	TLex16 parser( version );
	User::LeaveIfError( parser.BoundedVal( majorVersion, KMaxTInt ) );
	parser.Get();
	User::LeaveIfError( parser.BoundedVal( minorVersion, KMaxTInt ) );

	if ( KSchemaFileMajorVersion != majorVersion )
		{
		_LIT( KError, "Schema version mismatch" );
		LogError( KError );
		User::Leave( KErrCorrupt );
		}

    MMdsPreferences::InsertL( KMdsSchemaVersionName, MMdsPreferences::EPreferenceBothSet,
    		majorVersion, minorVersion );

	iVersionFlags |= EVersionAlreadyRead;
	}

void CMdsImportExport::ImportNamespaceFromDBL()
	{
	_LIT( MdsQueryGetNamespaceDefs, "SELECT NamespaceDefID,ReadOnly,VendorId,Name FROM NamespaceDef;" );
	TDefId namespaceDefId = KNoDefId;
	TInt32 vendorId = 0;
	TInt32 namespaceReadOnly = 0;
	TPtrC namespaceName;

	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();

	// importing namespaces
	RRowData emptyData;
	CleanupClosePushL( emptyData );
	RRowData getData;
	CleanupClosePushL( getData );
	getData.AppendL( TColumn( namespaceDefId ) );
	getData.AppendL( TColumn( namespaceReadOnly ) );
	getData.AppendL( TColumn( vendorId ) );
	getData.AppendL( TColumn( EColumnHBuf16 ) );
	RMdsStatement query;
	CleanupClosePushL( query );
	connection.ExecuteQueryL( MdsQueryGetNamespaceDefs, query, emptyData );
	
	// read query results and add namespaces to the schema
	while( connection.NextRowL( query, getData ) )
		{
		getData.Column( 0 ).Get( namespaceDefId );
		getData.Column( 1 ).Get( namespaceReadOnly );
		getData.Column( 2 ).Get( vendorId );
		getData.Column( 3 ).Get( namespaceName );
		CMdsNamespaceDef* nmsp = iSchema->NamespaceAddL( namespaceName,
				namespaceReadOnly ? ETrue : EFalse, vendorId, namespaceDefId );
		if ( !nmsp )
			{
			User::Leave( KErrGeneral );
			}
		nmsp->UnsetFirstRead();
		getData.Column( 3 ).Free();
		}
	CleanupStack::PopAndDestroy( 3, &emptyData ); // query, getData, emptyData
	}

TBool CMdsImportExport::ImportCheckVersionInfoL()
	{
	TInt32 majorVersion = 0;
	TInt64 minorVersion = 0;

	// DB version
    MMdsPreferences::GetL( KMdsDBVersionName, MMdsPreferences::EPreferenceBothGet,
    						  majorVersion, &minorVersion );
	if ( majorVersion != KMdSServMajorVersionNumber )
		{
		return EFalse;
		}

    if ( (TInt64)minorVersion < KMdSServMinorVersionNumber )
        {
        CMdSDatabaseUpdater* updater = CMdSDatabaseUpdater::NewL();
        TBool success( updater->UpdateDatabaseL( (TInt64)minorVersion ) );
        delete updater;
        updater = NULL;
        if( !success )
            {
            return EFalse;
            }
        }	
	
	// schema version
    MMdsPreferences::GetL( KMdsSchemaVersionName, MMdsPreferences::EPreferenceBothGet,
    						  majorVersion, &minorVersion );
	if ( majorVersion == KSchemaFileMajorVersion )
		{
		return ETrue;
		}

	return EFalse;
	}

void CMdsImportExport::ImportSchemaFromDBL( CMdsSchema& aSchema )
	{
	// clear iSchema (we read everything from DB)
	if ( !aSchema.iBaseObject )
		{
		iSchema->AddDefaultObjectL();
		iSchema->iBaseObject->SetStoredEveryInDB();
		aSchema.iBaseObject = iSchema->iBaseObject;
		}
    else
    	{
    	iSchema->iBaseObject = aSchema.iBaseObject;
    	}
	iSchema->Reset();

	if ( !ImportCheckVersionInfoL() )
		{
	    iSchema->iBaseObject = NULL;
    	User::Leave( KErrCorrupt );
		}

	TRAPD( err, ImportNamespaceFromDBL() );
	if ( err != KErrNone )
		{
	    iSchema->iBaseObject = NULL;
    	User::Leave( err );
		}

	const TInt count = iSchema->iNamespaceDefs.Count();
	
	for ( TInt i = 0; i < count; ++i )
		{
		CMdsNamespaceDef* namespaceDef = iSchema->iNamespaceDefs[i];
		TRAP( err, namespaceDef->ImportFromDBL() );
		if ( err != KErrNone )
			{
	    	iSchema->iBaseObject = NULL;
	    	User::Leave( err );
			}
		}

    TRAP( err, aSchema.MergeNamespacesL( *iSchema ) );
   	iSchema->iBaseObject = NULL;
    if ( err != KErrNone )
    	{
    	User::Leave( err );
    	}
	}


TBool CMdsImportExport::ReadMetadataFileLineL()
	{
	if ( iLastLineProcessed )
		{
		TRAPD( e, iReadStream.ReadL( iLine, KMdsLineFeed ) );
		if ( e == KErrEof )
            {
			return EFalse; // succesful exit
            }
		else if ( e != KErrNone )
		    {
		    iDefaultSchema = NULL;
		    User::Leave( e );
		    }
	    iLastLineProcessed = EFalse;
		}
    ++iLineNumber;
	return ETrue;
	}

TInt CMdsImportExport::ImportMetadataL( CMdSSqlObjectManipulate& aManipulate,
    CMdsSchema& aSchemaNew, const TDesC16& aFileName )
	{
	iReadStream.PushL();
    User::LeaveIfError( iReadStream.Open( iFs, aFileName, EFileStreamText ) );
    iFileName.Copy( aFileName );

	if ( !iBuffer )
        {
		iBuffer = CMdCSerializationBuffer::NewL( 8192 ); // 8kB - should be enough
        }

	iDefaultSchema = &aSchemaNew;
    iLineNumber = 0;
    iFailed = 0;
    
    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
    RMdSTransaction transaction( connection );
    CleanupClosePushL(transaction);
    const TInt beginError( transaction.Error() );
    if( beginError != KErrNone )
        {
        CleanupStack::PopAndDestroy( &transaction );
        }
    
    while( ReadMetadataFileLineL() )
        {
        if ( iLine.Length() > 0 )
            {
            // parse the line
            TLex8 parser( iLine );
            TRAPD( e, ImportMetadataItemL( parser, aManipulate ) );
            if ( e != KErrNone )
            	{
			    ++iFailed;
            	}
            }
		else
        	iLastLineProcessed = ETrue;
        }
    
    if( beginError == KErrNone )
        {
        transaction.CommitL();
        CleanupStack::PopAndDestroy( &transaction );
        }

	iDefaultSchema = NULL;
    CleanupStack::PopAndDestroy( &iReadStream ); // Closes stream.
    return iFailed;
	}

void CMdsImportExport::ImportMetadataItemL( TLex8& aParser, CMdSSqlObjectManipulate& aManipulate )
	{
    TPtrC8 token = aParser.NextToken();

    // valid keywords in beginning of line
    if ( token.Length() == 0 || token.Left(2) == KMdsKeywordComment )
        {
        // ignore
        iLastLineProcessed = ETrue;
        return;
        }
    else if ( token == KMdsKeywordObject )
        {
        TRAPD( err, ImportMetadataFileObjectL( aParser, aManipulate ) );
        if (err != KErrNone)
        	{
        	_LIT( KError, "Object error" );
     		LogError( KError );
			User::Leave( err );
        	}
        }
    else if ( token == KMdsKeywordRelation )
        {
        TRAPD( err, ImportMetadataFileRelationL( aParser, aManipulate ) );
        if (err != KErrNone)
        	{
        	_LIT( KError, "Relation error" );
     		LogError( KError );
			User::Leave( err );
        	}
        }
    else if ( token == KMdsKeywordEvent )
        {
		TRAPD( err, ImportMetadataFileEventL( aParser, aManipulate ) );
        if (err != KErrNone)
        	{
        	_LIT( KError, "Relation error" );
     		LogError( KError );
			User::Leave( err );
        	}
        }
    else
        {
        iLastLineProcessed = ETrue;
        _LIT( KError, "Keyword not recognized" );
        LogError( KError );
        User::Leave( KErrCorrupt );
        }
    TPtrC8 tokenLast = aParser.NextToken();
    if ( tokenLast.Length() != 0 && tokenLast.Left(2) != KMdsKeywordComment )
    	{
    	_LIT( KMdsUnknownToken, "Undefined metadata file item" );
    	LogError( KMdsUnknownToken );
    	User::Leave( KErrCorrupt );
    	}
	}

void CMdsImportExport::AddObjectToDBL( CMdSSqlObjectManipulate& aManipulate, CMdsNamespaceDef* aNamespaceDef )
	{
	iLastObjectDef = NULL;
	iBuffer->PositionL( KNoOffset );
	TItemId id = KNoId;
	aManipulate.SetNamespace( aNamespaceDef );
	RMdsStatement baseObjStmt;
    CleanupClosePushL(baseObjStmt);
	RMdsStatement objStmt;
	CleanupClosePushL(objStmt);
	
	TRAPD( err, id = aManipulate.AddObjectL( MMdSDbConnectionPool::GetDefaultDBL(), *iBuffer, baseObjStmt, objStmt ) );
	
	CleanupStack::PopAndDestroy(&objStmt);
	CleanupStack::PopAndDestroy(&baseObjStmt);
	
	aManipulate.SetNamespace( NULL );
	if ( err != KErrNone || id == 0 )
		{
		User::Leave( err );
		}
	}

void CMdsImportExport::ImportMetadataFileObjectL( TLex8& aParser, CMdSSqlObjectManipulate& aManipulate )
	{
	TInt error = KErrNone;

    if ( iLastObjectDef )
    	{
   		User::Leave( KErrCorrupt );
    	}
	iLastLineProcessed = ETrue;

	TMdCObject object;
	object.iId = KNoId;
	object.iFlags = EMdEObjectFlagModOpen;
	object.iUsageCount = 0;
	object.iGuidHigh = 0;
	object.iGuidLow = 0;
	object.iFreeTexts.iPtr.iCount = 0;
	object.iFreeTexts.iPtr.iOffset = KNoOffset;

	TMdCOffset freespaceOffset = sizeof(TMdCObject);
	
	TBuf16<KMdsMaxUriLenght> textValue;

    // namespace
    User::LeaveIfError( ImportText( textValue, aParser ) );

    CMdsNamespaceDef* namespaceDef = iDefaultSchema->GetNamespace( textValue );
	if ( !namespaceDef )
		{
		User::Leave( KErrNotFound );
		}

    // read object variables
    // objectDef name
    User::LeaveIfError( ImportText( textValue, aParser ) );
    
    iLastObjectDef = namespaceDef->GetObjectDef( textValue );
    if ( !iLastObjectDef )
    	{
    	User::Leave( KErrNotFound );
    	}
    object.iDefId = iLastObjectDef->GetId();
    
	// object uri
	User::LeaveIfError( ImportText( textValue, aParser ) );
	object.iUri.iPtr.iCount = textValue.Length();
	object.iUri.iPtr.iOffset = freespaceOffset;
	iBuffer->PositionL( freespaceOffset );
	freespaceOffset = iBuffer->InsertL( textValue );

	// Object must not be context object and 
	// the length of the URI must be atleast 3 (X:\) 
	if ( !(iLastObjectDef->GetFlags() & CMdsObjectDef::EObjectDefFlagsContext) && 
			textValue.Length() >= 3 )
	    {
	    // possible drive letter
	    TChar driveLetter( textValue[0] );
	    driveLetter.UpperCase();
	    
	    // possible colon and backslash
	    _LIT( KColonBackslashMatch, ":\\" );
	    TPtrC beginUri = textValue.Mid( 1, KColonBackslashMatch.iTypeLength );
	    TBool validUri( ETrue );
	    
	    // URI must begin with "X:\"
	    if( 'A' <= driveLetter && driveLetter <= 'Z' && 
	    		beginUri.Compare( KColonBackslashMatch ) == 0 )
	    	{
			// check if uri exists
			if ( !BaflUtils::FileExists( iFs, textValue ) )
				{
			    iLastObjectDef = NULL;
				_LIT( KError, "uri is not real" );
				LogError( KError );
				User::Leave( KErrNotFound );
				}
	    	}
	    else
	        {
	        validUri = EFalse;
	        }
	    
	    if( validUri )
	        {
	        User::LeaveIfError( ImportMediaId( object.iMediaId, aParser, driveLetter ) );
	        }
	    else
	        {
	        User::LeaveIfError( ImportUInt32( object.iMediaId, aParser ) );
	        }
	    }
	else
	    {
	    User::LeaveIfError( ImportUInt32( object.iMediaId, aParser ) );
	    }

    const TUint32 allPropCount = iLastObjectDef->GetAllPropertiesCount();
    object.iProperties.iPtr.iCount = allPropCount;
    object.iProperties.iPtr.iOffset = freespaceOffset;

    freespaceOffset += allPropCount * sizeof(TMdCProperty);
   
    // set property offset
	CDesC16ArrayFlat* freeTextBuffer = new(ELeave) CDesC16ArrayFlat(8);
	CleanupStack::PushL( freeTextBuffer );

	TUint32 propertyCount = 0;
	while ( ReadMetadataFileLineL() )
		{
		TLex8 parser( iLine );
		TPtrC8 token = parser.NextToken();

	    if ( iLine.Length() == 0 || token.Length() == 0 || token.Left(2) == KMdsKeywordComment )
	        {
	        // ignore line
	        iLastLineProcessed = ETrue;
	        }
		// now if exists there should be one or more properties		
		else if(token == KMdsKeywordProperty )
			{
			if ( object.iFreeTexts.iPtr.iOffset != KNoOffset || propertyCount >= allPropCount )
				{
				_LIT( KError, "Property after freetext" );
				LogError( KError );
				error = KErrCorrupt;
				}
			if (error != KErrNone)
				{
				// just fetch rest of object lines
				iLastLineProcessed = ETrue;
				}
			else
				{
				iBuffer->PositionL( object.iProperties.iPtr.iOffset
						+ propertyCount * sizeof(TMdCProperty) );
				TRAP( error, freespaceOffset = ImportMetadataFilePropertyL(
						parser, freespaceOffset ) );
				if (error != KErrNone)
					{
					_LIT( KError, "Property error" );
			        LogError( KError );
					}
				++propertyCount;
				}
			}
		// and after those there should be freetext
		else if ( token == KMdsKeywordFreeText )
		    {
			if (error != KErrNone)
				{
				// just fetch rest of object lines
				iLastLineProcessed = ETrue;
				}
			else
				{
			    if ( object.iFreeTexts.iPtr.iOffset == KNoOffset)
			    	{
			    	object.iFreeTexts.iPtr.iOffset = freespaceOffset;
			    	}
				else
					{
			    	TRAP( error, ImportMetadataFileFreeTextL( parser, *freeTextBuffer ) );
					if (error != KErrNone)
						{
						_LIT( KError, "Freetext error" );
				        LogError( KError );
						}
					}
				}
		    }
		else
			{
			if ( token == KMdsKeywordObject || token == KMdsKeywordRelation || token == KMdsKeywordEvent )
				{
				break;
				}
			iLastLineProcessed = ETrue;
			}
		}

	if ( error != KErrNone )
		{
		iLastObjectDef = NULL;
		User::Leave( error );
		}

	object.iProperties.iPtr.iCount = propertyCount;
	
	// add freetext
	object.iFreeTexts.iPtr.iCount = freeTextBuffer->Count();
	if ( object.iFreeTexts.iPtr.iCount > 0 )
		{
		// set flags
		object.iFlags |= EMdEObjectFlagFreetexts | EMdEObjectFlagModFreeText;

		iBuffer->PositionL( object.iFreeTexts.iPtr.iOffset );
		for ( TInt32 i = 0; i < object.iFreeTexts.iPtr.iCount; ++i )
			{
			TPtrC16 word = (*freeTextBuffer)[i];
			iBuffer->InsertL( word );
			}
		}
	else
		{
		object.iFreeTexts.iPtr.iOffset = KNoOffset;
		}

	iBuffer->PositionL( KNoOffset );
	object.SerializeL( *iBuffer );
	
	// add object to DB
	AddObjectToDBL(aManipulate, namespaceDef);

	CleanupStack::PopAndDestroy( freeTextBuffer );
	}

TMdCOffset CMdsImportExport::ImportMetadataFilePropertyL( TLex8& aParser, TMdCOffset aFreespaceOffset )
	{
    if ( !iLastObjectDef )
    	{
    	User::Leave( KErrCorrupt );
    	}
	iLastLineProcessed = ETrue;
	TBuf16<KMdsMaxUriLenght> textValue;

	// property name
    User::LeaveIfError( ImportText( textValue, aParser ) );
    
    _LIT( KGuidHigh, "GuidHigh" );
    _LIT( KGuidLow, "GuidLow" );
    
    if( textValue == KGuidHigh || textValue == KGuidLow )
    	{
    	User::Leave( KErrCorrupt );
    	}

    CMdsPropertyDef* propertyDef = iLastObjectDef->GetProperty( textValue );
    if ( !propertyDef )
    	{
    	User::Leave( KErrCorrupt );
    	}

    TMdCProperty property;
    property.iModFlags = EMdEPropertyModChange;
    property.iPropertyDefId = propertyDef->GetId();

    // find proper place to put property
    const TMdCOffset propertyOffset = iBuffer->Position();

	switch( propertyDef->GetType() )
		{
    	case EPropertyBool:
    		{
    		TInt32 intValue;
		    User::LeaveIfError( ImportNum( intValue, aParser ) );
    		TBool value = intValue ? ETrue : EFalse;
    		property.iValue.iInt32 = value;
    		break;
    		}
    	case EPropertyInt8:
    		{
    		TInt32 intValue;
		    User::LeaveIfError( ImportNum( intValue, aParser ) );
    		property.iValue.iInt32 = intValue;
    		break;
    		}
    	case EPropertyUint8:
    		{
    		TInt32 intValue;
		    User::LeaveIfError( ImportNum( intValue, aParser ) );
    		property.iValue.iUint32 = intValue;
    		break;
    		}
    	case EPropertyInt16:
    		{
    		TInt32 intValue;
		    User::LeaveIfError( ImportNum( intValue, aParser ) );
    		property.iValue.iInt32 = intValue;
    		break;
    		}
    	case EPropertyUint16:
    		{
    		TInt32 intValue;
		    User::LeaveIfError( ImportNum( intValue, aParser ) );
    		property.iValue.iUint32 = intValue;
    		break;
    		}
    	case EPropertyInt32:
    		{
    		TInt32 value;
		    User::LeaveIfError( ImportNum( value, aParser ) );
    		property.iValue.iInt32 = value;
    		break;
    		}
    	case EPropertyUint32:
    		{
    		TUint32 value;
		    User::LeaveIfError( ImportUInt32( value, aParser ) );
    		property.iValue.iUint32 = value;
    		break;
    		}
    	case EPropertyInt64:
    		{
    		TInt64 value;
		    User::LeaveIfError( ImportInt64( value, aParser ) );
    		property.iValue.iInt64 = value;
    		break;
    		}
    	case EPropertyTime:
    		{
    		TTime value;
		    User::LeaveIfError( ImportTime( value, aParser ) );
    		property.iValue.iInt64 = value.Int64();
    		break;
    		}
    	case EPropertyReal32:
    		{
    		TReal32 value;
		    User::LeaveIfError( ImportNum( value, aParser ) );
    		property.iValue.iReal = value;
    		break;
    		}
    	case EPropertyReal64:
    		{
    		TReal64 value;
		    User::LeaveIfError( ImportNum( value, aParser ) );
    		property.iValue.iReal = value;
    		break;
    		}
    	case EPropertyText:
    		{
			TBuf16<256> value;
    		User::LeaveIfError( ImportText( value, aParser ) );
    		property.iValue.iPtr.iCount = value.Length();
    		if (property.iValue.iPtr.iCount > 0)
    			{
    			property.iValue.iPtr.iOffset = aFreespaceOffset;
    			iBuffer->PositionL( aFreespaceOffset );
    			aFreespaceOffset = iBuffer->InsertL( value );
    			}
    		else
    			{
    			User::Leave( KErrCorrupt );
    			}
    		break;
    		}
    	default:
    		User::Leave( KErrNotFound );
		}

	iBuffer->PositionL( propertyOffset );
	property.SerializeL( *iBuffer );
	
	return aFreespaceOffset;
	}

void CMdsImportExport::ImportMetadataFileFreeTextL( TLex8& aParser, CDesC16ArrayFlat& aFreeTextArray )
	{
    if ( !iLastObjectDef )
    	{
    	User::Leave( KErrCorrupt );
    	}
	iLastLineProcessed = ETrue;

	TBuf16<256> freeText;
	while( ImportText( freeText, aParser ) == KErrNone )
		{
		aFreeTextArray.AppendL( freeText );
		}
	}

void CMdsImportExport::ImportMetadataFileRelationL( TLex8& aParser, CMdSSqlObjectManipulate& aManipulate )
	{
	_LIT( KImportRelationGetObjectId, "SELECT ObjectId FROM Object%u WHERE URI=? LIMIT 1;" );
    if(iLastObjectDef)
    	{
   		User::Leave( KErrCorrupt );
    	}
	iLastLineProcessed = ETrue;

	CMdsClauseBuffer* clauseBuffer = CMdsClauseBuffer::NewLC( KImportRelationGetObjectId.iTypeLength + 10 ); // one uint

	TBuf16<KMdsMaxUriLenght> textValue;

	// namespace
    User::LeaveIfError( ImportText( textValue, aParser ) );

    CMdsNamespaceDef* namespaceDef = iDefaultSchema->GetNamespace( textValue );
	if ( !namespaceDef )
		{
		User::Leave( KErrNotFound );
		}

	clauseBuffer->BufferL().Format( KImportRelationGetObjectId, namespaceDef->GetId() );

	TMdCRelation relation;
	relation.iId = KNoId;
	relation.iGuidHigh = 0;
	relation.iGuidLow = 0;
	relation.iLastModifiedDate.UniversalTime();

    // read relation variables
    // relationDef name
    User::LeaveIfError( ImportText( textValue, aParser ) );
    CMdsRelationDef* relationDef = namespaceDef->GetRelationDef( textValue );
    if ( !relationDef )
    	{
    	User::Leave( KErrNotFound );
    	}
    relation.iDefId = relationDef->GetId();

	RRowData dataRow;
	CleanupClosePushL( dataRow );
	RMdsStatement query;
	CleanupClosePushL( query );

	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();

	// left object name
	User::LeaveIfError( ImportText( textValue, aParser ) );
	relation.iLeftObjectId = KNoId;

	dataRow.AppendL( TColumn( textValue ) );
	connection.ExecuteQueryL( clauseBuffer->ConstBufferL(), query, dataRow );
	dataRow.Free();
	dataRow.Column( 0 ).Set( relation.iLeftObjectId );
	if( connection.NextRowL( query, dataRow ) )
		{
		dataRow.Column( 0 ).Get( relation.iLeftObjectId );
		}
	if (relation.iLeftObjectId == KNoId)
		{
		User::Leave( KErrNotFound );
		}

	// right object name
	User::LeaveIfError( ImportText( textValue, aParser ) );
	relation.iRightObjectId = KNoId;
	dataRow.Column( 0 ).Set( textValue );

	connection.ExecuteQueryL( clauseBuffer->ConstBufferL(), query, dataRow );
	dataRow.Free();
	dataRow.Column( 0 ).Set( relation.iLeftObjectId );
	if( connection.NextRowL( query, dataRow ) )
		{
		dataRow.Column( 0 ).Get( relation.iRightObjectId );
		}
	if (relation.iRightObjectId == KNoId)
		{
		User::Leave( KErrNotFound );
		}

	CleanupStack::PopAndDestroy( 2, &dataRow ); // query, dataRow

	relation.iParameter = 0;
	User::LeaveIfError( ImportNum( relation.iParameter, aParser ) );

	iBuffer->PositionL( KNoOffset );
	relation.SerializeL( *iBuffer );

	TItemId id = KNoId;
	iBuffer->PositionL( KNoOffset );
	aManipulate.SetNamespace( namespaceDef );
	TRAPD( err, id = aManipulate.AddRelationL( connection, *iBuffer ) );
	aManipulate.SetNamespace( NULL );
	if (err != KErrNone || id == KNoId)
		{
		User::Leave( err );
		}
	CleanupStack::PopAndDestroy( clauseBuffer ); // clauseBuffer
	}

void CMdsImportExport::ImportMetadataFileEventL( TLex8& aParser, CMdSSqlObjectManipulate& aManipulate )
	{
	_LIT( KImportEventGetObjectId, "SELECT ObjectId FROM Object%u WHERE URI=? LIMIT 1;" );
    if ( iLastObjectDef )
    	{
   		User::Leave( KErrCorrupt );
    	}
	iLastLineProcessed = ETrue;

	CMdsClauseBuffer* clauseBuffer = CMdsClauseBuffer::NewLC( KImportEventGetObjectId.iTypeLength + 10 ); // one uint

	TBuf16<KMdsMaxUriLenght> textValue;

	// namespace
    User::LeaveIfError( ImportText( textValue, aParser ) );

    CMdsNamespaceDef* namespaceDef = iDefaultSchema->GetNamespace( textValue );
	if ( !namespaceDef )
		{
		User::Leave( KErrNotFound );
		}

	clauseBuffer->BufferL().Format( KImportEventGetObjectId, namespaceDef->GetId() );

	TMdCEvent event;
	event.iId = KNoId;

    // read event variables
    // eventDef name
    User::LeaveIfError( ImportText( textValue, aParser ) );
    CMdsEventDef* eventDef = namespaceDef->GetEventDef( textValue );
    if ( !eventDef )
    	{
    	User::Leave( KErrNotFound );
    	}
    event.iDefId = eventDef->GetId();

	RRowData dataRow;
	CleanupClosePushL( dataRow );
	RMdsStatement query;
	CleanupClosePushL( query );

	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();

	// object name
	User::LeaveIfError( ImportText( textValue, aParser ) );
	event.iObjectId = KNoId;

	dataRow.AppendL( TColumn( textValue ) );
	connection.ExecuteQueryL( clauseBuffer->ConstBufferL(), query, dataRow );
	dataRow.Free();
	dataRow.Column( 0 ).Set( event.iObjectId );
	if ( connection.NextRowL( query, dataRow ) )
		{
		dataRow.Column( 0 ).Get( event.iObjectId );
		}
	if ( event.iObjectId == KNoId )
		{
		User::Leave( KErrNotFound );
		}

	CleanupStack::PopAndDestroy( 2, &dataRow ); // query, dataRow

	TMdCOffset freespaceOffset = sizeof(TMdCEvent);
	// source
	User::LeaveIfError( ImportText( textValue, aParser ) );
	event.iSourceText.iPtr.iCount = textValue.Length();
	if (event.iSourceText.iPtr.iCount > 0)
		{
		event.iSourceText.iPtr.iOffset = freespaceOffset;
		iBuffer->PositionL( freespaceOffset );
		freespaceOffset = iBuffer->InsertL( textValue );
		}
	else
		{
		event.iSourceText.iPtr.iOffset = KNoOffset;
		}

	// participant
	User::LeaveIfError( ImportText( textValue, aParser ) );
	event.iParticipantText.iPtr.iCount = textValue.Length();
	if (event.iParticipantText.iPtr.iCount > 0)
		{
		event.iParticipantText.iPtr.iOffset = freespaceOffset;
		iBuffer->PositionL( freespaceOffset );
		freespaceOffset = iBuffer->InsertL( textValue );
		}
	else
		{
		event.iParticipantText.iPtr.iOffset = KNoOffset;
		}

	// time
	User::LeaveIfError( ImportTime( event.iTime, aParser ) );

	iBuffer->PositionL( KNoOffset );
	event.SerializeL( *iBuffer );

	TItemId id = KNoId;
	iBuffer->PositionL( KNoOffset );
	aManipulate.SetNamespace( namespaceDef );
	TRAPD( err, id = aManipulate.AddEventL( connection, *iBuffer ) );
	aManipulate.SetNamespace( NULL );
	if (err != KErrNone || id == KNoId)
		{
		User::Leave( err );
		}

	CleanupStack::PopAndDestroy( clauseBuffer ); // clauseBuffer
	}

void CMdsImportExport::ExportMetadataL( CMdsSchema& aSchemaNew, const TDesC16& aFileName,
										CMdCSerializationBuffer& aItems )
	{
	iFs.PrivatePath( iFileName );
	if ( aFileName.Find(iFileName) != KErrNotFound )
		{
		User::Leave( KErrAccessDenied );
		}

	CleanupClosePushL( iWriteStream );
    User::LeaveIfError( iWriteStream.Replace( iFs, aFileName, EFileShareExclusive | EFileStreamText | EFileWrite ) );

	// reading import "filters"
	aItems.PositionL( KNoOffset ); // start from the beginning of buffer
	const TMdCItemIds& itemIds = TMdCItemIds::GetFromBufferL( aItems ); // read item ids from buffer
	const CMdsNamespaceDef* namespaceDefRestrict = NULL;
	if ( itemIds.iNamespaceDefId != KNoDefId )
		{
		namespaceDefRestrict = aSchemaNew.GetNamespaceByIdL( itemIds.iNamespaceDefId );
		}

	RPointerArray<CMdsObjectDef>   objectDefToExport;
	CleanupClosePushL( objectDefToExport );
	RPointerArray<CMdsEventDef>    eventDefToExport;
	CleanupClosePushL( eventDefToExport );
	RPointerArray<CMdsRelationDef> relationDefToExport;
	CleanupClosePushL( relationDefToExport );

	// Get all object definitions from buffer.
	if ( itemIds.iObjectIds.iPtr.iCount > 0 )
		{
		if ( !namespaceDefRestrict )
			{
			User::Leave( KErrCorrupt );
			}
		objectDefToExport.ReserveL( itemIds.iObjectIds.iPtr.iCount );
		aItems.PositionL( itemIds.iObjectIds.iPtr.iOffset );
		TDefId objectDefId;
		for ( TInt i = 0; i < itemIds.iObjectIds.iPtr.iCount; ++i )
			{
			aItems.ReceiveL( objectDefId );
			const CMdsObjectDef* objectDef = namespaceDefRestrict->GetObjectByIdL( objectDefId );
			if (objectDef)
				{
				objectDefToExport.AppendL( objectDef );
				}
			}
		}

	// Get all event definitions from buffer.
	if ( itemIds.iEventIds.iPtr.iCount > 0 )
		{
		if (!namespaceDefRestrict)
			{
			User::Leave( KErrCorrupt );
			}
		eventDefToExport.ReserveL( itemIds.iEventIds.iPtr.iCount );
		aItems.PositionL( itemIds.iEventIds.iPtr.iOffset );
		TDefId eventDefId;
		for ( TInt i = 0; i < itemIds.iEventIds.iPtr.iCount; ++i )
			{
			aItems.ReceiveL( eventDefId );
			const CMdsEventDef* eventDef = namespaceDefRestrict->GetEventByIdL( eventDefId );
			if (eventDef)
				{
				eventDefToExport.AppendL( eventDef );
				}
			}
		}

	// Get all relation definitions from buffer.
	if ( itemIds.iRelationIds.iPtr.iCount > 0 )
		{
		if ( !namespaceDefRestrict )
			{
			User::Leave( KErrCorrupt );
			}
		relationDefToExport.ReserveL( itemIds.iRelationIds.iPtr.iCount );
		aItems.PositionL( itemIds.iRelationIds.iPtr.iOffset );
		TDefId relationDefId;
		for ( TInt i = 0; i < itemIds.iRelationIds.iPtr.iCount; ++i )
			{
			aItems.ReceiveL( relationDefId );
			const CMdsRelationDef* relationDef = namespaceDefRestrict->GetRelationByIdL( relationDefId );
			if ( relationDef )
				{
				relationDefToExport.AppendL( relationDef );
				}
			}
		}

	CMdsClauseBuffer* clause = CMdsClauseBuffer::NewLC( 2048 );
	CMdsClauseBuffer* freeTextClause = CMdsClauseBuffer::NewLC( 512 );

	RRowData dataRow;
	CleanupClosePushL( dataRow );
	RRowData freeTextRow;
	CleanupClosePushL( freeTextRow );

	TInt j( 0 );
	
	const TInt namespaceCount = aSchemaNew.iNamespaceDefs.Count();
	
	for ( TInt i = 0; i < namespaceCount; ++i )
		{
		CMdsNamespaceDef* namespaceDef = aSchemaNew.iNamespaceDefs[i];
		if ( namespaceDefRestrict && namespaceDefRestrict != namespaceDef )
			{
			continue;
			}

		ExportMetadataMakeFreeTextSqlClauseL( *namespaceDef, *freeTextClause, freeTextRow );
		// writing object information
		
		const TInt objectDefCount = namespaceDef->iObjectDefs.Count();
		
		for ( j = 0; j < objectDefCount; ++j )
			{
			CMdsObjectDef* objectDef = namespaceDef->iObjectDefs[j];
			if ( !namespaceDefRestrict || objectDefToExport.Count() == 0 || objectDefToExport.Find( objectDef ) != KErrNotFound )
				{
				ExportMetadataMakeSqlObjectClauseL( *namespaceDef, *objectDef, *clause, dataRow );
				ExportMetadataWriteObjectInfoL( *namespaceDef, *objectDef, *clause, dataRow, *freeTextClause, freeTextRow );
				}
			}

		// writing relation information
		ExportMetadataMakeSqlRelationClauseL( *namespaceDef, *clause, dataRow );
		
		const TInt relationDefCount = namespaceDef->iRelationDefs.Count();
		
		for ( j = 0; j < relationDefCount; ++j )
			{
			CMdsRelationDef* relationDef = namespaceDef->iRelationDefs[j];
			if ( !namespaceDefRestrict || relationDefToExport.Count() == 0 || relationDefToExport.Find( relationDef ) != KErrNotFound )
				{
				ExportMetadataWriteRelationInfoL( *namespaceDef, *relationDef, *clause, dataRow );
				}
			if ( j == namespaceDef->iRelationDefs.Count() - 1 )
				{
				iWriteStream.WriteL( KExportMetadataNewLine );
				}
			}

		// writing event information
		ExportMetadataMakeSqlEventClauseL( *namespaceDef, *clause, dataRow );
		
		const TInt eventDefCount = namespaceDef->iEventDefs.Count();
		
		for ( j = 0; j < eventDefCount; ++j )
			{
			CMdsEventDef* eventDef = namespaceDef->iEventDefs[j];
			if ( !namespaceDefRestrict || eventDefToExport.Count() == 0 || eventDefToExport.Find( eventDef ) != KErrNotFound )
				{
				ExportMetadataWriteEventInfoL( *namespaceDef, *eventDef, *clause, dataRow );
				}
			}
		}

    CleanupStack::PopAndDestroy( 8, &iWriteStream ); // freeTextRow, dataRow, freeTextClause, clause, relationDefToExport, eventDefToExport, objectDefToExport, iWriteStream
	}

void CMdsImportExport::ExportMetadataMakeSqlObjectClauseL( const CMdsNamespaceDef& aNamespaceDef, const CMdsObjectDef& aObjectDef,
														  CMdsClauseBuffer& aClause, RRowData& aDataRow )
	{
	_LIT( KExportMetadataNotConfidential, " NOT (Flags&? OR Flags&? OR Flags&?);" );
	aClause.BufferL().Zero(); // reset clause
	aDataRow.Free(); // free and reset dataRow
	aDataRow.Reset();
	
	aClause.AppendL( KSelectPropertyFilterBegin );
	aDataRow.AppendL( TColumn( TItemId(0) ) ); // objectId
	aDataRow.AppendL( TColumn( TDefId(0) ) ); // objectDefId
	aDataRow.AppendL( TColumn( TUint32(0) ) ); // objectFlags
	aDataRow.AppendL( TColumn( TUint32(0) ) ); // objectMediaId
	aDataRow.AppendL( TColumn( TUint32(0) ) ); // usageCount
	aDataRow.AppendL( TColumn( TInt64(0) ) ); // objectGuidHigh
	aDataRow.AppendL( TColumn( TInt64(0) ) ); // objectGuidLow
	aDataRow.AppendL( TColumn( EColumnDes16 ) ); // objectURI

	const TInt allPropertiesCount = aObjectDef.GetAllPropertiesCount();
	for ( TInt i = 0; i < allPropertiesCount; ++i )
		{
		const CMdsObjectDef::TMdsColumnOrder& column = aObjectDef.GetPropertyColumnL( i );
		const CMdsPropertyDef& property = column.iPropertyDef;

		aClause.AppendL( KComma );
		aClause.AppendL( property.GetName() );

		aDataRow.AppendL( TColumn( property.GetSqlType() ) );
		}
	aClause.AppendL( KSpace );

	aClause.AppendL( KFromBaseObject, KMaxUintValueLength ); // + namespace id
	aClause.BufferL().AppendNum( aNamespaceDef.GetId() );
	aClause.AppendL( KAsBaseObject );

	aClause.AppendL( KComma );
	aClause.AppendL( aObjectDef.GetName(), KMaxUintValueLength ); // + namespace id
	aClause.BufferL().AppendNum( aNamespaceDef.GetId() );

	aClause.AppendL( KAsObjectOnEqual );

	aClause.AppendL( KWhere );
	aClause.AppendL( KSpace );
	aClause.AppendL( KExportMetadataNotConfidential );
	}

void CMdsImportExport::ExportMetadataMakeFreeTextSqlClauseL( const CMdsNamespaceDef& aNamespaceDef,
															CMdsClauseBuffer& aFreeTextClause, RRowData& aFreeTextRow )
	{
	_LIT( KExportMetadataFreeTextSearch,
		  "SELECT Word FROM TextSearchDictionary%u WHERE WordId IN (SELECT WordId FROM TextSearch%u WHERE ObjectId = ?);" );

	aFreeTextClause.BufferL().Zero();
	aFreeTextClause.ReserveSpaceL( KExportMetadataFreeTextSearch.iTypeLength + 2*KMaxUintValueLength );

	aFreeTextClause.BufferL().Format( KExportMetadataFreeTextSearch, aNamespaceDef.GetId(), aNamespaceDef.GetId() );

	aFreeTextRow.Free();
	aFreeTextRow.Reset();
	aFreeTextRow.AppendL( TItemId(0) );
	}

void CMdsImportExport::ExportMetadataWriteObjectInfoL( const CMdsNamespaceDef& aNamespaceDef, const CMdsObjectDef& aObjectDef,
													   CMdsClauseBuffer& aClause, RRowData& aDataRow,
													   CMdsClauseBuffer& aFreeTextClause, RRowData& aFreeTextRow )
	{
	TItemId objectId;
	TDefId objectDefId;
	TUint32 objectFlags, objectMediaId, usageCount;
	TInt64 objectGuidHigh, objectGuidLow;
	TPtrC16 objectURI;

	RRowData confidentialFlagRow;
	CleanupClosePushL( confidentialFlagRow );
	confidentialFlagRow.AppendL( TColumn( EMdEObjectFlagRemoved ) );
	confidentialFlagRow.AppendL( TColumn( EMdEObjectFlagNotPresent ) );
	confidentialFlagRow.AppendL( TColumn( EMdEObjectFlagConfidential ) );

	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();

	RMdsStatement query;
	CleanupClosePushL( query );

	RMdsStatement freeTextQuery;
	CleanupClosePushL( freeTextQuery );
	RRowData textRow;
	CleanupClosePushL( textRow );
	textRow.AppendL( TColumn( EColumnDes16 ) );

	RRowData queryResult;
	CleanupClosePushL( queryResult );
	queryResult.AppendColumnTypesL( aDataRow );
	connection.ExecuteQueryL( aClause.ConstBufferL(), query, confidentialFlagRow );
	while( connection.NextRowL( query, queryResult ) )
		{
		// get result from query
		queryResult.Column( 0 ).Get( objectId ); // objectId
		queryResult.Column( 1 ).Get( objectDefId ); // objectDefId
		__ASSERT_DEBUG( objectDefId == aObjectDef.GetId(), User::Panic( _L( "CMdsImportExport::ExportMetadataWriteObjectInfo" ), KErrGeneral ) );
		queryResult.Column( 2 ).Get( objectFlags ); // objectFlags
		queryResult.Column( 3 ).Get( objectMediaId ); // objectMediaId
		queryResult.Column( 4 ).Get( usageCount ); // usageCount
		queryResult.Column( 5 ).Get( objectGuidHigh ); // objectGuidHigh
		queryResult.Column( 6 ).Get( objectGuidLow ); // objectGuidLow
		queryResult.Column( 7 ).Get( objectURI ); // objectURI

		// writing basic object information
		iWriteStream.WriteL( KMdsKeywordObject );
		iWriteStream.WriteL( KExportMetadataSpace );
		Conv16To8( aNamespaceDef.GetName(), iLine );
		iWriteStream.WriteL( iLine );
		iWriteStream.WriteL( KExportMetadataSpace );
		Conv16To8( aObjectDef.GetName(), iLine );
		iWriteStream.WriteL( iLine );
		iWriteStream.WriteL( KExportMetadataSpace );
		Conv16To8( objectURI, iLine );
		iLine.Insert( 0, KExportMetadataQuotationMark );
		iLine.Append( KExportMetadataQuotationMark );
		iWriteStream.WriteL( iLine );
		iWriteStream.WriteL( KExportMetadataSpace );
		iLine.Num( objectMediaId );
		iWriteStream.WriteL( iLine );
		iWriteStream.WriteL( KExportMetadataNewLine );

		const TInt count = aObjectDef.GetAllPropertiesCount();
		
		// writing property information
		for ( TInt i = 0; i < count; ++i )
			{
			if ( queryResult.Column( i + KBaseObjectBasicValueColumnOffset ).IsNull() )
				{
				continue;
				}

			const CMdsObjectDef::TMdsColumnOrder& column = aObjectDef.GetPropertyColumnL( i );
			const CMdsPropertyDef& property = column.iPropertyDef;

			iWriteStream.WriteL( KMdsKeywordProperty );
			iWriteStream.WriteL( KExportMetadataSpace );
			Conv16To8( property.GetName(), iLine );
			iWriteStream.WriteL( iLine );
			iWriteStream.WriteL( KExportMetadataSpace );

			switch( property.GetSqlType() )
				{
				case EColumnBool:
					{
					TBool value;
					queryResult.Column( i + KBaseObjectBasicValueColumnOffset ).Get( value );
					iLine.Num( value );
					break;
					}
				case EColumnInt32:
					{
					TInt32 value;
					queryResult.Column( i + KBaseObjectBasicValueColumnOffset ).Get( value );
					iLine.Num( value );
					break;
					}
				case EColumnUint32:
					{
					TUint32 value;
					queryResult.Column( i + KBaseObjectBasicValueColumnOffset ).Get( value );
					iLine.Num( value );
					break;
					}
				case EColumnInt64:
					{
					TInt64 value;
					queryResult.Column( i + KBaseObjectBasicValueColumnOffset ).Get( value );
					iLine.Num( value );
					break;
					}
				case EColumnTime:
					{
					TTime value;
					queryResult.Column( i + KBaseObjectBasicValueColumnOffset ).Get( value );
					TDateTime time = value.DateTime();
					iLine.Format( KExportMetadataTimeFormat, time.Year(), time.Month()+1, time.Day()+1, time.Hour(), time.Minute(), time.Second() );
					break;
					}
				case EColumnReal32:
					{
					TReal32 value;
					queryResult.Column( i + KBaseObjectBasicValueColumnOffset ).Get( value );
					TRealFormat realFormat;
                	realFormat.iType |= KAllowThreeDigitExp | KDoNotUseTriads;
                	realFormat.iPoint = TChar('.');
					iLine.Num( value, realFormat );
					break;
					}
				case EColumnReal64:
					{
					TReal64 value;
					queryResult.Column( i + KBaseObjectBasicValueColumnOffset ).Get( value );
					TRealFormat realFormat;
                	realFormat.iType |= KAllowThreeDigitExp | KDoNotUseTriads;
                	realFormat.iPoint = TChar('.');
					iLine.Num( value, realFormat );
					break;
					}
				case EColumnDes16:
					{
					TPtrC16 value;
					queryResult.Column( i + KBaseObjectBasicValueColumnOffset ).Get( value );
					Conv16To8( value, iLine );
					iLine.Insert( 0, KExportMetadataQuotationMark );
					iLine.Append( KExportMetadataQuotationMark );
					break;
					}
				default:
					{
					User::Leave( KErrCorrupt );
					}
				}
			iWriteStream.WriteL( iLine );

			iWriteStream.WriteL( KExportMetadataNewLine );
			}

		// restore query
		queryResult.AppendColumnTypesL( aDataRow );

		// writing freetext
		// get freetext
		aFreeTextRow.Column( 0 ).Set( objectId );
		connection.ExecuteQueryL( aFreeTextClause.ConstBufferL(), freeTextQuery, aFreeTextRow );
		TBool freeTextPreamble = EFalse;
		TInt lineSize = KMdsKeywordFreeText().Size() + 3; // space + line ending
		while( connection.NextRowL( freeTextQuery, textRow ) )
			{
			// get result from query
			if ( !freeTextPreamble )
				{
				iWriteStream.WriteL( KMdsKeywordFreeText );
				iWriteStream.WriteL( KExportMetadataSpace );
				freeTextPreamble = ETrue;
				}
			TPtrC16 word;
			textRow.Column( 0 ).Get( word );
			Conv16To8( word, iLine );
			iLine.Insert( 0, KExportMetadataQuotationMark );
			iLine.Append( KExportMetadataQuotationMark );
			lineSize += iLine.Length() + 2;
			if ( lineSize >= KMdsMaxLineLenght )
				{
				iWriteStream.WriteL( KExportMetadataNewLine );
				iWriteStream.WriteL( KMdsKeywordFreeText );
				iWriteStream.WriteL( KExportMetadataSpace );
				lineSize = KMdsKeywordFreeText().Size() + 3; // space + line ending
				}
			iWriteStream.WriteL( iLine );
			iWriteStream.WriteL( KExportMetadataSpace );
			textRow.Free();
			}
		if ( freeTextPreamble )
			{
			iWriteStream.WriteL( KExportMetadataNewLine );
			}

		iWriteStream.WriteL( KExportMetadataNewLine );
		}

    CleanupStack::PopAndDestroy( 5, &confidentialFlagRow ); // queryResult, textRow, freeTextQuery, query, confidentialFlagRow
	}


void CMdsImportExport::ExportMetadataMakeSqlRelationClauseL( const CMdsNamespaceDef& aNamespaceDef,
									  		  				CMdsClauseBuffer& aClause, RRowData& aDataRow )
	{
	_LIT( KExportMetadataRelationQuery, "SELECT LO.URI,RO.URI,Parameter FROM Relations%u AS R,Object%u AS LO ON LeftObjectId=LO.ObjectId JOIN Object%u AS RO ON RightObjectId=RO.ObjectId WHERE RelationDefId=? AND NOT R.Flags&? AND NOT R.Flags&?;" );

	aClause.BufferL().Zero(); // reset clause
	aClause.ReserveSpaceL( KExportMetadataRelationQuery.iTypeLength + 30 );
	aClause.BufferL().Format( KExportMetadataRelationQuery, aNamespaceDef.GetId(), aNamespaceDef.GetId(), aNamespaceDef.GetId() );

	aDataRow.Free(); // free and reset dataRow
	aDataRow.Reset();
	aDataRow.AppendL( TColumn( TDefId(0) ) ); // relationDefId
	aDataRow.AppendL( TColumn( EMdERelationFlagDeleted ) );
	aDataRow.AppendL( TColumn( EMdERelationFlagNotPresent ) );
	}

void CMdsImportExport::ExportMetadataWriteRelationInfoL( const CMdsNamespaceDef& aNamespaceDef, const CMdsRelationDef& aRelationDef,
										  				CMdsClauseBuffer& aClause, RRowData& aDataRow )
	{
	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();

	RRowData resultRow;
	CleanupClosePushL( resultRow );
	resultRow.AppendL( TColumn( EColumnDes16 ) ); // LeftObjectIds URI
	resultRow.AppendL( TColumn( EColumnDes16 ) ); // RightObjectIds URI
	resultRow.AppendL( TColumn( TInt32(0) ) );  // RelationParameter

	RMdsStatement query;
	CleanupClosePushL( query );
	RRowData resultRowGet;
	CleanupClosePushL( resultRowGet );
	resultRowGet.AppendColumnTypesL( resultRow );

	aDataRow.Column( 0 ).Set( aRelationDef.GetId() );
	connection.ExecuteQueryL( aClause.ConstBufferL(), query, aDataRow );
	while( connection.NextRowL( query, resultRowGet ) )
		{
		iWriteStream.WriteL( KMdsKeywordRelation );
		iWriteStream.WriteL( KExportMetadataSpace );
		Conv16To8( aNamespaceDef.GetName(), iLine );
		iWriteStream.WriteL( iLine );
		iWriteStream.WriteL( KExportMetadataSpace );
		Conv16To8( aRelationDef.GetName(), iLine );
		iWriteStream.WriteL( iLine );
		iWriteStream.WriteL( KExportMetadataSpace );

		TPtrC16 uri;
		resultRowGet.Column( 0 ).Get( uri );
		Conv16To8( uri, iLine );
		iLine.Insert( 0, KExportMetadataQuotationMark );
		iLine.Append( KExportMetadataQuotationMark );
		iWriteStream.WriteL( iLine );
		iWriteStream.WriteL( KExportMetadataSpace );

		resultRowGet.Column( 1 ).Get( uri );
		Conv16To8( uri, iLine );
		iLine.Insert( 0, KExportMetadataQuotationMark );
		iLine.Append( KExportMetadataQuotationMark );
		iWriteStream.WriteL( iLine );
		iWriteStream.WriteL( KExportMetadataSpace );

		TInt32 relationParameter;
		resultRowGet.Column( 2 ).Get( relationParameter );
		iLine.Num( relationParameter );
		iWriteStream.WriteL( iLine );
		iWriteStream.WriteL( KExportMetadataNewLine );
		
		resultRowGet.AppendColumnTypesL( resultRow );
		}

	CleanupStack::PopAndDestroy( 3, &resultRow ); // query, resultRowGet, resultRow
	}


void CMdsImportExport::ExportMetadataMakeSqlEventClauseL( const CMdsNamespaceDef& aNamespaceDef,
									  	   				 CMdsClauseBuffer& aClause, RRowData& aDataRow )
	{
	_LIT( KExportMetadataEventQuery, "SELECT URI,Source,Participant,Timestamp FROM Event%u AS EL,Object%u AS O ON EL.ObjectId=O.ObjectId WHERE EventDefId=?;" );

	aClause.BufferL().Zero(); // reset clause
	aClause.ReserveSpaceL( KExportMetadataEventQuery.iTypeLength + 20 );
	aClause.BufferL().Format( KExportMetadataEventQuery, aNamespaceDef.GetId(), aNamespaceDef.GetId() );

	aDataRow.Free(); // free and reset dataRow
	aDataRow.Reset();
	aDataRow.AppendL( TColumn( TDefId(0) ) ); // eventDefId
	}

void CMdsImportExport::ExportMetadataWriteEventInfoL( const CMdsNamespaceDef& aNamespaceDef, const CMdsEventDef& aEventDef,
													  CMdsClauseBuffer& aClause, RRowData& aDataRow )
	{
	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();

	RRowData resultRow;
	CleanupClosePushL( resultRow );
	resultRow.AppendL( TColumn( EColumnDes16 ) ); // target
	resultRow.AppendL( TColumn( EColumnDes16 ) ); // source
	resultRow.AppendL( TColumn( EColumnDes16 ) ); // participant
	resultRow.AppendL( TColumn( TTime(0) ) );     // time

	RMdsStatement query;
	CleanupClosePushL( query );

	RRowData resultRowGet;
	CleanupClosePushL( resultRowGet );
	resultRowGet.AppendColumnTypesL( resultRow );

	aDataRow.Column( 0 ).Set( aEventDef.GetId() );
	connection.ExecuteQueryL( aClause.ConstBufferL(), query, aDataRow );
	while( connection.NextRowL( query, resultRowGet ) )
		{
		iWriteStream.WriteL( KMdsKeywordEvent );
		iWriteStream.WriteL( KExportMetadataSpace );
		Conv16To8( aNamespaceDef.GetName(), iLine );
		iWriteStream.WriteL( iLine );
		iWriteStream.WriteL( KExportMetadataSpace );
		Conv16To8( aEventDef.GetName(), iLine );
		iWriteStream.WriteL( iLine );
		iWriteStream.WriteL( KExportMetadataSpace );

		TPtrC16 word;
		// target
		resultRowGet.Column( 0 ).Get( word );
		Conv16To8( word, iLine );
		iLine.Insert( 0, KExportMetadataQuotationMark );
		iLine.Append( KExportMetadataQuotationMark );
		iWriteStream.WriteL( iLine );
		iWriteStream.WriteL( KExportMetadataSpace );

		// source
		resultRowGet.Column( 1 ).Get( word );
		Conv16To8( word, iLine );
		iLine.Insert( 0, KExportMetadataQuotationMark );
		iLine.Append( KExportMetadataQuotationMark );
		iWriteStream.WriteL( iLine );
		iWriteStream.WriteL( KExportMetadataSpace );

		// participant
		resultRowGet.Column( 2 ).Get( word );
		Conv16To8( word, iLine );
		iLine.Insert( 0, KExportMetadataQuotationMark );
		iLine.Append( KExportMetadataQuotationMark );
		iWriteStream.WriteL( iLine );
		iWriteStream.WriteL( KExportMetadataSpace );

		TTime value;
		resultRowGet.Column( 3 ).Get( value );
		TDateTime time = value.DateTime();
		iLine.Format( KExportMetadataTimeFormat, time.Year(), time.Month()+1, time.Day()+1, time.Hour(), time.Minute(), time.Second() );
		iWriteStream.WriteL( iLine );
		iWriteStream.WriteL( KExportMetadataNewLine );
		
		resultRowGet.AppendColumnTypesL( resultRow );
		}

	CleanupStack::PopAndDestroy( 3, &resultRow ); // query, resultRowGet, resultRow
	}


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//                         IMPORT HELPER FUNCTIONS
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// ImportText imports text which may be quoted and escaped
// ------------------------------------------------
//
TInt CMdsImportExport::ImportText( TDes16& aBuffer, TLex8& aParser )
    {
    TPtrC8 token;
    token.Set( aParser.NextToken() );

	if ( token.Length() == 0 || token.Length() >= aBuffer.MaxLength() )
        {
        return KErrCorrupt;
        }

    /**
    *	The string may be enclosed in quotes. Unfortunately the parser always stops at 
    *	whitespace, so we must loop with the NextToken() until we find a token that ends with
    * 	an unescaped quotation mark. The bit below will then mark everything inside the quotes 
    *	as a single uninterrupted token.
    */
    if ( token[0] == '\"' )
    	{
    	TInt currentTokenLength(token.Length());
    	TInt start = aParser.Offset() - currentTokenLength;
    	TInt end = aParser.Offset();
    	TBool forceContinue( EFalse );
    	while( ETrue )
    		{
			// if there is a closing quote in the token see if is escaped
			if ( currentTokenLength > 1 )
				{
				// First skip any escaped dollar signs in the string
				// to avoid a false match in case the string is something
				// like "foo$$"
				TInt loc = token.Find(_L8("$$"));
				while( loc != KErrNotFound )
					{
					token.Set(token.Mid(loc+2));
					loc = token.Find(_L8("$$"));
					}
				// Now the token contains only non-escaped dollars
				currentTokenLength = token.Length();
				if ( currentTokenLength > 2 && token.Right(2) == _L8("$\"") )
					{
					forceContinue = ETrue;
					}
				}
				
			if(token[currentTokenLength - 1] == '\"' && !forceContinue)
				{
				// The token contains a closing quote which is not escaped, 
				// leave loop because the string is finished
				break;
				}
    		forceContinue = EFalse;
    		token.Set(aParser.NextToken());
    		currentTokenLength = token.Length();
    		end = aParser.Offset();
    		if ( currentTokenLength == 0 || currentTokenLength >= aBuffer.MaxLength() - (end-start) )
			    {
			    return KErrCorrupt;
			    }
	  		}
	  	// We have the complete token length now, set the TPtrC accordingly
    	token.Set(iLine.Mid(start + 1, (end-start) - 2)); // skip the quotes
    	TBuf8<256> fp(token);
    	}
    // The token now contains the full string
   	Conv8To16( token, aBuffer );

   	return KErrNone;
    }
    
// ------------------------------------------------
// ImportUInt32
// ------------------------------------------------
//
TInt CMdsImportExport::ImportUInt32( TUint32& aValue, TLex8& aParser )
    {
    TLex8 tokenParser( aParser.NextToken() );
    aParser.SkipSpace();
    if ( tokenParser.Val( aValue, EDecimal ) != KErrNone )
        {
        _LIT( KError, "Expecting a numeric value" );
        LogError( KError );
        return KErrCorrupt;
        }
    return KErrNone;
    }

// ------------------------------------------------
// ImportMediaId
// ------------------------------------------------
//
TInt CMdsImportExport::ImportMediaId( TUint32& aValue, TLex8& aParser, TChar& aDriveLetter )
    {
    TLex8 tokenParser( aParser.NextToken() );
    aParser.SkipSpace();
    if ( tokenParser.Val( aValue, EDecimal ) != KErrNone )
        {
        _LIT( KError, "Expecting a numeric value" );
        LogError( KError );
        return KErrCorrupt;
        }

    TInt error( KErrNone );
    TInt driveNumber( -1 );
    error = iFs.CharToDrive( aDriveLetter, driveNumber );

    if ( error != KErrNone )
        {
        return error;
        }
        
    if( driveNumber != iLastDriveNumber )
        {
        error = iFs.Volume( iLastVolumeInfo, driveNumber );
            
        if ( error != KErrNone )
            {
            return error;
            }     
            
       iLastDriveNumber = driveNumber;
       }
        
    aValue = iLastVolumeInfo.iUniqueID;
        
    return KErrNone;
    }

// ------------------------------------------------
// ImportInt64
// ------------------------------------------------
//
TInt CMdsImportExport::ImportInt64( Int64& aValue, TLex8& aParser )
    {
    TInt error( KErrNone );
    // due to symbian int64 parser error
    // for now we will use ImportNum version
    error = ImportNum( aValue, aParser );

    return error;
    }

// ------------------------------------------------
// ImportTime
// ------------------------------------------------
//
TInt CMdsImportExport::ImportTime( TTime& aValue, TLex8& aParser )
    {
    // format: YYYYMMDDhhmmss
    TPtrC8 token( aParser.NextToken() );
    if ( token.Length() != 14 )
    	{
    	_LIT( KError, "Expecting a time value" );
        LogError( KError );
    	return KErrCorrupt;
    	}
    	
    TLex8 year_p( token.Mid( 0, 4 ) );
    TLex8 month_p( token.Mid( 4, 2 ) );
    TLex8 day_p( token.Mid( 6, 2 ) );
    TLex8 hour_p( token.Mid( 8, 2 ) );
    TLex8 minute_p( token.Mid( 10, 2 ) );
    TLex8 second_p( token.Mid( 12, 2 ) );
    TInt year;
    TInt month;
    TInt day;
    TInt hour;
    TInt minute;
    TInt second;

    year_p.Val( year );
    month_p.Val( month );
    day_p.Val( day );
    hour_p.Val( hour );
    minute_p.Val( minute );
    second_p.Val( second );

    TDateTime datetime;
    const TInt error = datetime.Set( year, (TMonth)(month-1), day-1, hour,
    							minute, second, 0 );
    if ( error != KErrNone )
    	{
    	return error;
    	}

    aValue = datetime;
    return KErrNone;
    }

// ------------------------------------------------
// Conv8To16
// ------------------------------------------------
//
TDesC16& CMdsImportExport::Conv8To16( const TDesC8& aUtf8, TDes16& aBuffer )
    {
    TInt conversionState = CCnvCharacterSetConverter::KStateDefault;
    iConverter->ConvertToUnicode( aBuffer, aUtf8, conversionState );
    return aBuffer;
    }

// ------------------------------------------------
// Conv16To8
// ------------------------------------------------
//
TDesC8& CMdsImportExport::Conv16To8( const TDesC16& aUnicode, TDes8& aBuffer )
    {
    iConverter->ConvertFromUnicode( aBuffer, aUnicode );
    return aBuffer;
    }

// ------------------------------------------------
// LogError
// ------------------------------------------------
//
#ifdef _DEBUG
void CMdsImportExport::LogError( const TDesC& aMessage )
    {
    if( iLogEnabled )
        {
        _LIT( KParseError, "Parse error: %S" );
        _LIT( KCurrentFile, "Current file: %S" );
        _LIT( KCurrentLineNum, "Current line number: %d" );
        _LIT( KCurrentLine, "Current line: %S" );
        iLog.WriteFormat( KParseError, &aMessage );
        iLog.WriteFormat( KCurrentFile, &iFileName );
        iLog.WriteFormat( KCurrentLineNum, iLineNumber );
        TBuf16<KMdsMaxLogLineLenght> line16;
        Conv8To16( iLine, line16 );
        iLog.WriteFormat( KCurrentLine, &line16 );
        }
    }
#else
void CMdsImportExport::LogError( const TDesC& /*aMessage*/ )
    {
    }
#endif


// ------------------------------------------------
// CheckForConstant
// ------------------------------------------------
//
TInt CMdsImportExport::CheckForConstant( TLex8& aParser )
	{
	TInt ret = KErrNone;
	TBuf<32> buf;
	aParser.Mark();
	ret = ImportText( buf, aParser );
	if ( ret < KErrNone )
		{
		aParser.UnGetToMark();
		return ret;
		}
	if ( buf.CompareF(KMdsKeywordMinValue) == 0 )
		{
		return KPropertyMinValue;
		}
	else if ( buf.CompareF(KMdsKeywordMaxValue) == 0 )
		{
		return KPropertyMaxValue;
		}
	else if ( buf.CompareF(KMdsKeywordDefValue) == 0 )
		{
		return KPropertyDefValue;
		}
	aParser.UnGetToMark();
	return KErrNotFound;
	}

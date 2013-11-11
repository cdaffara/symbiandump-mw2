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
* Description:  Metadata import/export metadata and schema files
*
*/

#ifndef __MDSIMPORTEXPORT_H__
#define __MDSIMPORTEXPORT_H__

#include <e32base.h>
#include <s32file.h>
#include <flogger.h>

#include "mdccommon.h"


class CMdsNamespaceDef;
class CMdsObjectDef;
class CMdsRelationDef;
class CMdsEventDef;
class CCnvCharacterSetConverter;
class CMdsClauseBuffer;
class CMdsSchema;
class CMdSSqlObjectManipulate;
class CMdCSerializationBuffer;
class CDesC16ArrayFlat;
class RRowData;

// declarations
const TInt KMdsMaxLineLenght = 1024;
const TInt KPropertyMinValue = 1;
const TInt KPropertyMaxValue = 2;
const TInt KPropertyDefValue = 3;

/**
 *  Metadata schema container within server
 *
 *  This class is responsible for read MDE schema file and store all
 *  information into one memory place.
 *
 */
class CMdsImportExport : public CBase
    {
//    friend class CMdsSchema;
public:
	/**
	 * Create new SchemaImport class
	 * @return new created class
	 */
    static CMdsImportExport* NewL();

	/**
	 * Create new SchemaImport class
	 * @return new created class
	 */
    static CMdsImportExport* NewLC();

	/**
	 * Destructor
	 */
    virtual ~CMdsImportExport();

    /**
     * Reads and process schema file to classess hierarchy.
     *
     * @param aFileName filename to read from
     * @param aSchema schema where to read file ontology
     */
    void ImportSchemaFromFileL( const TDesC& aFileName, CMdsSchema& aSchema, TUint32 aVendorId );

    /**
     * Reads schema from DB
     *
     * @param aConnection connection to DB
     * @param aSchema returning schema
     */
    void ImportSchemaFromDBL( CMdsSchema& aSchema );

	/**
	 * Checks for schema version information
	 * @return ETrue if match, otherwise EFalse
	 */
	TBool ImportCheckVersionInfoL();

    /**
     * Import items from file and add them do DB
     *
     * @return failed count
     */
    TInt ImportMetadataL( CMdSSqlObjectManipulate& aManipulate, CMdsSchema& aSchemaNew, const TDesC16& aFileName );

	/**
	 * Export all (without confidential) items to file
	 */    
    void ExportMetadataL( CMdsSchema& aSchemaNew, const TDesC16& aFileName,
						  CMdCSerializationBuffer& aItems );

protected:


	/*
	    IMPORT SCHEMA FILE
	 */

    /**
     * Imports a single schema file line
     * @param aParser a TLex8 object that parses a line of stream
     */
    void ImportSchemaLineL( TLex8& aParser );

    /**
     * imports a namespace
     * @param aParser the remaining parameters
     */
    void ImportSchemaNamespaceL( TLex8& aParser );

    /**
     * imports an object def
     * @param aParser the remaining parameters
     */
    void ImportSchemaObjectDefL( TLex8& aParser );

	/**
	* imports a property definition
	* @param aParser the remaining parameters
	*/
	void ImportSchemaPropertyDefL( TLex8& aParser );

	/**
	* imports a relation definition
	* @param aParser the remaining parameters
	*/
	void ImportSchemaRelationDefL( TLex8& aParser );

	/**
	* imports a event definition
	* @param aParser the remaining parameters
	*/
	void ImportSchemaEventDefL( TLex8& aParser );

	/**
	* imports a version number
	* @param aParser the remaining parameters
	*/
	void ImportSchemaVersionL( TLex8& aParser );
	
	/**
	 * Checks that no more parameters are given in the current schema line.
	 * @param aParser the remaining parameters
	 */
	void CheckNoMoreNumericParametersL( TLex8& aParser );

	/*
	    IMPORT METADATA FILE
	 */
    /**
     * Reads a single metadata file line
     * @return if reading is still possible (not end of file)
     */
    TBool ReadMetadataFileLineL();
	
	/**
	 * add object to DB
	 */
	void AddObjectToDBL( CMdSSqlObjectManipulate& aManipulate, CMdsNamespaceDef* aNamespaceDef );

    /**
     * Imports a single metadata item
     * @param aParser a TLex8 object that parses a line of stream
     */
    void ImportMetadataItemL( TLex8& aParser, CMdSSqlObjectManipulate& aManipulate );

	/**
	 * imports an metadata file object
	 * @param aParser the remaining parameters
	 */
	void ImportMetadataFileObjectL( TLex8& aParser, CMdSSqlObjectManipulate& aManipulate );

	/**
	 * imports an metadata file property
	 * @param aParser the remaining parameters
	 */
	TMdCOffset ImportMetadataFilePropertyL( TLex8& aParser, TMdCOffset aFreespaceOffset );

	/**
	 * imports an metadata file freetext
	 * @param aParser the remaining parameters
	 */
	void ImportMetadataFileFreeTextL( TLex8& aParser, CDesC16ArrayFlat& aFreeTextArray );

	/**
	 * imports an metadata file relation
	 * @param aParser the remaining parameters
	 */
	void ImportMetadataFileRelationL( TLex8& aParser, CMdSSqlObjectManipulate& aManipulate );

	/**
	 * imports an metadata file event
	 * @param aParser the remaining parameters
	 */
	void ImportMetadataFileEventL( TLex8& aParser, CMdSSqlObjectManipulate& aManipulate );


	/*
	    EXPORT MATADATA FILE
	 */
	// objects
	void ExportMetadataMakeSqlObjectClauseL( const CMdsNamespaceDef& aNamespaceDef, const CMdsObjectDef& aObjectDef,
									  		CMdsClauseBuffer& aClause, RRowData& aDataRow );

	void ExportMetadataMakeFreeTextSqlClauseL( const CMdsNamespaceDef& aNamespaceDef,
									  		  CMdsClauseBuffer& aFreeTextClause, RRowData& aFreeTextRow );

	void ExportMetadataWriteObjectInfoL( const CMdsNamespaceDef& aNamespaceDef, const CMdsObjectDef& aObjectDef,
										CMdsClauseBuffer& aClause, RRowData& aDataRow,
										CMdsClauseBuffer& aFreeTextClause, RRowData& aFreeTextRow );

	// relations
	void ExportMetadataMakeSqlRelationClauseL( const CMdsNamespaceDef& aNamespaceDef,
									  		  CMdsClauseBuffer& aClause, RRowData& aDataRow );

	void ExportMetadataWriteRelationInfoL( const CMdsNamespaceDef& aNamespaceDef, const CMdsRelationDef& aRelationDef,
										  CMdsClauseBuffer& aClause, RRowData& aDataRow );

	// events
	void ExportMetadataMakeSqlEventClauseL( const CMdsNamespaceDef& aNamespaceDef,
									  	   CMdsClauseBuffer& aClause, RRowData& aDataRow );

	void ExportMetadataWriteEventInfoL( const CMdsNamespaceDef& aNamespaceDef, const CMdsEventDef& aEventDef,
										CMdsClauseBuffer& aClause, RRowData& aDataRow );


    //
    // Helpers
    //

	/**
	* Parses a safe TUint32 from the lexical parser.
	* The type is assumed to be decimal.
	* @param aValue parsed TUint32 value
	* @param aParser the remaining patameters
	* @return system wide error code
	*/
	TInt ImportUInt32( TUint32& aValue, TLex8& aParser );
	
	/**
	 * Parses mediaID from the lexical parser.
	 * The type is assumed to be decimal.
	 * @param aValue parsed TUint32 value
	 * @param aParser the remaining patameters
	 * @param aDriveLetter drive letter of the drive where the file under processing is located
	 * @return system wide error code
	*/
	TInt ImportMediaId( TUint32& aValue, TLex8& aParser, TChar& aDriveLetter );

	/**
	* Parses a safe TInt64 from the lexical parser.
	* The type is assumed to be decimal.
	* @param aValue parsed TInt64 value
	* @param aParser the remaining patameters
	* @return system wide error code
	*/
	TInt ImportInt64( TInt64& aValue, TLex8& aParser );

	/**
	* Parses a safe TTime from the lexical parser.
	* implemented using GetNumericL template method.
	* @param aValue parsed TTime value
	* @param aParser the remaining patameters
	* @return system wide error code
	*/
	TInt ImportTime( TTime& aValue, TLex8& aParser );

	/**
	* Parses a safe descriptor from the lexical parser.
	* @param aValue parsed descriptor
	* @param aParser the remaining patameters
	* @return system wide error code
	*/
	TInt ImportText( TDes16& aBuffer, TLex8& aParser );

	/**
	* Parses a safe number type from given lexical parser.
	* The number type is given in a template parameter.
	* @param aValue parsed value of number type
	* @param aParser the remaining patameters
	* @return system wide error code
	*/
	template<class NumberType>
	TInt ImportNum( NumberType& aValue, TLex8& aParser )
		{
		TLex8 tokenParser( aParser.NextToken() );
		aParser.SkipSpace();
		if ( tokenParser.Val( aValue ) != KErrNone )
			{
			return KErrCorrupt;
			}
		return KErrNone;
		}
	
	TInt ImportNum( TReal32& aValue, TLex8& aParser )
		{
		TLex8 tokenParser( aParser.NextToken() );
		aParser.SkipSpace();
		if ( tokenParser.Val( aValue,TChar(TUint('.')) ) != KErrNone )
			{
			return KErrCorrupt;
			}
		return KErrNone;
		}
	
	TInt ImportNum( TReal64& aValue, TLex8& aParser )
		{
		TLex8 tokenParser( aParser.NextToken() );
		aParser.SkipSpace();
		if ( tokenParser.Val( aValue,TChar(TUint('.')) ) != KErrNone )
			{
			return KErrCorrupt;
			}
		return KErrNone;
		}
	/**
	* Converts from UTF8 -> Unicode
	* @param aUtf8 input descriptor
	* @param aBuffer result buffer
	* @return descriptor of TDesC16 type
	*/
	TDesC16& Conv8To16( const TDesC8& aUtf8, TDes16& aBuffer );

	/**
	* Converts from Unicode -> UTF8
	* @param aUnicode input descriptor
	* @param aBuffer result buffer
	* @return descriptor of TDesC8 type
	*/
	TDesC8& Conv16To8( const TDesC16& aUnicode, TDes8& aBuffer );

	
	/**	PROPERTY READING HELPER FUNCTIONS */

	/**
	 * Parses Uint32 value or check for one of min, max or def
	 * constant value
	 * @return depending on result: 
	 *            1 for min
	 *            2 for max
	 *            3 for default
	 *       -value for errors
	 */
	TInt CheckForConstant( TLex8& aParser );

	/**
	 * Parses Uint32 value or check for one of min, max or def
	 * constant value
	 */
	TInt ImportPropertyUInt32( TUint32& aValue, TLex8& aParser );

	/**
	 * Parses a safe TInt64 from the lexical parser.
	 * The type is assumed to be decimal.
	 * @param aValue parsed TInt64 value
	 * @param aParser parser
	 */
	TInt ImportPropertyInt64( TInt64& aValue, TLex8& aParser );

	/**
	* Parses a safe TTime from the lexical parser.
	* implemented using GetNumericL template method.
	* @param aValue parsed TTime value
	* @param aParser the remaining patameters
	* @return system wide error code
	*/
	TInt ImportPropertyTime( TTime& aValue, TLex8& aParser );

	/**
	* Parses a safe number type from given lexical parser.
	* The number type is given in a template parameter.
	* @param aValue parsed value of number type
	* @param aParser the remaining patameters
	* @return system wide error code
	*/
	template<class NumberType>
	TInt SetMinMaxDefValueL( TInt aError, NumberType& aValue,
			const NumberType& aMinValue, const NumberType& aMaxValue,
			const NumberType& aDefValue )
		{
		if ( aError < KErrNone )
			{
			return aError;
			}
		if ( aError == KPropertyMinValue)
			{
			aValue = aMinValue;
			return KErrNone;
			}
		else if ( aError == KPropertyMaxValue )
			{
			aValue = aMaxValue;
			return KErrNone;
			}
		else if ( aError == KPropertyDefValue )
			{
			aValue = aDefValue;
			return KErrNone;
			}
		return KErrArgument;
		}
private:

    CMdsImportExport();

	void ImportNamespaceFromDBL();
	/*
	 * 2nd phase constructor
	 */
    void ConstructL();
    
    /**
     * Data import error logger
     * @param aMessage a message to be logged
     */
    void LogError( const TDesC& aMessage );
    
private:
    /**
     * Store every namespace read from schema file.
     */
	CMdsSchema* iSchema;

	/**
	 * default schema
	 * not own - do not delete
	 */
	CMdsSchema* iDefaultSchema;

    /**
     * Last read objectdef, where new properties should go.
     * Just for speed access to property add access time.
     * !! DO NOT DELETE !! - NOT OWN
     */
	CMdsObjectDef* iLastObjectDef;

    /**
     * Unicode<->UTF8 converter
     */
    CCnvCharacterSetConverter* iConverter;

    /** logs import errors to file */
    RFileLogger iLog;

	/**
	 *	File stream used for reading the import file
	 */
	RFileReadStream iReadStream;

	/**
	 *	File stream used for reading the import file
	 */
	RFileWriteStream iWriteStream;


	/**
	 * Handle to actual processing file
	 */
    RFs iFs;

	/**
	 * current parsing filename
	 */
	TFileName iFileName;

	/**
	 * current verdor id
	 */
	TUint32 iVendorId;

	/**
	 * current parsing line buffer
	 */
	TBuf8<KMdsMaxLineLenght> iLine;

	/**
	 * buffer for reading metadata file
	 */
	CMdCSerializationBuffer* iBuffer;

	/**
	 * indicated if last line was processed
	 */
	TBool iLastLineProcessed;

    /**
     * current parsing line number
     */
    TInt iLineNumber;

	/**
	 * import failed count
	 */
    TUint32 iFailed;

	enum TImportVersionFlags
		{
		EVersionNone = 0x0000,
		EVersionAlreadyRead = 0x0001,
		};
    TUint32 iVersionFlags;
    
    TInt iLastDriveNumber;
    
    TVolumeInfo iLastVolumeInfo;
    
    TBool iLogEnabled;
    };


#endif // __MDSIMPORTEXPORT_H__

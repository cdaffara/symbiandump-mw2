/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  SQL database startup routines*
*/

// INCLUDE FILES
#include "mdssqldbmaintenance.h"
#include "mdccommon.h"
#include "mdspreferences.h"

// ========================= MEMBER FUNCTIONS ==================================

CMdSSqlDbMaintenance* CMdSSqlDbMaintenance::NewL()
    {
    CMdSSqlDbMaintenance* self = CMdSSqlDbMaintenance::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

CMdSSqlDbMaintenance* CMdSSqlDbMaintenance::NewLC()
    {
    CMdSSqlDbMaintenance* self = new ( ELeave ) CMdSSqlDbMaintenance();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

void CMdSSqlDbMaintenance::ConstructL( )
    {
    }

CMdSSqlDbMaintenance::CMdSSqlDbMaintenance()
    {
    }

CMdSSqlDbMaintenance::~CMdSSqlDbMaintenance()
    {
    }

TBool CMdSSqlDbMaintenance::ValidateL(  )
    {
    //validate content of critical tables
    _LIT( KValidateTableExistence, "SELECT COUNT(*) FROM MdE_Preferences;" );
    _LIT( KValidateObjectDef, "SELECT COUNT(*) FROM ObjectDef;" );
    _LIT( KValidatePropertyDef, "SELECT COUNT(*) FROM PropertyDef;" );
    _LIT( KValidateRelationDef, "SELECT COUNT(*) FROM RelationDef;" );
    _LIT( KValidateCol2Prop, "SELECT COUNT(*) FROM Col2Prop;" );

    TInt test(KErrNone);
    TUint32 count(0);
    
    RRowData emptyRowData;
    CleanupClosePushL( emptyRowData );
        
    RMdsStatement validationQuery;
    CleanupClosePushL( validationQuery );
    
    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
    
	TRAP( test, connection.ExecuteQueryL( KValidateTableExistence, validationQuery, emptyRowData ) );
	if(test == KErrNone)
        {
        emptyRowData.AppendL( TColumn( count ) );
        TRAP( test, connection.NextRowL(validationQuery, emptyRowData));
        if(test == KErrNone)
            {
            emptyRowData.Column(0).Get( count );
                    
            if(count <= 0)
                {
                test = KErrCorrupt;
                }
            }
        }
	
	CleanupStack::PopAndDestroy( &validationQuery );
	emptyRowData.Reset();
	
	RMdsStatement objectDefQuery;
	CleanupClosePushL( objectDefQuery );
	
	if( test == KErrNone )
        {
        TRAP( test, connection.ExecuteQueryL( KValidateObjectDef, objectDefQuery, emptyRowData ) );
        
        if(test == KErrNone)
            {
            emptyRowData.AppendL( TColumn( count ) );
            TRAP( test, connection.NextRowL(objectDefQuery, emptyRowData));
            if(test == KErrNone)
                {
                emptyRowData.Column(0).Get( count );
                        
                if(count <= 0)
                    {
                    test = KErrCorrupt;
                    }
                }
            }
        }
    
	CleanupStack::PopAndDestroy( &objectDefQuery );
	emptyRowData.Reset();
	
	RMdsStatement propertyDefQuery;
	CleanupClosePushL( propertyDefQuery );

	if( test == KErrNone )
	    {
        TRAP( test, connection.ExecuteQueryL( KValidatePropertyDef, propertyDefQuery, emptyRowData ) );
        if(test == KErrNone)
            {
            emptyRowData.AppendL( TColumn( count ) );
            TRAP( test, connection.NextRowL(propertyDefQuery, emptyRowData));
            if(test == KErrNone)
                {
                emptyRowData.Column(0).Get( count );
                
                if(count <= 0)
                    {
                    test = KErrCorrupt;
                    }
                }
            }
	    }
	
	CleanupStack::PopAndDestroy( &propertyDefQuery );
	emptyRowData.Reset();
	
    RMdsStatement relationDefQuery;
    CleanupClosePushL( relationDefQuery );
	
	if( test == KErrNone )
	    {
	    TRAP( test, connection.ExecuteQueryL( KValidateRelationDef, relationDefQuery, emptyRowData ) );
	    if(test == KErrNone)
	        {
            emptyRowData.AppendL( TColumn( count ) );
            TRAP( test, connection.NextRowL(relationDefQuery, emptyRowData));
            
            if(test == KErrNone)
                {
                emptyRowData.Column(0).Get( count );
                
                if(count <= 0)
                    {
                    test = KErrCorrupt;
                    }
                }
            }
	    }
	
	CleanupStack::PopAndDestroy( &relationDefQuery );
	emptyRowData.Reset();
    
    RMdsStatement col2propQuery;
    CleanupClosePushL( col2propQuery );
    
    if( test == KErrNone )
        {
        TRAP( test, connection.ExecuteQueryL( KValidateCol2Prop, col2propQuery, emptyRowData ) );
        if(test == KErrNone)
            {
            emptyRowData.AppendL( TColumn( count ) );
            TRAP( test, connection.NextRowL(col2propQuery, emptyRowData));
            
            if(test == KErrNone)
                {
                emptyRowData.Column(0).Get( count );
                
                if(count <= 0)
                    
                    {
                    test = KErrCorrupt;
                    }
                }
            }
          }
    CleanupStack::PopAndDestroy( &col2propQuery );
     
    CleanupStack::PopAndDestroy( &emptyRowData );
    
    if(test == KErrCorrupt )
        {
        User::Leave( test );
        }
    
    return ( test == KErrNone );
    }


void CMdSSqlDbMaintenance::CreateDatabaseL()
    {
    _LIT( KCreateTblMdE_Preferences, // Table for metadata engine use
    	"CREATE TABLE MdE_Preferences(Key TEXT,Value NONE,ExtraValue LARGEINT,UNIQUE(Key,Value));");

    _LIT( KCreateTblMdS_Medias, // Table for metadata engine use
		"CREATE TABLE MdS_Medias(MediaId INTEGER PRIMARY KEY,Drive INTEGER,PresentState INTEGER,Time LARGEINT);");
    
    RRowData emptyRowData;
    CleanupClosePushL( emptyRowData );

    // Create ontology tables
    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();

    connection.ExecuteL( KCreateTblMdE_Preferences, emptyRowData );

    connection.ExecuteL( KCreateTblMdS_Medias, emptyRowData );

    TInt majorVersion = KMdSServMajorVersionNumber;
	TInt64 minorVersion = KMdSServMinorVersionNumber;
    MMdsPreferences::InsertL( KMdsDBVersionName, MMdsPreferences::EPreferenceBothSet,
    						  majorVersion, minorVersion );

	CleanupStack::PopAndDestroy( &emptyRowData );
    }

TBool CMdSSqlDbMaintenance::CheckForCorruptionL()
    {
    // Check the MDS default namespace main table for validity
    // This table should always contain at least 1 default system folder object
    _LIT( KValidateData, "SELECT COUNT(*) FROM Object%u;" );
 
    RBuf commonClauseOne;
    User::LeaveIfError( commonClauseOne.Create( KValidateData.iTypeLength + KMaxUintValueLength ) );
    CleanupClosePushL( commonClauseOne ); 
    commonClauseOne.Format( KValidateData, KDefaultNamespaceDefId );    
    
    RRowData emptyRowData;
    CleanupClosePushL( emptyRowData );
        
    RMdsStatement validationQuery;
    CleanupClosePushL( validationQuery );
    
    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
    
    TInt test( KErrNone );
    TUint32 count( 0 );
    TRAP( test, connection.ExecuteQueryL( commonClauseOne, validationQuery, emptyRowData ) );
    if( test == KErrNone )
        {
        emptyRowData.AppendL( TColumn( count ) );
        TRAP( test, connection.NextRowL(validationQuery, emptyRowData));
        if(test == KErrNone)
            {
            emptyRowData.Column(0).Get( count );
                    
            if(count <= 0)
                {
                test = KErrCorrupt;
                }
            }
        }
    
    CleanupStack::PopAndDestroy( &validationQuery );
     
    CleanupStack::PopAndDestroy( &emptyRowData );
    
    CleanupStack::PopAndDestroy( &commonClauseOne );
    
    return ( test == KErrNone );
    }


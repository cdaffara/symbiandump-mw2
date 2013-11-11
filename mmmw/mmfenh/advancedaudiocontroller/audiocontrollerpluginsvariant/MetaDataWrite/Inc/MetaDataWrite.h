/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Header file for MetaDataWrite
*
*/


#ifndef METADATAWRITE_H
#define METADATAWRITE_H

// INCLUDES
#include <e32base.h>
#include <mdaaudiosampleeditor.h>

// CLASS DECLARATION

/**
*  This class implements CMetaDataWrite
*
*  @lib MetaDataWrite.lib
*  @since 3.0
*/
class CMetaDataWrite : public CBase
{
public:

    /**
    * Two-phased constructor
    * @param aUtility - Reference to the CMdaAudioRecorderUtility
    * @since 3.0
    * @return void
    */
    IMPORT_C static CMetaDataWrite* NewL(CMdaAudioRecorderUtility& aUtility);

    /**
    * Enable delay write, this should be called everytime a new file is opened
    * @since 3.0
    * @return TInt standard system error if any
    */
    IMPORT_C TInt EnableDelayWrite();

    /**
    * Commit meta data
    * @since 3.0
    * @return TInt standard system error if any
    */
    IMPORT_C TInt CommitMetaData();

private:

    /**
    * C++ default constructor
    * @param aUtility - Reference to the CMdaAudioRecorderUtility
    * @since 3.0
    * @return void
    */
    CMetaDataWrite(CMdaAudioRecorderUtility& aUtility);
    
private:

    // reference of CMdaAudioRecorderUtility
    CMdaAudioRecorderUtility& iUtility;

    // destination package for messages
    TMMFMessageDestinationPckg iDestinationPckg;
};


/**
*  This class implements MMetaDataWriteCustomCommandImplementor
*
*  @lib n/a
*  @since 3.0
*/
class MMetaDataWriteCustomCommandImplementor
{
public:

    /**
    * Implements request on MitmdcEnableDelayWrite
    * @since 3.0
    * @return TInt standard system error if any
    */
    virtual TInt MmdwcEnableDelayWrite() = 0;

    /**
    * Implements request on CommitMetaData
    * @since 3.0
    * @return TInt standard system error if any
    */
    virtual TInt MmdwcCommitMetaData() = 0;
};


/**
*  This class implements CMetaDataWriteCustomCommandParser
*
*  @lib MetaDataWrite.lib
*  @since 3.0
*/
class CMetaDataWriteCustomCommandParser : public CMMFCustomCommandParserBase
{
public:

    /**
    * Two-phased constructor
    * @param aImplementor - Reference to the MMetaDataWriteCustomCommandImplementor
    * @since 3.0
    * @return void
    */
    IMPORT_C static CMetaDataWriteCustomCommandParser* NewL(MMetaDataWriteCustomCommandImplementor& aImplementor);

    /**
    * From CMMFCustomCommandParserBase
    * Decode and handle the message from the client
    * @param aMessage - The message to be serviced
    * @since 3.0
    * @return void
    */
    virtual void HandleRequest(TMMFMessage& aMessage);

private:

    /**
    * C++ default constructor
    * @param aImplementor - a reference to the MMetaDataWriteCustomCommandImplementor
    * @since 3.0
    * @return void
    */
    CMetaDataWriteCustomCommandParser(MMetaDataWriteCustomCommandImplementor& aImplementor);

private:

	MMetaDataWriteCustomCommandImplementor& iImplementor;
};

#endif

// End of file

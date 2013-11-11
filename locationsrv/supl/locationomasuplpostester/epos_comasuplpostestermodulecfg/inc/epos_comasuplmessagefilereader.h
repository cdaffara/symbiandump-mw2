/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Reads instructions from message file.
*
*/

#ifndef C_COMASUPLMESSAGEFILEREADER_H
#define C_COMASUPLMESSAGEFILEREADER_H

#include <e32base.h>
#include <f32file.h>

#include "epos_comasuplmessageinfo.h"
class COMASuplPosPayload;
class RFs;
class RFile;
class COMASuplPosTesterLogger;

/** Constant .*/
_LIT8(KMsgBlockDelimiter, "\r\n\r\n");

/** Constant .*/
_LIT8(KTypeDelimiter, "\r\n");

/**
 *  Reads message from a file.
 *
 *  Reads message from a file of specified format and constructs objects
 *  of COMASuplPosPayload.
 *
 *  @lib PosTesterModuleCfg.lib
 *  @since S60 v3.1
 */
class COMASuplMessageFileReader : public CBase
	{
	public:
		static COMASuplMessageFileReader* NewL(TFileName& aFilePath
									, COMASuplPosTesterLogger* aLog);
		virtual ~COMASuplMessageFileReader();
		
		/**
		 * returns the list of objects of class COMASuplPosPayload.
		 * Objects are prepared by the messages read from the message file.
		 *
		 * @since S60 v3.1
		 * @return RPointerArray<COMASuplPosPayload>&: Contains the list
		 * of COMASuplPosPayload objects, prepared by the messages read from
		 * the message file.
		 */
		RPointerArray<COMASuplMessageInfo>& GetMessageListL();
	private:
		COMASuplMessageFileReader(COMASuplPosTesterLogger* aLog);
		void ConstructL(TFileName& aFilePath);
		
		/**
	     * Reads all the data of the file in a descriptor
	     *
	     * @since S60 v3.1
	     * @return TDesC8&: Reference to the descriptor that has file data.
	     */
		TDesC8& FileDataReaderL();

		/**
	     * Reads one message block from the descriptor
	     *
	     * @since S60 S60 v3.1
	     * @param aMessageBlock Descriptor containing one message
	     * block.
	     * @return TBool Returns ETrue if reading a message block is 
	     * successful.
	     */
		TBool ReadOneMessageBlockL(HBufC8*& aMessageBlock);

		/**
	     * Extracts Payload type and message from the message block
	     *
	     * @since S60 S60 v3.1
	     * @param aMessageBlock Reference to a message block.
	     * @param aOpType Instruction given in the message block
	     * @param aPayload Reference to a COMASuplPosPayload
	     * object, which is to be initialized from message block
	     * @return TInt Error code.
	     */
		TInt ParseMessageBlockL(HBufC8& aMessageBlock
						, COMASuplMessageInfo::TOMASuplOperationType& aOpType
						, COMASuplPosPayload& aPayload);
							
		/**
	     * Prepares COMASuplPosPayload object from the data parsed from
	     * the message block.
	     *
	     * @since S60 S60 v3.1
	     * @param aPayloadObj Object whose values 
	     * are to be set.
	     * @param aPayload Payload for aPayloadObj
	     * @param aPayloadType Contains the payload type for aPayloadObj
	     * @return void
	     */
		void PreparePayLoadL(
							COMASuplPosPayload& aPayloadObj
							, HBufC8& aPayload
							, TInt aPayloadType);
		
		/**
	     * Prepares COMASuplMessageInfo from opType, Payload type and 
	     * message from the message block.
	     *
	     * @since S60 S60 v3.1
	     * @param aMessageBlock Reference to a message block.
	     * @param msgInfo Reference to a COMASuplMessageInfo
	     * object, which is to be initialized from message block
	     * @return void.
	     */
		void GetMsgInfoL(HBufC8* aMessageBlock, COMASuplMessageInfo*& msgInfo);

		/**
	     * Identifies opType from the instruction extracted from 
	     * the message block.
	     *
	     * @since S60 S60 v3.1
	     * @param aOpType instruction parsed from the message block.
	     * @return COMASuplMessageInfo::TOMASuplOperationType.
	     */
		COMASuplMessageInfo::TOMASuplOperationType
		GetOperationTypeL(const TDesC8& aOpType);

		/**
	     * Reads Message file name from resource file.
	     *
	     * @since S60 S60 v3.1
	     * @return void.
	     */
		void ReadFileInfoFromResourceL();

		/**
	     * Searches for resource file and gets the Absolute path.
	     *
	     * @since S60 S60 v3.1
	     * @return void.
	     */
		void GetFileAbsolutePathL(TDes& aResourceFileName);

	private:
		/**
     	 * Stores the filename and path of the message file
     	 */
		TFileName iMsgFileName;
		
		/**
     	 * Object of File session.
     	 */
		RFs iRFs;
		
		/**
     	 * Object of RFile.
     	 */
     	 RFile iRFile;
     	 
		/**
     	 * Contains message file data.
     	 */
		HBufC8* iFileData;
		
		/**
     	 * Contains pointer to pos tester logger.
     	 * Not own.
     	 */
		COMASuplPosTesterLogger* iLogger;
	};
#endif
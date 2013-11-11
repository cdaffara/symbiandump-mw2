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
 * Description: AudioEnc Intfc Adaptation for GStreamer
 *
 */

#include <gst/gst.h>
#include "xamediarecorderadaptctx.h"
#include "xaaudioencoderitfadaptation.h"
#include "xaadaptationgst.h"
//#include "XAStaticCapsAdaptation.h"

/*
 * XAresult XAAudioEncoderItfAdapt_SetEncoderSettings( XAAdaptationGstCtx *ctx,
 *                                                     XAAudioEncoderSettings *pSettings )
 * @param XAAdaptationGstCtx *ctx - pointer to Media Recorer adaptation contex
 * @param XAAudioEncoderSettings *pSettings - Settings to be setted for encoder
 * @return XAresult ret - return success value
 * Description: Sets preferred encoder settings for pipeline.
 */
XAresult XAAudioEncoderItfAdapt_SetEncoderSettings(XAAdaptationBaseCtx *ctx,
        XAAudioEncoderSettings *pSettings)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAMediaRecorderAdaptationCtx* mCtx = NULL;

    DEBUG_API("->XAAudioEncoderItfAdapt_SetEncoderSettings");
    if (!ctx || (ctx->ctxId != XAMediaRecorderAdaptation) || !pSettings)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAAudioEncoderItfImpl_SetEncoderSettings");
        return XA_RESULT_PARAMETER_INVALID;
        }
    mCtx = (XAMediaRecorderAdaptationCtx*) ctx;

    ret = XAMediaRecorderAdapt_CheckCodec(mCtx, XACAP_AUDIO,
            pSettings->encoderId);
    if (ret == XA_RESULT_SUCCESS)
        {
        /*change of settings - pipeline must be regenerated**/
        mCtx->encodingchanged = XA_BOOLEAN_TRUE;
        memcpy(&mCtx->audioEncSettings, pSettings,
                sizeof(XAAudioEncoderSettings));
        }
    DEBUG_API("<-XAAudioEncoderItfAdapt_SetEncoderSettings");
    return ret;
    }

static gboolean
populate_field_settings (GQuark field, const GValue * value, gpointer pfx)
{
    gchar *field_name ;
   
    gpointer *pfxd = (gpointer*)pfx;
    XAMediaRecorderAdaptationCtx *ctxx = (XAMediaRecorderAdaptationCtx *) *pfxd;

    field_name = (gchar*)g_quark_to_string (field);
  
  if((strcasecmp((const char*)field_name,"channels") == 0))
      {
          if(GST_VALUE_HOLDS_INT_RANGE(value) == TRUE)
             {
                  (ctxx)->audioEncSettings.channelsIn = gst_value_get_int_range_max (value);
                  (ctxx)->audioEncSettings.channelsOut = gst_value_get_int_range_max (value);
             }else
                 {
                 (ctxx)->audioEncSettings.channelsIn = g_value_get_int(value);
                 (ctxx)->audioEncSettings.channelsOut = g_value_get_int(value);
                 }

      }                
  if((strcasecmp((const char*)field_name,"depth") == 0))
      {
          if(GST_VALUE_HOLDS_INT_RANGE(value) == TRUE)
            {
                (ctxx)->audioEncSettings.bitsPerSample = gst_value_get_int_range_min (value);
            }else
            {
                (ctxx)->audioEncSettings.bitsPerSample = g_value_get_int(value);
            }
      }
  if((strcasecmp((const char*)field_name,"endianness") == 0))
      {
          if(GST_VALUE_HOLDS_INT_RANGE(value) == TRUE)
        {
            (ctxx)->audioEncSettings.blockAlignment = gst_value_get_int_range_min (value);
        }else
        {
            (ctxx)->audioEncSettings.blockAlignment = g_value_get_int(value);
        }
      }
  if((strcasecmp((const char*)field_name,"bitrate") == 0))
      {
      if(GST_VALUE_HOLDS_INT_RANGE(value) == TRUE)
          {
              (ctxx)->audioEncSettings.bitRate = gst_value_get_int_range_min (value);
          }else
          {
              (ctxx)->audioEncSettings.bitRate = g_value_get_int(value);
          }
  
      }
  if((strcasecmp((const char*)field_name,"rate") == 0))
      {
        if(GST_VALUE_HOLDS_INT_RANGE(value) == TRUE)
            {
                (ctxx)->audioEncSettings.sampleRate = gst_value_get_int_range_min (value)*1000;
            }else
                {
                (ctxx)->audioEncSettings.sampleRate = g_value_get_int(value) * 1000;
                }

        }

return TRUE;
}
XAresult XAAudioEncoderItfAdapt_GetEncoderSettings(XAAdaptationBaseCtx *bCtx,
        XAAudioEncoderSettings *pSettings)
    {
    XAMediaRecorderAdaptationCtx* mCtx = NULL;
    XAresult ret = XA_RESULT_SUCCESS;
    
    GstElement *encoderelement=NULL,*actualencoder=NULL;
    GstPad * encoderpad = NULL;
    GstCaps *encodercaps =NULL;
    GstStructure *structure = NULL;
    XAAdaptationGstCtx *ctx = NULL;
    int value = -1 ;

    DEBUG_API("->XAAudioEncoderItfAdapt_GetEncoderSettings");
    if (!bCtx || (bCtx->ctxId != XAMediaRecorderAdaptation) || !pSettings)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        return XA_RESULT_PARAMETER_INVALID;
        }
    mCtx = (XAMediaRecorderAdaptationCtx*) bCtx;
    ctx = (XAAdaptationGstCtx *)bCtx; 
    
    encoderelement = gst_bin_get_by_name((GstBin*)ctx->bin ,"mrcodecbin");
    if (encoderelement != NULL)
    {   
        encoderpad = gst_element_get_pad(encoderelement , "sink");
        if(encoderpad != NULL){
            encodercaps = gst_pad_get_negotiated_caps(encoderpad);
            if(encodercaps == NULL)
                {
                //pipeline is not in playing state , so getting allowed caps
                encodercaps = gst_pad_get_allowed_caps(encoderpad);
                }
        }
        else{
            DEBUG_ERR("No pad named sink in codecbin pad");
        }
        if(encodercaps != NULL && encodercaps->structs->len > 0)
        {    
        structure = gst_caps_get_structure (encodercaps, 0);
        } 
        else{
          DEBUG_ERR("no allowed caps for encoderelement");
        }
        
      //populate the values based on neotiated caps or allowed caps. 
      gst_structure_foreach (structure, populate_field_settings, (gpointer) &mCtx);
      
      //finding the actual encoder.
      if(mCtx->codecbin != NULL){
        actualencoder = gst_bin_get_by_name((GstBin*)mCtx->codecbin ,"audioenc");
      }
      //finding bitrate property
      if(value == -1 && actualencoder != NULL)
            {
            //search in encoder element , property is there or not?
                g_object_get(G_OBJECT (actualencoder), "bitrate", &value,NULL);
             }
      if(mCtx->audioEncSettings.encoderId == XA_AUDIOCODEC_AMR)
          {
          g_object_get(G_OBJECT (mCtx->audiosource), "speechbitrate", &value,NULL);
          }
       if(value != -1)
            {
               mCtx->audioEncSettings.bitRate = value;
               value =-1;
            }
       
       //finding profile property        
        if(value == -1 && actualencoder != NULL)
            {
                g_object_get(G_OBJECT (actualencoder), "profile", &value,NULL);
            }
        if(value != -1)
          {
                mCtx->audioEncSettings.levelSetting = value;
                value =-1;
          }
        
        //finding output-format property
        if(value == -1 && actualencoder != NULL)
          {
                //try to see search in encoder element , property is there or not?
               g_object_get(G_OBJECT (actualencoder), "output-format", &value,NULL);
          }
        if(value != -1)
          {
                mCtx->audioEncSettings.streamFormat = value;
                value =-1;
          }
        
        
        //setting these settings to default.
        mCtx->audioEncSettings.channelMode=0;
        mCtx->audioEncSettings.rateControl = XA_RATECONTROLMODE_CONSTANTBITRATE;
        mCtx->audioEncSettings.encodeOptions = 0;
        
    //unref the caps, pads , elements      
    if(encodercaps != NULL)
    gst_caps_unref(encodercaps);
    if(encoderpad!= NULL)
    gst_object_unref(encoderpad);
    if(actualencoder!= NULL)
    gst_object_unref (actualencoder);
    if(encoderelement!= NULL)
    gst_object_unref (encoderelement);
    }else{
        DEBUG_ERR("Codecbin element Not found ");
    }
    memcpy(pSettings, &(mCtx->audioEncSettings), 
							sizeof(XAAudioEncoderSettings));
    DEBUG_API("<-XAAudioEncoderItfAdapt_GetEncoderSettings");
    return ret;
    }

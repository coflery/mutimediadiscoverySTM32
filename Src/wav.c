#include "wav.h"

/**
* @brief  Reads a number of bytes from the storge and reorder(重排) them in Big
*         or little endian.
* @param  size: number of bytes to read.
*         This parameter must be a number between 1 and 4.
* @param  offset: external memory address to read from.
* @param  Endians: specifies(指定) the bytes endianness.
*         This parameter can be one of the following values:
*             - LittleEndian
*             - BigEndian
* @retval 4-Bytes read from the storge.
*/
uint32_t ReadUnit(uint8_t *buffer, uint8_t offset, uint8_t size, Endianness BytesFormat)
{
  uint32_t index = 0;
  uint32_t temp = 0;

  for (index = 0; index < size; index++)
  {
    temp |= buffer[offset + index] << (index * 8);
  }
  
  if (BytesFormat == BigEndian)
  {
    temp = __REV(temp);
  }
  return temp;
}

/**
  * @brief  Checks the format of the .WAV file and gets information about
  *   the audio format. This is done by reading the value of a
  *   number of parameters stored in the file header and comparing
  *   these to the values expected authenticates the format of a
  *   standard .WAV  file (44 bytes will be read). If  it is a valid
  *   .WAV file format, it continues reading the header to determine
  *   the  audio format such as the sample rate and the sampled data
  *   size. If the audio format is supported by this application,
  *   it retrieves the audio format in WAVE_Format structure and
  *   returns a zero value. Otherwise the function fails and the
  *   return value is nonzero.In this case, the return value specifies
  *   the cause of  the function fails. The error codes that can be
  *   returned by this function are declared in the header file.
  * @param  None
  * @retval Zero value if the function succeed, otherwise it return
  *         a nonzero value which specifies the error code.
  */
WAVE_FormatTypeDef WavePlayer_WaveParsing(uint16_t * waveFileHeader)
{
  uint32_t temp = 0x00;
	WAVE_FormatTypeDef WAVE_Format;
	WAVE_Format.Result = Valid_WAVE_File;//设定默认值为文件有效
  
    /* Read chunkID, must be 'RIFF' */
  temp = ReadUnit((uint8_t*)waveFileHeader, 0, 4, BigEndian);
  if (temp != RIFF_ID)
  {
		WAVE_Format.Result = Unvalid_RIFF_ID;
    return WAVE_Format;
  }printf(" >RIFF_ID:0x%x\r\n",temp);

    /* Read the file length */
  WAVE_Format.RIFFchunksize = ReadUnit((uint8_t*)waveFileHeader, 4, 4, LittleEndian);
  printf(" >RIFF文件包大小：%dkB\r\n",WAVE_Format.RIFFchunksize/1024);

    /* Read the file format, must be 'WAVE' */
  temp = ReadUnit((uint8_t*)waveFileHeader, 8, 4, BigEndian);
  if (temp != WAVE_ID)
  {
		WAVE_Format.Result = Unvalid_WAVE_Format;
    return WAVE_Format;
  }printf(" >WAVE文件格式: 0x%x\r\n",temp);

    /* Read the format chunk, must be'fmt ' */
  temp = ReadUnit((uint8_t*)waveFileHeader, 12, 4, BigEndian);
  if (temp != FORMAT_ID)
  {
		WAVE_Format.Result = Unvalid_FormatChunk_ID;
    return WAVE_Format;
  }printf(" >格式标识块ID: 0x%x\r\n",temp);

    /* Read the length of the 'fmt' data, must be 0x10 */
	WAVE_Format.ExtraTag = ReadUnit((uint8_t*)waveFileHeader, 16, 4, LittleEndian);
	printf(" >fmt 数据: 0x%x\r\n",WAVE_Format.ExtraTag);

    /* Read the audio format, must be 0x01 (PCM) */
  WAVE_Format.FormatTag = ReadUnit((uint8_t*)waveFileHeader, 20, 2, LittleEndian);
	
	  /* Read the number of channels, must be 0x01 (Mono) or 0x02 (Stereo) */
  WAVE_Format.NumChannels = ReadUnit((uint8_t*)waveFileHeader, 22, 2, LittleEndian);
	
	  /* Read the Sample Rate */
  WAVE_Format.SampleRate = ReadUnit((uint8_t*)waveFileHeader, 24, 4, LittleEndian);
  printf(" >采样率：%.1fkHz\r\n",(float)WAVE_Format.SampleRate/1000);
	
	  /* Read the Byte Rate */
  WAVE_Format.ByteRate = ReadUnit((uint8_t*)waveFileHeader, 28, 4, LittleEndian);
  printf(" >比特率：%.1fkbit\r\n",(float)WAVE_Format.ByteRate/1000);  

    /* Read the block alignment */
  WAVE_Format.BlockAlign = ReadUnit((uint8_t*)waveFileHeader, 32, 2, LittleEndian);
  
    /* Read the number of bits per sample */
  WAVE_Format.BitsPerSample = ReadUnit((uint8_t*)waveFileHeader, 34, 2, LittleEndian);
	
  if (WAVE_Format.FormatTag != WAVE_IS_PCM)
  {
    printf(" >不支持格式：可变PCM\r\n");
		WAVE_Format.Result = Unsupporetd_FormatTag;
    return WAVE_Format;
  } printf(" >音频编码格式: 线性PCM\r\n");
	
  if (WAVE_Format.BitsPerSample != BITS_PER_SAMPLE_16 &&
		  WAVE_Format.BitsPerSample != BITS_PER_SAMPLE_8) 
  {
    printf(" >不支持采样深度：%dbit\r\n",WAVE_Format.BitsPerSample);
		WAVE_Format.Result = Unsupporetd_Bits_Per_Sample;
    return WAVE_Format;
	} printf(" >采样深度:%dbit\r\n",WAVE_Format.BitsPerSample);
	
	if(WAVE_Format.NumChannels == CHANNEL_MONO)
  	printf(" >声道：单声道\r\n");
  else
	if(WAVE_Format.NumChannels == CHANNEL_STEREO)
  	printf(" >声道：立体声\r\n");

  WAVE_Format.FileOffset = 36;
  /* If there is Extra format bytes, these bytes will be defined in "Fact Chunk" */
  if (WAVE_Format.ExtraTag != NO_EXTRA_CHNUK)
  {
    /* Read th Extra format bytes, must be 0x00 */
    temp = ReadUnit((uint8_t*)waveFileHeader, 36, 2, LittleEndian);
    if (temp != 0x00)
    {
			WAVE_Format.Result = Unsupporetd_ExtraFormatBytes;
			return WAVE_Format;
    }
    /* Read the Fact chunk, must be 'fact' */
    temp = ReadUnit((uint8_t*)waveFileHeader, 38, 4, BigEndian);
    if (temp != FACT_ID)
    {
			WAVE_Format.Result = Unvalid_FactChunk_ID;
			return WAVE_Format;
    }
    /* Read Fact chunk data Size */
    temp = ReadUnit((uint8_t*)waveFileHeader, 42, 4, LittleEndian);
    
    WAVE_Format.FileOffset += 10 + temp;
  }
	
	 /* Read the Data chunk, must be 'data' */
  temp = ReadUnit((uint8_t*)waveFileHeader, WAVE_Format.FileOffset, 4, BigEndian);
  WAVE_Format.FileOffset += 4;
  if (temp != DATA_START)
  {
		WAVE_Format.Result = Unvalid_DataChunk_ID;
		return WAVE_Format;
  }
  
   /* Read the number of sample data */
  WAVE_Format.DataSize = ReadUnit((uint8_t*)waveFileHeader, WAVE_Format.FileOffset, 4, LittleEndian);
  WAVE_Format.FileOffset += 4;
  return WAVE_Format;
}

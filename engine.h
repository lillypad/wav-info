#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <shlobj.h>   
#include <windows.h>
#include ".\\inc\\mmreg.h"
typedef struct _wavmainheader{
        char            RIFF[4];        // File type is RIFF
        unsigned long   ChunkSize;      // Size of file - 8
        char            WAVE[4];        // WAVE Header      
        char            fmt[4];         // FMT header       
        unsigned long   Subchunk1Size;  // Size of the fmt chunk                                
        unsigned short  AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM 
        unsigned short  NumOfChan;      // Number of channels 1=Mono 2=Sterio                   
        unsigned long   SamplesPerSec;  // Sampling Frequency in Hz                             
        unsigned long   bytesPerSec;    // bytes per second 
        unsigned short  blockAlign;     // 2=16-bit mono, 4=16-bit stereo 
        unsigned short  bitsPerSample;  // Number of bits per sample   
        char            FACT[4];        // Fact Chunk Flag
        unsigned long   FactChunkSize;  // Fact Chunk Size
        unsigned long   FactData;       // Fact chunk data
        char            DATA[4];        // Data Chunk Flag   
        unsigned long   Subchunk2Size;  // Sampled data length
        }WAVMAINHEADER;
        WAVMAINHEADER wavheader;

using namespace std;



//Get File Extension
string getExt(string pathName)
{
	// Finds the last persiod character of the string
	int period = pathName.find_last_of(".");
	// I use  + 1 because I don't really need the to include the period
	string ext = pathName.substr(period + 1);
	return ext;
}

string BrowseFolder(void)
{
    char path[MAX_PATH];
    BROWSEINFO bi = { 0 };
    bi.lpszTitle = ("All WAVS In Root Folder Will be Analized and saved to .txt");
    LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );

    if ( pidl != 0 )
    {
        // get the name of the folder and put it in path
        SHGetPathFromIDList ( pidl, path );

        //Set the current directory to path
        SetCurrentDirectory ( path );
        
        
        // free memory used
        IMalloc * imalloc = 0;
        if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
        {
            imalloc->Free ( pidl );
            imalloc->Release ( );
        }
     
    }
    else return string("CANCEL"); 
    return path;
}//BROWSE FOLDER

//Load File
string load_file(HWND hwnd){
    OPENFILENAME ofn;
    char szFileName[MAX_PATH] = "";

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = "Wav Files (*.wav)\0*.wav\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "txt";
    ofn.lpstrTitle = "Open FIle";
    if(GetOpenFileName(&ofn))
    {
        return string(szFileName);
    }
    else return string("CANCEL");
}

//Save File
string save_file(HWND hwnd)
{
 OPENFILENAME ofn;
 char saveFileName[MAX_PATH] = "";

 ZeroMemory( &ofn, sizeof( ofn ) );

 ofn.lStructSize = sizeof(OPENFILENAME);
 ofn.hwndOwner = hwnd;
 ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
 ofn.lpstrFile = saveFileName;
 ofn.nMaxFile = MAX_PATH;
 ofn.lpstrDefExt = "txt";
 ofn.Flags  = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
 ofn.lpstrTitle = "Save File";
 if (GetSaveFileName(&ofn)){
      return string(saveFileName);
      }
 else return string("CANCEL");
}

int filelength(const char *filename){
    unsigned long result;
    ifstream file(filename);
    file.seekg(0, ios_base::end);
    result = file.tellg();
    file.close();
    return result;
} 

int dataOffset(char * filename){ 
    int pos_seek = 0;
    int dataOffset;
    char DATA[5] = "data";
    int long wavFileSize = filelength(filename);
    while ( pos_seek < wavFileSize ){
          ifstream wavFile(filename, ios::in | ios::binary);
          wavFile.seekg(pos_seek);
          char buffer[5];
          wavFile.read(buffer,4);
          if (strncmp(buffer, DATA, sizeof(buffer)) == 0){
             dataOffset = pos_seek;
             //cout << "Data Offset: " << dataOffset << endl;
             //system("PAUSE > nul");
             break;
          }
          wavFile.close();
          pos_seek++;
    }
return dataOffset;
}

//String to Char
char* string_char(string somestring){
       char * somechar = new char[somestring.size() + 1];
       std::copy(somestring.begin(), somestring.end(), somechar);
       somechar [somestring.size()] = '\0'; 
       return somechar;
}


//Add Backslashes to a string
string pathSlashAdd(string path){
       stringstream ss;

       for (int i = 0; i < path.length(); ++i) {
       if (path[i] == '\\') {
          ss << "\\\\";
          }
       else {
       ss << path[i];
     }
  }
  return ss.str();
}

//Int to String
string int_string(int number)
{
     string result;          // string which will contain the result
     ostringstream convert;   // stream used for the conversion
     convert << number;      // insert the textual representation of 'Number' in the characters in the stream
     result = convert.str();
     return result;
}
//DEBUG SEGMENTATION FAULT
WIN32_FIND_DATA data;
char *SearchDirectory(string filepath, string extension){
    filepath.append(extension);
    HANDLE h = FindFirstFile(string_char(filepath), &data);
	
	if( h!=INVALID_HANDLE_VALUE ) 
	{
		do
		{
			char*   nPtr = new char [lstrlen( data.cFileName ) + 1];
			for( int i = 0; i < lstrlen( data.cFileName ); i++ )
				nPtr[i] = char( data.cFileName[i] );

			nPtr[lstrlen( data.cFileName )] = '\0';
			cout << nPtr << endl;
            return nPtr;
		} while(FindNextFile(h,&data));
	} 
	else 
		cout << "Error: No such folder." << endl;
	
	FindClose(h);
}

//Convert string to LPSTR
LPSTR string_lpstr(string strString, LPSTR lpstrLpstr){
       lpstrLpstr = string_char(lpstrLpstr);
       return lpstrLpstr;
}

LPSTR wav_info(string strfilename, int chunk){
       unsigned long   Subchunk2Size; 
       LPSTR lpstrFail = "FAILED";
       string wavinfo[12];
       //Convert string to char
       char * charfilename = string_char(strfilename);
       //Read The File
       fstream wavFile(charfilename, ios::in | ios::out | ios::binary);
       //Read File Struct
       wavFile.read((char *)&wavheader, sizeof(WAVMAINHEADER));
       //Check if File Exists
       //if (!wavFile){MessageBox(NULL, TEXT("File Doesn't Exit"), TEXT("About"), MB_OK); }
       //string extension = getExt(strfilename);
       if (strncmp(string_char(getExt(strfilename)), "wav", sizeof(string_char(getExt(strfilename)))) != 0){MessageBox(NULL, "File Read Error: Not *.wav file", "ERROR!", MB_ICONSTOP|MB_SETFOREGROUND); return lpstrFail; }
       if (strncmp(wavheader.RIFF, "RIFF", sizeof(wavheader.RIFF)) != 0){MessageBox(NULL, "File Read Error: Not Type RIFF", "ERROR!", MB_ICONSTOP|MB_SETFOREGROUND); return lpstrFail; }
       
       wavinfo[0] = wavheader.RIFF;
       wavinfo[1] = int_string(wavheader.SamplesPerSec);
       wavinfo[2] = int_string(wavheader.ChunkSize);
       wavinfo[3] = int_string(wavheader.Subchunk1Size);
       wavinfo[4] = int_string(wavheader.NumOfChan);
       wavinfo[5] = int_string(wavheader.bytesPerSec);
       wavinfo[6] = int_string(wavheader.bitsPerSample);
       wavinfo[7] = int_string(dataOffset(string_char(strfilename))+8);
       
       int pos_seek = dataOffset(string_char(strfilename)) + 4;
       wavFile.seekg(pos_seek);
       wavFile.read((char *)&Subchunk2Size, sizeof(Subchunk2Size));
       wavinfo[8] = int_string(Subchunk2Size);
       wavinfo[9] = int_string(wavheader.blockAlign);
       wavinfo[10] = int_string(wavheader.AudioFormat);
       wavinfo[11] = int_string((filelength(charfilename)-dataOffset(string_char(strfilename))+8)/wavheader.bytesPerSec);
       LPSTR lpstrWavInfo = string_char(wavinfo[chunk]);
       wavFile.close();
       return lpstrWavInfo;
}

//Get Audio Format
string getAudioFormat(string strfilename){
       unsigned short AudioFormat;
       string strAudioFormat;
       char * charfilename = string_char(strfilename);
       fstream wavFormat(charfilename, ios::in | ios::out | ios::binary);
       wavFormat.read((char *)&wavheader, sizeof(WAVMAINHEADER));
       AudioFormat = wavheader.AudioFormat;
       switch (AudioFormat){
              case WAVE_FORMAT_PCM:
                   strAudioFormat = "PCM by Microsoft Corp.";
                   break;
              case WAVE_FORMAT_UNKNOWN:
                   strAudioFormat = "Unknown by Microsoft Corp.";
                   break;
              case WAVE_FORMAT_ADPCM:
                   strAudioFormat = "ADPCM by Microsoft Corp.";
                   break;
              case WAVE_FORMAT_IEEE_FLOAT:
                   strAudioFormat = "IEEE Float by Microsfot Corp.";
                   break;
              case WAVE_FORMAT_VSELP:
                   strAudioFormat = "VSELP by Compaq Computer Corp.";
                   break;
              case WAVE_FORMAT_IBM_CVSD:
                   strAudioFormat = "IBM CVSD by IBM Corp.";
                   break;
              case WAVE_FORMAT_ALAW:
                   strAudioFormat = "ALAW by Microsoft Corp.";
                   break;
              case WAVE_FORMAT_MULAW:
                   strAudioFormat = "Mulaw by Microsoft Corp.";
                   break;
              case WAVE_FORMAT_DTS:
                   strAudioFormat = "DTS by Microsoft Corp.";
                   break;
              case WAVE_FORMAT_OKI_ADPCM:
                   strAudioFormat = "ADPCM by OKI";
                   break;
              case WAVE_FORMAT_DVI_ADPCM:
                   strAudioFormat = "DVI ADPCM by Intel Corp.";
                   break;
              case WAVE_FORMAT_MEDIASPACE_ADPCM:
                   strAudioFormat = "MediaSpace ADPCM by Videologic";
                   break;
              case WAVE_FORMAT_SIERRA_ADPCM:
                   strAudioFormat = "Sierra ADPCM by Sierra Semiconductor Corp.";
                   break;
              case WAVE_FORMAT_G723_ADPCM:
                   strAudioFormat = "G723 ADPCM by Antex Electronics Corp.";
                   break;
              case WAVE_FORMAT_DIGISTD:
                   strAudioFormat = "DIGISTD by DSP Solutions Inc.";
                   break;
              case WAVE_FORMAT_DIGIFIX:
                   strAudioFormat = "DIGIFIX by DSP Solutions Inc.";
                   break;
              case WAVE_FORMAT_DIALOGIC_OKI_ADPCM:
                   strAudioFormat = "Dialogic OKI ADPCM by Dialogic Corp.";
                   break;
              case WAVE_FORMAT_MEDIAVISION_ADPCM:
                   strAudioFormat = "Media Vision ADPCM by Media Vision Inc.";
                   break;
              case WAVE_FORMAT_CU_CODEC:
                   strAudioFormat = "CU Codec by Hewlett Packard Company";
                   break;
              case WAVE_FORMAT_YAMAHA_ADPCM:
                   strAudioFormat = "Yahama ADPCM by Yahama Corp. of America";
                   break;
              case WAVE_FORMAT_SONARC:
                   strAudioFormat = "Sonarc by Speech Compression";
                   break;
              case WAVE_FORMAT_DSPGROUP_TRUESPEECH:
                   strAudioFormat = "True Speech by DSP Group Inc.";
                   break;
              case WAVE_FORMAT_ECHOSC1:
                   strAudioFormat = "Echosc1 by Eco Speech Corp.";
                   break;
              case WAVE_FORMAT_AUDIOFILE_AF36:
                   strAudioFormat = "AudioFile AF36 by Virtual Music Inc.";
                   break;
              case WAVE_FORMAT_APTX:
                   strAudioFormat = "APTX by Audio Processing Technology";
                   break;
              case WAVE_FORMAT_AUDIOFILE_AF10:
                   strAudioFormat = "AudioFile AF10 by virtual Music Inc.";
                   break;
              case WAVE_FORMAT_PROSODY_1612:
                   strAudioFormat = "Prosody 1612 by Aculab plc";
                   break;
              case WAVE_FORMAT_LRC:
                   strAudioFormat = "LRC by Merging Technologies S.A.";
                   break;
              case WAVE_FORMAT_DOLBY_AC2:
                   strAudioFormat = "AC2 by Dolby Laboratories";
                   break;
              case WAVE_FORMAT_GSM610:
                   strAudioFormat = "GSM610 by Microsoft Corp.";
                   break;
              case WAVE_FORMAT_MSNAUDIO:
                   strAudioFormat = "MSN Audio by Microsoft Corp.";
                   break;
              case WAVE_FORMAT_ANTEX_ADPCME:
                   strAudioFormat = "ADPCME by Antex Electronics Corp.";
                   break;
              case WAVE_FORMAT_CONTROL_RES_VQLPC:
                   strAudioFormat = "VQLPC by Control Resources Limited";
                   break;
              case WAVE_FORMAT_DIGIREAL:
                   strAudioFormat = "DigiReal by DSP Solutions Inc.";
                   break;
              case WAVE_FORMAT_DIGIADPCM:
                   strAudioFormat = "Digi ADPCM by DSP Solutions Inc.";
                   break;
              case WAVE_FORMAT_CONTROL_RES_CR10:
                   strAudioFormat = "CR10 by Control Resources Limited";
                   break;
              case WAVE_FORMAT_NMS_VBXADPCM:
                   strAudioFormat = "VBXADPCM by Natural MicroSystems";
                   break;
              case WAVE_FORMAT_CS_IMAADPCM:
                   strAudioFormat = "IMA ADPCM by Crystal Semiconductor";
                   break;
              case WAVE_FORMAT_ECHOSC3:
                   strAudioFormat = "SC3 by Echo Speech Corp.";
                   break;
              case WAVE_FORMAT_ROCKWELL_ADPCM:
                   strAudioFormat = "ADPCM by Rockwell International";
                   break;
              case WAVE_FORMAT_ROCKWELL_DIGITALK:
                   strAudioFormat = "DigiTalk by Rockwell International";
                   break;
              case WAVE_FORMAT_XEBEC:
                   strAudioFormat = "XEBEC by Xebec Multimedia Solutions Limited";
                   break;
              case WAVE_FORMAT_G721_ADPCM:
                   strAudioFormat = "G721 ADPCM by Antex Electronics Corporation";
                   break;
              case WAVE_FORMAT_G728_CELP:
                   strAudioFormat = "G728 CELP by Antex Electronics Corporation";
                   break;
              case WAVE_FORMAT_MSG723:
                   strAudioFormat = "MSG723 by Microsoft Corp.";
                   break;
              case WAVE_FORMAT_MPEG:
                   strAudioFormat = "MPEG by Microsoft Corp.";
                   break;
              case WAVE_FORMAT_RT24:
                   strAudioFormat = "RT24 by InSoft Inc.";
                   break;
              case WAVE_FORMAT_PAC:
                   strAudioFormat = "PAC by InSoft Inc.";
                   break;
              case WAVE_FORMAT_MPEGLAYER3:
                   strAudioFormat = "MPEGLAYER3 by ISO MPEG Layer 3 Format Tag";
                   break;
              case WAVE_FORMAT_LUCENT_G723:
                   strAudioFormat = "G723 by Lucent Technologies";
                   break;
              case WAVE_FORMAT_CIRRUS:
                   strAudioFormat = "Cirrus by Cirrus Logic";
                   break;
              case WAVE_FORMAT_ESPCM:
                   strAudioFormat = "ESS Techonology";
                   break;
              case WAVE_FORMAT_VOXWARE:
                   strAudioFormat = "VOXWARE by Voxware";
                   break;
              case WAVE_FORMAT_CANOPUS_ATRAC:
                   strAudioFormat = "ATRAC by Canopus co., Ltd.";
                   break;
              case WAVE_FORMAT_G726_ADPCM:
                   strAudioFormat = "G726 ADPCM by APICOM";
                   break;
              case WAVE_FORMAT_G722_ADPCM:
                   strAudioFormat = "G722 ADPCM by APICOM";
                   break;
              case WAVE_FORMAT_DSAT_DISPLAY:
                   strAudioFormat = "DSAT Display by Microsoft Corp.";
                   break;
              case WAVE_FORMAT_VOXWARE_BYTE_ALIGNED:
                   strAudioFormat = "Byte Aligned by Voxware Inc.";
                   break;
              case WAVE_FORMAT_VOXWARE_AC8:
                   strAudioFormat = "AC8 by Voxware Inc.";
                   break;
              case WAVE_FORMAT_VOXWARE_AC10:
                   strAudioFormat = "AC10 by Voxware Inc.";
                   break;
              case WAVE_FORMAT_VOXWARE_AC16:
                   strAudioFormat = "AC16 by Voxware Inc.";
                   break;
              case WAVE_FORMAT_VOXWARE_AC20:
                   strAudioFormat = "AC20 by Voxware Inc.";
                   break;
              case WAVE_FORMAT_VOXWARE_RT24:
                   strAudioFormat = "RT24 by Voxware Inc.";
                   break;
              case WAVE_FORMAT_VOXWARE_RT29:
                   strAudioFormat = "RT29 by Voxware Inc.";
                   break;
              case WAVE_FORMAT_VOXWARE_RT29HW:
                   strAudioFormat = "RT29HW by Voxware Inc.";
                   break;
              case WAVE_FORMAT_VOXWARE_VR12:
                   strAudioFormat = "VR12 by Voxware Inc.";
                   break;
              case WAVE_FORMAT_VOXWARE_VR18:
                   strAudioFormat = "VR18 by Voxware Inc.";
                   break;
              case WAVE_FORMAT_VOXWARE_TQ40:
                   strAudioFormat = "TQ40 by Voxware Inc.";
                   break;
              case WAVE_FORMAT_SOFTSOUND:
                   strAudioFormat = "Softsound by Softsound Ltd.";
                   break;
              case WAVE_FORMAT_VOXWARE_TQ60:
                   strAudioFormat = "TQ60 by Voxware Inc.";
                   break;
              case WAVE_FORMAT_MSRT24:
                   strAudioFormat = "MRST24 by Microsoft Corp.";
                   break;
              case WAVE_FORMAT_G729A:
                   strAudioFormat = "G729A by AT&T Labs Inc.";
                   break;
              case WAVE_FORMAT_MVI_MVI2:
                   strAudioFormat = "MVI MVI2 by Motion Pixels";
                   break;
              case WAVE_FORMAT_DF_G726:
                   strAudioFormat = "G726 by DataFusion Systems";
                   break;
              case WAVE_FORMAT_DF_GSM610:
                   strAudioFormat = "GSM610 by DataFusion Systems";
                   break;
              case WAVE_FORMAT_ISIAUDIO:
                   strAudioFormat = "ISIAUDIO by Iterated Systems Inc.";
                   break;
              case WAVE_FORMAT_ONLIVE:
                   strAudioFormat = "ONLIVE by OnLive! Technologies Inc.";
                   break;
              case WAVE_FORMAT_SBC24:
                   strAudioFormat = "SBC24 by Siemens Business Communications Sys";
                   break;
              case WAVE_FORMAT_DOLBY_AC3_SPDIF:
                   strAudioFormat = "AC3 SPDIF by Sonic Foundry";
                   break;
              case WAVE_FORMAT_MEDIASONIC_G723:
                   strAudioFormat = "G723 by MediaSonic";
                   break;
              case WAVE_FORMAT_PROSODY_8KBPS:
                   strAudioFormat = "Prosody 8KBPS by Aculap plc";
                   break;
              case WAVE_FORMAT_ZYXEL_ADPCM:
                   strAudioFormat = "ADPCM by ZyXEL Communications Inc.";
                   break;
              case WAVE_FORMAT_PHILIPS_LPCBB:
                   strAudioFormat = "LPCBB by Philips Speech Processing";
                   break;
              case WAVE_FORMAT_PACKED:
                   strAudioFormat = "Packed by Studer Professional Audio AG";
                   break;
              case WAVE_FORMAT_MALDEN_PHONYTALK:
                   strAudioFormat = "PhonyTalk by Malden Electronics Ltd.";
                   break;
              case WAVE_FORMAT_RHETOREX_ADPCM:
                   strAudioFormat = "ADPCM by Rhetorex Inc.";
                   break;
              case WAVE_FORMAT_IRAT:
                   strAudioFormat = "IRAT by BeCubed Software Ltd.";
                   break;
              case WAVE_FORMAT_VIVO_G723:
                   strAudioFormat = "G723 by Vivo Software";
                   break;
              case WAVE_FORMAT_VIVO_SIREN:
                   strAudioFormat = "Siren by Vivo Software";
                   break;
              case WAVE_FORMAT_DIGITAL_G723:
                   strAudioFormat = "G723 by Digital Equipment Corp.";
                   break;
              case WAVE_FORMAT_SANYO_LD_ADPCM:
                   strAudioFormat = "LD ADPCM by Sany Electric Co., Ltd.";
                   break;
              case WAVE_FORMAT_SIPROLAB_ACEPLNET:
                   strAudioFormat = "ACEPLNET by Spiro Lab Telecom Inc.";
                   break;
              case WAVE_FORMAT_SIPROLAB_ACELP4800:
                   strAudioFormat = "ACELP4800 by Spiro Lab Telecom Inc.";
                   break;
              case WAVE_FORMAT_SIPROLAB_ACELP8V3:
                   strAudioFormat = "ACELP8V3 by Spiro Lab Telecom Inc.";
                   break;
              case WAVE_FORMAT_SIPROLAB_G729:
                   strAudioFormat = "G729 by Spiro Lab Telecom Inc.";
                   break;
              case WAVE_FORMAT_SIPROLAB_G729A:
                   strAudioFormat = "G729A by Spiro Lab Telecom Inc.";
                   break;
              case WAVE_FORMAT_SIPROLAB_KELVIN:
                   strAudioFormat = "Kelvin by Spiro Lab Telecom Inc.";
                   break;
              case WAVE_FORMAT_G726ADPCM:
                   strAudioFormat = "G726ADPCM by Dictaphone Corp.";
                   break;
              case WAVE_FORMAT_QUALCOMM_PUREVOICE:
                   strAudioFormat = "PureVoice by Qualcomm Inc.";
                   break;
              case WAVE_FORMAT_QUALCOMM_HALFRATE:
                   strAudioFormat = "HalfRate by Qualcomm Inc.";
                   break;
              case WAVE_FORMAT_TUBGSM:
                   strAudioFormat = "TUBGSM by Ring Zero Systems Inc.";
                   break;
              case WAVE_FORMAT_MSAUDIO1:
                   strAudioFormat = "MSAudio1 by Microsoft Corp.";
                   break;
              case WAVE_FORMAT_CREATIVE_ADPCM:
                   strAudioFormat = "ADPCM by Creative Labs Inc.";
                   break;
              case WAVE_FORMAT_CREATIVE_FASTSPEECH8:
                   strAudioFormat = "FastSpeech 8  by Creative Labs Inc.";
                   break;
              case WAVE_FORMAT_CREATIVE_FASTSPEECH10:
                   strAudioFormat = "FastSpeech 10 by Creative Labs Inc.";
                   break;
              case WAVE_FORMAT_UHER_ADPCM:
                   strAudioFormat = "ADPCM by Usher Informatic GmbH";
                   break;
              case WAVE_FORMAT_QUARTERDECK:
                   strAudioFormat = "QuarterDeck by Quarterdeck Corp.";
                   break;
              case WAVE_FORMAT_ILINK_VC:
                   strAudioFormat = "VC by I-link Worldwide";
                   break;
              case WAVE_FORMAT_RAW_SPORT:
                   strAudioFormat = "RAW Sport by Aureal Semiconductor";
                   break;
              case WAVE_FORMAT_IPI_HSX:
                   strAudioFormat = "HSX by Interactive Products Inc.";
                   break;
              case WAVE_FORMAT_IPI_RPELP:
                   strAudioFormat = "RPELP by Interactive Products Inc.";
                   break;
              case WAVE_FORMAT_CS2:
                   strAudioFormat = "CS2 by Consistent Software";
                   break;
              case WAVE_FORMAT_SONY_SCX:
                   strAudioFormat = "SCX by Sony Corp.";
                   break;
              case WAVE_FORMAT_FM_TOWNS_SND:
                   strAudioFormat = "FM Towns Snd by Fujitsu Corp.";
                   break;
              case WAVE_FORMAT_BTV_DIGITAL:
                   strAudioFormat = "BTV Digital by Brooktree Corp.";
                   break;
              case WAVE_FORMAT_QDESIGN_MUSIC:
                   strAudioFormat = "QDesign Music by QDesign Music Corp.";
                   break;
              case WAVE_FORMAT_VME_VMPCM:
                   strAudioFormat = "VME VMPCM by AT&T Labs Inc.";
                   break;
              case WAVE_FORMAT_TPC:
                   strAudioFormat = "TPC by AT&T Labs Inc.";
                   break;
              case WAVE_FORMAT_OLIGSM:
                   strAudioFormat = "OLIGSM by Ing C. Olivetti & C., S.p.A.";
                   break;
              case WAVE_FORMAT_OLIADPCM:
                   strAudioFormat = "OLIADPCM by Ing C. Olivetti & C., S.p.A.";
                   break;
              case WAVE_FORMAT_OLICELP:
                   strAudioFormat = "OLICELP by Ing C. Olivetti & C., S.p.A.";
                   break;
              case WAVE_FORMAT_OLISBC:
                   strAudioFormat = "OLISBC by Ing C. Olivetti & C., S.p.A.";
                   break;
              case WAVE_FORMAT_OLIOPR:
                   strAudioFormat = "OLIOPR by Ing C. Olivetti & C., S.p.A.";
                   break;
              case WAVE_FORMAT_LH_CODEC:
                   strAudioFormat = "LH Codec by Lernout & Hauspie";
                   break;
              case WAVE_FORMAT_NORRIS:
                   strAudioFormat = "Norris By Norris Communications Inc.";
                   break;
              case WAVE_FORMAT_SOUNDSPACE_MUSICOMPRESS:
                   strAudioFormat = "SoundSpace Musicompress by AT&T Labs Inc.";
                   break;
              case WAVE_FORMAT_DVM:
                   strAudioFormat = "DVM by FAST Multimedia AG";
                   break;
              case WAVE_FORMAT_EXTENSIBLE:
                   strAudioFormat = "Format Extensible by Microsoft Corp.";
                   break;
              case WAVE_FORMAT_DEVELOPMENT:
                   strAudioFormat = "Development by Unknown";
                   break;
              default:
                   break;
       }
       wavFormat.close();
       return strAudioFormat;
}
//Hex Dump WAV Header to LPSTR by line
void hexdump(string strfilename, string strsavepath)
{
 freopen (string_char(strsavepath),"w",stdout);
 string filename = strfilename.substr( strfilename.find_last_of("\\") + 1 );
 printf("%s\n", string_char(filename)); 
 long lSize = dataOffset(string_char(strfilename))+8;
 char pAddressIn[lSize];
 char * charfilename = string_char(strfilename);
 fstream wavFile(charfilename, ios::in | ios::out | ios::binary);
 wavFile.seekg(0);
 wavFile.read((char *)&pAddressIn, sizeof(pAddressIn));
 wavFile.close();
 int lines = 0;
 char szBuf[100];
 long lIndent = 1;
 long lOutLen, lIndex, lIndex2, lOutLen2;
 long lRelPos;
 struct { char *pData; unsigned long lSize; } buf;
 unsigned char *pTmp,ucTmp;
 unsigned char *pAddress = (unsigned char *)pAddressIn;

   buf.pData   = (char *)pAddress;
   buf.lSize   = lSize;

   while (buf.lSize > 0)
   {
      pTmp     = (unsigned char *)buf.pData;
      lOutLen  = (int)buf.lSize;
      if (lOutLen > 16)
          lOutLen = 16;

      // create a 64-character formatted output line:
      sprintf(szBuf, " >                            "
                     "                      "
                     "    %08lX", pTmp-pAddress);
      lOutLen2 = lOutLen;

      for(lIndex = 1+lIndent, lIndex2 = 53-15+lIndent, lRelPos = 0;
          lOutLen2;
          lOutLen2--, lIndex += 2, lIndex2++
         )
      {
         ucTmp = *pTmp++;

         sprintf(szBuf + lIndex, "%02X ", (unsigned short)ucTmp);
         if(!isprint(ucTmp))  ucTmp = '.'; // nonprintable char
         szBuf[lIndex2] = ucTmp;

         if (!(++lRelPos & 3))     // extra blank after 4 bytes
         {  lIndex++; szBuf[lIndex+2] = ' '; }
      }

      if (!(lRelPos & 3)) lIndex--;

      szBuf[lIndex  ]   = '<';
      szBuf[lIndex+1]   = ' ';
      
      //printf("%s\n", szBuf);
      //printf("%s\n", szBuf);
      
      printf("%s\n", szBuf);
      
      buf.pData   += lOutLen;
      buf.lSize   -= lOutLen;
   }
   fclose (stdout);
}
void XSleepAnotherWay(DWORD dwWaitInMSecs)

{

    DWORD dwStopTime = GetTickCount() + dwWaitInMSecs;

    DWORD dwTimeout = dwWaitInMSecs;

    MSG msg;

    while(1) {

        switch(MsgWaitForMultipleObjects(0, NULL, FALSE, dwTimeout, QS_ALLINPUT)) {

        case WAIT_TIMEOUT:

            break;

        default:

#ifdef __AFX_H__

            while(::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) 

                AfxGetApp()->PumpMessage();

#else

            while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

                ::TranslateMessage(&msg);

                ::DispatchMessage(&msg);

            }

#endif

        }

        dwTimeout = dwStopTime - GetTickCount();

        if(!dwTimeout || dwTimeout>dwWaitInMSecs)

            break;

    }

}

//Multi HexDump
void hexdump_multi(string strfilename, string* charOut)
{
 //char charOutTmp[500];
 int i = 0;
 //freopen (string_char(strsavepath),"w",stdout);
 string filename = strfilename.substr( strfilename.find_last_of("\\") + 1 );
 printf("%s\n", string_char(filename)); 
 long lSize = dataOffset(string_char(strfilename))+8;
 char pAddressIn[lSize];
 char * charfilename = string_char(strfilename);
 fstream wavFile(charfilename, ios::in | ios::out | ios::binary);
 wavFile.seekg(0);
 wavFile.read((char *)&pAddressIn, sizeof(pAddressIn));
 wavFile.close();
 int lines = 0;
 char szBuf[100];
 long lIndent = 1;
 long lOutLen, lIndex, lIndex2, lOutLen2;
 long lRelPos;
 struct { char *pData; unsigned long lSize; } buf;
 unsigned char *pTmp,ucTmp;
 unsigned char *pAddress = (unsigned char *)pAddressIn;

   buf.pData   = (char *)pAddress;
   buf.lSize   = lSize;

   while (buf.lSize > 0)
   {
      pTmp     = (unsigned char *)buf.pData;
      lOutLen  = (int)buf.lSize;
      if (lOutLen > 16)
          lOutLen = 16;

      // create a 64-character formatted output line:
      sprintf(szBuf, " >                            "
                     "                      "
                     "    %08lX", pTmp-pAddress);
      lOutLen2 = lOutLen;

      for(lIndex = 1+lIndent, lIndex2 = 53-15+lIndent, lRelPos = 0;
          lOutLen2;
          lOutLen2--, lIndex += 2, lIndex2++
         )
      {
         ucTmp = *pTmp++;

         sprintf(szBuf + lIndex, "%02X ", (unsigned short)ucTmp);
         if(!isprint(ucTmp))  ucTmp = '.'; // nonprintable char
         szBuf[lIndex2] = ucTmp;

         if (!(++lRelPos & 3))     // extra blank after 4 bytes
         {  lIndex++; szBuf[lIndex+2] = ' '; }
      }

      if (!(lRelPos & 3)) lIndex--;

      szBuf[lIndex  ]   = '<';
      szBuf[lIndex+1]   = ' ';
      
      //printf("%s\n", szBuf);
      //printf("%s\n", szBuf);
      
      printf("%s\n", szBuf);
      charOut[i] = szBuf;
      i++;
      buf.pData   += lOutLen;
      buf.lSize   -= lOutLen;
   }
   //fclose (stdout);
}

void drawstuff(HDC hdc,LPSTR lpstrFileName, LPSTR lpstrLine1, LPSTR lpstrLine2, LPSTR lpstrLine3){
     TextOut(hdc, 0, 0, lpstrFileName, strlen(lpstrFileName));
     TextOut(hdc, 0, 14, lpstrLine1, strlen(lpstrLine1));
     TextOut(hdc, 0, 28, lpstrLine2, strlen(lpstrLine2));
     TextOut(hdc, 0, 42, lpstrLine3, strlen(lpstrLine3));
     /*
     TextOut(hdc, 0, 56, lpstrLine4, strlen(lpstrLine4));
     TextOut(hdc, 0, 70, lpstrLine5, strlen(lpstrLine5));
     TextOut(hdc, 0, 84, lpstrLine6, strlen(lpstrLine6));
     TextOut(hdc, 0, 98, lpstrLine7, strlen(lpstrLine7));
     TextOut(hdc, 0, 112, lpstrLine8, strlen(lpstrLine8));
     TextOut(hdc, 0, 126, lpstrLine9, strlen(lpstrLine9));
     TextOut(hdc, 0, 140, lpstrLine10, strlen(lpstrLine10));
     TextOut(hdc, 0, 154, lpstrAudioFormat, strlen(lpstrAudioFormat));
     */
}

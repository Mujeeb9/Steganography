#ifndef DECODE_H
#define DECODE_H
//Includes types.h file.
#include "types.h"
#define MAX_FILE_SUFFIX 4
//Structure to hold source image file information and secret file information
typedef struct _DecodeInfo
{
	//Stores source file info
	char *src_image_fname;
	FILE *fptr_src_image;
	char *buffer;


    //Stores secret file information.
	char secret_fname[20];
	FILE *fptr_secret;
	int size_secret_file;
	int extn_size;
	char extn_secret_file[MAX_FILE_SUFFIX];
	char ch_data;
	int int_data;

} DecodeInfo;
//read_and_validate_for_decode function to validate arguments for decode.
Status read_and_validate_for_decode(char *argv[],DecodeInfo *decInfo);

//do_decoding function for decoding data.
Status do_decoding(DecodeInfo *decInfo);

//open_files_for_decode function to open required files.
Status open_files_for_decode(DecodeInfo *decInfo);

//decode_magic_string function to decode magic string.
Status decode_magic_string(DecodeInfo *decInfo);

//decode_data_to_image function to decode character from bytes of image data.
Status decode_data_to_image(int size ,DecodeInfo *decInfo);

//decode_byte_to_lsb functiion to get data from lsb of image data.
Status decode_byte_to_lsb(char *img_data, char *data);

//decode_size_to_lsb function to get integer value from lsb of image data array.
Status decode_size_to_lsb(int size,DecodeInfo *decInfo);

//decode_extn_size function to decode extn size.
Status decode_file_extn_size(int size,DecodeInfo *decInfo);

//decode_file_extn function to decode file extension.
Status decode_file_extn(int size,DecodeInfo *decInfo);

//decode_secret_file_size function to decode secret file size.
Status decode_secret_file_size(int size,DecodeInfo *decInfo);

//decode_secret_file_data function to decode file data from image data.
Status decode_secret_file_data(int size,DecodeInfo *decInfo);

#endif

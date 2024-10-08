//Includes stdio.h ,stdlib.h ,string.h header files and  decode.h ,types.h,common.h files. 
#include <stdio.h>
#include "decode.h"
#include "types.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>

//read_and_validate_for_decode function to validate arguments for decode.
Status read_and_validate_for_decode(char *argv[],DecodeInfo *decInfo)
{
	//Checks if argv[2] contains .bmp or not.
	if(strcmp(strchr(argv[2],'.'),".bmp") == 0)
	{
		//Prints the message as mentioned file is a .bmp file.
		printf("INFO : The mentioned file is a bmp file \n");
		//Stores argv[2] into src_image_fname.
		decInfo->src_image_fname = argv[2];
	}
	else
	{
		//Prints the error message as mentioned file is not a .bmp file and returns e_failure.
		printf("INFO : The mentioned bmp file is not valid \n");
		return e_failure;
	}
	//Checks if argv[3] is NULLL.
	if(argv[3] == NULL)
	{
		//copies the decode string into secret_fname structure member.
		strcpy(decInfo->secret_fname,"decode");
    }
	else
	{
		//Copies the argv[3] string into structure member secret_fname.
		strcpy(decInfo->secret_fname,argv[3]);
	}
	//Returns e_success.
	return e_success;
}
//do_decoding function defination for decoding image data.
Status do_decoding(DecodeInfo *decInfo)
{
	//Checks if open_files_for_decode function returns e_success.
	if(open_files_for_decode(decInfo) == e_success)
	{
		//Prints the message as Opened required files.
		printf("INFO : Opening required files \nINFO : Opened %s \n",decInfo->src_image_fname);
		//Set image file pointer to 54th byte.
		fseek(decInfo->fptr_src_image,54,SEEK_SET);
		//Prints the message as Skipped header of image file.
		    
			//Checks if function decode_magic_string returns e_success.
			if(decode_magic_string(decInfo) == e_success)
			{
				printf("INFO : Decoding Magic String Signature \n");
				//Checks decoded magic string matches with known magic string.
				if(strcmp(MAGIC_STRING,decInfo->buffer) == 0)
				{
					printf("INFO : Done\n");
					//Checks if function decode_file_extn_size returns e_success.
					if(decode_file_extn_size(32,decInfo) == e_success)
					{
						printf("INFO : Decoding size of file extension \nINFO : Done\n");
						decInfo->extn_size = decInfo->int_data;
						//Checks if function decode_file_extn returns e_success.
						if(decode_file_extn(decInfo->extn_size*8,decInfo) == e_success)
						{
							printf("INFO : Decoding file extnesion \nINFO : Done\n");
							//copies the decoded data into extn_secret_file.
                           	strcpy(decInfo->extn_secret_file,decInfo->buffer);
							//Concatenate the extn with file name.
							strcat(decInfo->secret_fname,decInfo->extn_secret_file);
							//Checks if function decode_secret_file_size returns e_success.
							if(decode_secret_file_size(32,decInfo) == e_success)
							{
								printf("INFO : Decoding File Size \nINFO : Done\n");
								//Stores decoded size into size_secret_file.
								decInfo->size_secret_file = decInfo->int_data;
								//Checks if function decode_secret_file_size returns e_success.
								if(decode_secret_file_data(decInfo->size_secret_file*8,decInfo) == e_success)
								{

									printf("INFO : Decoding File Data \nINFO : Done\n");
									//Frees the memory allocated for buffer.
									free(decInfo->buffer);
									//Close the secret file and src_image file.
									fclose(decInfo->fptr_secret);
									fclose(decInfo->fptr_src_image);
								}
								else
								{
									//Prints error message and returns e_failure.
									printf("INFO : Failed to decode secret file data \n");
									return e_failure;
								}
							}
							else
							{
								//Prints error message and returns e_failure.
								printf("INFO : Failed to decode secret file size\n");
								return e_failure;
							}
						}
						else
						{
							//Prints error message and returns e_failure.
							printf("INFO : Failed to decode file extension\n");
							return e_failure;
						}
					}
					else
					{
						//Prints error message and returns e_failure.
						printf("INFO : Failed to decode the file extension size \n");
						return e_failure;
					}
				}
				else
				{
					//Prints error message and returns e_failure.
					printf("INFO : Magic String not matched \n");
					return e_failure;
				}
			}
			else
			{
				//Prints error message and returns e_failure.
				printf("INFO : Failed to decode magic magic string\n");
				return e_failure;
			}
	}
	else
	{
		//Prints error message and returns e_failure.
		printf("INFO : Failed to open required files \n");
		return e_failure;
	}
	//returns e_success.
	return e_success;
}



//Function defination of open_files_for_decode.
Status open_files_for_decode(DecodeInfo *decInfo)
{
	//Open src_image file in read mode with fptr_src_image.
	decInfo->fptr_src_image = fopen(decInfo->src_image_fname,"r");
	//Checks if fptr_src_image points to NULL.
	if(decInfo->fptr_src_image == NULL)
	{
		//Prints messaga as failed to open file.
		printf("INFO : ## Failed to open %s file ##\n",decInfo->src_image_fname);
		//returs e_failure.
		return e_failure;
	}
	//Returns e_success.
	return e_success;
}



//Function defination of decode_magic_string.
Status decode_magic_string(DecodeInfo *decInfo)
{
	    //checks if decode_data_to_image fucntion returns e_success after passing arguments as length of magic string * 8 and structure pointer.
		if(decode_data_to_image(strlen(MAGIC_STRING)*8,decInfo) == e_success)
		{
			//Returns e_success.
			return e_success;
		}
	//returns e_failure.
	return e_failure;
}



//Function defination of decode_data_to_image.
Status decode_data_to_image(int size,DecodeInfo *decInfo)
{
	//creates character array of 8 size.
	char temp[8];
	//Allocates dynamically memory for buffer with size mention. 
	decInfo->buffer = malloc(sizeof(char) * (size/8));
	//Loop to decode  bytes of image file.
	for(int i = 0;i < (size/8);i++)
	{
		decInfo->ch_data = 0;
		//Reads 8 bytes 1 times from scr_image file and stores into temp array. 
		fread(temp,8,1,decInfo->fptr_src_image);
		//Calls decode_byte_to_lsb function with temp address and ch_data adddress.
		decode_byte_to_lsb(temp,&decInfo->ch_data);
		//Stores decoded data into buffer[i].
		decInfo->buffer[i] = decInfo->ch_data;
	}
	//returns e_success.
	return e_success;
}



//Function defination of decode_byte_to_lsb.
Status decode_byte_to_lsb(char *img_data,char *data)
{
	//Loop to decode each byte from 8 bytes array.
	for(int i = 0;i < 8;i++)
	{
		//get the lsb of a byte from array and assigns to MSB's of data. 
		*data = *data | ((img_data[i] & 1) << (7 - i));
	}
	//Returns e_success.
	return e_success;
}



//Function defination of decode_size_to_lsb.
Status decode_size_to_lsb(int size,DecodeInfo *decInfo)
{
	//character array of 32 size.
	char str[32];
	//initializes 0 to int_data member of structure.
	decInfo->int_data = 0;
	//Reads 32 bytes 1 time form src_image and stores into str array.
	fread(str,32,1,decInfo->fptr_src_image);
	//loop to get each lsb bit of byte from image data.
	for(int i = 0;i < 32;i++)
	{
		//Assigns to MSB' side of int_data of fetched lsb of byte.
		decInfo->int_data =  decInfo->int_data | ((str[i] & 1) << (31 - i));
	}
	//Returnss e_success.
	return e_success;
}


//Function defination of decode_file_extn_size.
Status decode_file_extn_size(int size,DecodeInfo *decInfo)
{
	//Calls fucntion decode_size_to_lsb to decode size of extension.
	decode_size_to_lsb(size,decInfo);
	//Returns e_success.
	return e_success;
}
//Function defination of decode_file_extn.
Status decode_file_extn(int size,DecodeInfo *decInfo)
{
	//calls decode_data_to_image function with size and decInfo as parameter.
	decode_data_to_image(size,decInfo);
	//returns e_success.
	return e_success;
}
//Function defination of decode_secret_file_size.
Status decode_secret_file_size(int size,DecodeInfo *decInfo)
{
	//calls decode_size_to_lsb function with size and decInfo as parameter.
	decode_size_to_lsb(size,decInfo);
	//returns e_success.
	return e_success;
}
//Function defination of decode_secret_file_data.
Status decode_secret_file_data(int size,DecodeInfo *decInfo)
{
	//Open file secret with the specified name in write mode with help of pointer fptr-secret.
	decInfo->fptr_secret = fopen(decInfo->secret_fname,"w");
	printf("INFO : Opened all required files\nINFO : Done\n");
	//Calls decode_data_to_image with size and decInfo as parameter.
	decode_data_to_image(size,decInfo);
	//Writes sixe_secret_file data from decInfo->buffer into secret_file(output file).
	fwrite(decInfo->buffer,decInfo->size_secret_file,1,decInfo->fptr_secret);
	//Returns e_success.
	return e_success;
}


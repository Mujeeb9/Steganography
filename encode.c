#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
//Function to read and valide arguments for encode.
Status read_and_validate_encode_args(char *argv[],EncodeInfo *encInfo)
{
	//Checks if argv[2] is with .bmp extension.
	if(strcmp(strchr(argv[2],'.'),".bmp") == 0)
	{
		//Assigns the argument string to structure memeber src_image_fname.
		encInfo->src_image_fname = argv[2];
	}
	else
	{
		//Prints the error message as mention file is not a .bmp file.
		printf("INFO : ## The source file is not a .bmp file ##\n");
		//returns e_failure .
		return e_failure;
	}
	//Checks argv[3] string contains .txt or not.
	if(strcmp(strchr(argv[3],'.'),".txt") == 0)
	{
		//Assigns the argv[3] string to structure member secret_fname.
		encInfo->secret_fname = argv[3];
	}
	else
	{
		//Prints the error message as secret file is not a .txt file.
		printf("The secret file is not a .txt file\n");
		//returns e_failure.
		return e_failure;
	}
	//Checks for arg[4] equals to NULL.
	if(argv[4] == NULL)
	{
		//Prints the message and create default file name.
		printf("Output file not mentioned.Creating steged_img.bmp as default\n");
		//Assigns name of file to structure member stego_image_fname.
		encInfo->stego_image_fname = "steged_img.bmp";

	}
	else
	{
		//Checks arg[4] contains .bmp or not.
		if(strcmp(strchr(argv[4],'.'),".bmp") == 0)
		{
			//Assigns argv[4] string to stego_image_fname.
			encInfo->stego_image_fname = argv[4];
			return e_success;
		}
		else
		{
			//Prints the error message as file mentioned is not a .bmp file.
			printf("The output file mentioned is not a .bmp file\n");
			//returns e_failure.
			return e_failure;
		}
	}
	//retyrns e_success after all comparision.
	return e_success;

}
//Function defination of do_encoding.
Status do_encoding(EncodeInfo *encInfo)
{
	//Checks if return value of open_files is e_success.
	if(open_files(encInfo) == e_success)
	{
		printf("INFO: Opening required files \nINFO : Opened beautiful.bmp \nINFO : Opened secret.txt\nINFO : Opened %s \nINFO : Done\n",encInfo->stego_image_fname);
	    //Checks if return value of function check_capacity is e_success.
		if(check_capacity(encInfo) == e_success)
		{
			printf("INFO: Checking for beautiful.bmp capacity to handle secret.txt \nINFO: Done.Found OK \n");
			//Checks if return value of function copy_bmp_header is e_success.
			if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
			{
				printf("INFO: Copying image header \nINFO : done\n");
				//Checks if return value of function encode_magic_string is e_success.
				if(encode_magic_string(MAGIC_STRING,encInfo) == e_success)
				{
					printf("INFO: Encoding Magic String Signature \nINFO : Done\n");
					//copies the extension into structure member extn_secret_file.
					strcpy(encInfo->extn_secret_file,strchr(encInfo->secret_fname,'.'));
					//Checks if return value of function size_to_lsb  is e_success.
					if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file),encInfo) == e_success)
					{
						printf("INFO: Encodeding secret file extension size\nINFO : Done\n");
						//Checks if return value of function encode_secret_file_extn is e_success.
						if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo) == e_success)
						{
							printf("INFO: Encoding secret file extension \nINFO : Done\n");
							//Checks if return value of function encode_secret_file_size is e_success.
							if(encode_secret_file_size(encInfo->size_secret_file,encInfo) == e_success)
							{
								printf("INFO: Encoding secret file size \nINFO : Done\n");
								//Checks if return value of function encode_secret_file_data is e_success.
								if(encode_secret_file_data(encInfo) == e_success)
								{
									printf("INFO: Encoding secret file data \nINFO : Done\n");
									//Checks if return value of function copy_remaining_img_data is e_success.
									if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
									{
										printf("INFO: Copying Left Over Data\nINFO : Done\n");
									}
									else
									{
										//Else part prints error message and return e_failure.
										printf("INFO: Failed to copy Left Over Data \n");
										return e_failure;
									}
								}
								else
								{
									//Else part prints error message and return e_failure.
									printf("INFO: Failed to encode secret file data \n");
									return e_failure;
								}
							}
							else
							{
								//Else part prints error message and return e_failure.
								printf("INFO: Failed to encode secret file size \n");
							}
						}
						else
						{
							//Else part prints error message and return e_failure.
							printf("INFO: Failed to encode secret file extension \n");
							return e_failure;
						}
					}
					else
					{
						//Else part prints error message and return e_failure.
						printf("INFO: Failed to encode secret file extension size \n");
						return e_failure;
					}
				
				}
				else
				{
					//Else part prints error message and return e_failure.
					printf("INFO: Failed to Encode Magic String \n");
					return e_failure;
				}
			}
			else
			{
				//Else part prints error message and return e_failure.
				printf("INFO: Failed to copy image header\n");
				return e_failure;
			}

		}
		else
		{
			//Else part prints error message and return e_failure.
			printf("INFO: The capacity of beautiful.bmp is not enough to handle secret.txt \n");
			return e_failure;
		}
	}
	else
	{
		//Else part prints error message and return e_failure.
		printf("INFO: Failed to open required files \n");
		return e_failure;
	}
	return e_success;
}
//Function check_capacity to check capacity of source .bmp file.
Status check_capacity(EncodeInfo *encInfo)
{
	//Calls get_image_size_for_bmp function for source file ,assigns return value to image_capacity structure member.
	encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
	//Calls get_file_size function for secret file ,assigns return value to size_secret_file structure member.
	encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
	//Checks the source imag file can handle all the data to be encoded.
	if(encInfo->image_capacity > ((encInfo->size_secret_file + 4 + strlen(MAGIC_STRING) + 4 + 4)*8)+54)
	{
		//returns e_success.
		return e_success;
	}
	else
		//Returns e_failure.
		return e_failure;
}
//Function get_file_size to calaculate file size.
uint get_file_size(FILE *fptr)
{
	//rewinds file pointer.
	rewind(fptr);
	//Set file pointer to end of file.
	fseek(fptr,0,SEEK_END);
	//Returns position of file pointer.
	return ftell(fptr);
}
//Function copy_bmp_header to copy bmp header.
Status copy_bmp_header(FILE *fptr_src_image,FILE *fptr_dest_image)
{
	//Declares temp as character array of 54 bytes
	char temp[54];
	//Set the source file pointer to firt position.
	fseek(fptr_src_image,0,SEEK_SET);
	//Reads 54 bytes 1 time from source file and store into temp array.
	fread(temp,sizeof(temp),1,fptr_src_image);
	//Writes 54 bytes 1 time into dest_image file from temp.
	fwrite(temp,sizeof(temp),1,fptr_dest_image);
	//Returns e_success.
	return e_success;
}
//Function defination of  encode_magic_string to encode magic string.
Status encode_magic_string(char *magic_string,EncodeInfo *encInfo)
{
	//calls encode_data_to_image by passing arguments magic string,size of magic_string and structure pointer.
	if(encode_data_to_image(magic_string,strlen(magic_string),encInfo) == e_success)
	{
		//Returns e_success.
		return e_success;
	}
	//Returns e_failure.
	return e_failure;
}
//Function encode_data_to_image defination.
Status encode_data_to_image(char *data,int size,EncodeInfo *encInfo)
{
	//character array of 8 size.
	char img_data[8];
	//Loop to modify 8 bytes size times.
	for(int i = 0;i < size;i++)
	{
		//Reads 8 bytes from source file 1 time and store in img_data array
		fread(img_data,8,1,encInfo->fptr_src_image);
		//Calls encode_byte_to_lsb by passing parameter data[i] and img_data.
		encode_byte_to_lsb(data[i],img_data);
		//Writes the 8 bytes from img_data 1 time into stego_img file.
		fwrite(img_data,8,1,encInfo->fptr_stego_image);
	}
	//Returns e_success.
	return e_success;
}
//Function defination of encode_byte_to_lsb.
Status encode_byte_to_lsb(char data,char *image_buffer)
{
	//Loop to get lsb of each byte.
	for(int i = 0;i < 8;i++)
	{
		//Clears lsb of image_buffer and replace with data's bits from MSB. 
		image_buffer[i] = (image_buffer[i] & 0xFE) | ((data >> (7 - i)) & 1);
	}
	//Returns e_success.
	return e_success;
}
//Function defination of encode_size_to_lsb.
Status encode_size_to_lsb(int size,EncodeInfo *encInfo)
{
	//character array of 32 size.
	char temp[32];
	//Reads 32 bytes from src_img and store into temp array.
	fread(temp,32,1,encInfo->fptr_src_image);
	//Loop to encode each size'MSB into lsb of temp.
	for(int i = 0;i < 32;i++)
	{
		//Clears lsb of tmep[i] abd replace with size' MSB bits.
		temp[i] = (temp[i] & 0xFE) | ((size >> (31 - i)) & 1);
	}
	//Writes 32 bytes into stego_image from temp array.
	fwrite(temp,32,1,encInfo->fptr_stego_image);
	//returns e_success.
    return e_success;
}

//Function defination of encode_secret_file_extn_size to encode size of extension.
Status encode_secret_file_extn_size(int size,EncodeInfo *encInfo)
{
	//Calls function byte_to_lsb to encode size into image data.
	encode_size_to_lsb(size,encInfo);
	//Returns e_success.
	return e_success;
}


//Function defination of encode_secret_file_extn.
Status encode_secret_file_extn(char *file_extn,EncodeInfo *encInfo)
{
	//Calls function encode_data_to_image by passing extn,sizeof extn and structure pointer.
	encode_data_to_image(file_extn,strlen(file_extn),encInfo);
	//Returns e_success.
	return e_success;
}
//Function defination of encode_secret_file_size.
Status encode_secret_file_size(long file_size,EncodeInfo *encInfo)
{
	//Calls function encode_size_to_lsb with file_size and structure pointer.
	encode_size_to_lsb(file_size,encInfo);
	//Returns e_success.
	return e_success;
}
//Function defination of encode_secret_file_data.
Status encode_secret_file_data(EncodeInfo *encInfo)
{
	//Set the secret file pointer to starting position. 
	fseek(encInfo->fptr_secret,0,SEEK_SET);
	//Character array of size_secret_file size.
	char str[encInfo->size_secret_file];
    //Reads size_secret_file bytes from secret_file and store into str.
    fread(str,encInfo->size_secret_file,1,encInfo->fptr_secret);
	//Calls encode_data_to_image function with str,size_secret_file as parameter.
	encode_data_to_image(str,encInfo->size_secret_file,encInfo);
	//Returns e_success.
	return e_success;
}
//Function defination of copy_remaining_img_data.
Status copy_remaining_img_data(FILE *fptr_src,FILE *fptr_dest)
{
	//Declares ch as characeter.
	char ch;
	//Reads 1 byte 1 time form src file ,stores in ch and check fread returns greater than zero value.
	while((fread(&ch,1,1,fptr_src)) > 0)
	{
		//Writes 1 byte from ch to dest_file.
		fwrite(&ch,1,1,fptr_dest);
	}
	//Returns e_success.
	return e_success;
}


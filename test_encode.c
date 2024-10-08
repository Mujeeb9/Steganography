/*
Name : Mujeeb Ur Rahman
Date : 12/01/2024
Description : Digital LSB image Steganography.
Sample Input: 
*/


//Includes stdio.h header files,encode.h file,types.h file,string.h header file and decode.h file. 
#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include "decode.h"
//Main function of retrun type integer and have parameters argc and argv[] vector.
int main(int argc,char *argv[])
{
	//Crates a structure variable encInfo.
    EncodeInfo encInfo;
	//Checks argc greater than 1 and less than 6.
	if(argc > 1 && argc < 6)
	{
		//Checks return type of function call operation is e_unsupported.
		if(check_operation_type(argv) == e_unsupported)
		{
			//prints the error message for user to pass correct options.
			printf("Please pass the correct option \n1.Usage For encoding\n./a.out -e beautiful.bmp secret.txt [steged.bmp]\n2.Usage For decoding\n ./a.out -d beautiful.bmp [decode.txt]\n");
			//returns 0 and terminate execution.
			return 0;
		}
		//Checks if check_operation_type function returns e_encode.
		else if(check_operation_type(argv) == e_encode)
		{

			printf("INFO: ## Encoding Procedure Started ## \n");
			    //Checks if read_and_validate_encode_args function returns e_success. 
				if(read_and_validate_encode_args(argv,&encInfo) == e_success)
				{
					//checks if do_encoding function returns e_success.
					if(do_encoding(&encInfo) == e_success)
					{
						//Prints the message as Encoding done successfully.
						printf("INFO: ## Encoding Done Successfully ##\n");
					}
					else
					{
						//Prints error message as Encoding failed and return o.
						printf("INFO: ## Encoding failed\n");
						return 0;
					}
				}
				else
				{
					//Returns 0.
					return 0;
				}
		}
        //Checks if check_operation_type function returns e_decode.
		else if(check_operation_type(argv) == e_decode)
		{
			//Creates decInfo as structure variable.
			DecodeInfo decInfo;
			//Prints the message as Decoding procedure started.
			printf("INFO: ## Decoding Procedure Started ##\n");
			//Checks read_and_validate_for_decode function returns e_success.
			if(read_and_validate_for_decode(argv,&decInfo) == e_success)
			{
				//Checks if do_decoding function returns e_success. 
				if(do_decoding(&decInfo) == e_success)
				{
					//Prints message as Decoding procedure done successfully.
					printf("INFO : ## Decoding Done Successfully ##\n");
				}
				else
				{
					//Returns 0.
					return 0;
				}
			}
			else
			{
				//returns 0.
				return 0;
			}
		}
	}
	else
	{
		//Prints error message as please pass required no of arguments.
		printf("INFO: ## Please Pass required no of arguments ##\n");
	}
	//Returns 0.
    return 0;
}
//Function defination of check_operation_type.
OperationType check_operation_type(char *argv[])
{
	//Checks if argv[1] is NULL.
	if(argv[1] == NULL)
	{
		//Returns  e_unsupported.
		return e_unsupported;
	}
	//Checks argv[1] and -e are equal .
	if(strcmp(argv[1],"-e") == 0)
	{
		//returns e_encode.
		return e_encode;
	}
	//Checks if argv[1] and -d are equal.
	if(strcmp(argv[1],"-d") == 0)
	{
		//Returns e_decode.
		return e_decode;
	}
    else
		//Returns e_unsupported.
		return e_unsupported;
}

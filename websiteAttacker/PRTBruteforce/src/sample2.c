#include <stdio.h>
#include "bruteforce.h"

int main(int argc, char **argv) {

	BRUTEFORCE_HANDLE bf_handle; //Declare a bruteforce handler.
	//Initialize it.
	if(bruteforce_init(
		&bf_handle, 			//The handler to be initialized.
		3, 						//Maximum length upto which bruteforce is performed.
		BF_FLAG_ALL,		//Include alphabets and numbers.
		NULL,					//I'm not using custom bruteforce character set, so I set this argument to NULL.
		NULL					//I'll start from the very beginning, I'm not having any particular entry point.
	) == 0)
		return !printf("ERROR : FUCKED UP!\n");
	while(bruteforce_update(&bf_handle)) {

		//This code works until bruteforce_udpate returns 0.
		printf("%s\n", bf_handle.bfText); //bf_handle.`bfText` is the text that gets updated.

		//Instead of printing bf_handle.bfText - I can use this loop against a database, SSH or POST requests.
	}
	/*int i;
	for(i = 0; i< 200; i++)
		bruteforce_update(&bf_handle);*/
	printf("%d | %d | %s\n", bf_handle.bfmaxLen, bf_handle.bfCurrIdx, bf_handle.bfText);
	printf("BLAA\n%s\n", bf_handle.bfData);
	bruteforce_finalize(&bf_handle);
	return 0;
}

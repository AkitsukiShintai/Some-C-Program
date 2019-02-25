/**
* @file splitter.c
* @author Wang Zhongqiu
* @date 20 Sep 2018
* @brief Impiment the functions SplitFile and JoinFiles
*
* impliment functions in splitter.h, if error, return element of error enum
*/

#include <stdio.h>     /* printf, sprintf */
#include "splitter.h"  /* SplitFile, JoinFiles */
#include<stdlib.h>
#define _CRT_SECURE_NO_WARNINGS
#define MAX_BUFFER_SIZE  4096
/** \brief Split file into some chunks

# SplitFile
before starting to split, check if the the input file is read over. if not, then create a new file. need to check if can write completely at once.
\param char const *filename | splited file name
\param char const * output | output file name
\param size_t size | size of one output file
\return  1 – if input file cannot be open  2 – if output file(s) cannot be open  3 – if malloc fails
*/

int SplitFile(char const *  filename, char const * output, size_t size) {
	/*buffer size*/
	size_t bufferSize;
	/*input file*/
	FILE* file;
	/*output file*/
	FILE* outputFile;
	/*how many buffer should write*/
	size_t currentBufferSize;
	/*char current_file_name[256];*/
	void* readBuffer;
	int file_count = 0,i = 0;
	char a[256];
	printf("split %s into chunks of %d named\n", filename, (int)size);
	/*judge buffer size*/
	bufferSize = size < MAX_BUFFER_SIZE ? size : MAX_BUFFER_SIZE;
	/*open file*/
	file = fopen(filename, "rb");
	if (!file) {
		perror(filename);
		return E_BAD_SOURCE; /* inpute file open error */
	}
	else {

		int total_bytes_read = 0, bytes_read = 0, bytes_written = 0;
		/*current file number*/
		int fileCount = 1;
		int bufferCount;
		/*create read buffer*/
		readBuffer = malloc(bufferSize);
		if (!readBuffer)
		{
			return E_NO_MEMORY;
		}

		do {
			bytes_read = 0;
			bytes_written = 0;
			currentBufferSize = bufferSize;
			/*get how many times should read*/		
			bufferCount = size / currentBufferSize;
			if (size % currentBufferSize)
			{
				bufferCount += 1;
			}
			/*get first piece of current chunk*/
			bytes_read = fread(readBuffer, 1, currentBufferSize, file);
			if (bytes_read == 0)
			{
				fclose(file);
				free(readBuffer);
				return 0;
			}
			/*create new file*/			
			sprintf(a, "%s%04d", output, fileCount);
			outputFile = fopen(a, "wb");
			if (!outputFile) {
				perror(output);
				return E_BAD_DESTINATION; /* output file open error */
			}
			fwrite(readBuffer, 1, bytes_read, outputFile);
			bytes_written += bytes_read;
			total_bytes_read += bytes_read;
			/*read and write the remain*/
			for (i = 1; i < bufferCount; ++i)
			{
				if (i == bufferCount - 1)
				{
					currentBufferSize = size - bytes_written;
				}
				/*read*/
				bytes_read = fread(readBuffer, 1, currentBufferSize, file);
				/*write*/
				fwrite(readBuffer, 1, bytes_read, outputFile);
				bytes_written += bytes_read;
				total_bytes_read += bytes_read;
			}
			fclose(outputFile);
			/*file count plus one*/
			fileCount++;
			file_count++;
		} while (!feof(file));

		fclose(file);
	}
	free(readBuffer);
	return 0;
}


/** \brief Join files into one file

# JoinFiles
for each chunk, read it, put into buffer, and write into ouput file until down. the end reading condition is if read-byte is zero.
\param char char** filenames | joined files' names
\param num_files | how many files need to join
\param char const * output | output file name
\return  1 – if input file(s) cannot be open  2 – if output file cannot be open  3 – if malloc fails
*/
int JoinFiles(char** filenames, int num_files, char const * output) {
	int bufferSize;
	void* readBuffer;
	int bytes_read = 0;
	int i;
	
	FILE* outputFile;
	FILE* file;	

	/*create joined file*/
	outputFile = fopen(output, "wb");
	if (!outputFile) {
		perror(output);
		return E_BAD_DESTINATION; /* output file open error */
	}

	/*create read buffer*/
	bufferSize = 4096;
	readBuffer = malloc(bufferSize);
	if (!readBuffer)
	{
		return E_NO_MEMORY;
	}
	
	for (i = 0; i < num_files; i++)
	{	
		/*open current file*/
		file = fopen(*filenames, "rb");
		if (!file) {
			perror(*filenames);
			return E_BAD_SOURCE; /* inpute file open error */
		}
		do {
			/*read*/
			bytes_read = fread(readBuffer, 1, bufferSize, file);			
			/*write*/
			fwrite(readBuffer, 1, bytes_read, outputFile);
		} while (bytes_read > 0);
		fclose(file);
		/*update file*/
		filenames++;
	}
	fclose(outputFile);
	free(readBuffer);
	return 0;
}

/**
* @file splitter.h
* @author Wang Zhongqiu
* @date 20 Sep 2018
* @brief The header of splitter.c
*
* Accroding to this file, you can split a file into some chunks or join some files into one file.
* Define two functions. Function SplitFile: used to split file into chunks. Function JoinFiles: used to join files into one file.
* Define an error enum. return  1 – if input file(s) cannot be open  2 – if output file(s) cannot be open  3 – if malloc fails  4 - used only in main 5 - not used now
*/
#ifndef SPLITTER_H
#define SPLITTER_H
#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

	/* error enum
	detiles: 1 – if input file(s) cannot be open  2 – if output file(s) cannot be open  3 – if malloc fails  4 - used only in main 5 - not used now*/
	enum
	{
		E_BAD_SOURCE = 1, E_BAD_DESTINATION, E_NO_MEMORY, E_NO_ACTION, E_SMALL_SIZE
	};

	/** \brief Split file into some chunks

	# SplitFile
	Parameter filename is the file name that you want to splite .
	Parameter output is the file name that you will get. You may get output_0001, output_0002....
	Parameter size is the size of ouput file.
	If not success, you will get a not-zero integer. Detiles see return and error enum.

	\param char const *filename | splited file name
	\param char const * output | output file name
	\param size_t size | size of one output file
	\return  1 – if input file cannot be open  2 – if output file(s) cannot be open  3 – if malloc fails

	*/
	int SplitFile(char const *  filename, char const * output, size_t size);

	/** \brief Join files into one file

	# JoinFiles
	Parameter filenames is the array of file names that you want to join .
	Parameter num_files is the number of joined files.
	Parameter output is the file name that you will get.	
	If not success, you will get a not-zero integer. Detiles see return and error enum.

	\param char char** filenames | joined files' names
	\param num_files | how many files need to join
	\param char const * output | output file name
	\return  1 – if input file(s) cannot be open  2 – if output file cannot be open  3 – if malloc fails
	*/
	int JoinFiles(char** filenames, int num_files, char const * output);


#ifdef __cplusplus
}
#endif/*__cplusplus*/
#endif 


#include <iostream>  /* printf */
#include <cstdio>  /* printf */
#include <cstring> /* strstr,strlen */
#include <cstdlib> /* atoi for C++ compiler */
#include "splitter.h"

int     test_chunks( char * in, char * out, int size );
int     test_combined( char * combined, char ** chunks, int num_files );
int     filesize( char * filename );
char ** find_files( char * prefix, int* num_files );
void    clean_filenames( char ** pp, int num_files );

void test0(void);
void test1(void);
void test2(void);
void test3(void);
void test4(void);
void test5(void);
void test6(void);
void test7(void);
void test8(void);
void test9(void);
void test10(void);

int filesize( char * filename ) {
    FILE * F = fopen(filename,"rb");
    int size;
    fseek(F, 0L, SEEK_END);
    size = ftell( F );
    fclose( F );
    return size;
}

void clean_filenames( char ** pp, int num_files ) {
    char ** head = pp;
    int i = 0;
    for ( ; i<num_files; ++i ) { 
        /*printf( "clean --> %s\n", pp[i] );*/
        free( pp[i] );
    }
    free( head );
}

char ** find_files( char * prefix, int* num_files ) {
    char ** pp_filenames;
    int count = 0, i = 0, stop = 0;
    char filename[256];

    while ( !stop ) {
		FILE * OUT;
        sprintf(filename,"%s%04u",prefix,(unsigned)++count);
		OUT = fopen( filename, "rb" );
        /* does file exist? */
        if ( !OUT ) {
            /*printf("NO %s\n",filename);*/
            --count;
            stop=1;
        } else {
            /*printf("found %s\n",filename);*/
	        fclose( OUT );
		}
    }

    pp_filenames = (char**) malloc( (count+1) * sizeof( char* ) );
    while ( i<count ) {
        sprintf(filename,"%s%04u",prefix,(unsigned)(i+1));
        pp_filenames[i] = (char*) malloc( strlen(filename)+1 );
        strcpy( pp_filenames[i], filename );
        ++i;
    }

    *num_files = count;
    /*printf("found %i files\n",*num_files);*/

    pp_filenames[count] = NULL;
    return pp_filenames;
}

int test_chunks( char * in, char * out, int size ) {
    size_t file_count = 0;
    FILE *IN=fopen(in,"rb"), *OUT;
    char outfile_name[256], *buffer = (char*)malloc( 2*size );
    int size_in;
    int pos_in = 0;

    fseek(IN, 0L, SEEK_END);
    size_in = ftell( IN );
    fseek(IN, 0L, SEEK_SET);

    printf( "+++Input file \"%s\" size is %d\n", in, size_in);

    /* try to open chunks */
    while( pos_in < size_in ) {
        int i = 0;
        if ( size_in-pos_in < size ) { /* for the last chunk */
            size = size_in-pos_in;
        }
        sprintf(outfile_name,"%s%04u",out,(unsigned)++file_count);
        printf( "+++Trying to open file \"%s\" ", outfile_name );
        OUT = fopen( outfile_name, "rb" );
        /* does file exist? */
        if ( !OUT ) { 
            printf( "FAILED\n" );
            free( buffer ); fclose( IN ); return 1; 
        } else {
            printf( "OK\n" );
        }
        /* is it the right size? */
        fseek(OUT, 0L, SEEK_END);
        printf( "+++    checking chunk size, should be %d ", size );
        if ( size != ftell(OUT) ) { 
            printf( "FAILED\n" );
            free( buffer ); fclose( OUT ); fclose( IN ); return 2; 
        } else {
            printf( "OK\n" );
        }
        /* is the content correct? */
        printf( "+++    checking the content of the chunk " );
        fseek(OUT, 0L, SEEK_SET);
        pos_in += fread(buffer,     1,size,IN); 
        fread(buffer+size,1,size,OUT);
        fclose( OUT );
        for ( ; i<size; ++i ) {
            if ( buffer[i] != buffer[i+size] ) {
                printf( "FAILED at byte %d\n+++ should be %c, read %c", i, buffer[i], buffer[i+size] );
                free( buffer ); 
                fclose( IN ); 
                return 3;
            }
        }
        printf( "OK\n" );


    }
    /* check no extra chunks exist */
    sprintf(outfile_name,"%s%04u",out,(unsigned)++file_count);
    printf( "+++Trying to open file \"%s\" ", outfile_name );
    OUT = fopen( outfile_name, "rb" );
    /* does file exist? */
    if ( OUT ) { 
        printf( "extra file exists - FAILED\n" );
        free( buffer ); 
        fclose( IN ); 
        fclose( OUT ); 
        return 4; 
    } else {
        printf( "no extra file - OK\n" );
    }

    free( buffer ); 
    fclose( IN ); 
    return 0;
}

int test_combined( char * combined, char ** pp, int num_files ) {
    FILE * COMBINED=fopen( combined,"rb" );
    int size_combined, pos_combined = 0, i = 0;

    if ( !COMBINED ) {
        printf( "+++FAILED to open combined file \"%s\"\n", combined );
        return 1;
    }

    /* size of combined */
    fseek(COMBINED, 0L, SEEK_END);
    size_combined = ftell( COMBINED );
    fseek(COMBINED, 0L, SEEK_SET);

    for( ; i<num_files; ++i ) {
        /* read the whole chunk, read same number of bytes from combined */
        FILE * CHUNK = fopen( pp[i], "rb" );
        int   size_chunk, j;
        char * buffer;
        printf( "+++Open chunk \"%s\" ", pp[i] );
        if ( !CHUNK ) {
            printf( "FAILED\n" );
            fclose( COMBINED );
            return 2;
        } else {
            printf( "OK\n" );
        }

        /* size of chunk */
        fseek( CHUNK, 0L, SEEK_END);
        size_chunk = ftell( CHUNK );
        fseek( CHUNK, 0L, SEEK_SET);
        buffer = (char*) malloc( 2*size_chunk );

        fread( buffer,     1, size_chunk, CHUNK ); 
        fread( buffer+size_chunk, 1, size_chunk, COMBINED );
        fclose( CHUNK ); 
        pos_combined += size_chunk;

        for ( j=0; j<size_chunk; ++j ) {
            if ( buffer[j] != buffer[j+size_chunk] ) {
                printf( "+++FAILED at byte %d\n should be %c, read %c", j, buffer[j], buffer[j+size_chunk] );
                free( buffer ); 
                fclose( COMBINED ); 
                return 3;
            }
        }

        free( buffer ); 
        printf( "+++  chunk and joined file match\n" );
    }
   
    fclose( COMBINED );
  
    if ( pos_combined != size_combined ) {
        printf( "Combined file size is not the sum of the chunk sizes\n" );
        return 5;
    }

    return 0;
}

void test0(void) {
    char  in  []= "input0";
    char  out []= "student_test0_";
    int size   = 100;
    SplitFile( in,out,size );
    test_chunks(in,out,size  );
}

void test1(void) {
    char in  []= "input1";
    char out []= "student_test1_";
    int size   = 100;
    SplitFile( in,out,size );
    test_chunks(in,out,size  );
}

void test2(void) { /* 20000 */
    char in  []= "input2";
    char out []= "student_test2_";
    int size   = 10000;
    SplitFile( in,out,size );
    test_chunks(in,out,size  );
}

void test3(void) { /* 21000 */
    char in  []= "input3";
    char out []= "student_test3_";
    int size   = 10000;
    SplitFile( in,out,size );
    test_chunks(in,out,size  );
}

void test4(void) { /* 21000 */
    char in  []= "input4";
    char out []= "student_test4_";
    int size   = 100000;
    SplitFile( in,out,size );
    test_chunks(in,out,size  );
}

/* join tests */
void test5(void) { 
    char combined  []= "student_combined5";
    char chunks    []= "input5_";
    int    num_files;
    char** pp = find_files( chunks, &num_files );
    JoinFiles( pp, num_files, combined );
    test_combined( combined, pp, num_files ) ;
    clean_filenames( pp, num_files );
}

void test6(void) { 
    char combined  []= "student_combined6";
    char chunks    []= "input6_";
    int    num_files;
    char** pp = find_files( chunks, &num_files );
    JoinFiles( pp, num_files, combined );
    test_combined( combined, pp, num_files ) ;
    clean_filenames( pp, num_files );
}

/* bigger test */
void test7(void) {
    {
        char in  []= "may.jpg";
        char out []= "student_test7_";
        int size   = 10000;
        SplitFile( in,out,size );
        test_chunks(in,out,size  );
    }
    
    {
        char combined  []= "student_may.jpg";
        char chunks    []= "student_test7_";
        int    num_files;
        char** pp = find_files( chunks, &num_files );
        JoinFiles( pp, num_files, combined );
        test_combined( combined, pp, num_files ) ;
        clean_filenames( pp, num_files );
    }
}

/* huge chunk size - use for mem test */
void test8(void) { 
    char in  []= "input8";
    char out []= "student_test8_";
    int size   = 1<<31;
    SplitFile( in,out,size );
    test_chunks( in, out, filesize( in ) );
}

/* input does not exist - use for mem test */
void test9(void) { 
    char in  []= "input9"; /* no such file */
    char out []= "student_test9_";
    int size   = 10;
    if ( SplitFile( in,out,size ) == E_BAD_SOURCE ) {
        printf( "+++OK - returned E_BAD_SOURCE\n" );
    } else {
        printf( "+++FAILED - should have returned E_BAD_SOURCE\n" );
    }
}

/* input chunk does not exist - use for mem test */
void test10(void) { 
    char combined  []= "student_combined10";
    char chunks    []= "nosuchfiles_";
    char** pp_filenames = (char**) malloc( sizeof( char* ) );
    pp_filenames[0] = (char*) malloc( strlen(chunks)+1 );
    strcpy( pp_filenames[0], chunks );
    
    if ( JoinFiles( pp_filenames, 1, combined ) == E_BAD_SOURCE ) {
        printf( "+++OK - returned E_BAD_SOURCE\n" );
    } else {
        printf( "+++FAILED - should have returned E_BAD_SOURCE\n" );
    }

    free( *pp_filenames );
    free( pp_filenames );
}

/* stress test */
void test11(void) {
    {
        char * in  = "../../large.file.data"; /*  */
        char * out = "student_test11_";
        int size   = 1<<24; /* 16 megabytes */
        SplitFile( in,out,size );
        test_chunks(in,out,size  );
    }
    
    {
        char * combined  = "student_may.jpg";
        char * chunks    = "student_test11_";
        int    num_files;
        char** pp = find_files( chunks, &num_files );
        JoinFiles( pp, num_files, combined );
        test_combined( combined, pp, num_files ) ;
        clean_filenames( pp, num_files );
    }
}

void (*pTests[])(void) = { 
    test0, test1, test2, test3, test4, test5, test6, test7, 
    test8, test9, test10, test11
};

int main(int argc, char *argv[] ) {
    if (argc >1) {
        int test = 0;
        std::sscanf(argv[1],"%i",&test);
        try {
            pTests[test]();
        } catch( const char* msg) {
            std::cerr << msg << std::endl;
        }
    }

    return 0;
}

#ifndef _DBG_UTIL_
#define _DBG_UTIL_
//=============================================================================
// �f�o�b�O�p���[�e�B���e�B�W
//=============================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined __cplusplus
extern "C" {
#endif

//=============================================================================
// DebugPrint
//=============================================================================
#if DEBUG
  #define DebugPrint(args) printf("***[DEBUG]%s(%d) ", __FILE__, __LINE__); printf args
#else
  #define DebugPrint(x)
#endif


//=============================================================================
// �������֘A���[�e�B���e�B�W
//		�����̃c�[���ŏo�͂���铝�v���͂P�v���O�����ɂ����Ă̓��v���ł��B
//		  �P�֐����Ƃ̓��v�ł͂Ȃ��̂ŁA���ʂ̌����ɒ��ӁI
//=============================================================================
#if DEBUG
	void dbg_print_alloc_count(FILE * fp);
	void dbg_print_alloc_block(FILE * fp, const char * file, int line);
	void dbg_print_all_alloc_block(FILE * fp);
	
	void *dbg_malloc(size_t size, const char * file, int line);
	void *dbg_calloc(size_t number, size_t size, const char * file, int line);
	void *dbg_realloc(void * ptr, size_t size, const char * file, int line);
	char *dbg_strdup(const char * str, const char * file, int line);
	void  dbg_free(void * ptr, const char * file, int line);

	#ifndef DBG_MALLOC_NOCHANGE
		#define malloc(s)     dbg_malloc((s),__FILE__,__LINE__)
		#define calloc(n,s)   dbg_calloc((n),(s),__FILE__,__LINE__)
		#define realloc(p,s)  dbg_realloc((p),(s),__FILE__,__LINE__)
		#define strdup(str)   dbg_strdup((str),__FILE__,__LINE__)
		#define free(p)       dbg_free((p),__FILE__,__LINE__)
	#endif

#else
	#define dbg_print_alloc_count(x)
	#define dbg_print_alloc_block(x)
	#define dbg_print_all_alloc_block(x)

#endif	//DEBUG


#if defined __cplusplus
}
#endif

#endif	//_DBG_UTIL_

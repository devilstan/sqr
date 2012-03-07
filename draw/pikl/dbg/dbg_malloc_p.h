#ifndef _DBG_MALLOC_P_H_INCLUDED_
#define _DBG_MALLOC_P_H_INCLUDED_

#define DBG_MALLOC_NOCHANGE

#include <time.h>
#include "dbg.h"

/*
 * ソース中で dbg_malloc() を利用している場所ごとに作成される構造体
 */
struct memory_count {
  const char * file;                /* ファイル名 */
  int line;                         /* 行番号 */
  int number;                       /* 現在獲得されている領域の個数 */
  unsigned int alloc_count;         /* 過去に獲得された領域の総数 */
  unsigned int free_count;          /* 過去に解放された領域の総数 */
  int size;                         /* 現在獲得されている領域の総サイズ */
  unsigned int alloc_size;          /* 過去に獲得された領域の総サイズ */
  unsigned int free_size;           /* 過去に解放された領域の総サイズ */
  struct memory_block * block_list; /* 現在獲得している領域のリスト */
  struct free_count * free_list;    /* 過去に解放した領域のリスト */
  struct memory_count * prev;       /* 前の struct memory_count へのポインタ */
  struct memory_count * next;       /* 次の struct memory_count へのポインタ */
};

/*
 * メモリ管理用ブロック．１回の dbg_malloc() 呼び出しごとに１個作成され，
 * 対応する memory_count の block_list メンバに接続される．
 */
struct memory_block {
  int id;   /* ID番号．メモリ領域ごとに固有の値が入る */
  int size; /* 獲得サイズ */
  time_t t; /* 獲得された時刻 */
  struct memory_count * mcp;  /* バックポインタ */
  struct memory_block * prev; /* struct memory_count の block_list のリンク */
  struct memory_block * next; /* struct memory_count の block_list のリンク */
  struct memory_block * hash_prev; /* ハッシュ用のリンク */
  struct memory_block * hash_next; /* ハッシュ用のリンク */
  void * data; /* malloc() によって確保されたメモリ領域へのポインタ */
};

/*
 * free() 管理用のブロック．
 * どの malloc() がどこの free() で解放されたかの統計情報を管理する．
 * 対応する memory_count の free_list メンバに接続される．
 */
struct free_count {
  const char * file;        /* ファイル名 */
  int line;                 /* 行番号 */
  unsigned int count;       /* 過去に解放された領域の個数 */
  unsigned int size;        /* 過去に解放された領域の総サイズ */
  struct free_count * next; /* 次の struct free_count へのポインタ */
};

/* malloc() 統計情報 */
extern unsigned int dbg_malloc_count;  /* malloc() 呼び出し回数 */
extern unsigned int dbg_free_count;    /* free() 呼び出し回数 */
extern unsigned int dbg_malloc_number; /* 現在のメモリ取得数 */
extern unsigned int dbg_malloc_size;   /* 現在のメモリ取得量 */

extern struct memory_count * dbg_memory_count_head; /* リストの先頭 */
extern struct memory_count * dbg_memory_count_tail; /* リストの終端 */

/*
 * struct memory_block 検索用のハッシュ．
 * アドレスはたぶん４バイトアラインメントされてるので，下２桁は
 * 必ず４の倍数になってしまう．ハッシュ数，ハッシュ関数には注意すること．
 */
#define DBG_HASH_NUM 997 /* ハッシュなので素数にする */
#define DBG_HASH_FUNC(p) \
	(( \
	(((unsigned long int)(p) >> 4) & 0xf) + \
	((unsigned long int)(p) >> 8) \
	) % DBG_HASH_NUM)
/* ハッシュ用配列 */
extern struct memory_block * dbg_memory_block_hash[DBG_HASH_NUM];

void dbg_init(); /* 初期化用関数 */

#endif /* _DBG_MALLOC_P_H_INCLUDED_ */

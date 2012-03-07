#ifndef _DBG_MALLOC_P_H_INCLUDED_
#define _DBG_MALLOC_P_H_INCLUDED_

#define DBG_MALLOC_NOCHANGE

#include <time.h>
#include "dbg.h"

/*
 * ��������� dbg_malloc() �����Ѥ��Ƥ����ꤴ�Ȥ˺�������빽¤��
 */
struct memory_count {
  const char * file;                /* �ե�����̾ */
  int line;                         /* ���ֹ� */
  int number;                       /* ���߳�������Ƥ����ΰ�θĿ� */
  unsigned int alloc_count;         /* ���˳������줿�ΰ����� */
  unsigned int free_count;          /* ���˲������줿�ΰ����� */
  int size;                         /* ���߳�������Ƥ����ΰ�������� */
  unsigned int alloc_size;          /* ���˳������줿�ΰ�������� */
  unsigned int free_size;           /* ���˲������줿�ΰ�������� */
  struct memory_block * block_list; /* ���߳������Ƥ����ΰ�Υꥹ�� */
  struct free_count * free_list;    /* ���˲��������ΰ�Υꥹ�� */
  struct memory_count * prev;       /* ���� struct memory_count �ؤΥݥ��� */
  struct memory_count * next;       /* ���� struct memory_count �ؤΥݥ��� */
};

/*
 * ��������ѥ֥�å�������� dbg_malloc() �ƤӽФ����Ȥˣ��ĺ������졤
 * �б����� memory_count �� block_list ���Ф���³����롥
 */
struct memory_block {
  int id;   /* ID�ֹ桥�����ΰ褴�Ȥ˸�ͭ���ͤ����� */
  int size; /* ���������� */
  time_t t; /* �������줿���� */
  struct memory_count * mcp;  /* �Хå��ݥ��� */
  struct memory_block * prev; /* struct memory_count �� block_list �Υ�� */
  struct memory_block * next; /* struct memory_count �� block_list �Υ�� */
  struct memory_block * hash_prev; /* �ϥå����ѤΥ�� */
  struct memory_block * hash_next; /* �ϥå����ѤΥ�� */
  void * data; /* malloc() �ˤ�äƳ��ݤ��줿�����ΰ�ؤΥݥ��� */
};

/*
 * free() �����ѤΥ֥�å���
 * �ɤ� malloc() ���ɤ��� free() �ǲ������줿�������׾����������롥
 * �б����� memory_count �� free_list ���Ф���³����롥
 */
struct free_count {
  const char * file;        /* �ե�����̾ */
  int line;                 /* ���ֹ� */
  unsigned int count;       /* ���˲������줿�ΰ�θĿ� */
  unsigned int size;        /* ���˲������줿�ΰ�������� */
  struct free_count * next; /* ���� struct free_count �ؤΥݥ��� */
};

/* malloc() ���׾��� */
extern unsigned int dbg_malloc_count;  /* malloc() �ƤӽФ���� */
extern unsigned int dbg_free_count;    /* free() �ƤӽФ���� */
extern unsigned int dbg_malloc_number; /* ���ߤΥ�������� */
extern unsigned int dbg_malloc_size;   /* ���ߤΥ�������� */

extern struct memory_count * dbg_memory_count_head; /* �ꥹ�Ȥ���Ƭ */
extern struct memory_count * dbg_memory_count_tail; /* �ꥹ�Ȥν�ü */

/*
 * struct memory_block �����ѤΥϥå��塥
 * ���ɥ쥹�Ϥ��֤󣴥Х��ȥ��饤����Ȥ���Ƥ�Τǡ��������
 * ɬ�������ܿ��ˤʤäƤ��ޤ����ϥå�������ϥå���ؿ��ˤ���դ��뤳�ȡ�
 */
#define DBG_HASH_NUM 997 /* �ϥå���ʤΤ��ǿ��ˤ��� */
#define DBG_HASH_FUNC(p) \
	(( \
	(((unsigned long int)(p) >> 4) & 0xf) + \
	((unsigned long int)(p) >> 8) \
	) % DBG_HASH_NUM)
/* �ϥå��������� */
extern struct memory_block * dbg_memory_block_hash[DBG_HASH_NUM];

void dbg_init(); /* ������Ѵؿ� */

#endif /* _DBG_MALLOC_P_H_INCLUDED_ */

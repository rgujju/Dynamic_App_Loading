/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: /usr/bin/gperf --output-file kobject_hash_preprocessed.c kobject_hash.gperf  */
/* Computed positions: -k'1-2' */

#line 6 "kobject_hash.gperf"

#include <kernel.h>
#include <toolchain.h>
#include <syscall_handler.h>
#include <string.h>
#line 12 "kobject_hash.gperf"
struct z_object;
static u8_t Z_GENERIC_SECTION(.priv_stacks.noinit)  __aligned(Z_PRIVILEGE_STACK_ALIGN) priv_stacks[4][CONFIG_PRIVILEGED_STACK_SIZE];
static struct z_stack_data stack_data[4] = {
	{ 2048, (u8_t *)(&priv_stacks[0]) },
	{ 1024, (u8_t *)(&priv_stacks[1]) },
	{ 1024, (u8_t *)(&priv_stacks[2]) },
	{ 512, (u8_t *)(&priv_stacks[3]) }
};

#define TOTAL_KEYWORDS 18
#define MIN_WORD_LENGTH 4
#define MAX_WORD_LENGTH 4
#define MIN_HASH_VALUE 0
#define MAX_HASH_VALUE 55
/* maximum key range = 56, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
/*ARGSUSED*/
static unsigned int
hash (register const char *str, register size_t len)
{
  static unsigned char asso_values[] =
    {
       5, 30, 25, 25, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      23, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 18, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      18, 56, 56, 56, 13, 56, 56, 56, 56, 56,
      56, 56, 20, 56, 56, 56,  8, 56, 56, 56,
       3, 56, 56, 56, 56, 56,  0, 10, 30, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 25, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      20, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 25, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 15, 56, 56, 56, 15, 56,
      56, 56, 25, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56,  0, 56, 56, 56, 56, 56,
      56, 56,  0, 56, 56, 56
    };
  return asso_values[(unsigned char)str[1]] + asso_values[(unsigned char)str[0]];
}

static unsigned char lengthtable[] =
  {
     4,  0,  0,  0,  0,  4,  0,  0,  4,  0,  4,  0,  0,  4,
     0,  4,  0,  0,  4,  0,  4,  0,  0,  4,  0,  4,  0,  0,
     4,  0,  4,  0,  0,  4,  0,  4,  0,  0,  0,  0,  4,  0,
     0,  0,  0,  4,  0,  0,  0,  0,  4,  0,  0,  0,  0,  4
  };

static struct z_object wordlist[] =
  {
#line 33 "kobject_hash.gperf"
    {"\364V\000 ", {}, K_OBJ_DRIVER_UART, 0 | K_OBJ_FLAG_DRIVER, { .unused = 0 }},
    {""}, {""}, {""}, {""},
#line 24 "kobject_hash.gperf"
    {"\374\000\000 ", {}, K_OBJ_THREAD, 0, { .thread_id = 1 }},
    {""}, {""},
#line 31 "kobject_hash.gperf"
    {"\000P\000 ", {}, K_OBJ_THREAD_STACK_ELEMENT, 0, { .stack_data = &stack_data[3] }},
    {""},
#line 21 "kobject_hash.gperf"
    {"\000\000\000 ", {}, K_OBJ_TIMER, 0, { .unused = 0 }},
    {""}, {""},
#line 30 "kobject_hash.gperf"
    {"\000L\000 ", {}, K_OBJ_THREAD_STACK_ELEMENT, 0, { .stack_data = &stack_data[2] }},
    {""},
#line 32 "kobject_hash.gperf"
    {"\344V\000 ", {}, K_OBJ_DRIVER_UART, 0 | K_OBJ_FLAG_DRIVER, { .unused = 0 }},
    {""}, {""},
#line 28 "kobject_hash.gperf"
    {"\000@\000 ", {}, K_OBJ_THREAD_STACK_ELEMENT, 0, { .stack_data = &stack_data[0] }},
    {""},
#line 23 "kobject_hash.gperf"
    {"\340\000\000 ", {}, K_OBJ_MEM_SLAB, 0, { .unused = 0 }},
    {""}, {""},
#line 22 "kobject_hash.gperf"
    {"<\000\000 ", {}, K_OBJ_THREAD, 0, { .thread_id = 0 }},
    {""},
#line 29 "kobject_hash.gperf"
    {"\000H\000 ", {}, K_OBJ_THREAD_STACK_ELEMENT, 0, { .stack_data = &stack_data[1] }},
    {""}, {""},
#line 35 "kobject_hash.gperf"
    {"$W\000 ", {}, K_OBJ_DRIVER_GPIO, 0 | K_OBJ_FLAG_DRIVER, { .unused = 0 }},
    {""},
#line 36 "kobject_hash.gperf"
    {"HW\000 ", {}, K_OBJ_SEM, 0 | K_OBJ_FLAG_INITIALIZED, { .unused = 0 }},
    {""}, {""},
#line 34 "kobject_hash.gperf"
    {"\024W\000 ", {}, K_OBJ_DRIVER_GPIO, 0 | K_OBJ_FLAG_DRIVER, { .unused = 0 }},
    {""},
#line 38 "kobject_hash.gperf"
    {"lW\000 ", {}, K_OBJ_MUTEX, 0 | K_OBJ_FLAG_INITIALIZED, { .unused = 0 }},
    {""}, {""}, {""}, {""},
#line 37 "kobject_hash.gperf"
    {"XW\000 ", {}, K_OBJ_MUTEX, 0 | K_OBJ_FLAG_INITIALIZED, { .unused = 0 }},
    {""}, {""}, {""}, {""},
#line 27 "kobject_hash.gperf"
    {"x\003\000 ", {}, K_OBJ_THREAD, 0, { .thread_id = 2 }},
    {""}, {""}, {""}, {""},
#line 26 "kobject_hash.gperf"
    {"\350\002\000 ", {}, K_OBJ_THREAD, 0, { .thread_id = 3 }},
    {""}, {""}, {""}, {""},
#line 25 "kobject_hash.gperf"
    {"\220\001\000 ", {}, K_OBJ_MEM_SLAB, 0, { .unused = 0 }}
  };

struct z_object *
z_object_lookup (register const char *str, register size_t len)
{
  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        if (len == lengthtable[key])
          {
            register const char *s = wordlist[key].name;

            if (*str == *s && !memcmp (str + 1, s + 1, len - 1))
              return &wordlist[key];
          }
    }
  return 0;
}
#line 39 "kobject_hash.gperf"

struct z_object *z_object_gperf_find(void *obj)
{
    return z_object_lookup((const char *)obj, sizeof(void *));
}

void z_object_gperf_wordlist_foreach(_wordlist_cb_func_t func, void *context)
{
    int i;

    for (i = MIN_HASH_VALUE; i <= MAX_HASH_VALUE; i++) {
        if (wordlist[i].name != NULL) {
            func(&wordlist[i], context);
        }
    }
}

#ifndef CONFIG_DYNAMIC_OBJECTS
struct z_object *z_object_find(void *obj)
	ALIAS_OF(z_object_gperf_find);

void z_object_wordlist_foreach(_wordlist_cb_func_t func, void *context)
	ALIAS_OF(z_object_gperf_wordlist_foreach);
#endif

Z_GENERIC_SECTION(.kobject_data.data) u8_t _thread_idx_map[2] = { 0xf0,  0xff, };

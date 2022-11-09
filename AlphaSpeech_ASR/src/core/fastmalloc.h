#ifndef _FAST_MALLOC_H_
#define _FAST_MALLOC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <malloc.h>

#define FAST_MALLOC_ALIGN 16
#define FAST_MALLOC_OVERREAD 64

#define FAST_MALLOC_METHOD 0  // 0, 1, 2

static inline unsigned char **alignPtr(unsigned char **ptr, int n) {
    return (unsigned char **)(((size_t)ptr + n - 1) & -n);
}

static inline size_t alignSize(size_t sz, int n) {
    return (sz + n - 1) & -n;
}

static inline void *fastMalloc(size_t size) {
    #if FAST_MALLOC_METHOD == 0
    void *ptr = 0;
    if (posix_memalign(&ptr, FAST_MALLOC_ALIGN, size + FAST_MALLOC_OVERREAD))
        ptr = 0;
    return ptr;

    #elif FAST_MALLOC_METHOD == 1
    return memalign(FAST_MALLOC_ALIGN, size + FAST_MALLOC_OVERREAD);

    #else
    unsigned char *udata = (unsigned char *)malloc(size + sizeof(void *) + FAST_MALLOC_ALIGN + FAST_MALLOC_OVERREAD);
    if (!udata)
        return 0;
    unsigned char **adata = alignPtr((unsigned char **)udata + 1, FAST_MALLOC_ALIGN);
    adata[-1] = udata;
    return adata;
    
    #endif
}

static inline void fastFree(void *ptr) {
    if (ptr) {
        #if FAST_MALLOC_METHOD == 0
        free(ptr);
        
        #elif FAST_MALLOC_METHOD == 1
        free(ptr);

        #else
        unsigned char *udata = ((unsigned char **)ptr)[-1];
        free(udata);
        
        #endif
    }
}


#ifdef __cplusplus
}
#endif

#endif
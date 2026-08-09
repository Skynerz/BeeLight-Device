#include <lvgl.h>
#include <stdlib.h>

/* LV MEM */
void lv_mem_init(void)
{
    return; /*Nothing to init*/
}

void lv_mem_deinit(void)
{
    return; /*Nothing to deinit*/
}

void * lv_malloc_core(size_t size)
{
    return malloc(size);
}

void * lv_realloc_core(void * p, size_t new_size)
{
    return realloc(p, new_size);
}

void lv_free_core(void * p)
{
    free(p);
}

void lv_mem_monitor_core(lv_mem_monitor_t *mon_p)
{
    /*Not supported*/
    LV_UNUSED(mon_p);
    return;
}

lv_result_t lv_mem_test_core(void)
{
    /*Not supported*/
    return LV_RESULT_OK;
}
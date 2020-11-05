#include <assert.h>
#include <stdint.h>
#include <setjmp.h>
#include <sys/time.h>

static jmp_buf escape;

static void test_abort(void)
{
	longjmp(escape, 1);
}

#define BUF_ABORT test_abort
#include "buf.h"

static uint32_t pcg32(uint64_t *s)
{
    uint64_t m = 0x5851f42d4c957f2d;
    uint64_t a = 0x8b260b70b8e98891;
    uint64_t p = *s;
    uint32_t x = ((p >> 18) ^ p) >> 27;
    uint32_t r = p >> 59;
    *s = p * m + a;
    return (x >> r) | (x << (-r & 31u));
}

static unsigned long bench(uint64_t *rng)
{
    unsigned long r = 0;
    uint32_t n = 1000000 + pcg32(rng) % 4000000;
    float *buf = 0;

    for (uint32_t i = 0; i < n; i++)
        buf_push(buf, pcg32(rng) / (double)UINT32_MAX);

    float threshold = pcg32(rng) / (double)UINT32_MAX;
    for (uint32_t i = 0; i < n; i++)
        r += buf[i] > threshold;

    buf_free(buf);
    return r;
}

static uint64_t uepoch(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return 1000000LL * tv.tv_sec + tv.tv_usec;
}

static void test_benchmark()
{
	uint64_t rng = 0x558d2750ae0e0887; 
	unsigned long r = 0;
	uint64_t start = uepoch();
	for (int i = 0; i < 300; i++)
		r += bench(&rng);
	double t = (uepoch() - start) / 1e6;
	assert(t < 15);	
	assert(r == 428859598); 
}

static void test_buffer_init()
{
	float *a = NULL;
    assert(buf_capacity(a) == 0);
	assert(buf_size(a) == 0);
}

static void test_buffer_push_and_free()
{
	float *a = NULL;
	buf_push(a, 1.3f);
	assert(buf_size(a) == 1);
	assert(a[0] == 1.3f);
	buf_free(a);
	assert(a == NULL);
}

static void test_buffer_push()
{
	const int test_size = 10000;
	int* a = NULL;
	for (int i = 0; i < test_size; ++i)
		   buf_push(a, i);
	assert(buf_size(a) == test_size);
	
	int match = 0;
	for (int i = 0; i < test_size; match += a[i] == i, ++i);
	assert(match == test_size);
	buf_free(a);
	assert(a == NULL);
}

static void test_buffer_grow_and_trunc()
{
	const int test_size = 1000;
	int* a = NULL;
	buf_grow(a, test_size);
	assert(buf_capacity(a) == test_size);
	assert(buf_size(a) == 0);
	buf_trunc(a, test_size / 10);
	assert(buf_capacity(a) == test_size / 10);
	buf_free(a);
	assert(a == NULL);
}

static void test_buffer_pop()
{
	float* a = NULL;
	buf_push(a, 1.1);
	buf_push(a, 1.2);
	buf_push(a, 1.3);
	buf_push(a, 1.4);
	assert(buf_size(a) == 4);
	assert(buf_pop(a) == 1.4f);
	buf_trunc(a, 3);
	assert(buf_size(a) == 3);
	assert(buf_pop(a) == 1.3f);
	assert(buf_pop(a) == 1.2f);
	assert(buf_pop(a) == 1.1f);
	assert(buf_size(a) == 0);
	buf_free(a);
	assert(a == NULL);
}

static void test_buffer_clear_empty()
{
	int* a = NULL;
	buf_clear(a);
	assert(a == NULL);
}

static void test_buffer_clear_not_empty()
{
	int* a = NULL;
	buf_push(a, 1);
	assert(a != NULL);
	buf_clear(a);
	assert(buf_size(a) == 0);
	assert(a != NULL);
}

static void test_buffer_grow_mem_failure()
{
	int* volatile p = 0;
	if (!setjmp(escape)) {
		size_t max = (PTRDIFF_MAX - sizeof(struct buf)) / sizeof(*p) + 1;
		buf_grow(p, max);
		buf_grow(p, max);
		assert(0);
	}
	buf_free(p);
}

static void test_buffer_trunc_mem_failure()
{
	int* volatile p = 0;
	if (!setjmp(escape)) {
		buf_trunc(p, PTRDIFF_MAX);
		assert(0);
	}
	buf_free(p);
}

static void test_buffer_two_trunc_mem_failure()
{
	int* volatile p = 0;
	if (!setjmp(escape)) {
		buf_trunc(p, 1);
		buf_trunc(p, PTRDIFF_MAX);
		assert(0);
	}
	buf_free(p);
}

typedef void (*test_t)(void);

int main(int argc, char** argv)
{
	test_t tests[] = {
		test_buffer_init,
		test_buffer_push_and_free,
		test_buffer_push,
		test_buffer_clear_empty,
		test_buffer_clear_not_empty,
		test_buffer_pop,
		test_buffer_grow_and_trunc,
		test_buffer_grow_mem_failure,
		test_buffer_trunc_mem_failure,
		test_buffer_two_trunc_mem_failure,
		test_benchmark
	};

	for (int i = 1; i < argc; ++i)
	{
		int test_num = atoi(argv[i]);
		assert(test_num < sizeof(tests) / sizeof(test_t));
		tests[test_num]();
	}
	return 0;
}

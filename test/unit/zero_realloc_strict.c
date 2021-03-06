#include "test/jemalloc_test.h"

static uint64_t
allocated() {
	if (!config_stats) {
		return 0;
	}
	uint64_t allocated;
	size_t sz = sizeof(allocated);
	assert_d_eq(mallctl("thread.allocated", (void *)&allocated, &sz, NULL,
	    0), 0, "Unexpected mallctl failure");
	return allocated;
}

static uint64_t
deallocated() {
	if (!config_stats) {
		return 0;
	}
	uint64_t deallocated;
	size_t sz = sizeof(deallocated);
	assert_d_eq(mallctl("thread.deallocated", (void *)&deallocated, &sz,
	    NULL, 0), 0, "Unexpected mallctl failure");
	return deallocated;
}

TEST_BEGIN(test_realloc_strict) {
	void *ptr = mallocx(1, 0);
	assert_ptr_not_null(ptr, "Unexpected mallocx error");
	uint64_t allocated_before = allocated();
	uint64_t deallocated_before = deallocated();
	ptr = realloc(ptr, 0);
	uint64_t allocated_after = allocated();
	uint64_t deallocated_after = deallocated();
	if (config_stats) {
		assert_u64_lt(allocated_before, allocated_after,
		    "Unexpected stats change");
		assert_u64_lt(deallocated_before, deallocated_after,
		    "Unexpected stats change");
	}
	dallocx(ptr, 0);
}
TEST_END
int
main(void) {
	return test(
	    test_realloc_strict);
}

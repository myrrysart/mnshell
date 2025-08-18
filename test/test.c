#define MUNIT_ENABLE_ASSERT_ALIASES
#include "lexer.h"
#include "arena.h"
#include "munit.h"
#include <stdio.h>

static MunitResult test_lexer(const MunitParameter params[], void* data)
{
	(void)params;
	(void)data;

	char *str = "hello";
	t_lexer l = build_lexer(str);
	t_token *t = build_token_list(&l);
	
	munit_assert_int(strlen(str), ==, 5);
	assert_string_equal(t->text, "hello");
	assert_int(t->type, ==, WORD);
	assert_int(t->text_len, ==, 5);
	arena_free(get_static_arena());
	return MUNIT_OK;
}

static MunitTest test_suite_tests[] = {
	{
		(char *) "/test/lexer",
		test_lexer,
		NULL,
		NULL,
		MUNIT_TEST_OPTION_NONE,
		NULL,
	},
	{ NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
};

static const MunitSuite test_suite = {
	(char *) "",
	test_suite_tests,
	NULL,
	1,
	MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char **argv)
{
	return munit_suite_main(&test_suite, (void*) "lexer", argc, argv);
}

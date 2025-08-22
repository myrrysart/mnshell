#define MUNIT_ENABLE_ASSERT_ALIASES
#include "lexer.h"
#include "arena.h"
#include "munit.h"

static MunitResult test_lexer_general(const MunitParameter params[], void* data)
{
	(void)params;
	(void)data;

	{
		char *str = "echo hello";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_string_equal(t->text, "echo");
		assert_int(t->type, ==, WORD);
		assert_int(t->text_len, ==, 4);
		t = t->next;
		assert_string_equal(t->text, "hello");
		assert_int(t->type, ==, WORD);
		assert_int(t->text_len, ==, 5);
	}
	{
		char *str = "echo hello | wc -l";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_string_equal(t->text, "echo");
		assert_int(t->type, ==, WORD);
		assert_int(t->text_len, ==, 4);
		t = t->next;
		assert_string_equal(t->text, "hello");
		assert_int(t->type, ==, WORD);
		assert_int(t->text_len, ==, 5);
		t = t->next;
		assert_string_equal(t->text, "|");
		assert_int(t->type, ==, PIPE);
		assert_int(t->text_len, ==, 1);
		t = t->next;
		assert_string_equal(t->text, "wc");
		assert_int(t->type, ==, WORD);
		assert_int(t->text_len, ==, 2);
		t = t->next;
		assert_string_equal(t->text, "-l");
		assert_int(t->type, ==, WORD);
		assert_int(t->text_len, ==, 2);
	}
	return MUNIT_OK;
}

static MunitResult test_lexer_dquote(const MunitParameter params[], void* data)
{
	(void)params;
	(void)data;
	{
		char *str = "\" echo\"";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_string_equal(t->text, "\" echo\"");
		assert_int(t->type, ==, DQUOTE);
		assert_int(t->text_len, ==, 7);
	}
	{
		char *str = "\" echo";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_string_equal(t->text, "\" echo");
		assert_int(t->type, ==, INVALID);
		assert_int(t->text_len, ==, 6);
	}
	{
		char *str = "echo \" hello\"";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_string_equal(t->text, "echo");
		assert_int(t->type, ==, WORD);
		assert_int(t->text_len, ==, 4);
		t = t->next;
		assert_string_equal(t->text, "\" hello\"");
		assert_int(t->type, ==, DQUOTE);
		assert_int(t->text_len, ==, 8);
	}
	return MUNIT_OK;
}
static MunitResult test_lexer_squote(const MunitParameter params[], void* data)
{
	(void)params;
	(void)data;
	{
		char *str = "' echo'";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_string_equal(t->text, "' echo'");
		assert_int(t->type, ==, SQUOTE);
		assert_int(t->text_len, ==, 7);
	}
	{
		char *str = "' echo";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_string_equal(t->text, "' echo");
		assert_int(t->type, ==, INVALID);
		assert_int(t->text_len, ==, 6);
	}
	{
		char *str = "echo ' hello'";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_string_equal(t->text, "echo");
		assert_int(t->type, ==, WORD);
		assert_int(t->text_len, ==, 4);
		t = t->next;
		assert_string_equal(t->text, "' hello'");
		assert_int(t->type, ==, SQUOTE);
		assert_int(t->text_len, ==, 8);
	}
	return MUNIT_OK;
}

static MunitResult test_lexer_dollar(const MunitParameter params[], void* data)
{
	(void)params;
	(void)data;
	{
		char *str = "$USER";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_string_equal(t->text, "$USER");
		assert_int(t->type, ==, DOLLAR);
		assert_int(t->text_len, ==, 5);
	}
	{
		char *str = "$ ";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_string_equal(t->text, "$");
		assert_int(t->type, ==, DOLLAR);
		assert_int(t->text_len, ==, 1);
	}
	return MUNIT_OK;
}
static MunitResult test_lexer_append(const MunitParameter params[], void* data)
{
	(void)params;
	(void)data;
	{
		char *str = "echo >> file";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_string_equal(t->text, "echo");
		assert_int(t->type, ==, WORD);
		assert_int(t->text_len, ==, 4);
		t = t->next;
		assert_string_equal(t->text, ">>");
		assert_int(t->type, ==, APPEND);
		assert_int(t->text_len, ==, 2);
		t = t->next;
		assert_string_equal(t->text, "file");
		assert_int(t->type, ==, WORD);
		assert_int(t->text_len, ==, 4);
	}
	return MUNIT_OK;
}

static MunitResult test_arena(const MunitParameter params[], void* data)
{
	(void)params;
	(void)data;

	t_arena *arena = arena_init(ARENA_CAP);
	assert_ptr_not_equal(arena, NULL);
	assert_ptr_not_equal(arena->data, NULL);
	assert_int(arena->capacity, ==, ARENA_CAP);
	assert_int(arena->cursor, ==, 0);
	arena_free(arena);
	return MUNIT_OK;
}
static MunitTest test_suite_tests[] = {
	{ (char *) "/test/arena", test_arena, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL, },
	{ (char *) "/test/lexer_general", test_lexer_general, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL, },
	{ (char *) "/test/lexer_dquote", test_lexer_dquote, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL, },
	{ (char *) "/test/lexer_squote", test_lexer_squote, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL, },
	{ (char *) "/test/lexer_dollar", test_lexer_dollar, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL, },
	{ (char *) "/test/lexer_append", test_lexer_append, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL, },
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

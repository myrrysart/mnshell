#define MUNIT_ENABLE_ASSERT_ALIASES
#include "minishell.h"
#include "munit.h"

// separated unit test files
#include "test_environment.c"

#include "bash_comparison_test.c"

static MunitResult test_lexer_general(const MunitParameter params[], void* data)
{
	(void)params;
	(void)data;

	{
		char *str = "echo hello";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_string_equal(t->content, "echo");
		assert_int(t->type, ==, WORD);
		assert_int(t->content_len, ==, 4);
		t = t->next;
		assert_string_equal(t->content, "hello");
		munit_assert_ptr_null(t->next);
		assert_int(t->type, ==, WORD);
		assert_int(t->content_len, ==, 5);
	}
	// {
	// 	char *str = "echo    ";
	// 	t_lexer l = build_lexer(str);
	// 	t_token *t = build_token_list(&l);
	// 	assert_string_equal(t->content, "echo");
	// 	munit_assert_ptr_null(t->next);
	// 	assert_int(t->type, ==, WORD);
	// 	assert_int(t->content_len, ==, 4);
	// }
	{
		char *str = "echo hello | wc -l";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_string_equal(t->content, "echo");
		assert_int(t->type, ==, WORD);
		assert_int(t->content_len, ==, 4);
		t = t->next;
		assert_string_equal(t->content, "hello");
		assert_int(t->type, ==, WORD);
		assert_int(t->content_len, ==, 5);
		t = t->next;
		assert_string_equal(t->content, "|");
		assert_int(t->type, ==, PIPE);
		assert_int(t->content_len, ==, 1);
		t = t->next;
		assert_string_equal(t->content, "wc");
		assert_int(t->type, ==, WORD);
		assert_int(t->content_len, ==, 2);
		t = t->next;
		assert_string_equal(t->content, "-l");
		assert_int(t->type, ==, WORD);
		assert_int(t->content_len, ==, 2);
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
		assert_string_equal(t->content, "\" echo\"");
		assert_int(t->type, ==, DQUOTE);
		assert_int(t->content_len, ==, 7);
	}
	{
		char *str = "\" echo";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_string_equal(t->content, "\" echo");
		assert_int(t->type, ==, INVALID);
		assert_int(t->content_len, ==, 6);
	}
	{
		char *str = "echo \" hello\"";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_string_equal(t->content, "echo");
		assert_int(t->type, ==, WORD);
		assert_int(t->content_len, ==, 4);
		t = t->next;
		assert_string_equal(t->content, "\" hello\"");
		assert_int(t->type, ==, DQUOTE);
		assert_int(t->content_len, ==, 8);
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
		assert_string_equal(t->content, "' echo'");
		assert_int(t->type, ==, SQUOTE);
		assert_int(t->content_len, ==, 7);
	}
	{
		char *str = "' echo";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_string_equal(t->content, "' echo");
		assert_int(t->type, ==, INVALID);
		assert_int(t->content_len, ==, 6);
	}
	{
		char *str = "echo ' hello'";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_string_equal(t->content, "echo");
		assert_int(t->type, ==, WORD);
		assert_int(t->content_len, ==, 4);
		t = t->next;
		assert_string_equal(t->content, "' hello'");
		assert_int(t->type, ==, SQUOTE);
		assert_int(t->content_len, ==, 8);
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
		assert_string_equal(t->content, "$USER");
		assert_int(t->type, ==, DOLLAR);
		assert_int(t->content_len, ==, 5);
	}
	{
		char *str = "$ ";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_string_equal(t->content, "$");
		assert_int(t->type, ==, DOLLAR);
		assert_int(t->content_len, ==, 1);
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
		assert_string_equal(t->content, "echo");
		assert_int(t->type, ==, WORD);
		assert_int(t->content_len, ==, 4);
		t = t->next;
		assert_string_equal(t->content, ">>");
		assert_string_equal(t->prev->content, "echo");
		assert_int(t->type, ==, APPEND);
		assert_int(t->prev->type, ==, WORD);
		assert_int(t->content_len, ==, 2);
		t = t->next;
		assert_string_equal(t->content, "file");
		assert_string_equal(t->prev->content, ">>");
		assert_int(t->type, ==, WORD);
		assert_int(t->prev->type, ==, APPEND);
		assert_int(t->content_len, ==, 4);
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

static MunitResult test_parser_syntax_check(const MunitParameter params[], void* data)
{
	(void)params;
	(void)data;

	{
		char *str = "<out";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_int(parser_is_syntax_correct(t), ==, true);
	}
	{
		char *str = "< |";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_int(parser_is_syntax_correct(t), ==, false);
	}
	{
		char *str = "> |";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_int(parser_is_syntax_correct(t), ==, false);
	}
	{
		char *str = "> out";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_int(parser_is_syntax_correct(t), ==, true);
	}
	{
		char *str = "| out";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_int(parser_is_syntax_correct(t), ==, false);
	}
	{
		char *str = "| |";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_int(parser_is_syntax_correct(t), ==, false);
	}
	{
		char *str = "|";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_int(parser_is_syntax_correct(t), ==, false);
	}
	{
		char *str = ">";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_int(parser_is_syntax_correct(t), ==, false);
	}
	{
		char *str = "<";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_int(parser_is_syntax_correct(t), ==, false);
	}
	{
		char *str = "echo >";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_int(parser_is_syntax_correct(t), ==, false);
	}
	{
		char *str = "echo >>";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_int(parser_is_syntax_correct(t), ==, false);
	}
	{
		char *str = "echo | |";
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		assert_int(parser_is_syntax_correct(t), ==, false);
	}
	return MUNIT_OK;
}

static MunitResult test_ft_realloc(const MunitParameter params[], void* data)
{
	(void)params;
	(void)data;
	{
		t_arena *arena = arena_init(ARENA_CAP);
		char *str = arena_strdup(arena, "hi mom");
		void *buf = ft_realloc(arena, &str, 6, 12);
		assert_ptr_not_null(buf);
	}
	return MUNIT_OK;
}

static MunitResult test_dynamic_array()
{
	{
		t_arena *arena = arena_init(ARENA_CAP);
		t_da *arr = da_cmd_init(arena, 2);
		munit_assert_int(arr->cap, ==, 2);
		munit_assert_int(arr->count, ==, 0);
		da_append(arena, arr, "hello");
		da_append(arena, arr, "world");
		da_append(arena, arr, "hi mom");
		da_append(arena, arr, "hi mom");
		da_append(arena, arr, "hi mom");
		da_append(arena, arr, "hi mom");
		da_append(arena, arr, "hi mom");
		munit_assert_string_equal(arr->items[0], "hello");
		munit_assert_string_equal(arr->items[1], "world");
		munit_assert_string_equal(arr->items[2], "hi mom");
		munit_assert_string_equal(arr->items[3], "hi mom");
		munit_assert_string_equal(arr->items[4], "hi mom");
		munit_assert_int(arr->cap, ==, 8);
	}
	return MUNIT_OK;
}

static MunitResult test_parser_cmd_build_one()
{
	char *str = "echo hello -n";
	t_shell shell = {0};
	t_arena *arena = arena_init(ARENA_CAP);
	t_lexer l = build_lexer(str);
	t_token *t = build_token_list(&l);
	t_cmd_table *cmd = parser_cmd_build_one(&shell, arena, t);
	munit_assert_string_equal(cmd->cmd_da->items[0], "echo");
	munit_assert_string_equal(cmd->cmd_da->items[1], "hello");
	munit_assert_string_equal(cmd->cmd_da->items[2], "-n");
	return MUNIT_OK;
}

static MunitResult test_parser_cmd_table()
{
	t_shell shell = {0};
	{
		char *str = "echo hello | wc -l | grep he | echo hi";
		t_arena *arena = arena_init(ARENA_CAP);
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		t_cmd_table *cmd_table = parser_cmd_build_many(&shell, arena, t);
		munit_assert_string_equal(cmd_table->cmd_da->items[0], "echo");
		munit_assert_string_equal(cmd_table->cmd_da->items[1], "hello");
		munit_assert_null(cmd_table->cmd_da->items[2]);
		munit_assert_string_equal(cmd_table->next->cmd_da->items[0], "wc");
		munit_assert_string_equal(cmd_table->next->cmd_da->items[1], "-l");
		munit_assert_string_equal(cmd_table->next->next->cmd_da->items[0], "grep");
		munit_assert_string_equal(cmd_table->next->next->cmd_da->items[1], "he");
		munit_assert_string_equal(cmd_table->next->next->next->cmd_da->items[0], "echo");
		munit_assert_string_equal(cmd_table->next->next->next->cmd_da->items[1], "hi");
		munit_assert_null(cmd_table->next->next->next->next);
	}
	{
		char *str = "echo 'hello'|ls -la";
		t_arena *arena = arena_init(ARENA_CAP);
		t_lexer l = build_lexer(str);
		t_token *t = build_token_list(&l);
		t_cmd_table *cmd_table = parser_cmd_build_many(&shell,arena, t);
		munit_assert_string_equal(cmd_table->cmd_da->items[0], "echo");
		munit_assert_string_equal(cmd_table->cmd_da->items[1], "'hello'");
		munit_assert_string_equal(cmd_table->next->cmd_da->items[0], "ls");
		munit_assert_string_equal(cmd_table->next->cmd_da->items[1], "-la");
	}
	return MUNIT_OK;
}

static MunitTest test_suite_tests[] = {
	{ "/test/arena", test_arena, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL, },
	{ "/test/lexer_general", test_lexer_general, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL, },
	{ "/test/lexer_dquote", test_lexer_dquote, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL, },
	{ "/test/lexer_squote", test_lexer_squote, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL, },
	{ "/test/lexer_dollar", test_lexer_dollar, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL, },
	{ "/test/lexer_append", test_lexer_append, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL, },
	{ "/test/parser_syntax_check", test_parser_syntax_check, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL, },
	{ "/env/get_basic", test_get_env_var_basic, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
	{ "/env/get_missing", test_get_env_var_missing, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
	{ "/env/set_overwrite", test_set_env_var_overwrite, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
	{ "/env/set_expand", test_set_env_var_expand, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
	{ "/env/unset_basic", test_unset_env_var_basic, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
	{ "/env/unset_missing", test_unset_env_var_missing, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
	{ "/test/ft_realloc", test_ft_realloc, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL, },
	{ "/test/dynamic_array", test_dynamic_array, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL, },
	{ "/test/parser_cmd_build_one", test_parser_cmd_build_one, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL, },
	{ "/test/parser_cmd_table", test_parser_cmd_table, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL, },
	{ NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
};

static const MunitSuite test_suite = {
	"minishell",
	test_suite_tests,
	NULL,
	1,
	MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char **argv)
{
	bash_comparison();
	return munit_suite_main(&test_suite, (void*) "lexer", argc, argv);
}

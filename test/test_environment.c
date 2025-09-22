#define MUNIT_ENABLE_ASSERT_ALIASES
#include "minishell.h"
#include "munit.h"
#include <string.h>

void init_min_shell(t_shell *shell)
{
	memset(shell, 0, sizeof(*shell));
	shell->arena = arena_init(ARENA_CAP);
	shell->env_capacity = 4;
	shell->env_count = 0;
	shell->heap_env = arena_alloc(shell->arena, sizeof(char*) * (shell->env_capacity + 1));
	shell->heap_env[0] = NULL;
}

MunitResult test_get_env_var_basic(const MunitParameter params[], void *data)
{
	(void)params; (void)data;
	t_shell shell = {0}; init_min_shell(&shell);
	set_env_var(&shell, "USER", "jyry");
	set_env_var(&shell, "HOME", "/somewhere/beyond");
	char *v1 = get_env_var(&shell, "USER");
	char *v2 = get_env_var(&shell, "HOME");
	assert_string_equal(v1, "jyry");
	assert_string_equal(v2, "/somewhere/beyond");
	arena_free(shell.arena);
	return MUNIT_OK;
}

MunitResult test_get_env_var_missing(const MunitParameter params[], void *data)
{
	(void)params; (void)data;
	t_shell shell; init_min_shell(&shell);
	set_env_var(&shell, "A", "1");
	assert_ptr_equal(get_env_var(&shell, "B"), NULL);
	assert_ptr_equal(get_env_var(NULL, "A"), NULL);
	assert_ptr_equal(get_env_var(&shell, NULL), NULL);
	arena_free(shell.arena);
	return MUNIT_OK;
}

MunitResult test_set_env_var_overwrite(const MunitParameter params[], void *data)
{
	(void)params; (void)data;
	t_shell shell; init_min_shell(&shell);
	set_env_var(&shell, "VAR", "one");
	char *first = get_env_var(&shell, "VAR");
	assert_string_equal(first, "one");
	set_env_var(&shell, "VAR", "two");
	char *second = get_env_var(&shell, "VAR");
	assert_string_equal(second, "two");
	assert_true(shell.state & ENV_MODIFIED);
	arena_free(shell.arena);
	return MUNIT_OK;
}

MunitResult test_set_env_var_expand(const MunitParameter params[], void *data)
{
	(void)params; (void)data;
	t_shell shell; init_min_shell(&shell);
	set_env_var(&shell, "A", "1");
	set_env_var(&shell, "B", "2");
	set_env_var(&shell, "C", "3");
	int old_cap = shell.env_capacity;
	set_env_var(&shell, "D", "4");
	assert_true(shell.env_capacity > old_cap);
	assert_string_equal(get_env_var(&shell, "D"), "4");
	arena_free(shell.arena);
	return MUNIT_OK;
}

MunitResult test_unset_env_var_basic(const MunitParameter params[], void *data)
{
	(void)params; (void)data;
	t_shell shell; init_min_shell(&shell);
	set_env_var(&shell, "A", "1");
	set_env_var(&shell, "B", "2");
	set_env_var(&shell, "C", "3");
	assert_int(shell.env_count, ==, 3);
	unset_env_var(&shell, "B");
	assert_int(shell.env_count, ==, 2);
	assert_ptr_equal(get_env_var(&shell, "B"), NULL);
	assert_string_equal(get_env_var(&shell, "A"), "1");
	assert_string_equal(get_env_var(&shell, "C"), "3");
	arena_free(shell.arena);
	return MUNIT_OK;
}

MunitResult test_unset_env_var_missing(const MunitParameter params[], void *data)
{
	(void)params; (void)data;
	t_shell shell; init_min_shell(&shell);
	set_env_var(&shell, "A", "1");
	unset_env_var(&shell, "B");
	assert_int(shell.env_count, ==, 1);
	unset_env_var(NULL, "A");
	unset_env_var(&shell, NULL);
	arena_free(shell.arena);
	return MUNIT_OK;
}

/* New extended tests */
MunitResult test_set_env_empty_value(const MunitParameter params[], void *data)
{
	(void)params; (void)data;
	t_shell shell; init_min_shell(&shell);
	set_env_var(&shell, "EMPTY", "");
	char *v = get_env_var(&shell, "EMPTY");
	assert_string_equal(v, "");
	assert_true(shell.state & ENV_MODIFIED);
	arena_free(shell.arena);
	return MUNIT_OK;
}

MunitResult test_set_env_large_value(const MunitParameter params[], void *data)
{
	(void)params; (void)data;
	char big[1024];
	memset(big, 'A', sizeof(big) - 1);
	big[sizeof(big) - 1] = '\0';
	t_shell shell; init_min_shell(&shell);
	set_env_var(&shell, "BIG", big);
	char *v = get_env_var(&shell, "BIG");
	assert_string_equal(v, big);
	arena_free(shell.arena);
	return MUNIT_OK;
}

MunitResult test_set_env_multiple_expansions(const MunitParameter params[], void *data)
{
	(void)params; (void)data;
	t_shell shell; init_min_shell(&shell);
	int target = 50; /* trigger several expansions */
	char key[16]; char val[16];
	for (int i = 0; i < target; i++)
	{
		snprintf(key, sizeof(key), "K%d", i);
		snprintf(val, sizeof(val), "%d", i);
		set_env_var(&shell, key, val);
	}
	assert_int(shell.env_count, ==, target);
	assert_string_equal(get_env_var(&shell, "K0"), "0");
	assert_string_equal(get_env_var(&shell, "K49"), "49");
	arena_free(shell.arena);
	return MUNIT_OK;
}

MunitResult test_set_env_partial_key_collision(const MunitParameter params[], void *data)
{
	(void)params; (void)data;
	t_shell shell; init_min_shell(&shell);
	set_env_var(&shell, "PATH", "/bin");
	set_env_var(&shell, "PATHNAME", "something");
	assert_string_equal(get_env_var(&shell, "PATH"), "/bin");
	assert_string_equal(get_env_var(&shell, "PATHNAME"), "something");
	arena_free(shell.arena);
	return MUNIT_OK;
}

MunitResult test_unset_env_first_and_last(const MunitParameter params[], void *data)
{
	(void)params; (void)data;
	t_shell shell; init_min_shell(&shell);
	set_env_var(&shell, "A", "1");
	set_env_var(&shell, "B", "2");
	set_env_var(&shell, "C", "3");
	set_env_var(&shell, "D", "4");
	unset_env_var(&shell, "A");
	assert_ptr_equal(get_env_var(&shell, "A"), NULL);
	unset_env_var(&shell, "D");
	assert_ptr_equal(get_env_var(&shell, "D"), NULL);
	assert_int(shell.env_count, ==, 2);
	assert_string_equal(get_env_var(&shell, "B"), "2");
	assert_string_equal(get_env_var(&shell, "C"), "3");
	arena_free(shell.arena);
	return MUNIT_OK;
}

MunitResult test_env_state_flag(const MunitParameter params[], void *data)
{
	(void)params; (void)data;
	t_shell shell; init_min_shell(&shell);
	assert_false(shell.state & ENV_MODIFIED);
	set_env_var(&shell, "X", "1");
	assert_true(shell.state & ENV_MODIFIED);
	shell.state &= ~ENV_MODIFIED; /* reset */
	set_env_var(&shell, "X", "2");
	assert_true(shell.state & ENV_MODIFIED);
	shell.state &= ~ENV_MODIFIED;
	unset_env_var(&shell, "X");
	assert_true(shell.state & ENV_MODIFIED);
	arena_free(shell.arena);
	return MUNIT_OK;
}

MunitResult test_set_env_empty_key_ignored(const MunitParameter params[], void *data)
{
	(void)params; (void)data;
	t_shell shell; init_min_shell(&shell);
	set_env_var(&shell, "", "value"); /* undefined behaviour: ensure not added */
	assert_int(shell.env_count, ==, 0);
	arena_free(shell.arena);
	return MUNIT_OK;
}


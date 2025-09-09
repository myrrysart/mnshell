#!/bin/bash
test_cmd="$1"
bash_output=$(timeout 5 bash -c "$test_cmd" 2>&1)
bash_exit=$?
mini_output=$(timeout 5 echo "$test_cmd" 2>&1 | ./minishell 2>&1)
mini_exit=$?
if [[ "$bash_output" == "$mini_output" && $bash_exit -eq $mini_exit ]]; then
    echo "PASS: $test_cmd"
else
    echo "FAIL: $test_cmd"
    echo "BASH: '$bash_output' (exit: $bash_exit)"
    echo "MINI: '$mini_output' (exit: $mini_exit)"
fi

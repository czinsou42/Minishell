#!/bin/bash

CMDS=(
    "echo hello"
    "echo \$HOME"
    "ls | grep .c"
    "echo 43 | echo 43"
    "echo a | cat | cat"
    "cat < /etc/passwd | grep root"
    "echo hello > /tmp/test.txt"
    "cat /tmp/test.txt"
    "echo a && echo b"
    "false || echo fallback"
    "export TEST=42 && echo \$TEST"
    "cd /tmp && pwd"
)

RESULT_FILE="valgrind_results.txt"
> "$RESULT_FILE"

LEAK_COUNT=0

for CMD in "${CMDS[@]}"; do
    echo "Testing: $CMD"
    echo "=============================" >> "$RESULT_FILE"
    echo "CMD: $CMD" >> "$RESULT_FILE"
    echo "=============================" >> "$RESULT_FILE"

    echo "$CMD" | valgrind --leak-check=full \
                           --show-leak-kinds=all \
                           --track-origins=yes \
                           --error-exitcode=42 \
                           --suppressions=readline.supp \
                           ./minishell >> "$RESULT_FILE" 2>&1

    EXIT=$?
    if [ $EXIT -eq 42 ]; then
        echo "  ❌ LEAK détecté"
        ((LEAK_COUNT++))
    else
        echo "  ✅ OK"
    fi

    echo "" >> "$RESULT_FILE"
done

echo ""
echo "Done. $LEAK_COUNT leak(s) détecté(s). Voir $RESULT_FILE"
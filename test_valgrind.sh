#!/bin/bash

CMD_FILE="commands.txt"
RESULT_FILE="valgrind_results.txt"

> "$RESULT_FILE"

LEAK_COUNT=0

while IFS= read -r CMD
do
    echo "Testing: $CMD"
    echo "=============================" >> "$RESULT_FILE"
    echo "CMD: $CMD" >> "$RESULT_FILE"
    echo "=============================" >> "$RESULT_FILE"

    printf "%s\nexit\n" "$CMD" | valgrind --leak-check=full \
                           --show-leak-kinds=all \
                           --track-origins=yes \
                           --error-exitcode=42 \
                           --suppressions=readline.supp \
                           ./minishell >> "$RESULT_FILE" 2>&1

    EXIT=$?
    if [ $EXIT -eq 42 ]; then
        echo "❌ LEAK détecté"
        ((LEAK_COUNT++))
    else
        echo "✅ OK"
    fi

    echo "" >> "$RESULT_FILE"

done < "$CMD_FILE"

echo ""
echo "Done. $LEAK_COUNT leak(s) détecté(s). Voir $RESULT_FILE"
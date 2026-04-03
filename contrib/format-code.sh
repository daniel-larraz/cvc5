#!/usr/bin/env bash
set -euo pipefail

mode="${1:-check}"  # default = check

case "$mode" in
  check)
    clang_flags=(--dry-run --Werror)
    ;;
  fix)
    clang_flags=(-i)
    ;;
  *)
    echo "Usage: $0 [check|fix]"
    exit 1
    ;;
esac

find src include test examples \
  -type f \
  \( -name '*.cpp' -o -name '*.h' -o -name '*.java' \) -print0 | \
xargs -0 clang-format \
  "${clang_flags[@]}" \
  -style=file

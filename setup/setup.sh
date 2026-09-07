#!/usr/bin/env bash

EXECUTABLE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

export PATH="$EXECUTABLE_DIR:$PATH"

echo "Added to PATH:"
echo "  $EXECUTABLE_DIR"
echo

echo "Available commands:"

for exe in "$EXECUTABLE_DIR"/*; do
    [ -e "$exe" ] || continue
    echo "  $(basename "$exe")"
done
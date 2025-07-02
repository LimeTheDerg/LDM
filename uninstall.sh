#!/bin/bash
set -e

if [ -d LDM ]; then
  BIN_PATH=$(realpath LDM)
  rm -rf LDM && echo "Deleted LDM directory at $BIN_PATH"
else
  echo "LDM does not exist — are you in the wrong directory?"
fi

cp "$HOME/.bashrc" "$HOME/.bashrc.backup"

grep -vF "$BIN_PATH" "$HOME/.bashrc" > "$HOME/.bashrc.tmp" && mv "$HOME/.bashrc.tmp" "$HOME/.bashrc" || echo "An error has occurred when trying to remove LDM from PATH"
echo "Removal complete, ~/.bashrc has been modified, and if something has gone wrong, there is a backup at ~/.bashrc.backup"


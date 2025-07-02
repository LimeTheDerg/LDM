#!/bin/bash
set -e

git clone https://github.com/LimeTheDerg/LDM || exit
cd LDM || exit

mv bin ../bin
cd ..
rm -rf LDM

mv bin LDM

BIN_PATH=$(realpath LDM)

export PATH="$PATH:$BIN_PATH"

if ! grep -qF "$BIN_PATH" "$HOME/.bashrc"; then
  echo "export PATH=\"$BIN_PATH:\$PATH\"" >> "$HOME/.bashrc"
  echo "Installed LDM successfully."
else
  echo "LDM seems to already exist on your system."
  rm -rf LDM
fi
echo "Reopen terminal for changes to take effect."

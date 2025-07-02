#!/bin/bash
set -e

if [ -d LDM ]; then
  BIN_PATH=$(realpath LDM)
  rm -rf LDM
else
  echo "LDM does not exist — are you in the wrong directory?"
fi



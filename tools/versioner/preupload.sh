#!/bin/bash

if ! which versioner > /dev/null; then
  echo "versioner not found (lunch and mma in bionic)"
  exit 1
fi

versioner -r arm -r arm64
exit $?

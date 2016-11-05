#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR

which versioner >/dev/null 2>&1
if [ $? -ne 0 ]; then
  >&2 echo "versioner not in path; run mma in $DIR/versioner"
  exit 1
fi

VERSION=$(git rev-parse --short HEAD)
git diff-index --quiet HEAD
DIRTY=$?
git branch -r --contains HEAD | grep -q aosp/master
SUBMITTED=$?

if [ $DIRTY -ne 0 ]; then
  >&2 echo "Warning: bionic has uncommitted changes"
  VERSION="${VERSION}-dirty"
elif [ $SUBMITTED -ne 0 ]; then
  >&2 echo "Warning: current HEAD does not exist in aosp/master"
  VERSION="${VERSION}-unsubmitted"
fi

PREBUILTS_DIR=$ANDROID_BUILD_TOP/prebuilts/ndk
BRANCH_NAME=$(git -C $PREBUILTS_DIR symbolic-ref --short -q HEAD)
if [ $? -ne 0 ]; then
  BRANCH_NAME=update-bionic-headers-$VERSION
  echo "prebuilts/ndk has detached head; creating branch $BRANCH_NAME"
  repo start $BRANCH_NAME $PREBUILTS_DIR
else
  echo "prebuilts/ndk already on branch $BRANCH_NAME"
fi

HEADERS_INSTALL=$PREBUILTS_DIR/headers
if [ -d "$HEADERS_INSTALL" ]; then
  git -C $PREBUILTS_DIR rm -r --ignore-unmatch $HEADERS_INSTALL
  if [ -d $HEADERS_INSTALL ]; then
    rm -r $HEADERS_INSTALL
  fi
fi

versioner -p versioner/platforms versioner/current versioner/dependencies \
  -o $HEADERS_INSTALL
if [ $? -ne 0 ]; then
  >&2 echo "Header preprocessing failed"
  exit 1
fi

cp ../libc/NOTICE $PREBUILTS_DIR

git -C $PREBUILTS_DIR add $HEADERS_INSTALL $PREBUILTS_DIR/NOTICE
git -C $PREBUILTS_DIR commit -m "Update bionic headers to $VERSION."

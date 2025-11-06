#!/bin/bash

cd "$(dirname "$(readlink -f "$0")")" || exit 1

DIRTY="$(git status --porcelain)"
if [ -n "$DIRTY" ]; then
	>&2 echo "dirty git, exit"
	exit 1
fi

LOCK_FILE=".build.lock"
exec 200>"$LOCK_FILE"
flock -n 200 || {
    echo "$0 is running, exit"
    exit
}
echo $$ > "$LOCK_FILE"

git pull --rebase || exit 1

PREV_COMMIT=""
COMMIT_FILE=".prev-commit"
if [ -f "$COMMIT_FILE" ]; then
	PREV_COMMIT="$(< "$COMMIT_FILE")"
fi

COMMIT="$(git rev-parse HEAD)"
if [ "$PREV_COMMIT" == "$COMMIT" ]; then
	>&2 echo "no new commit, exit"
	exit 0
fi

sudo docker build \
	--progress=plain -t \
	ruin \
	-f Dockerfile .. || exit 1

echo "$COMMIT" | tee "$COMMIT_FILE"

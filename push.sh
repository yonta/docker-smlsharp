#!/bin/bash

# https://qiita.com/youcune/items/fcfb4ad3d7c1edf9dc96
# -euでエラーか未定義変数でストップする

set -eu

# environment variables

source .env

# color message

normal=$(tput sgr0)
gray=$(tput setaf 8)
function message() {
    echo -e "$gray##### $* ###############$normal"
}

# build Dockerfile

name=${DOCKERHUB_USER}/smlsharp

for version in ${VERSIONS}; do
    for image in ${IMAGES}; do
        if [ -f ${version}/${image}/Dockerfile ]; then
            short_tag=${name}:${version}
            tag=${short_tag}-${image}
            message "push ${tag}"
            docker push ${tag}
            if [ "${image}" = "${SHORT_IMAGE}" ]; then
                docker push ${short_tag}
            fi
        fi
    done
done

latest_version=(${VERSIONS})
message "push latest"
docker push ${name}:latest
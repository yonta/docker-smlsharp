#!/bin/bash

# https://qiita.com/youcune/items/fcfb4ad3d7c1edf9dc96
# -euでエラーか未定義変数でストップする

set -eu

# environment variables

source .env
name=${DOCKERHUB_USER}/smlsharp

# color message

normal=$(tput sgr0)
gray=$(tput setaf 8)
function message() {
    echo -e "$gray##### $* ###############$normal"
}

# build Dockerfile

for version in ${VERSIONS}; do
    for image in ${IMAGES}; do
        short_tag=${name}:${version}
        tag=${short_tag}-${image}
        case ${version} in
            3.7.0|3.7.1|4.0.0)
                if [ -f 3.7.0/${image}/Dockerfile ]; then
                    message "push ${tag}"
                    docker push ${tag}
                    if [ "${image}" = "${SHORT_IMAGE}" ]; then
                        docker push ${short_tag}
                    fi
                fi ;;
            *)
                if [ -f ${version}/${image}/Dockerfile ]; then
                    message "push ${tag}"
                    docker push ${tag}
                    if [ "${image}" = "${SHORT_IMAGE}" ]; then
                        docker push ${short_tag}
                    fi
                fi ;;
        esac
    done
done

latest_version=(${VERSIONS})
message "push latest"
docker push ${name}:latest

# syntax=docker/dockerfile:1.3-labs
FROM debian:buster-slim

ARG smlsharp_version="4.0.0"
ARG sources_url="https://github.com/smlsharp/repos/raw/main/debian/dists/buster/smlsharp.list"

SHELL ["/bin/bash", "-eo", "pipefail", "-c"]

RUN <<EOF
  apt-get update -qq
  apt-get upgrade -qq --yes
  apt-get install --yes -qq --no-install-recommends wget=1.21* ca-certificates=20210119*
  wget --quiet --directory-prefix=/usr/share/keyrings \
  https://www.pllab.riec.tohoku.ac.jp/smlsharp/download/debian/dists/buster/smlsharp-archive-keyring.gpg
  wget --quiet --directory-prefix=/etc/apt/sources.list.d $sources_url
  apt-get update -qq
  apt-get install --yes -qq --no-install-recommends smlsharp=$smlsharp_version*
  apt-get purge --yes -qq wget ca-certificates
  apt-get autopurge --yes -qq
  rm --recursive --force /var/cache/apt/archives/* /var/lib/apt/lists/* /tmp/* /var/tmp/*
  truncate --size=0 /var/log/**/*log
EOF

CMD ["smlsharp"]

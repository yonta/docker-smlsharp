# syntax=docker/dockerfile:1.3-labs
FROM debian:bullseye-slim

SHELL ["/bin/bash", "-eo", "pipefail", "-c"]

RUN <<EOF
  apt-get update -qq
  apt-get upgrade -qq --yes
  apt-get install --yes -qq --no-install-recommends wget=1.21* ca-certificates=20210119*
  apt-get install --yes -qq --no-install-recommends \
    libc6=2.31* libgcc-s1=10.2.* libgmp10=2:6.2.* libstdc++6=10.2.* \
    gcc=4:10.2.* llvm-11=1:11.0.* libc6-dev=2.31* libgmp-dev=2:6.2.*
  apt-mark auto \
    libc6 libgcc-s1 libgmp10 libstdc++6 gcc llvm-11 libc6-dev libgmp-dev
  wget --quiet \
    https://github.com/smlsharp/repos/raw/main/debian/pool/s/smlsharp/smlsharp_4.0.0-1_amd64.deb \
    https://github.com/smlsharp/repos/raw/main/debian/pool/m/massivethreads/massivethreads-dev_1.00-2_amd64.deb \
    https://github.com/smlsharp/repos/raw/main/debian/pool/m/massivethreads/massivethreads_1.00-2_amd64.deb
  dpkg --install massivethreads_1.00-2_amd64.deb \
                 massivethreads-dev_1.00-2_amd64.deb \
                 smlsharp_4.0.0-1_amd64.deb
  rm massivethreads_1.00-2_amd64.deb \
     massivethreads-dev_1.00-2_amd64.deb \
     smlsharp_4.0.0-1_amd64.deb
  apt-get purge --yes -qq wget ca-certificates
  apt-get autopurge --yes -qq
  rm --recursive --force /var/cache/apt/archives/* /var/lib/apt/lists/* /tmp/* /var/tmp/*
  truncate --size=0 /var/log/**/*log
EOF

CMD ["smlsharp"]

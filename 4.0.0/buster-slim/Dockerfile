# syntax=docker/dockerfile:1.3-labs
FROM debian:buster-slim

SHELL ["/bin/bash", "-eo", "pipefail", "-c"]

RUN <<EOF
  apt-get update -qq
  apt-get upgrade -qq --yes
  apt-get install --yes -qq --no-install-recommends wget=1.20.* ca-certificates=20200601*
  apt-get install --yes -qq --no-install-recommends \
    libc6=2.28* libgcc1=1:8.3.* libgmp10=2:6.1.* libstdc++6=8.3.* gcc=4:8.3.* \
    llvm-7=1:7.0.* libc6-dev=2.28* libgmp-dev=2:6.1.*
  apt-mark auto \
    libc6 libgcc1 libgmp10 libstdc++6 gcc llvm-7 libc6-dev libgmp-dev
  wget --quiet \
    https://github.com/smlsharp/repos/raw/main/debian/pool/s/smlsharp/smlsharp_4.0.0-1%2B1%7Ebuster1_amd64.deb \
    https://github.com/smlsharp/repos/raw/main/debian/pool/m/massivethreads/massivethreads-dev_1.00-2%2B1%7Ebuster1_amd64.deb \
    https://github.com/smlsharp/repos/raw/main/debian/pool/m/massivethreads/massivethreads_1.00-2%2B1%7Ebuster1_amd64.deb
  dpkg --install massivethreads_1.00-2+1~buster1_amd64.deb \
                 massivethreads-dev_1.00-2+1~buster1_amd64.deb \
                 smlsharp_4.0.0-1+1~buster1_amd64.deb
  rm massivethreads_1.00-2+1~buster1_amd64.deb \
     massivethreads-dev_1.00-2+1~buster1_amd64.deb \
     smlsharp_4.0.0-1+1~buster1_amd64.deb
  apt-get purge --yes -qq wget ca-certificates
  apt-get autopurge --yes -qq
  rm --recursive --force /var/cache/apt/archives/* /var/lib/apt/lists/* /tmp/* /var/tmp/*
  truncate --size=0 /var/log/**/*log
EOF

CMD ["smlsharp"]

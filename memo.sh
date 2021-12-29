# get massivethreads deb packages
wget https://github.com/smlsharp/repos/raw/main/debian/pool/m/massivethreads/massivethreads-dev_1.00-2%2B1%7Ebuster1_amd64.deb \
     https://github.com/smlsharp/repos/raw/main/debian/pool/m/massivethreads/massivethreads_1.00-2%2B1%7Ebuster1_amd64.de

# get SML# deb packages for bullseye
wget https://github.com/smlsharp/repos/raw/main/debian/pool/s/smlsharp/smlsharp_3.5.0-1_amd64.deb \
     https://github.com/smlsharp/repos/raw/main/debian/pool/s/smlsharp/smlsharp_3.6.0-1_amd64.deb \
     https://github.com/smlsharp/repos/raw/main/debian/pool/s/smlsharp/smlsharp_3.7.0-1_amd64.deb \
     https://github.com/smlsharp/repos/raw/main/debian/pool/s/smlsharp/smlsharp_3.7.1-1_amd64.deb \
     https://github.com/smlsharp/repos/raw/main/debian/pool/s/smlsharp/smlsharp_4.0.0-1_amd64.deb

# get dependency of SML# deb packages
ls | xargs -n 1 dpkg -I | grep -e '^ Version' -e '^ Depends'

# get versions of bullseye deb packages
apt show wget ca-certificates libc6 libgcc-s1 libgmp10 libstdc++6 libyajl2 gcc \
    llvm-9 llvm-10 llvm-11 libc6-dev libgmp-dev libyajl-dev | \
    grep -e '^Package:' -e '^Version'

# BULLSEYE
#   SML# v3.6.0がllvm-10に依存するが、bullseyeのレポジトリにはすでに存在しない

# bullseye$ ls | xargs -n 1 dpkg -I | grep -e '^ Version' -e '^ Depends'
# Version: 3.5.0-1
# Depends: libc6 (>= 2.29), libgcc1 (>= 1:3.0), libgmp10, libstdc++6 (>= 5.2), libyajl2 (>= 2.0.4), massivethreads (>= 1.00), gcc, llvm-9, libc-dev, libgmp-dev, libyajl-dev (>= 2), massivethreads-dev
# Version: 3.6.0-1
# Depends: libc6 (>= 2.29), libgcc-s1 (>= 3.0), libgmp10, libstdc++6 (>= 5.2), libyajl2 (>= 2.0.4), massivethreads (>= 1.00), gcc, llvm-10, libc-dev, libgmp-dev, libyajl-dev (>= 2), massivethreads-dev
# Version: 3.7.0-1
# Depends: libc6 (>= 2.29), libgcc-s1 (>= 3.0), libgmp10, libstdc++6 (>= 5.2), massivethreads (>= 1.00), gcc, llvm-11, libc-dev, libgmp-dev, massivethreads-dev
# Version: 3.7.1-1
# Depends: libc6 (>= 2.29), libgcc-s1 (>= 3.0), libgmp10, libstdc++6 (>= 5.2), massivethreads (>= 1.00), gcc, llvm-11, libc-dev, libgmp-dev, massivethreads-dev
# Version: 4.0.0-1
# Depends: libc6 (>= 2.29), libgcc-s1 (>= 3.0), libgmp10, libstdc++6 (>= 5.2), massivethreads (>= 1.00), gcc, llvm-11, libc-dev, libgmp-dev, massivethreads-dev
#
# bullseye# apt show wget ca-certificates libc6 libgcc-s1 libgmp10 libstdc++6 libyajl2 gcc llvm-9 llvm-10 llvm-11 libc6-dev libgmp-dev libyajl-dev | grep -e '^Package:' -e '^Version'
# Package: wget
# Version: 1.21-1+deb11u1
# Package: ca-certificates
# Version: 20210119
# Package: libc6
# Version: 2.31-13+deb11u2
# Package: libgcc-s1
# Version: 10.2.1-6
# Package: libgmp10
# Version: 2:6.2.1+dfsg-1+deb11u1
# Package: libstdc++6
# Version: 10.2.1-6
# Package: libyajl2
# Version: 2.1.0-3
# Package: gcc
# Version: 4:10.2.1-1
# Package: llvm-9
# Version: 1:9.0.1-16.1
# Package: llvm-11
# Version: 1:11.0.1-2
# Package: libc6-dev
# Version: 2.31-13+deb11u2
# Package: libgmp-dev
# Version: 2:6.2.1+dfsg-1+deb11u1
# Package: libyajl-dev
# Version: 2.1.0-3

# BUSTER

# get SML# deb packages for buster
wget https://github.com/smlsharp/repos/raw/main/debian/pool/s/smlsharp/smlsharp_3.5.0-1%2B1%7Ebuster1_amd64.deb \
     https://github.com/smlsharp/repos/raw/main/debian/pool/s/smlsharp/smlsharp_3.6.0-1%2B1%7Ebuster1_amd64.deb \
     https://github.com/smlsharp/repos/raw/main/debian/pool/s/smlsharp/smlsharp_3.7.0-1%2B1%7Ebuster1_amd64.deb \
     https://github.com/smlsharp/repos/raw/main/debian/pool/s/smlsharp/smlsharp_3.7.1-1%2B1%7Ebuster1_amd64.deb \
     https://github.com/smlsharp/repos/raw/main/debian/pool/s/smlsharp/smlsharp_4.0.0-1%2B1%7Ebuster1_amd64.deb

# get versions of buster deb packages
apt show wget ca-certificates libc6 libgcc1 libgmp10 libstdc++6 libyajl2 gcc \
    llvm-7 libc6-dev libgmp-dev libyajl-dev | \
    grep -e '^Package:' -e '^Version'

# buster$ ls | xargs -n 1 dpkg -I | grep -e '^ Version' -e '^ Depends'
# Version: 3.5.0-1+1~buster1
# Depends: libc6 (>= 2.27), libgcc1 (>= 1:3.0), libgmp10, libstdc++6 (>= 5.2), libyajl2 (>= 2.0.4), massivethreads (>= 1.00), gcc, llvm-7, libc-dev, libgmp-dev, libyajl-dev (>= 2), massivethreads-dev
# Version: 3.6.0-1+1~buster1
# Depends: libc6 (>= 2.27), libgcc1 (>= 1:3.0), libgmp10, libstdc++6 (>= 5.2), libyajl2 (>= 2.0.4), massivethreads (>= 1.00), gcc, llvm-7, libc-dev, libgmp-dev, libyajl-dev (>= 2), massivethreads-dev
# Version: 3.7.0-1+1~buster1
# Depends: libc6 (>= 2.27), libgcc1 (>= 1:3.0), libgmp10, libstdc++6 (>= 5.2), massivethreads (>= 1.00), gcc, llvm-7, libc-dev, libgmp-dev, massivethreads-dev
# Version: 3.7.1-1+1~buster1
# Depends: libc6 (>= 2.27), libgcc1 (>= 1:3.0), libgmp10, libstdc++6 (>= 5.2), massivethreads (>= 1.00), gcc, llvm-7, libc-dev, libgmp-dev, massivethreads-dev
# Version: 4.0.0-1+1~buster1
# Depends: libc6 (>= 2.27), libgcc1 (>= 1:3.0), libgmp10, libstdc++6 (>= 5.2), massivethreads (>= 1.00), gcc, llvm-7, libc-dev, libgmp-dev, massivethreads-dev
#
# buster# apt show wget ca-certificates libc6 libgcc1 libgmp10 libstdc++6 libyajl2 gcc llvm-7 libc6-dev libgmp-dev libyajl-dev | grep -e '^Package:' -e '^Version'
# Package: wget
# Version: 1.20.1-1.1
# Package: ca-certificates
# Version: 20200601~deb10u2
# Package: libc6
# Version: 2.28-10
# Package: libgcc1
# Version: 1:8.3.0-6
# Package: libgmp10
# Version: 2:6.1.2+dfsg-4
# Package: libstdc++6
# Version: 8.3.0-6
# Package: libyajl2
# Version: 2.1.0-3
# Package: gcc
# Version: 4:8.3.0-1
# Package: llvm-7
# Version: 1:7.0.1-8+deb10u2
# Package: libc6-dev
# Version: 2.28-10
# Package: libgmp-dev
# Version: 2:6.1.2+dfsg-4
# Package: libyajl-dev
# Version: 2.1.0-3

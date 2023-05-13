FROM fedora:latest

RUN sudo yum  -y install wget

RUN sudo wget -O /etc/yum.repos.d/jenkins.repo \
    https://pkg.jenkins.io/redhat-stable/jenkins.repo

RUN sudo rpm --import https://pkg.jenkins.io/redhat-stable/jenkins.io-2023.key

RUN sudo yum -y update

RUN sudo dnf -y upgrade --refresh

RUN sudo dnf -y install \
  gcc \
  gcc-c++ \
  cmake \
  ninja-build \
  pkgconf-pkg-config \
  libtirpc-devel \
  rpcgen \
  git \
  glib2-devel \
  gtest \
  gtest-devel \
  gmock \
  gmock-devel \
  rpmdevtools \
  gcovr \
  lcov \
  libxml2-devel \
  fuse-devel \
  doxygen \
  rsync \
  libcurl-devel \
  openssl-devel \
  python3-pyyaml \
  python3-websockets \
  python3-beautifulsoup4 \
  python3-requests \
  libsoup-devel \
  libarchive-devel \
  sqlite-devel \
  json-c-devel \
  dbus-devel \
  libjpeg-devel \
  cryptsetup-devel \
  python3-jira \
  python-pylint \
  python3-gitlab \
  python3-pygithub \
  ftp \
  glibc-devel.i686 \
  uuid-devel \
  libudev-devel \
  device-mapper-devel \
  giflib-devel \
  java-latest-openjdk.x86_64 \
  wget \
  jenkins \
  helm \
  && sudo dnf clean all

WORKDIR /all_in_one

COPY . /all_in_one

CMD ["echo" , "welcome to fedora:latest"]

ENTRYPOINT [  ]


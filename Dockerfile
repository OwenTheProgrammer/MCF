FROM ubuntu:latest AS mcf-devel
ARG WATCHEXEC_TARGET="https://github.com/watchexec/watchexec/releases/download/v2.1.2/watchexec-2.1.2-x86_64-unknown-linux-gnu.deb"
RUN apt-get update \
    && DEBIAN_FRONTEND=nointeractive apt-get install -y --no-install-recommends \
        gcc make cmake git wget ca-certificates libc6-dev \
    && wget -O /tmp/watchexec.deb ${WATCHEXEC_TARGET} && dpkg -i /tmp/watchexec.deb \
    && rm /tmp/watchexec.deb \
    && mkdir -p /home/ubuntu/build \
    && chown ubuntu:ubuntu -R /home/ubuntu

WORKDIR /home/ubuntu
USER ubuntu
CMD cmake -S . -B build/ -D CMAKE_BUILD_TYPE=Debug \
    && watchexec -w src -w include cmake --build build/ -j$(nproc)

FROM ubuntu:latest AS mcf-release
RUN apt-get update \
    && DEBIAN_FRONTEND=nointeractive apt-get install -y --no-install-recommends \
        gcc make cmake libc6-dev libc6
WORKDIR /mcf
COPY . .
RUN cmake -S . -B build/ -D CMAKE_BUILD_TYPE=Release \
    && cmake --build build/ -j$(nproc)
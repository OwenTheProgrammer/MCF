FROM ubuntu:latest AS base

RUN apt-get update \
    && DEBIAN_FRONTEND=nointeractive apt-get install -y --no-install-recommends \
    git wget ca-certificates gcc libc6-dev gdb make

# == COMPILE WATCHEXEC FOR UBUNTU ==
FROM base AS build-watchexec
ARG WATCHEXEC_TARGET="https://github.com/watchexec/watchexec/releases/download/v2.1.2/watchexec-2.1.2-x86_64-unknown-linux-gnu.deb"
RUN wget -O /tmp/watchexec.deb ${WATCHEXEC_TARGET} && dpkg -i /tmp/watchexec.deb

# == WATCHEXEC BINARY IMAGE DATA ==
FROM scratch AS watchexec-ubuntu
COPY --from=build-watchexec /usr/share/bash-completion/completions/watchexec /usr/share/bash-completion/completions/watchexec
COPY --from=build-watchexec /usr/share/zsh/site-functions/_watchexec /usr/share/zsh/site-functions/_watchexec
COPY --from=build-watchexec /usr/share/fish/vendor_completions.d/watchexec.fish /usr/share/fish/vendor_completions.d/watchexec.fish
COPY --from=build-watchexec /usr/share/man/man1/watchexec.1.gz /usr/share/man/man1/watchexec.1.gz
COPY --from=build-watchexec /usr/share/doc/watchexec/watchexec.1.md /usr/share/doc/watchexec/watchexec.1.md
COPY --from=build-watchexec /usr/bin/watchexec /usr/bin/watchexec

FROM base AS devel
COPY --from=watchexec-ubuntu . .
RUN chown ubuntu:ubuntu -R /home/ubuntu
WORKDIR /home/ubuntu
USER ubuntu
CMD ["watchexec", "-w", "src", "-w", "include", "-w", "Makefile", "make", "debug", "-j$(nproc)"]
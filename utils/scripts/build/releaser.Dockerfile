FROM debian:bullseye

LABEL maintainer="Akkadius <akkadius1@gmail.com>"

ENV DEBIAN_FRONTEND=noninteractive \
    PUID=1000 \
    PGID=1000 \
    TZ=US/Central \
    CC=/usr/bin/clang \
    CXX=/usr/bin/clang++ \
    GO_VERSION=1.23.5 \
    GOPATH=/home/eqemu \
    GOROOT=/usr/local/go \
    PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/local/go/bin:/home/eqemu/bin \
    CGO_ENABLED=0

# Install dependencies
RUN apt-get update && \
    apt-get install -y sudo git curl wget build-essential clang clang++ \
    libmysqlclient-dev libperl-dev liblua5.2-dev lua5.2 \
    cmake unzip logrotate npm && \
    useradd -m -s /bin/bash -u $PUID eqemu && \
    groupmod -g $PGID eqemu

# Copy logrotate
COPY logrotate.conf /etc/logrotate.d/var-log

# Copy entrypoint
COPY --chown=eqemu:eqemu ./entrypoint.pl /bin/entrypoint.pl
RUN chmod +x /bin/entrypoint.pl

# Install Go
RUN cd /tmp && \
    wget https://go.dev/dl/go${GO_VERSION}.linux-amd64.tar.gz && \
    tar -C /usr/local -xzf go${GO_VERSION}.linux-amd64.tar.gz && \
    rm go${GO_VERSION}.linux-amd64.tar.gz

USER eqemu
WORKDIR /home/eqemu

# Optional: Copy source code and build
# COPY --chown=eqemu:eqemu ./Server /home/eqemu/server
# RUN cd /home/eqemu/server && mkdir build && cd build && cmake .. && make -j$(nproc)

ENTRYPOINT ["/bin/bash", "-c", "entrypoint.pl; /bin/bash"]
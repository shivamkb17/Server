#############################################
# debian
#############################################
FROM debian:11-slim

LABEL maintainer="Your Name <your@email.com>"

ENV DEBIAN_FRONTEND=noninteractive
ARG PUID=1000
ARG PGID=1000
ENV PUID=${PUID}
ENV PGID=${PGID}
ENV TZ=US/Central
ENV CC=/usr/bin/clang
ENV CXX=/usr/bin/clang++
ENV GO_VERSION=1.23.5

#############################################
# basics
#############################################
RUN apt-get update && apt-get install -y \
        clang \
		rclone \
        build-essential \
        cmake \
        git \
        curl \
        wget \
		make \
        sudo \
        jq \
        nodejs \
        npm \
        mariadb-client \
        libmariadb-dev \
        libssl-dev \
        libtool \
        libluabind-dev \
        libsodium-dev \
        liblua5.2-0 \
        liblua5.2-dev \
        libboost-dev \
        unzip \
        nano \
        locales \
        python3 \
        python3-pip \
        gdb \
        valgrind \
        telnet \
        && rm -rf /var/lib/apt/lists/*

#############################################
# eqemu user
#############################################
RUN groupadd -g ${PGID} eqemu && \
    useradd -u ${PUID} -g eqemu -m eqemu && \
    echo "eqemu ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

#############################################
# go
#############################################
RUN wget https://go.dev/dl/go${GO_VERSION}.linux-amd64.tar.gz && \
    tar -C /usr/local -xzf go${GO_VERSION}.linux-amd64.tar.gz && \
    rm go${GO_VERSION}.linux-amd64.tar.gz

ENV GOPATH=/home/eqemu
ENV GOROOT=/usr/local/go/
ENV PATH=$PATH:/usr/local/go/bin:/home/eqemu/bin
ENV CGO_ENABLED=0

#############################################
# copy source code
#############################################
COPY --chown=eqemu:eqemu . /home/eqemu/eqemu-server

WORKDIR /home/eqemu/eqemu-server

#############################################
# build project
#############################################
RUN sudo -u eqemu make

#############################################
# copy entrypoint
#############################################
COPY --chown=eqemu:eqemu utils/scripts/build/entrypoint.pl /bin/entrypoint.pl

USER eqemu
WORKDIR /home/eqemu/eqemu-server

ENTRYPOINT ["/bin/bash", "-c", "entrypoint.pl; /bin/bash"]

#############################################
# node
#############################################
RUN npm install -g gh-release


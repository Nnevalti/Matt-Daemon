FROM debian:buster

RUN apt-get update && apt-get install -y \
    bash \
    clang \
    make \
    less \
    procps \
    libc-dev \
    libssl-dev \
    libcurl4-openssl-dev

WORKDIR /matt-daemon

CMD ["tail", "-f"]

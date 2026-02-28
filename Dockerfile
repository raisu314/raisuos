# RaisuOS Build Environment
# This Dockerfile creates a complete cross-compilation environment
# No WSL/Linux needed on user's machine - just Docker Desktop or GitHub Actions

FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install all build tools
RUN apt-get update && apt-get install -y \
    build-essential \
    nasm \
    xorriso \
    grub-pc-bin \
    grub-common \
    mtools \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Use system GCC for build (compatible with freestanding)
ENV CC=gcc
ENV LD=ld
ENV PATH="/usr/bin:${PATH}"

WORKDIR /raisuos
COPY . .

CMD ["make", "iso"]

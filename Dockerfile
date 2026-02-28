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

# Download pre-built x86_64-elf cross-compiler
RUN mkdir -p /opt/cross && \
    curl -L "https://newos.org/toolchains/x86_64-elf-7.5.0-Linux-x86_64.tar.xz" \
    -o /tmp/cross.tar.xz && \
    tar -xf /tmp/cross.tar.xz -C /opt/cross --strip-components=1 && \
    rm /tmp/cross.tar.xz

ENV PATH="/opt/cross/bin:${PATH}"

WORKDIR /raisuos
COPY . .

CMD ["make", "iso"]

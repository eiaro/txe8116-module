FROM ubuntu:24.04

# Metadata
LABEL org.opencontainers.image.source="https://github.com/eiaro/txe8116-module"
LABEL org.opencontainers.image.description="KiBot with KiCad 9.0.5 for txe8116-module"
LABEL org.opencontainers.image.licenses="MIT"

# Avoid interactive prompts during installation
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Oslo

# Install basic dependencies
RUN apt-get update && \
    apt-get install -y \
    software-properties-common \
    python3 \
    python3-pip \
    git \
    wget \
    && rm -rf /var/lib/apt/lists/*

# Install KiCad 9.0.5
RUN add-apt-repository ppa:kicad/kicad-9.0-releases && \
    apt-get update && \
    apt-get install -y kicad && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Install KiBot and necessary Python packages
RUN pip3 install --no-cache-dir --no-compile \
    kibot \
    --break-system-packages

# Verify installations
RUN kicad-cli version && \
    kibot --version

# Set working directory
WORKDIR /workspace

# Note: Running as root is fine for CI/CD containers
# The container isolation provides sufficient security

ENTRYPOINT ["kibot"]
CMD ["--help"]

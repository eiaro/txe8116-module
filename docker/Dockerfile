FROM ghcr.io/inti-cmnb/kicad9_auto:latest

# Metadata
LABEL org.opencontainers.image.source="https://github.com/eiaro/txe8116-module"
LABEL org.opencontainers.image.description="KiBot with KiCad 9.0.5 for txe8116-module"
LABEL org.opencontainers.image.licenses="MIT"

# Avoid interactive prompts
ENV DEBIAN_FRONTEND=noninteractive

# Update KiCad to 9.0.5
RUN apt-get update && \
    apt-get install -y software-properties-common && \
    add-apt-repository ppa:kicad/kicad-9.0-releases && \
    apt-get update && \
    apt-get upgrade -y kicad && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Verify versions
RUN kicad-cli version && \
    kibot --version

WORKDIR /workspace

ENTRYPOINT ["kibot"]
CMD ["--help"]

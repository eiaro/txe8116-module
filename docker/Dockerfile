FROM ghcr.io/inti-cmnb/kicad9_auto:latest

# Metadata
LABEL org.opencontainers.image.source="https://github.com/eiaro/txe8116-module"
LABEL org.opencontainers.image.description="KiBot with KiCad 9.0.5 for txe8116-module"
LABEL org.opencontainers.image.licenses="MIT"

ENV DEBIAN_FRONTEND=noninteractive

# Add KiCad official Debian repository for 9.x
RUN apt-get update && \
    apt-get install -y wget gnupg && \
    wget -qO - https://repos.kicad.org/kicad-developers.gpg | apt-key add - && \
    echo "deb https://repos.kicad.org/debian bookworm main" > /etc/apt/sources.list.d/kicad.list && \
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

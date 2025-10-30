FROM ghcr.io/inti-cmnb/kicad9_auto:latest

# Metadata
LABEL org.opencontainers.image.source="https://github.com/eiaro/txe8116-module"
LABEL org.opencontainers.image.description="KiBot with KiCad 9.0.5 for txe8116-module"
LABEL org.opencontainers.image.licenses="MIT"

ENV DEBIAN_FRONTEND=noninteractive

# Add KiCad official Debian repository for 9.x
RUN apt-get update && \    
    apt-get upgrade -y kicad kicad-footprints kicad-symbols kicad-templates && \
    apt-get upgrade -y && \
    apt-get -y autoremove && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/* /var/cache/debconf/templates.dat-old /var/lib/dpkg/status-old && \
	rm -rf /usr/share/icons/Adwaita/ /*.deb

# Verify versions
RUN kicad-cli version && \
    kibot --version

WORKDIR /workspace

ENTRYPOINT ["kibot"]
CMD ["--help"]

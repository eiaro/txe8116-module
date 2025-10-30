# KiBot Docker Image for txe8116-module

This Docker image contains KiBot with KiCad 9.0.5 to ensure consistent
versioning between local development and CI/CD.

## Why?

We use KiCad 9.0.5 locally, but the standard KiBot Docker image uses 9.0.1,
which can cause incompatibility issues.

## Usage

### Local Development
```bash
# Build the image
docker build -t txe8116-kibot:latest docker/

# Run ERC/DRC
docker run --rm -v $(pwd):/workspace txe8116-kibot:latest \
  -c hardware/kibot.yaml \
  -b hardware/txe8116-module.kicad_pcb
```

### In CI/CD
The image is automatically built and published to GitHub Container Registry
when changes are pushed to the main branch.

## Updating KiCad Version

Edit `Dockerfile` and update the KiCad version as needed.

FROM ubuntu:24.04

# Install dependencies
RUN apt-get update && apt-get install -y \
  build-essential \
  cmake \
  git \
  libasound2-dev \
  libjack-jackd2-dev \
  libfreetype6-dev \
  libcurl4-openssl-dev \
  libwebkit2gtk-4.1-dev \
  libgtk-3-dev \
  libx11-dev \
  libxrandr-dev \
  libxinerama-dev \
  libxcursor-dev \
  libxi-dev \
  && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /workspace

# Copy project files
COPY . .

# Clone JUCE and build Projucer
RUN git clone https://github.com/juce-framework/JUCE.git && \
    cd JUCE && \
    git checkout 7.0.12 && \
    cd extras/Projucer/Builds/LinuxMakefile && \
    make -j$(nproc)

# Try a different approach - use the exact same path format but with correct relative path
# The original paths were relative to the project root, so let's use that
RUN sed -i 's|path="../../../Downloads/JUCE/modules"|path="JUCE/modules"|g' Pulse24Sync.jucer && \
    sed -i 's|path="../JUCE/modules"|path="JUCE/modules"|g' Pulse24Sync.jucer

# Debug: Show what the paths look like after sed
RUN echo "=== PATHS AFTER SED ===" && grep -n "path=" Pulse24Sync.jucer

# Debug: Check if the path actually exists
RUN echo "=== PATH EXISTS CHECK ===" && \
    pwd && \
    ls -la && \
    echo "JUCE modules exist:" && ls -la JUCE/modules/ && \
    echo "Testing relative path:" && ls -la JUCE/modules/juce_core/

# Generate project with Projucer
RUN ./JUCE/extras/Projucer/Builds/LinuxMakefile/build/Projucer --resave Pulse24Sync.jucer

# Build the project
RUN cd Builds/LinuxMakefile && make -j$(nproc)

# Keep container running to copy artifacts
CMD ["tail", "-f", "/dev/null"]

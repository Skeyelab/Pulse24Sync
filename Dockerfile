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
  pkg-config \
  && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /workspace

# Copy project files
COPY . .

# Clean up any existing build artifacts and cache
RUN rm -rf build/ && rm -rf CMakeCache.txt && rm -rf CMakeFiles/

# Create build directory and build with CMake
RUN mkdir -p build && \
    cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release \
      -DJUCE_BUILD_EXTRAS=OFF \
      -DJUCE_BUILD_EXAMPLES=OFF \
      -DJUCE_ENABLE_MODULE_SOURCE_GROUPS=ON && \
    make -j$(nproc)

# Keep container running to copy artifacts
CMD ["tail", "-f", "/dev/null"]

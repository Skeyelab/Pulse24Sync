# Multi-stage build for better layer management and smaller final image
FROM ubuntu:24.04 AS builder

# Install build dependencies
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
  && rm -rf /var/lib/apt/lists/* \
  && apt-get clean

# Set working directory
WORKDIR /workspace

# Copy only necessary build files first (for better layer caching)
COPY CMakeLists.txt Pulse24Sync.jucer ./
COPY Source/ ./Source/

# Clean up any existing build artifacts and cache
RUN rm -rf build/ CMakeCache.txt CMakeFiles/ *.cmake

# Create build directory and build with CMake
RUN mkdir -p build && \
    cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release \
      -DJUCE_BUILD_EXTRAS=OFF \
      -DJUCE_BUILD_EXAMPLES=OFF \
      -DJUCE_ENABLE_MODULE_SOURCE_GROUPS=ON && \
    make -j$(nproc) && \
    # Clean up build intermediates to reduce image size
    rm -rf CMakeFiles/ *.cmake CMakeCache.txt && \
    find . -name "*.o" -delete && \
    find . -name "*.a" -delete

# Runtime stage - much smaller final image
FROM ubuntu:24.04 AS runtime

# Install only runtime dependencies
RUN apt-get update && apt-get install -y \
  libasound2 \
  libjack-jackd2-0 \
  libfreetype6 \
  libcurl4 \
  libwebkit2gtk-4.1-0 \
  libgtk-3-0 \
  libx11-6 \
  libxrandr2 \
  libxinerama1 \
  libxcursor1 \
  libxi6 \
  && rm -rf /var/lib/apt/lists/* \
  && apt-get clean

WORKDIR /workspace

# Copy only the built artifacts from the builder stage
COPY --from=builder /workspace/build ./build

# Keep container running to copy artifacts
CMD ["tail", "-f", "/dev/null"]

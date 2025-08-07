#!/bin/bash

# Pulse24Sync Test Runner Script
# This script builds and runs the test suite

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Cleanup function
cleanup() {
    local exit_code=$?
    echo "🧹 Performing cleanup..."
    
    # Clean up temporary files
    find . -name "*.tmp" -o -name "*.temp" -o -name "*~" -type f -delete 2>/dev/null || true
    
    if [ $exit_code -ne 0 ]; then
        echo -e "${RED}❌ Tests failed - temporary files cleaned up${NC}"
    fi
    
    exit $exit_code
}

# Set trap to ensure cleanup runs on script exit
trap cleanup EXIT INT TERM

echo -e "${BLUE}🧪 Running Pulse24Sync Test Suite${NC}"
echo "===================================="

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ]; then
    echo -e "${RED}❌ Error: CMakeLists.txt not found. Please run this script from the project root directory.${NC}"
    exit 1
fi

# Parse command line arguments
BUILD_TYPE="Release"
VERBOSE=false
CLEAN_BUILD=false
TEST_CATEGORY=""
HELP=false

while [[ $# -gt 0 ]]; do
    case $1 in
        --debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        --verbose|-v)
            VERBOSE=true
            shift
            ;;
        --clean)
            CLEAN_BUILD=true
            shift
            ;;
        --category=*)
            TEST_CATEGORY="${1#*=}"
            shift
            ;;
        --help|-h)
            HELP=true
            shift
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            HELP=true
            shift
            ;;
    esac
done

if [ "$HELP" = true ]; then
    echo "Usage: $0 [options]"
    echo "Options:"
    echo "  --debug           Build tests in Debug mode (default: Release)"
    echo "  --verbose, -v     Verbose test output"
    echo "  --clean           Clean build before running tests"
    echo "  --category=NAME   Run only tests in specified category"
    echo "  --help, -h        Show this help"
    exit 0
fi

# Check if CMake is installed
if ! command -v cmake &> /dev/null; then
    echo -e "${RED}❌ Error: CMake is not installed. Please install CMake 3.15 or higher.${NC}"
    exit 1
fi

# Check CMake version
CMAKE_VERSION=$(cmake --version | head -n1 | cut -d' ' -f3)
echo -e "${BLUE}📋 CMake version: $CMAKE_VERSION${NC}"

# Create or clean build directory
BUILD_DIR="build-tests"

if [ "$CLEAN_BUILD" = true ] && [ -d "$BUILD_DIR" ]; then
    echo -e "${YELLOW}🧹 Cleaning previous test build...${NC}"
    rm -rf "$BUILD_DIR"
fi

if [ ! -d "$BUILD_DIR" ]; then
    echo -e "${BLUE}📁 Creating test build directory...${NC}"
    mkdir "$BUILD_DIR"
fi

cd "$BUILD_DIR"

# Configure with CMake
echo -e "${BLUE}⚙️  Configuring tests with CMake...${NC}"
cmake .. -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DPULSE24SYNC_BUILD_TESTS=ON

# Build the tests
echo -e "${BLUE}🔨 Building tests...${NC}"
if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS
    make -j$(sysctl -n hw.ncpu)
else
    # Linux
    make -j$(nproc)
fi

# Check if test executable was built
if [ ! -f "tests/Pulse24SyncTests" ]; then
    echo -e "${RED}❌ Error: Test executable was not built successfully.${NC}"
    exit 1
fi

echo -e "${GREEN}✅ Tests built successfully!${NC}"
echo ""

# Run the tests
echo -e "${BLUE}🏃 Running tests...${NC}"
echo "===================="

TEST_ARGS=""
if [ "$VERBOSE" = true ]; then
    TEST_ARGS="$TEST_ARGS --verbose"
fi

if [ -n "$TEST_CATEGORY" ]; then
    TEST_ARGS="$TEST_ARGS --category=$TEST_CATEGORY"
fi

# Run the test executable
cd tests
if [ "$VERBOSE" = true ]; then
    echo -e "${YELLOW}Command: ./Pulse24SyncTests $TEST_ARGS${NC}"
fi

./Pulse24SyncTests $TEST_ARGS
TEST_RESULT=$?

# Also run CTest for additional reporting
cd ..
echo ""
echo -e "${BLUE}📊 Running CTest...${NC}"
ctest --output-on-failure

echo ""
if [ $TEST_RESULT -eq 0 ]; then
    echo -e "${GREEN}🎉 All tests passed successfully!${NC}"
else
    echo -e "${RED}❌ Some tests failed. Check the output above for details.${NC}"
    exit 1
fi

echo ""
echo -e "${BLUE}📋 Test Summary${NC}"
echo "================"
echo "Build Type: $BUILD_TYPE"
echo "Build Directory: $BUILD_DIR"
if [ -n "$TEST_CATEGORY" ]; then
    echo "Test Category: $TEST_CATEGORY"
fi

if [ -f "tests/test_results.log" ]; then
    echo "Test log: $BUILD_DIR/tests/test_results.log"
fi

echo ""
echo -e "${GREEN}✨ Test run completed successfully!${NC}"
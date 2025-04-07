# Dog Tracker

A C++ application for real-time dog tracking using IP cameras. The system captures video streams, detects motion, and saves video segments when activity is detected.

## Features

- Real-time video capture from IP cameras
- Motion detection and activity tracking
- Configurable motion sensitivity
- Video buffering and saving
- Support for RTSP protocol
- PTZ camera control (if supported by camera) **WIP**

## Prerequisites

### System Requirements
- Windows 10 or later
- C++17 compatible compiler (MinGW-w64 recommended)
- CMake 3.10 or later
- OpenCV 4.11.0 or later (built with MinGW)

### Development Environment Setup

1. **Compiler Installation**
   ```bash
   # Install MinGW-w64
   # Download from: https://www.mingw-w64.org/
   # Add to PATH: C:\msys64\mingw64\bin
   ```

2. **OpenCV Setup**
   ```bash
   # Build OpenCV with MinGW
   # Ensure OpenCV is built with the following modules:
   # - core
   # - imgproc
   # - videoio
   # - tracking
   ```

3. **CMake Installation**
   ```bash
   # Download and install CMake from: https://cmake.org/download/
   # Add to PATH during installation
   ```

## Project Structure

```
dog_tracker/
├── bin/                   # Compiled executables
├── build/                 # CMake build files
├── configuration/         # Configuration files
│   └── config.txt         # Camera settings
├── include/               # Header files
├── source/                # Source files
└── CMakeLists.txt         # CMake build configuration
```

## Configuration

### Camera Configuration

Create a `config/camera_config.txt` file with the following format:

```ini
# Camera Configuration
CAMERA_IP=192.168.1.100
CAMERA_USERNAME=admin
CAMERA_PASSWORD=password
CAMERA_PORT=554
STREAM_PATH=/stream1
```

### Build Configuration

The project uses CMake for building. The main configuration points are:

- OpenCV path: `C:/opencvSrc/opencv-4.11.0/build/install`
- MinGW path: `C:/msys64/mingw64`
- C++17 standard
- Debug symbols enabled

## Building the Project

1. **Configure CMake**
   ```bash
   # From the project root directory
   cmake -G "MinGW Makefiles" -B build -S .
   ```

2. **Build the Project**
   ```bash
   # Build the project
   cmake --build build
   ```

   Alternatively, use the provided build script:
   ```bash
   .\build.bat
   ```

3. **Output**
   - Executable: `bin/dog_tracker.exe`
   - Required DLLs are automatically copied to the bin directory

## Usage

1. **Configure Camera Settings**
   - Edit `configuration/config.txt` with your camera details
   - Ensure the camera supports RTSP protocol

2. **Run the Application**
   ```bash
   cd bin
   .\dog_tracker.exe
   ```

3. **Controls**
   - Press 'q' to quit the application
   - Motion detection is automatic
   - Video segments are saved when motion is detected

## Camera Support

The application is designed to work with any IP camera that supports RTSP protocol. It has been tested with:

- Tenvis IPROBOT 3
- Other RTSP-compatible cameras

To use a different camera:
1. Update the camera configuration in `configuration/config.txt`
2. Ensure the camera supports RTSP streaming
3. Verify the stream path matches your camera's RTSP URL structure

## Troubleshooting

1. **Build Issues**
   - Ensure all prerequisites are installed
   - Verify OpenCV and MinGW paths in CMakeLists.txt
   - Check compiler version supports C++17

2. **Runtime Issues**
   - Verify camera configuration
   - Check network connectivity
   - Ensure OpenCV DLLs are in the bin directory

## Contributing

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.


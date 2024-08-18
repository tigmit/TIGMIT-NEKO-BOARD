FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-24.04

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="none"


RUN sudo apt update && sudo apt upgrade
RUN sudo apt-get install -y python3
RUN apt install -y python3.12-venv
RUN apt-get install -y git
RUN apt-get install -y gitg

RUN apt-get install -y clang-format 

RUN apt-get install -y curl
RUN mkdir /etc/udev
RUN mkdir /etc/udev/rules.d
RUN curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core/develop/platformio/assets/system/99-platformio-udev.rules | sudo tee /etc/udev/rules.d/99-platformio-udev.rules
#RUN apt-get install git
# [Optional] Uncomment this section to install additional vcpkg ports.
# RUN su vscode -c "${VCPKG_ROOT}/vcpkg install <your-port-name-here>"

# [Optional] Uncomment this section to install additional packages.
# RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
#     && apt-get -y install --no-install-recommends <your-package-list-here>

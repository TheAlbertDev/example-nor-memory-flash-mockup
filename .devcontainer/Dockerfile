FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-24.04

ENV CPPUTEST_HOME=/opt/cpputest

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="3.22.2"

# Optionally install the cmake for vcpkg
COPY ./reinstall-cmake.sh /tmp/

RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
    chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi \
    && rm -f /tmp/reinstall-cmake.sh \
    && apt-get update \
    && apt-get install -y --no-install-recommends automake autoconf libtool build-essential clang-format \
    && git clone -c http.sslverify=false https://github.com/cpputest/cpputest.git /opt/cpputest \
    && cd /opt/cpputest \
    && autoreconf . -i \
    && ./configure \
    && make tdd \
    && rm -rf /var/lib/apt/lists/* \
    && rm -rf /tmp

# [Optional] Uncomment this section to install additional vcpkg ports.
# RUN su vscode -c "${VCPKG_ROOT}/vcpkg install <your-port-name-here>"

# [Optional] Uncomment this section to install additional packages.
# RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
#     && apt-get -y install --no-install-recommends <your-package-list-here>

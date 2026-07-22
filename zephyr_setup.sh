#!/usr/bin/env bash
# zephyr_setup.sh
# Recreates the Zephyr T3-topology workspace after cloning the repo
# on a new machine.
#
# Usage:
#   git clone <your-repo-url> zephyr_workspace
#   cd zephyr_workspace
#   ./zephyr_setup.sh

set -e

echo ">>> Installing OS-level dependencies (Ubuntu)"
sudo apt update
sudo apt install -y --no-install-recommends \
    git cmake ninja-build gperf ccache dfu-util device-tree-compiler wget \
    python3-dev python3-pip python3-setuptools python3-venv python3-tk \
    xz-utils file make gcc gcc-multilib g++-multilib libsdl2-dev libmagic1

echo ">>> Creating Python virtual environment (zephyr-env)"
python3 -m venv zephyr-env
source zephyr-env/bin/activate

echo ">>> Installing west"
pip install --upgrade pip
pip install west

echo ">>> Initializing west workspace from local manifest"
# manifest/ already came with the git clone and contains west.yml
west init -l manifest

echo ">>> Fetching zephyr + modules (this pulls zephyr/ and modules/)"
west update

echo ">>> Installing Zephyr's Python requirements"
pip install -r zephyr/scripts/requirements.txt

echo ">>> Exporting Zephyr CMake package"
west zephyr-export

echo ""
echo "=================================================================="
echo "Base workspace is ready."
echo ""
echo "Still needed (one-time, manual):"
echo "  1. Install the Zephyr SDK (toolchains):"
echo "     cd ~ && wget <zephyr-sdk-release-url> && ./zephyr-sdk-*-setup.sh"
echo "  2. Set env vars (add to ~/.bashrc if you want it permanent):"
echo "     export ZEPHYR_SDK_INSTALL_DIR=~/zephyr-sdk-<version>"
echo ""
echo "Every new terminal session, activate the venv first:"
echo "     source zephyr-env/bin/activate"
echo "     source zephyr/zephyr-env.sh"
echo "=================================================================="

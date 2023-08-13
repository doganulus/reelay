#!/bin/bash

# Trigger an error if non-zero exit code is encountered
set -e

# Make the command executable
# chmod +x ${1}

# Execute the command
exec ryjson ${@}

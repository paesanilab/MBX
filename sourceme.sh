# sourcing this file will export MBX_HOME to the directory that contains this file
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
export MBX_HOME=${SCRIPT_DIR}
echo "set MBX_HOME=${SCRIPT_DIR}"

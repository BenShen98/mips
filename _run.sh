#! /bin/sh
# rebuild prog if necessary
echo "\n\t**********************************"
echo "\t*   build start, building ...    *"
echo "\t**********************************\n"

if make simulator; then
  echo "\n\t**********************************"
  echo "\t* build finished, start programe *"
  echo "\t**********************************\n"
  ./bin/mips_simulator "$@"
else
  echo "error while build file"
fi

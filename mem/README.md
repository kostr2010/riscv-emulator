# CONTENTS

## `tests/`

tests folder. run `make tests_mem` to build these tests
run `./mem/tests_mem` from build folder to run them

## `mem.*`

memory module implementation

## `regfile.*`

register file implementation

## `memory_interface.*`

interface that every memory impementation should provide for interpreter. interpreter only includes that header file, so it doesn't know about any speific memory structure. thus, separating memory and runtime. they are only meeting together during linking process.

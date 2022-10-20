package main

// #cgo CFLAGS: -g -Wall -I/home/alex/Repos/libnspire/src/api
// #cgo LDFLAGS: -L/usr/lib/libnspire.so -lnspire
// #include <stdlib.h>
// #include "upload.h"
import "C"
import (
	"os"
	"path"
)

func main() {
	if len(os.Args) < 2 {
		return
	}

	fsrc := C.CString(os.Args[1])
	fdest := C.CString(path.Base(os.Args[1]))

	ret := C.upload(fsrc, fdest)

	os.Exit(int(ret))
}

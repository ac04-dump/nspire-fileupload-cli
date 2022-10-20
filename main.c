#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "nspire.h"

int std_libnspire_err(int ret) {
	switch (ret) {
	case NSPIRE_ERR_SUCCESS:	return 0;
	case -NSPIRE_ERR_TIMEOUT:	return -ETIMEDOUT;
	case -NSPIRE_ERR_NOMEM:		return -ENOMEM;
	case -NSPIRE_ERR_INVALID:	return -EPERM;
	case -NSPIRE_ERR_LIBUSB:	return -EIO;
	case -NSPIRE_ERR_NODEVICE:	return -ENODEV;
	case -NSPIRE_ERR_INVALPKT:	return -EIO;
	case -NSPIRE_ERR_NACK:		return -EIO;
	case -NSPIRE_ERR_BUSY:		return -EBUSY;
	case -NSPIRE_ERR_EXISTS:	return -EEXIST;
	case -NSPIRE_ERR_NONEXIST:	return -ENOENT;
	case -NSPIRE_ERR_OSFAILED:	return -EPERM;
	default:			return -EINVAL;
	}
}

#define ERR_EXIT(errstr, errnum) do { \
		fprintf(stderr, "Error (%d): %s\n", (errnum), (errstr)); \
		exit(errnum); \
	} while (0)

int main(int argc, char **argv) {
    printf("starting...\n");
    int ret;
    nspire_handle_t *handle;

    if ((ret = nspire_init(&handle))) {
		ERR_EXIT(nspire_strerror(ret), std_libnspire_err(ret));
    }

    FILE *f = fopen("../prime-checker/prime-checker.tns", "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

    char *string = malloc(fsize + 1);
    fread(string, fsize, 1, f);
    fclose(f);

    string[fsize] = 0;

    const char *path = "primes.tns";

    printf("Uploading primes.tns\n");
    ret = nspire_file_write(handle, path, string, fsize);

    nspire_free(handle);

    printf("done\n");
    return ret;
}

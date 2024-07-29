#define FUSE_USE_VERSION 26

#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <stdio.h>
#include <string.h>

#include "cs1550.h"

/**
 * Called whenever the system wants to know the file attributes, including
 * simply whether the file exists or not.
 *
 * `man 2 stat` will show the fields of a `struct stat` structure.
 */
static int cs1550_getattr(const char *path, struct stat *statbuf)
{
	// Clear out `statbuf` first -- this function initializes it.
	memset(statbuf, 0, sizeof(struct stat));

	// Check if the path is the root directory.
	if (strcmp(path, "/") == 0) {
		statbuf->st_mode = S_IFDIR | 0755;
		statbuf->st_nlink = 2;

		return 0; // no error
	}

	// Check if the path is a subdirectory.
	// 	if (path is a subdirectory) {
	// 		statbuf->st_mode = S_IFDIR | 0755;
	// 		statbuf->st_nlink = 2;
	//
	// 		return 0; // no error
	// 	}

	// Check if the path is a file.
	// 	if (path is a file) {
	// 		// Regular file
	// 		statbuf->st_mode = S_IFREG | 0666;
	//
	// 		// Only one hard link to this file
	// 		statbuf->st_nlink = 1;
	//
	// 		// File size -- replace this with the real size
	// 		statbuf->st_size = 0;
	//
	// 		return 0; // no error
	// 	}

	// Otherwise, the path doesn't exist.
	return -ENOENT;
}

/**
 * Called whenever the contents of a directory are desired. Could be from `ls`,
 * or could even be when a user presses TAB to perform autocompletion.
 */
static int cs1550_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			  off_t offset, struct fuse_file_info *fi)
{
	// This assumes no subdirectories exist. You'll need to change this.
	if (strcmp(path, "/") != 0)
		return -ENOENT;

	// The filler function allows us to add entries to the listing.
	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);

	// Add the subdirectories or files.
	// The +1 hack skips the leading '/' on the filenames.
	//
	//	for (each filename or subdirectory in path) {
	//		filler(buf, filename + 1, NULL, 0);
	//	}

	return 0;
}

/**
 * Creates a directory. Ignore `mode` since we're not dealing with permissions.
 */
static int cs1550_mkdir(const char *path, mode_t mode)
{
	return 0;
}

/**
 * Removes a directory.
 */
static int cs1550_rmdir(const char *path)
{
	return 0;
}

/**
 * Does the actual creation of a file. `mode` and `dev` can be ignored.
 */
static int cs1550_mknod(const char *path, mode_t mode, dev_t dev)
{
	return 0;
}

/**
 * Deletes a file.
 */
static int cs1550_unlink(const char *path)
{
	return 0;
}

/**
 * Read `size` bytes from file into `buf`, starting from `offset`.
 */
static int cs1550_read(const char *path, char *buf, size_t size, off_t offset,
		       struct fuse_file_info *fi)
{
	size = 0;

	return size;
}

/**
 * Write `size` bytes from `buf` into file, starting from `offset`.
 */
static int cs1550_write(const char *path, const char *buf, size_t size,
			off_t offset, struct fuse_file_info *fi)
{
	return size;
}

/**
 * Called when a new file is created (with a 0 size) or when an existing file
 * is made shorter. We're not handling deleting files or truncating existing
 * ones, so all we need to do here is to initialize the appropriate directory
 * entry.
 */
static int cs1550_truncate(const char *path, off_t size)
{
	return 0;
}

/**
 * Called when we open a file.
 */
static int cs1550_open(const char *path, struct fuse_file_info *fi)
{
        // If we can't find the desired file, return an error
        return -ENOENT;

	// Otherwise, return success
	// return 0;
}

/**
 * Called when close is called on a file descriptor, but because it might
 * have been dup'ed, this isn't a guarantee we won't ever need the file
 * again. For us, return success simply to avoid the unimplemented error
 * in the debug log.
 */
static int cs1550_flush(const char *path, struct fuse_file_info *fi)
{
	// Success!
	return 0;
}

/**
 * This function should be used to open and/or initialize your `.disk` file.
 */
static void *cs1550_init(struct fuse_conn_info *fi)
{
	// Add your initialization routine here.

	return NULL;
}

/**
 * This function should be used to close the `.disk` file.
 */
static void cs1550_destroy(void *args)
{
	// Add your teardown routine here.
}

/*
 * Register our new functions as the implementations of the syscalls.
 */
static struct fuse_operations cs1550_oper = {
	.getattr	= cs1550_getattr,
	.readdir	= cs1550_readdir,
	.mkdir		= cs1550_mkdir,
	.rmdir		= cs1550_rmdir,
	.read		= cs1550_read,
	.write		= cs1550_write,
	.mknod		= cs1550_mknod,
	.unlink		= cs1550_unlink,
	.truncate	= cs1550_truncate,
	.flush		= cs1550_flush,
	.open		= cs1550_open,
	.init		= cs1550_init,
	.destroy	= cs1550_destroy,
};

/*
 * Don't change this.
 */
int main(int argc, char *argv[])
{
	return fuse_main(argc, argv, &cs1550_oper, NULL);
}

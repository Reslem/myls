#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>

int main(int argc, char** argv) {
	
	if (argc < 2) argv[1] = "./";

	char* path = argv[1];
	DIR* dirp = opendir(path);
	if (dirp == NULL) {
		perror("opendir");
		exit(EXIT_FAILURE);
	}

	struct dirent* curdir;
	while ((curdir = readdir(dirp)) != NULL) {
		printf("%16.16s\t", curdir->d_name);
		
		struct stat statbuf;
		if (stat(curdir->d_name, &statbuf) == -1) {
			perror("stat");
			exit(EXIT_FAILURE);
		}

		printf("%" PRIdMAX "\t", statbuf.st_nlink);
		switch(statbuf.st_mode & S_IFMT) {
			case S_IFREG:
				printf("REG\t");
				break;
			case S_IFDIR:
				printf("DIR\t");
				break;
			case S_IFSOCK:
				printf("SOCK\t");
				break;
			case S_IFLNK:
				printf("LNK\t");
				break;
			case S_IFBLK:
				printf("BLK\t");
				break;
			case S_IFCHR:
				printf("CHR\t");
				break;
			case S_IFIFO:
				printf("FIFO\t");
				break;
			default:
				break;
		}

		printf("%" PRIdMAX "\t", statbuf.st_size);

		int fd = open(curdir->d_name, O_RDONLY);
		if (fd != -1) {
			const int BUFSIZE = 16;
			char buf[BUFSIZE];
			int bytes = read(fd, buf, BUFSIZE);
			if (bytes != -1) {
				for (int i = 0; i < bytes; i++) {
					if (buf[i] > 31 && buf[i] < 127) printf("%c", buf[i]);
					else printf(" ");
				}
			}
			if (close(fd) == -1) {
				perror("close");
				exit(EXIT_FAILURE);
			}

		}

		printf("\n");
	}
	
	if (closedir(dirp) == -1) {
		perror("closedir");
		exit(EXIT_FAILURE);
	}
	
	return 0;
}

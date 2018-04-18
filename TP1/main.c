#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define VERSION 1.0

size_t fsize(const char *filename) {
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    fprintf(stderr, "Cannot determine size of %s: %s\n",
            filename, strerror(errno));

    return -1;
}

void show_version() {
	printf("v%f\n", VERSION);
}

void show_help() {
	char archivo_help[] = "help.txt";
	FILE *fp = fopen(archivo_help, "r");
	if (!fp) return;

	size_t size_archive = fsize(archivo_help);
	char buffer[size_archive + 1];

	size_t read_bytes = fread(buffer, sizeof(char), size_archive, fp);
	if (size_archive != read_bytes) {
		fprintf(stderr, "Error with help.txt\n");
		return;
	}
	buffer[size_archive] = '\0';

	puts(buffer);
	fclose(fp);
}


int main (int argc, char *argv[]) {
	bool help, version, output;
	help = version = output = false;
	char* output_file;
	output_file = NULL;
	int flag = 0;
	struct option opts[] = {
		{"version", no_argument, 0, 'V'},
		{"help", no_argument, 0, 'h'},
		{"output", required_argument, 0, 'o'}
	};

	while ((flag = getopt_long(argc, argv, "Vho:", opts, NULL)) != -1) {
		switch (flag) {
			case 'V' :
				version = true;
				break;
			case 'h' :
				help = true;
				break;
			case 'o' :
				output_file = optarg;
				output = true;
				break;
		}
	}

	if (help) {
		show_help();
	} else if (version) {
		show_version();
	} else if (output) {
		printf("%s\n", output_file);
		//(output_file);
	}
	return EXIT_SUCCESS;
}
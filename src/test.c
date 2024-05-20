#include "test.h"

#include <stdint.h>

#define ENCODE(buf, ch, shift)                                                                                         \
	do {                                                                                                               \
		lsb = buf & 1;                                                                                                 \
		bit = ((ch >> 8 - shift) & 1);                                                                                 \
		if (lsb != bit && lsb == 0) {                                                                                  \
			buf |= 1;                                                                                                  \
		} else if (lsb != bit && lsb == 1) {                                                                           \
			buf ^= 1;                                                                                                  \
		}                                                                                                              \
	} while (0)

int test_encode_new(char *image_name, char *output_image_name) {
	FILE *src, *dest;
	size_t image_size, file_size, i, j, pos, magic_size, password_size, secret_text_size;
	char *buf;
	uint32_t lsb, bit;

	const char *magic = "$$$###";
	const char *password = "p4ssw0rd";
	const char *secret_text = "very.secret.passphrase!!!!";

	if ((src = fopen(image_name, "r+")) == NULL) {
		fprintf(stderr, "Couldn't open file %s\n", image_name);
		return 1;
	}

	fseek(src, 0L, SEEK_END);
	file_size = ftell(src);
	rewind(src);

	image_size = size_of_image(src);
	buf = (char *)malloc(file_size * sizeof(char));
	printf("file_size: %d\n", file_size);

	if (!buf) {
		fprintf(stderr, "Failed to allocate memory for `buf`\n");
		return 1;
	}

	fread(buf, sizeof(char), file_size, src);
	buf[file_size] = 0;
	fclose(src);

	if ((dest = fopen(output_image_name, "w")) == NULL) {
		fprintf(stderr, "Couldn't open file %s\n", image_name);
		return 1;
	}

	magic_size = strlen(magic);
	password_size = strlen(password);
	secret_text_size = strlen(secret_text);

	// encode the size of the "magic string"
	// skip header bytes
	pos = 54;
	for (i = 1; i <= 8; ++i) {
		ENCODE(buf[pos], magic_size, i);
		pos++;
	}

	for (i = 0; i < magic_size; ++i) {
		for (j = 1; j <= 8; ++j) {
			ENCODE(buf[pos], magic[i], j);
			++pos;
		}
	}

	for (i = 1; i <= 8; ++i) {
		ENCODE(buf[pos], password_size, i);
		pos++;
	}

	for (i = 0; i < password_size; ++i) {
		for (j = 1; j <= 8; ++j) {
			ENCODE(buf[pos], password[i], j);
			++pos;
		}
	}

	for (i = 1; i <= 8; ++i) {
		ENCODE(buf[pos], secret_text_size, i);
		++pos;
	}

	for (i = 0; i < secret_text_size; ++i) {
		for (j = 1; j <= 8; ++j) {
			ENCODE(buf[pos], secret_text[i], j);
			++pos;
		}
	}
	// write to file
	fwrite(buf, sizeof(char), file_size, dest);
	fclose(dest);
	free(buf);
}

int test_encode_old() {
	FILE *src, *dest, *secret;
	size_t image_size, file_size, i, j, pos, magic_size, password_size, secret_text_size;
	char *buf;
	uint32_t lsb, bit;

	const char *magic = "$$$###";
	const char *password = "p4ssw0rd";
	const char *secret_text = "very.secret.passphrase!!!!";

	if ((src = fopen("linux.bmp", "r+")) == NULL) {
		fprintf(stderr, "Couldn't open file %s\n", "linux.bmp");
		return 1;
	}

	image_size = size_of_image(src);
	secret_text_size = strlen(secret_text);
	password_size = strlen(password);

	if ((secret = fopen("secret.txt", "w+")) == NULL) {
		fprintf(stderr, "Couldn't open file %s\n", "secret.txt");
		return 1;
	}

	// copy secret_text into secret.txt
	for (i = 0; i < secret_text_size; i++)
		fputc(secret_text[i], secret);

	if ((dest = fopen("output.bmp", "w")) == NULL) {
		fprintf(stderr, "Couldn't open file %s\n", "output.bmp");
		return 1;
	}

	// copy the header to the destination
	char c;
	for (i = 0; i < 54; i++) {
		c = fgetc(src);
		fputc(c, dest);
	}

	size_encrypt(magic_size, src, dest);
	string_encrypt(magic, src, dest);

	size_encrypt(password_size, src, dest);
	string_encrypt(password, src, dest);

	size_encrypt(secret_text_size, src, dest);
	stega_encrypt(src, secret, dest);

	fclose(src);
	fclose(secret);
	fclose(dest);
}
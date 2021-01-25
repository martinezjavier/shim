// SPDX-License-Identifier: BSD/Tiano-XXX-FIXME
/*
 * sbat.c - parse SBAT data from the .rsrc section data
 */

#ifndef SBAT_H_
#define SBAT_H_

struct sbat_metadata {
	CHAR16 *InternalName;
	CHAR16 *ComponentGeneration;
	CHAR16 *ProductName;
	CHAR16 *ProductGeneration;
	CHAR16 *ProductVersion;
	CHAR16 *VersionGeneration;
};

int parse_sbat(char *sbat_base, size_t sbat_size, char *buffer,
	       struct sbat_metadata *sbat);

#endif /* !SBAT_H_ */
// vim:fenc=utf-8:tw=75:noet

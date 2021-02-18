// SPDX-License-Identifier: BSD-2-Clause-Patent
/*
 * sbat.c - parse SBAT data from the .rsrc section data
 */

#ifndef SBAT_H_
#define SBAT_H_

struct sbat_entry {
	const CHAR8 *component_name;
	const CHAR8 *component_generation;
	const CHAR8 *vendor_name;
	const CHAR8 *vendor_package_name;
	const CHAR8 *vendor_version;
	const CHAR8 *vendor_url;
};

EFI_STATUS parse_sbat(char *sbat_base, size_t sbat_size, size_t *sbats, struct sbat_entry ***sbat);

#endif /* !SBAT_H_ */
// vim:fenc=utf-8:tw=75:noet

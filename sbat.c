// SPDX-License-Identifier: BSD/Tiano-XXX-FIXME
/*
 * sbat.c - parse SBAT data from the .sbat section data
 */

#include "shim.h"

CHAR16 *get_sbat_field(CHAR16 *current, CHAR16 *end, CHAR16 **field)
{
	UINTN comma = StrCSpn(current, L",");
	if (comma == 0 || current + comma + 1 > end)
		return NULL;

	current[comma] = L'\0';
	*field = current;
	return current + comma + 1;
}

/*
 * Parse SBAT data from the .sbat section data
 */
int parse_sbat(char *sbat_base, size_t sbat_size, char *buffer,
	       struct sbat_metadata *sbat)
{
	CHAR16 *start = (CHAR16*) sbat_base;
	CHAR16 *end = (CHAR16 *) sbat_base + sbat_size;

	/* The file may or may not start with the Unicode byte order marker.
	 * Sadness ensues.  Since UEFI is defined as LE, I'm going to decree
	 * that these files must also be LE.
	 *
	 * IT IS THUS SO.
	 *
	 * But if we find the LE byte order marker, just skip it.
	 */
	if (*start == 0xfeff)
		start++;

	while ((*start == L'\r' || *start == L'\n') && start < end)
		start++;

	if (start == end)
		return -1;

	UINTN l = StrCSpn(start, L"\r\n");
	if (l == 0 && start[l] == L'\0')
		return -1;

	if (start + l > end)
		return -1;

	start[l] = L'\0';
	CHAR16 *current = start;

	current = get_sbat_field(current, end, &sbat->InternalName);
	if (!sbat->InternalName)
		return -1;

	current = get_sbat_field(current, end, &sbat->ComponentGeneration);
	if (!sbat->ComponentGeneration)
		return -1;

	current = get_sbat_field(current, end, &sbat->ProductName);
	if (!sbat->ProductName)
		return -1;

	current = get_sbat_field(current, end, &sbat->ProductGeneration);
	if (!sbat->ProductGeneration)
		return -1;

	current = get_sbat_field(current, end, &sbat->ProductVersion);
	if (!sbat->ProductVersion)
		return -1;

	current = get_sbat_field(current, end, &sbat->VersionGeneration);
	if (!sbat->VersionGeneration)
		return -1;

	return 0;
}

// vim:fenc=utf-8:tw=75:noet

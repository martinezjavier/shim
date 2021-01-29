// SPDX-License-Identifier: BSD/Tiano-XXX-FIXME
/*
 * sbat.c - parse SBAT data from the .sbat section data
 */

#include "shim.h"
#include <stdlib.h>

CHAR16 *get_sbat_field(CHAR16 *current, CHAR16 *end, CHAR16 **field)
{
	UINTN comma = StrCSpn(current, L",");
	if (comma == 0 || current + comma + 1 > end)
		return NULL;

	current[comma] = L'\0';
	*field = current;
	return current + comma + 1;
}

struct node* parse_sbat_var()
{
	UINT8 *data = 0;
	UINTN datazise;
	EFI_STATUS efi_status;

    efi_status = get_variable(L"SBAT", &data, &datazise, SHIM_LOCK_GUID);
	if (EFI_ERROR(efi_status)) {
			return NULL;
	}
	struct node *root = new_node(L"components",L"0",L"root");
	struct node *nodename = root;
	CHAR16 *start = (CHAR16*) data;
	if (*start == 0xfeff)
					start++;
    while (*start) {
					while (*start == L'\r' || *start == L'\n')
									start++;
					UINTN l = StrCSpn(start, L"\r\n");
					if (l == 0) {
									if (start[l] == L'\0')
													break;
									start++;
									continue;
	}
	CHAR16 c = start[l];
	start[l] = L'\0';
	CHAR16 *entry = start;
	UINTN comma0 = StrCSpn(entry, L",");
	entry[comma0] = L'\0';

    CHAR16 *version = entry + comma0 + 1;
	UINTN comma1 = StrCSpn(version, L",");
	version[comma1] = L'\0';

	CHAR16 *parent = entry + comma0 +1 +comma1 +1;
	UINTN comma2 = StrCSpn(parent, L";");
	parent[comma2] = L'\0';
	nodename = SearchAll(root, parent);
	console_print(L"node name %s, version %s, parent %s add to %s \n", entry, version, parent, nodename->data);
	add_child(nodename, entry, version , parent);
	start[l] = c;
	start += l;
    }
	return root;
}

struct node* new_node(CHAR16 *data, CHAR16 *version, CHAR16 *parent)
{
	struct node *new_node = malloc(sizeof(struct node));
	new_node->next = NULL;
	new_node->child = NULL;
	new_node->data = data;
	new_node->parent = parent;
	new_node->version = version;
	return new_node;
}

struct node* add_sibling(struct node *n, CHAR16 *data, CHAR16 *version, CHAR16 *parent)
{
	if ( n == NULL )
		return NULL;
	while (n->next)
		n = n->next;
	return (n->next = new_node(data, version, parent));
}

struct node* add_child(struct node *n, CHAR16 *data, CHAR16 *version, CHAR16 *parent)
{
	if ( n == NULL )
		return NULL;

    if ( n->child )
        return add_sibling(n->child, data, version, parent);
    else if ( n->next)
        return NULL;
    else
        return (n->child = new_node(data, version, parent));
}

struct node* SearchAll(struct node *root, CHAR16 * compname)
{
    struct node *child = NULL;
    if (root == NULL)
	{
        return NULL;
    }
    while (root)
    {
		if (StrCmp(root->data,compname) == 0)
			return root;
        if (root->child)
		{
			child = SearchAll(root->child, compname);
			if (child != NULL)
				return child;
        }
        root = root->next;
    }
    return root;
}

struct node* SearchComp(struct node *root, CHAR16 * rootname, CHAR16 * compname)
{
	struct node *child = NULL;
	if (root == NULL)
		return NULL;
	while (root)
	{
		if ((StrCmp(root->data,compname) == 0) && (StrCmp(root->parent,rootname) == 0))
		{	
			return root;
    	}
		if (root->child)
        {
			child = SearchComp(root->child, rootname, compname);
				if (child != NULL)
					return child;
		}
		root = root->next;
	}
	return root;
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
